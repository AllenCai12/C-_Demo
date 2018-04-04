/*****************************************************************
  这个程序测试,多线程的文件操作
 *****************************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#define FILE_PATH   "./test_file"

struct file{
    char  file_name[256];
    char *data;
    int   size;
};


int save_data_to_file(char *file_name, void *data, int size)
{
    FILE *fp;
    int ret;

    assert(data);
    do {
        fp = fopen(file_name, "r+");
        if (fp == NULL) {
            fp = fopen(file_name, "w+");
            if (fp == NULL) {
                perror("open file");
                break;
            }
        }

        fseek(fp, 0, SEEK_END);
        fwrite(data, 1, size, fp);
        sleep(1);

    } while(0);

    if (fp != NULL) {
        fclose(fp);
    }
}


void print_message(void *ptr)
{
    struct file *file;
    file = (struct file *)ptr; 
    save_data_to_file(file->file_name, file->data, file->size);

    pthread_exit((void *)0);
}

//测试多线程,循环传递变量问题
int thread_test(pthread_t * pthread, struct file  *file)
{

    int ret = 0;

    ret = pthread_create(pthread, NULL, (void *)&print_message, (void*)file);
    if (ret != 0) {
        printf("thread %d\n", (int)*pthread);
        perror("pthread create");
    }

    return ret;
}

int main(int argc, char **argv)
{
    char *msg_a = "hello world";
    char *msg_b = "I am comming";
    char buf[256];

    void *reatval;
    int ret;
    struct file file[16];
    int i, j;
    pthread_t pthreads[16];

    do {
        for(j=0; j < 10; j++) {
            memset(buf, 0, sizeof(buf));
            sprintf(buf , "hello come on I am comming %d\n", j);

            for (i = 0; i < 16; i++) {

                /*这里就是用数组,和单个变量的区别*/
#if 1
                sprintf(file[i].file_name, "%s/test%d.txt", FILE_PATH, i);
                file[i].data = buf;
                file[i].size = strlen(buf);

                thread_test(&pthreads[i], &file[i]); 
#else 
                int t = i;
                ret = thread_test(&pthreads[i], &t); 
                if (ret != 0) {
                    break;
                }
#endif
            }
            for (i=0; i < 16; i++) {
                ret = pthread_join(pthreads[i], &reatval);
                printf("pthrad a is %d\n", ret);
                printf("pthrad a value is %d\n", (int)reatval);
                if (ret != 0) {
                    perror("pthread join");
                    return -1;
                }

            }
            printf("==========j:%d============\n", j);
        } 

    }while (0);

}
