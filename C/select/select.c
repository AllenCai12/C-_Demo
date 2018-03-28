#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include <linux/input.h>

#include <sys/stat.h>
#include <fcntl.h>

static int s_mouse_fd;
static int s_key_fd;

void handle(int signum)
{
    printf("signal handle\n");
    close(s_mouse_fd);
    close(s_key_fd);
    exit(1); 
}

int main(int argc, char **argv)
{
    fd_set rfds; 
    struct timeval tv;
    int maxfd;
    int retval;
    struct input_event ev;

    signal(SIGINT, handle);

    do {
        s_mouse_fd = open("/dev/input/event4", O_RDONLY);
        if (s_mouse_fd == -1) {
            perror("open()");
            break;
        }

        s_key_fd = open("/dev/input/event5", O_RDONLY);
        if (s_mouse_fd == -1) {
            perror("open()");
            break;
        }


        /*wait up to five seconds*/
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        /*get the max fd*/
        if (s_mouse_fd < s_key_fd) {
            maxfd = s_key_fd;
        }
        else {
            maxfd = s_mouse_fd;
        }

        while (1) {
            /*每次调用select, 内核都会修改rfds, 所以要重新设置*/
            FD_ZERO(&rfds);
            FD_SET(s_mouse_fd, &rfds);
            FD_SET(s_key_fd, &rfds);

            /*select文件描述符,必须是集合里最大文件描述符+1*/
            retval = select(maxfd+1, &rfds, NULL, NULL, &tv);

            if (retval == -1) {
                perror("select()"); 
                break;
            }

            if (retval) {
                /*判断文件描述符是否可读*/
                if (FD_ISSET(s_mouse_fd, &rfds)) {
                    printf("***********mouse is readable\n");
                    retval = read(s_mouse_fd, &ev, sizeof(ev));
                    if (retval < 0) {
                        perror("read()"); 
                    }

                    printf("type: %d\n", ev.type);
                    printf("code: %d\n", ev.code);
                    printf("value: %d\n", ev.value);
                }

                /*判断文件描述符是否可读*/
                if (FD_ISSET(s_key_fd, &rfds)) {
                    printf("************key is readable\n");
                    retval = read(s_key_fd, &ev, sizeof(ev));
                    if (retval < 0) {
                        perror("read()"); 
                    }

                    printf("type: %d\n", ev.type);
                    printf("code: %d\n", ev.code);
                    printf("value: %d\n", ev.value);
                }

            } 
            else {
                printf("no data within five seconds\n");
            }

            /* 阻塞时间会一直倒数,所以需要重新设置时间*/
            tv.tv_sec = 5;
            tv.tv_usec = 0;
            printf("==============sec: %ld\n", tv.tv_sec);
            printf("==============usec: %ld\n", tv.tv_usec);

        }

    } while(0);

    if (s_mouse_fd >= 0) {
        close(s_mouse_fd);
    }

    if (s_key_fd >= 0) {
        close(s_key_fd);
    }


    return 0;

}
