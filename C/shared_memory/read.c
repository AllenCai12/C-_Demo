#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>


int main()
{
    key_t key = ftok("/home",1122);
    if(key == -1)
    {
        perror("ftok fail!\n");
        return -1;
    }
    int id = shmget(key,0,0);          //打开共享内存段
    if(id == -1)
    {
        perror("open fail!\n");
        return -1;
    }

    char *p = shmat(id,NULL,0);         //返回映射后的地址
    if(p == NULL)
    {
        perror("shmat fail!\n");
        return -1;
    }

    printf("%s\n",p);                //读出内容
    printf("%s\n",p + 250);         //读出内容

    shmdt(p);                         //撤销共享内存映射
    shmctl(id,IPC_RMID,NULL);         //删除共享内存对象
    return 0;
}



