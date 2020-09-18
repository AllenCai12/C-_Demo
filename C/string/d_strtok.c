#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{

    //char test[]="a b c d  hello \n work ad \n ad dd";
    char test[]="a b c d  hell";
    printf("test: %s \n", test);

    char *str;
    char *sub_token;
    char *token = strtok(test, "\n");
    while (token != NULL) {
       printf("token: %s\n", token);
       printf("====\n");
       token = strtok(NULL, "\n");
    }

    return 0;
}
