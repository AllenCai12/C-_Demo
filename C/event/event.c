#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define MAX_KEY_NAME_LEN 256

struct key_node {
    char name[MAX_KEY_NAME_LEN];
    int  code;
};


struct key_node g_key_tables[] = {
    {"KEY_B",      KEY_B,},
    {"EV_KEY",     EV_KEY},
    {"KEY_B",      KEY_B},
    {"BTN_LEFT",    BTN_LEFT},
    {"BTN_RIGHT",   BTN_RIGHT},   
    {"BTN_MIDDLE",  BTN_MIDDLE}, 
    {"BTN_SIDE",    BTN_SIDE},  
    {"BTN_EXTRA",   BTN_EXTRA},  
    {"BTN_FORWARD", BTN_FORWARD},  
    {"BTN_BACK",    BTN_BACK},  
    {"BTN_TASK)",   BTN_TASK}

};

typedef void (*btn_handle)(int code, int value);


int find_key(const char *name, struct key_node *node) 
{
    int i = 0;
    int count = sizeof(g_key_tables) / sizeof(struct key_node);
    for (i = 0; i < count; i ++) {
        if (0 == strcmp(g_key_tables[i].name, name)) {
            *node = g_key_tables[i];
            return 0;
        }
    }
    return -1;
};

int get_key_name (char *buf, int node) {
    int i = 0;
    int count = sizeof(g_key_tables) / sizeof(struct key_node);
    for (i = 0; i < count; i ++) {
        if (g_key_tables[i].code == node) {
            strcpy(buf, g_key_tables[i].name);
        }
    }
    return 0;
};

void handle(int code, int value)
{
    if (code == BTN_RIGHT) {
        printf("ritht value: %d\n", value); 
    }
    if (code == BTN_LEFT ) {
        printf("left value: %d\n", value); 
    }
}

int btn_linsten(char *dev, btn_handle handle)
{
    int ret = 0;
    int fd;
    struct input_event ev;
    char name[256];
    fd = open(dev, O_RDONLY);    
    if (fd < 0) {
        perror("open device");
        ret = -1;
        return ret;
    }

    do {  

        ret = read(fd, &ev, sizeof(ev));
        if (ret <= 0) {
            perror("read error"); 
            break;
        } 

        if (ev.type == EV_KEY) {
            handle(ev.code, ev.value);
            get_key_name(name, ev.code);
            printf("code : %s value : %d\n", name, ev.code);
        }

    } while(1);

    if (fd >= 0 ) {
        close(fd);
    }
    return ret;
}

int main(int argc, char **argv)
{
    int ret;
    int i = 0;

    btn_linsten(argv[1], handle);

    return 0;
}
