/*
 * @File: jsonrpc-c-client.c
 * @Author: Terry Lei
 * @Email: freedom@ema-tech.com
 * @Brief: Implementation of types defined in jsonrpc-c-client.h
 */
#include "jsonrpc-c-client.h"
#include "ec_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>

#define RECONNECT_TIMEOUT   (1.0)

static void close_connection(struct ev_loop *loop, ev_io *w) {
    struct jrpc_client *client = (struct jrpc_client *) w->data;
    /* close */
    ev_io_stop(loop, w);
    close(w->fd);
    w->fd = -1;

    /* reset buffer */
    if(client->buffer){
        free(client->buffer);
        client->buffer = NULL;
    }
    client->buffer_size = 0;
    client->pos = 0;
    struct jrpc_response **pp;
    for (pp = &client->response_waiting_list; *pp; pp = &(*pp)->next) {
        struct jrpc_response *p = *pp;
        *pp = p->next; 
        free(p);
    }

    /* reconnect */
    struct ev_timer *timer_ptr = &client->reconnect_timer;
    ev_timer_set(timer_ptr, RECONNECT_TIMEOUT, 0.);
    ev_timer_start(client->loop, &client->reconnect_timer);
}

static void handle_response(struct jrpc_client *client, cJSON *root) {
    cJSON *error = cJSON_GetObjectItem(root, "error");
    cJSON *id = cJSON_GetObjectItem(root, "id");
    assert(id && id->type == cJSON_Number);

    if (error && client->debug_level > 0) {
        char *str = cJSON_Print(root);
        ec_log_error("Got error response: %s", str);
        free(str);
    }

    if (id->valueint < 0)
        return;

    //printf("[handle respone]=========id->valueint===%d\n",id->valueint);
    struct jrpc_response **pp;
    for (pp = &client->response_waiting_list; *pp; pp = &(*pp)->next) {

        struct jrpc_response *p = *pp;
        if (p->id == (unsigned int)id->valueint) {
            /* notify response if no error */
            if (!error)
                p->function(cJSON_GetObjectItem(root, "result"), p->data);
            /* remove it from list */

            *pp = p->next; 
            free(p);
            return;
        }
    }

    if (client->debug_level > 0)
        printf("No response for session id %d\n", id->valueint);
}

static struct jrpc_response * lookup_event_handle(struct jrpc_client *client, const char *name) {
    struct jrpc_response *p;
    for (p = client->event_handle_list; p; p = p->next) {
        if (strcmp(name, p->event_name) == 0)
            return p;
    }
    return NULL;
}

static void handle_event(struct jrpc_client *client, cJSON *root) {
    cJSON *result = cJSON_GetObjectItem(root, "result");
    cJSON *id = cJSON_GetObjectItem(root, "id");
    assert(id && id->type == cJSON_String);
    struct jrpc_response *event_handle = lookup_event_handle(client, id->valuestring);
    if (event_handle){
        //printf("[handle event]=========id->string===%s\n",id->valuestring);
        event_handle->function(result, event_handle->data);
    }
}

static void handle_package(struct jrpc_client *client, cJSON *root) {
    cJSON *id = cJSON_GetObjectItem(root, "id");
    if (id == NULL)
        return;
    if (id->type == cJSON_String) 
        return handle_event(client, root);
    else if (id->type == cJSON_Number)
        return handle_response(client, root);
    else
        return;
}

static void read_cb(struct ev_loop *loop, ev_io *w, int revents) {
    (void)revents;
    struct jrpc_client *client = (struct jrpc_client *)w->data;

    if (client->buffer_size <= (unsigned int)client->pos) {
        if (client->buffer_size == 0)
            client->buffer_size = 1500;
        else
            client->buffer_size *= 2;

        // allocate one more byte for '\0'
        client->buffer = realloc(client->buffer, client->buffer_size + 1); 
        if (!client->buffer) {
            perror("Memory error");
            client->buffer_size = 0;
            return close_connection(loop, w);
        }
    }

    int len = read(client->fd, client->buffer + client->pos, client->buffer_size - client->pos);
    if (len < 0) {
        perror("read");
        return close_connection(loop, w);
    }
    if (len == 0) 
        return close_connection(loop, w);

    client->pos += len;
    client->buffer[client->pos] = '\0';

    cJSON *root;
    char *end_ptr = NULL;
   
try_parse_again: 
    if ((root = cJSON_Parse_Stream(client->buffer, &end_ptr)) != NULL) {
        if (client->debug_level > 1) {
            char * str_result = cJSON_Print(root);
            printf("Valid JSON Received:\n%s\n", str_result);
            free(str_result);
        }
    
        if (root->type == cJSON_Object) {
            handle_package(client, root);
        }

        //shift processed request, discarding it
        memmove(client->buffer, end_ptr, strlen(end_ptr));
    
        client->pos = client->pos - (int)(end_ptr - client->buffer);
        memset(client->buffer + client->pos, 0,
               client->buffer_size - client->pos - 1);
    
        cJSON_Delete(root);

        if (client->pos > 0)
            goto try_parse_again;
    } 
    else {
        // did we parse the all buffer? If so, just wait for more.
        // else there was an error before the buffer's end
        if (end_ptr != (client->buffer + client->pos)) {
            if (client->debug_level) {
                printf("INVALID JSON Received:\n---\n%s\n---\n",
                        client->buffer);
            }
            return close_connection(loop, w);
        }
    }
}

