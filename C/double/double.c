#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int linenum ;

void double_show(char *name, FILE *fp);
int getWord(FILE *fp, char *buf, int size);


int main(int argc, char **argv)
{
    printf("hello world\n");
    int i;
    FILE *fp;
    for (i=1; i < argc; i++) {
        fp = fopen(argv[i], "r"); 
        if (fp == NULL) {
            printf("fail to open %s \n", argv[i]);
            perror("open");
        }
        else {
            printf("success to open file %s\n", argv[i]);
            double_show(argv[i], fp);
            fclose(fp);
        }
    }

    if (argc == 1) {
        double_show(NULL, stdin);
    }

    return 0;
}

void double_show(char *name, FILE *fp)
{

    char pre[128] = {0};
    char word[128] = {0};
    int ret = -1;
    linenum = 1;
    
    do {
        ret = getWord(fp, word, sizeof(word));
        if (isalpha(word[0]) && strcmp(word, pre) == 0) {
            if (name) {
                printf("%s:", name);
            }
            printf("%d:%s\n", linenum, word);
        }

        strcpy(pre, word);
    } while(ret != 0);
}

int getWord(FILE *fp, char *buf, int size)
{
    char c;
    do {
        c = fgetc(fp);   
        if (c == '\n') {
            linenum++;
        }
    } while(isspace(c) && c != EOF);

    
    int i=0;
    for (; c!=EOF && !isspace(c); c=getc(fp)) {
        if (i < size-1) {
            buf[i++]=tolower(c);
        }
    } 

    if (i < size) {
        buf[i] = '\0';
    }

    if (c != EOF) {
        ungetc(c, fp);
        return 1;
    }

    return  0;
}
