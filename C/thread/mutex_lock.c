/********************************************
*                                           *
* 这个例子,是为了演示分离线程,参数传递      *
*的注意事项,如下例子,创建线程的时候,不能    *
*直接传递局域变量的地址,因为,执行线程后,    *
*函数执行完成.那么传递的传递局域变量的地    *
*址指向的变量值,就不复存在.`                *
*                                           *
********************************************/



#include <pthread.h>
#include <stdio.h>
#include <unistd.h>



static pthread_mutex_t testlock;

//error demo
#if 0
void *handle(void *ptr)
{
    printf("value : %d\n", *(int *)ptr);
}

void create_pthread_a(int i)
{
    pthread_t thread;

    pthread_create(&thread, NULL, handle, (void *)&i);
    pthread_detach(thread);
    
}
#endif

//right demo
void *handle(void *ptr)
{
    while(1) {
        pthread_mutex_lock(&testlock); 
        printf("value : %d\n", (int)ptr);
        pthread_mutex_unlock(&testlock); 
    }
}

void create_pthread(int i)
{
    
    pthread_t thread;

    pthread_create(&thread, NULL, handle, (void *)i);
    pthread_detach(thread);
}

int main(int argc, char *argv)
{
    pthread_mutex_init(&testlock, NULL);

    create_pthread(10);
    create_pthread(2);
    #if 0
    create_pthread(3);
    create_pthread(11);
    create_pthread(21);
    create_pthread(31);
    #endif

    sleep(10);
    pthread_mutex_destroy(&testlock); 
    return 0;
}

