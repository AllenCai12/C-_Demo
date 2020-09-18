#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void signal_crash_handler(int sig)
{
    printf("sig: %d crash\n", sig);
    exit(-1);
}

void signal_exit_handler(int sig)
{
    printf("sig: %d exit\n", sig);
    exit(0);
}

void server_on_exit(void)
{
    printf("sig:  exit\n");
}


int main(int argc, char *argv[])
{
    atexit(server_on_exit);
    signal(SIGTERM, signal_exit_handler);
    signal(SIGINT, signal_exit_handler);

    // ignore SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    signal(SIGBUS, signal_crash_handler);     // 总线错误
    signal(SIGSEGV, signal_crash_handler);    // SIGSEGV，非法内存访问
    signal(SIGFPE, signal_crash_handler);       // SIGFPE，数学相关的异常，如被0除，浮点溢出，等等
    signal(SIGABRT, signal_crash_handler);     // SIGABRT，由调用abort函数产生，进程非正常退出

    int a[1];
    a[100] = 256;
    printf("a= %d\n", a[3]);

    return 0;
}