static void connection_cb(struct ev_loop *loop, ev_io *w, int revents) {
    (void)revents;
    struct jrpc_client *client = (struct jrpc_client *)w->data;
    int valopt = 0;
    socklen_t len = sizeof(int); 

    if (getsockopt(w->fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &len) < 0) {
        ec_log_error("Error in getsockopt() %d - %s", errno, strerror(errno));
        return close_connection(loop, w);
    }
    if (valopt) {
        ec_log_error("Error in delayed connection() %d - %s", valopt, strerror(valopt));
        return close_connection(loop, w);
    }

    if (client->debug_level) 
        printf("Connected to %s:%d!\n", client->host, client->port);

    if (client->on_connected)
        client->on_connected(client, client->on_connected_data);

    ev_io_stop(loop, w);
    ev_io_init(w, read_cb, w->fd, EV_READ); 
    ev_io_start(loop, w);
}

static void start_connect(struct jrpc_client *client) {
    struct sockaddr_in addr; 
    addr.sin_family = AF_INET; 
    addr.sin_port = htons(client->port); 
    addr.sin_addr.s_addr = inet_addr(client->host); 
    static int callcnt = 0;
    printf("[start connect]=========callcnt===%d=======\n",callcnt++);
    int fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (fd < 0) {
        perror("socket");
        ev_timer_set(&client->reconnect_timer,  RECONNECT_TIMEOUT , 0.);
        ev_timer_start(client->loop, &client->reconnect_timer);
        return;
    }

    /* set non-blocking */
    long arg; 
    if ( (arg = fcntl(fd, F_GETFL, NULL)) < 0 ) { 
        perror("F_GEFL");
        close(fd);
        ev_timer_set(&client->reconnect_timer,  RECONNECT_TIMEOUT , 0.);
        ev_timer_start(client->loop, &client->reconnect_timer);
        return;
    }
    arg |= O_NONBLOCK; 
    if (fcntl(fd, F_SETFL, arg) < 0) { 
        perror("F_SETFL");
        close(fd);
        ev_timer_set(&client->reconnect_timer,  RECONNECT_TIMEOUT , 0.);
        ev_timer_start(client->loop, &client->reconnect_timer);
        return;
    }

    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0 && errno != EINPROGRESS) {
        perror("connect");
        close(fd);
        ev_timer_set(&client->reconnect_timer,  RECONNECT_TIMEOUT , 0.);
        ev_timer_start(client->loop, &client->reconnect_timer);
        return;
    }

    client->fd = fd;
    struct ev_io *io_ptr = &client->connect_watcher;
    ev_io_init(io_ptr, connection_cb, fd, EV_WRITE);
    client->connect_watcher.data = client;
    ev_io_start(client->loop, &client->connect_watcher);
}

static void reconnect_timeout(struct ev_loop *loop, struct ev_timer *w, int revents) {
    (void)loop;
    (void)revents;
    struct jrpc_client *client = (struct jrpc_client *) w->data;
    if (client->debug_level > 0)
        printf("Reconnecting...\n");
    start_connect(client);
}

void jrpc_client_init_with_ev_loop(struct jrpc_client *client, const char *host, int port, struct ev_loop *loop) {
    /* ignore the broken pipe signal which will caused process exit by default */
    signal(SIGPIPE,SIG_IGN); 

    memset(client, 0, sizeof(*client));
    client->event_handle_list = NULL;
    client->response_waiting_list = NULL;
    client->host = strdup(host);
    client->port = port;
    client->fd = -1;
    client->buffer_size = 0;
    client->session_id = 0;
    client->loop = loop;
    if (getenv("JRPC_DEBUG")) {
        client->debug_level = strtol(getenv("JRPC_DEBUG"), NULL, 10);
        printf("JSONRPC-C Debug level %d\n", client->debug_level);
    }

    struct ev_timer *timer_ptr = &client->reconnect_timer;
    ev_timer_init(timer_ptr, reconnect_timeout, RECONNECT_TIMEOUT, 0.0);
    client->reconnect_timer.data = client;

    start_connect(client);
}

