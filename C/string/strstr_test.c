#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char t[] = "hello world";

    printf("strstr:   %s\n", strstr(t, "wo"));

    return 0;
}
