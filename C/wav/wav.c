#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>


struct wave_header{
    char         fccID[4];       //内容为""RIFF
    unsigned int dwSize;   //最后填写，WAVE格式音频的大小
    char         fccType[4];     //内容为"wave"
};

struct wave_fmt{
    char            fccID[4];          //内容为"fmt "
    unsigned int    dwSize;     //内容为WAVE_FMT占的字节数，为16 不包括fccID和dwSize
    unsigned short  wFormatTag; //如果为PCM，改值为 1
    unsigned short  wChannels;  //通道数，单通道=1，双通道=2
    unsigned int    dwSamplesPerSec;//采用频率
    unsigned int    dwAvgBytesPerSec;/* ==dwSamplesPerSec*wChannels*uiBitsPerSample/8 */
    unsigned short  wBlockAlign;//==wChannels*uiBitsPerSample/8
    unsigned short  uiBitsPerSample;//每个采样点的bit数，8bits=8, 16bits=16
};

struct wave_data{
    char         fccID[4];       //内容为"data"
    unsigned int dwSize;   //==NumSamples*wChannels*uiBitsPerSample/8
};

struct wave {

    struct wave_header  header;
    struct wave_fmt     fmt;
    struct wave_data    data;
};

int show_wav_value(FILE *fp)
{
    struct wave head;
    int ret;
    int status = 0;
    int i;

    do {
        ret = fread(&head, sizeof(head), 1, fp);
        if (ret <= 0) {
            printf("ret = %d this is not wav file\n", ret);
            perror("read");
            status = 0;
            break;
        }

        printf("header fccID: ");
        for (i=0; i < 4; i++) {
            printf("%c",  head.header.fccID[i]);
        }
        printf("\n");

        printf("header size: %d\n", head.header.dwSize);
        printf("header fcctype: ");
        for (i=0; i < 4; i++) {
            printf("%c",  head.header.fccType[i]);
        }
        printf("\n");



        printf("fmt fccID ");
        for (i=0; i < 4; i++) {
            printf("%c",  head.fmt.fccID[i]);
        }
        
        printf("\n");

        printf("channel: %d\n", head.fmt.wChannels);
        printf("sample : %d\n", head.fmt.dwSamplesPerSec);
        printf("bit: %d\n", head.fmt.uiBitsPerSample);
        printf("format: %d\n", head.fmt.wFormatTag);
        printf("dwAvgBytesPerSec: %d\n", head.fmt.dwAvgBytesPerSec);


        printf("fccID: ");
        for (i=0; i < 4; i++) {
            printf("%c",  head.data.fccID[i]);
        }
        printf("\n");

        printf("data size: %d\n", head.data.dwSize);


        //         printf("fccID: %c\n", head.data.fccID);
    } while(0);

    return status;
}

int get_source_code(FILE *fp)
{
    char *name="test.source";
    FILE *src_fp;
    char buf[256];
    int ret;
    rewind(fp);
    fseek(fp, sizeof(struct wave), SEEK_SET);

    src_fp = fopen(name, "wb+");
    if (src_fp == NULL) {
        perror("open source file");
        return -1;
    }

    while(1) {
        ret = fread(buf, sizeof(buf), 1, fp);
        if (ret == 0) {
            break;
        }

        fwrite(buf, sizeof(buf), 1, src_fp);
    }
    fclose(src_fp);
}

int main(int argc, char **argv)
{
    FILE *fp = NULL; 
    int ret;
    char buf[256];
    char c;

    //   assert(argv[1]);
    //   fp = fopen(argv[1], "r");

    while(( c = getopt(argc, argv, "sof:" )) != -1 )
        switch ( c )
        {
            case 'f':
                printf("open file: %s\n", optarg); 
                fp = fopen(optarg, "r");
                break;
            case 's':
                if (fp == NULL){
                    printf("Usage: -f [filename] -s");
                    break;
                }
                printf("show wav file format\n");
                show_wav_value(fp);
                break;

            case 'o':
                if (fp == NULL){
                    printf("Usage: -f [filename] -o");
                    break;
                }
                printf("out put the audio source file frome wav file\n");
                get_source_code(fp);
                break;
            case '?':
                fprintf( stderr, "Unrecognized option!\n" );
                break;
        }



    if (fp) {
        fclose(fp);
    }

    return 0;
}