void jrpc_client_destroy(struct jrpc_client *client) {
    free(client->host);
    free(client->buffer);
    // TODO free response & event handles
}

// Make the code work with both the old (ev_loop/ev_unloop)
// and new (ev_run/ev_break) versions of libev.
#ifdef EVUNLOOP_ALL
  #define EV_RUN ev_loop
  #define EV_BREAK ev_unloop
  #define EVBREAK_ALL EVUNLOOP_ALL
#else
  #define EV_RUN ev_run
  #define EV_BREAK ev_break
#endif

void jrpc_client_run(struct jrpc_client *client) {
    EV_RUN(client->loop, 0);
}

void jrpc_client_register_on_connected(struct jrpc_client *client, jrpc_on_connected_func func, void *data) {
    client->on_connected = func;
    client->on_connected_data = data;
}

void jrpc_client_register_event_handle(struct jrpc_client *client, 
        const char *event_name, jrpc_response_func func, void *data) {

    assert(! lookup_event_handle(client, event_name));

    struct jrpc_response *event_handle = malloc(sizeof(*event_handle)); 
    memset(event_handle, 0, sizeof(*event_handle));

    snprintf(event_handle->event_name, sizeof(event_handle->event_name), "%s", event_name);
    event_handle->function = func;
    event_handle->data = data;

    event_handle->next = client->event_handle_list;
    client->event_handle_list = event_handle;
}

static int safe_write(int fd, char *buf, int size) {
    int nwrite = 0;
    while (nwrite < size) {
        int len = write(fd, buf + nwrite, size - nwrite);
        if (len <= 0) {
            perror("safe_write");
            return nwrite;
        }
        nwrite += len;
    }
    return nwrite;
}

/* send rpc request, and return the id of this request (can be NULL) */
static int send_request(struct jrpc_client *client, const char *method, const cJSON *params, unsigned long *id) {
    assert(method);
    assert(params);
    char *str_params = cJSON_Print((cJSON *)params);
    if (!str_params) {
        ec_log_error("Invalid json object!");
        return -1;
    }

    client->session_id++;
    if (id)
        *id = client->session_id;

    int len = snprintf(NULL, 0, "{\"method\":\"%s\", \"params\":%s, \"id\":%lu}", 
                        method, str_params, client->session_id);
    char buf[len+1];
    snprintf(buf, len+1, "{\"method\":\"%s\", \"params\":%s, \"id\":%lu}", 
             method, str_params, client->session_id);

    if (client->debug_level > 0)
        printf("Sent: %s\n", buf);

    free(str_params);
    if (safe_write(client->fd, buf, len) != len)
        return -1;

    return 0;
}

int jrpc_client_call_without_response(struct jrpc_client *client, const char *method, const cJSON *params) {
    if (client->fd < 0) {
        ec_log_error("Server is not connected yet!");
        return -1;
    }
    return send_request(client, method, params, NULL);
}

int  jrpc_client_call(struct jrpc_client *client, const char *method, const cJSON *params, 
        jrpc_response_func on_response, void *data) {
    if (client->fd < 0) {
        ec_log_error("Server is not connected yet!");
        return -1;
    }

    unsigned long session_id = 0;
    if (send_request(client, method, params, &session_id) < 0)
        return -1;

    struct jrpc_response *response = malloc(sizeof(*response));
    memset(response, 0, sizeof(*response));
    response->function = on_response;
    response->data = data;
    response->id = session_id;
    /* add to list head */
    response->next = client->response_waiting_list;
    client->response_waiting_list = response;
    return 0;
}

void jrpc_client_dump(struct jrpc_client *client) {
    struct jrpc_response *p;
   
    printf("\nJRPC Client Dump:\n");  
    printf("===================\n");  

    printf("Event handles:\n");
    for (p = client->event_handle_list; p; p = p->next) {
        printf("* %s\n", p->event_name);
    }
    printf("\n");

    printf("Response waiting list (ID):\n");
    for (p = client->response_waiting_list; p; p = p->next) {
        printf("* %lu\n", p->id);
    }
    printf("\n");
}
