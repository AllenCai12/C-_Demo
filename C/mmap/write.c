#include<sys/types.h>  
#include<fcntl.h>  
#include<string.h>  
#include<stdio.h>  
#include <stdlib.h>
#include <sys/mman.h>
#include<unistd.h>  
#include <string.h>


#define BUFF_SIZE   (1*1024*1024)
#define FRAME_SIZE  (4096)

static void *s_buff;
static char *s_file_name = "data";
static unsigned int s_file_length;
static unsigned int s_save_length;
static unsigned int s_count;
static unsigned int s_test;

/* 获取要写入文件的长度 */
unsigned int getFileSize(const char *filename)
{
    unsigned int size = 0;
    FILE *fp = NULL;

    fp=fopen(filename,"r");
    if( NULL == fp)  {
        return size;
    }

    fseek(fp,0,SEEK_END);
    size = (unsigned int)ftell(fp);
    fclose(fp);

    return size;
}


void wav_init_mmap_buff(char *file_name)
{
    unsigned int offset = 0;

    if (s_save_length != 0)  {
        return;
    }

    printf("start init mmap\n");


//    file_length = getFileSize(file_name);

    offset = s_file_length + BUFF_SIZE;
    int fd = open(file_name, O_CREAT | O_RDWR | O_APPEND, 00777);
    if(fd < 0)  {
        printf("OPEN FILE ERROR!\n");
    }
    ftruncate(fd, offset);
    fsync(fd);

    s_buff = mmap(NULL, BUFF_SIZE, PROT_WRITE, MAP_SHARED, fd, s_file_length);
    if (s_buff == MAP_FAILED ) {
        printf("fail to mmap\n");
        perror("reason");
        close(fd);
        exit(1);
    }

    close(fd);

}

//|| s_save_length >= BUFF_SIZE)
int wav_sync()
{
    if (s_save_length >= BUFF_SIZE) {
        printf("sync size: %d\n", s_save_length);
        s_count++;


        printf("start sync\n");
        msync(s_buff, BUFF_SIZE, MS_SYNC);
        munmap(s_buff, BUFF_SIZE);
        s_file_length += BUFF_SIZE;
    }

    if(s_count != s_test) {
        printf("sync size: %d\n", s_save_length);
        printf("count: %d\n", s_count);
        printf("test: %d\n", s_test);
        
        exit(1);
    }
        
}

int wav_write(const char *data, unsigned int length)
{

    if (s_save_length >= BUFF_SIZE) {
        printf("save length :%d\n", s_save_length);
        s_save_length = 0;
    }

    wav_init_mmap_buff(s_file_name);

    if (s_buff == NULL) {
        printf("mmap ptr is null\n");
        exit(-1);
    }

    //    printf("copy data\n");
    /* 清零该段内存，防止乱码 */
    memset(s_buff+s_save_length, 0, length);
    /* 写入数据 */
    memcpy(s_buff+s_save_length, data, length);

    s_save_length += length;
    wav_sync();

    return 0;
}

int main()
{  
   // char *data = "123456789";
#if 1
    char data[4*1024] = {0};

    int i = 0;  
    for (i; i < (4*1024); i++) {
        data[i] = '1';
    }

    //data[i-1] = '\0';
#endif
//    printf("data length : %ld\n", strlen(data));

    i=0;
    while(i < (256*1024))
    {
        if (((i+1)%256) == 0) {
            s_test++;
        }

        wav_write(data, FRAME_SIZE);
        i++;
    }

    printf("count :%u\n", s_count);

    return 0;
}
