#include <stdio.h>
#include <assert.h>
#include <string.h>



int test_file_position(FILE *fp)
{
    
    printf("position:%ld\n", ftell(fp));

    fseek(fp, 0, SEEK_END); 
    printf("position:%ld\n", ftell(fp));

    rewind(fp);
    printf("position:%ld\n", ftell(fp));
}

int save_data_to_file(void *data, int size)
{
    FILE *fp;
    int ret;

    assert(data);
    do {
        fp = fopen("/tmp/led_data.dsp", "r+");
        if (fp == NULL) {
            perror("open file");
            break;
        }

        fseek(fp, 0, SEEK_END);
        fwrite(data, 1, size, fp);
    } while(0);

    if (fp != NULL) {
        fclose(fp);
    }
}

int read_line_by_key_words(FILE *fp, char *key_words)
{
    size_t len = 0;
    char buf[256] = {0};
    char *line = buf;

    while (1) {
//        int ret = fread(buf, 1, sizeof(buf), fp);
//        if (ret < 0 || ret == 0) {
//            break;
//        }

        int ret = getline(&line, &len, fp);
        if (ret < 0 || ret == 0) {
            break;
        }

        char *tmp = strstr(line, key_words);
        if (tmp == NULL) {
            memset(buf, 0, sizeof(buf));
            continue;
        }

        printf("%s\n", line);
        memset(buf, 0, sizeof(buf));
        break;
    }

}



int main(int argc, char **argv)
{

    //save_data_to_file(argv[1], strlen(argv[1]));

    FILE *fp_a = fopen(argv[1], "r");
    read_line_by_key_words(fp_a, argv[2]);
    
    fclose(fp_a);
    

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


    return 0;

}
