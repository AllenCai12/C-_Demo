#include <stdio.h>  
#include <pthread.h>  
#include <semaphore.h>  
#include <unistd.h>

#define MAX 2  //队列长度  

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
sem_t full;     //填充的个数  
sem_t empty;    //空槽的个数  

int top = 0;     //队尾  
int bottom = 0;  //队头  

char test[20];

void* produce(void* arg)  
{  
    int i;  
    int temp;
    for ( i = 0; i < MAX*2; i++)  
    {  
//        printf("producer is preparing data\n");  
        sem_wait(&empty);//若空槽个数低于0阻塞  
        pthread_mutex_lock(&mutex);  
        top++;
        temp = top;
        printf("[produce in] top %d\n", temp);
 //       top = (top+1) % MAX;  
  //      printf("now top is %d\n", top);  

//        pthread_mutex_unlock(&mutex);  
 //       sem_post(&full);  
        printf("[produce out] top %d\n", temp);
    }  
    return (void*)1;  
}  

void* consume(void* arg)  
{  
    int i;  
    int temp;
    for ( i = 0; i < MAX*2; i++)  
    {  
        printf("consumer is preparing data\n");  

        sem_wait(&full);//若填充个数低于0阻塞  
        pthread_mutex_lock(&mutex);  
        top++;
        temp = top;
        sleep(1);
        printf("[consume in] top %d\n", temp);
        pthread_mutex_unlock(&mutex);  
        sem_post(&empty);  
        sem_post(&empty);  
        sem_post(&empty);  
        printf("[consume ouT] top %d\n", temp);
    }  

    return (void*)2;  
}  

int main(int argc, char *argv[])  
{  
    pthread_t thid1;  
    pthread_t thid2;  
//    pthread_t thid3;  
//    pthread_t thid4;  

    int  ret1;  
    int  ret2;  
    int  ret3;  
    int  ret4;  

    sem_init(&full, 0, 0);  
    sem_init(&empty, 0, MAX);  

    pthread_create(&thid1, NULL, produce, NULL);  
    pthread_create(&thid2, NULL, consume, NULL);  
//    pthread_create(&thid3, NULL, produce, NULL);  
//    pthread_create(&thid4, NULL, consume, NULL);  

    pthread_join(thid1, (void**)&ret1);  
    pthread_join(thid2, (void**)&ret2);  
//    pthread_join(thid3, (void**)&ret3);  
//    pthread_join(thid4, (void**)&ret4);  

    return 0;  
}  
