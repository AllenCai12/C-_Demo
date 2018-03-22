#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/statfs.h>
#include <stdlib.h>

void show_free_used_bak(const char *path)
{
    struct statfs disk_info;
    int ret;
    unsigned long long block_size;

    unsigned long long   availabl_size;
    unsigned long long   free_size;

    printf("path = %s\n", path);

    ret = statfs("/media/alan/8C61-A36F", &disk_info);
    if (ret < 0) {
        perror("statefs");
        return;
    }

    block_size = disk_info.f_bsize; 
    availabl_size=disk_info.f_bfree * block_size;
    free_size = disk_info.f_bavail * block_size;

    printf("总大小：%llu\n", disk_info.f_blocks*block_size/1024/1024);
    printf("有效大小：%llu 剩余大小: %llu\n", availabl_size, free_size);
}

int is_free_disk(const char *dev, long unsigned size)
{
    FILE *fp = NULL;
    char cmd[256] = {0};
    char buf[256] = {0};
    char *ret;
    long  unsigned remaind_size = 0; 

    do {

        sprintf(cmd, " df %s | grep %s | awk  \'{print $4}\'", dev, dev);

        fp = popen(cmd, "r");
        if (fp == NULL) {
                perror("execute command");
                break;
        }

        ret = fgets(buf, sizeof(buf), fp);
        if (ret == NULL) {
            break;
        }
        remaind_size = atol(buf);
        printf("available = %lu\n", remaind_size);
        printf("avail = %lu\n", size);

    } while(0);

    if (fp != NULL) {
        fclose(fp);
    }

    if(remaind_size  > size) {
        return 1;
    }
    else {
        return 0;
    }

}

int main(int argc, char **argv)
{
    struct dirent *dir;
    DIR *d;
    char path[256];

    if (argc < 2) {
        printf("Usage: [directory]\n");
        return -1;
    }

   d = opendir(argv[1]);
   if (d == NULL) {
       perror("open dir");
       return 1;
   }

    do {

        dir = readdir(d);
        if (dir == NULL) {
            break;
        }
        
        /*寻找SD卡和U盘 设备符*/
        if (strlen(dir->d_name) == 4\
            && strstr(dir->d_name, "sd") != NULL) {

            printf("file_name = %s\n", dir->d_name);
            sprintf(path, "/dev/%s", dir->d_name);
            /*检查磁盘容量是否满*/
            if (is_free_disk(path, 12)) {
                printf("save.....\n");
            }
            else {
                printf("not save.....\n");
            }
        }

    } while(dir != NULL);

    closedir(d);
    return 0;
}
