#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>



int test_file_position(FILE *fp)
{
    
    printf("position:%ld\n", ftell(fp));

    fseek(fp, 0, SEEK_END); 
    printf("position:%ld\n", ftell(fp));

    rewind(fp);
    printf("position:%ld\n", ftell(fp));
}

int main(int argc, char **argv)
{
    FILE *fp, *fp_a;
    int ret;
    char buf[256];

    assert(argv[1]);
    fp = fopen(argv[1], "w");

    fseek(fp, 0, SEEK_END);
    fwrite("hello world", 1, sizeof("hello world"), fp);

#if 0
    fp_a = fopen("/tmp/test.wav", "w+");
    printf("cur=%ld\n", ftell(fp));
    fseek(fp, sizeof(struct wave), SEEK_CUR);
    printf("cur1=%ld\n", ftell(fp));

    while (1) {
        ret = fread(buf, 1, sizeof(buf), fp);
        printf("ret = %d\n", ret);
        if (ret < 0 || ret == 0) {
            break;
        }

        fwrite(buf, 1, sizeof(buf), fp_a);
      //  sleep(1);
    }

    fclose(fp_a);
 #endif
//    test_file_position(fp);
    
    //test_file_position(fp);

    //if (check_if_wav(fp)) {
    //    printf("yes ,it is wav file\n"); 
    //}

#if  0  
    fwrite("hello world", sizeof(char), 12, fp);
    test_file_position(fp);

    rewind(fp);

    fwrite("OK", sizeof(char), 3, fp);
 #endif



    fclose(fp);

    return 0;

}
