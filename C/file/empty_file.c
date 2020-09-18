#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>


int _empty_file(const char *file_name)
{

   int ret = truncate(file_name, 0);
   if (ret < 0) {
       printf("fail to empty file\n");
       return ret;
   }

   return ret;
}

int main(int argc, char **argv) {

    if (argc != 2) {
       printf("[usage:]: file_name \n");
       return -1;
    }


    _empty_file(argv[1]);
   return 0;

}
