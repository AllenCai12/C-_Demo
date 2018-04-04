#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



#define handle_error(msg) \
    do {perror(msg); exit(EXIT_FAILURE);} while(0)

#define BUF_SIZE (256)
#define MY_SOCKER_PATH  "/tmp/local_socket"

int main(int argc, char **argv)
{
    int sfd, cfd;
    int ret;
    struct sockaddr_un my_addr, peer_addr;
    socklen_t peer_addr_size;
    char buf[BUF_SIZE];
   
    struct timeval time;

    
    printf("hellow world\n");
    

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        handle_error("socket");
    }

    memset(&my_addr, 0, sizeof(struct sockaddr_un));
    my_addr.sun_family = AF_UNIX;
    strncpy(my_addr.sun_path, MY_SOCKER_PATH, sizeof(my_addr.sun_path) -1);

    ret = bind(sfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_un));
    if (ret == -1) {
        handle_error("bind");
    }

    ret = listen(sfd, 1);
    if (ret == -1) {
        handle_error("listen");
    }

    peer_addr_size = sizeof(struct sockaddr_un);
    cfd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_size);
    if (cfd == -1) {
        handle_error("accept");
    }
    
    ret = recv(cfd, buf, BUF_SIZE, 0);
    if (ret == -1) {
        handle_error("recv");
    }

    printf("recv : %s\n", buf);


    return 0;
}


