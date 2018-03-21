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

    do {
         ret = fread(&head, sizeof(head), 1, fp);
         if (ret <= 0) {
                 printf("ret = %d this is not wav file\n", ret);
                 perror("read");
                 status = 0;
                 break;
         }

         printf("fcctype: ");
         for (int i=0; i < 4; i++) {
            printf("%c",  head.header.fccType[i]);
         }
         printf("\n");

         printf("head size: %d\n", head.header.dwSize);

         printf("channel: %d\n", head.fmt.wChannels);
         printf("sample : %d\n", head.fmt.dwSamplesPerSec);
         printf("bit: %d\n", head.fmt.uiBitsPerSample);
         printf("format: %d\n", head.fmt.wFormatTag);


         printf("fccID: ");
         for (int i=0; i < 4; i++) {
            printf("%c",  head.data.fccID[i]);
         }
         printf("\n");

         printf("data size: %d\n", head.data.dwSize);


//         printf("fccID: %c\n", head.data.fccID);
    } while(0);

    return status;
}

int main(int argc, char **argv)
{
    FILE *fp, *fp_a;
    int ret;
    char buf[256];

    assert(argv[1]);
    fp = fopen(argv[1], "r");
    
    show_wav_value(fp);


    fclose(fp);

    return 0;

}
