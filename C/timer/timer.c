#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

static char msg[] = "time is running out/n";
static int len;

// 向标准错误输出信息，告诉用户时间到了
void prompt_info(int signo)
{
    //write(STDERR_FILENO, msg, len);
    printf("hello world\n");
}
// 建立信号处理机制
void init_sigaction(void)
{
    struct sigaction tact;
    /*信号到了要执行的任务处理函数为prompt_info*/
    tact.sa_handler = prompt_info;
    tact.sa_flags = 0;
    /*初始化信号集*/
    sigemptyset(&tact.sa_mask);
    /*建立信号处理机制*/
    sigaction(SIGALRM, &tact, NULL);
}

void init_time(int timeout)
{
    struct itimerval value;
    /*设定执行任务的时间间隔为2秒0微秒*/
    value.it_value.tv_sec = timeout;
    value.it_value.tv_usec = 0;
    /*设定初始时间计数也为2秒0微秒*/
    value.it_interval = value.it_value;
    /*设置计时器ITIMER_REAL*/
    setitimer(ITIMER_REAL, &value, NULL);
}
int main()
{
    //len = strlen(msg);
    int i;
    unsigned long test = 123456;
    init_sigaction();
    init_time(1);
    scanf("%d", &i);
    init_time(0);

    
    printf("finishe \n");
    while ( 1 );
    exit(0);
}

