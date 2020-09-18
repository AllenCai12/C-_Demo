#define _GNU_SOURCE     

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define FALLOC_FL_COLLAPSE_RANGE        0x08

int main(int argc, const char * argv[])
{
    int ret;
    printf("start test....\n");
    char * page = malloc(4096);
    int fd = open("test.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);

    if (fd == -1) {
        free(page);
        return (-1);
    }

    // Page A
    printf("Write page A\n");
    memset(page, 'A', 4096);
    write(fd, page, 4096);

    // Page B
    printf("Write page B\n");
    memset(page, 'B', 4096);
    write(fd, page, 4096);

    // Remove page A
    ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE, 0, 4096);
    if (ret < 0) {
        printf("Page A should be removed, ret = %d for %s\n", ret, strerror(errno));
    }

    close(fd);
    free(page);

    return (0);
}
