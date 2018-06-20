#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

int main()
{   
    key_t key = ftok("/home",1122);      //用路径和整数创建唯一的key
    if(key == -1)
    {
        perror("ftok fail!\n");
        return -1;
    }
    int id = shmget(key,4096,IPC_CREAT | 0660);    //生成共享内存段标识符
    if(id == -1)
    {
        perror("shmget fail!\n");
        return -1;
    }

    char *p = shmat(id,NULL,0);       //返回映射地址
    if(p == NULL)
    {
        perror("shmat fail!\n");
        return -1;
    }

    strcpy(p,"hello word!");       //写入内容
    strcpy(p+250, "come on bybe !");


    return 0;
}
