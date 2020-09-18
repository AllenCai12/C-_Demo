#include <stdio.h>

#ifndef   UINT64_C
#define   UINT64_C(value)__CONCAT(value,ULL)
#endif

#include "avformat.h"
#include "swscale.h"
#include "ffmpegInterface.h"


CFfmpegInterface myffmpegInterface;

typedef unsigned short u_int16_t;
typedef unsigned int u_int32_t;

typedef struct BITMAPFILEHEADER  
{   
    u_int16_t bfType;   
    u_int32_t bfSize;   
    u_int16_t bfReserved1;   
    u_int16_t bfReserved2;   
    u_int32_t bfOffBits;   
}BITMAPFILEHEADER;   

typedef struct BITMAPINFOHEADER  
{   
    u_int32_t biSize;   
    u_int32_t biWidth;   
    u_int32_t biHeight;   
    u_int16_t biPlanes;   
    u_int16_t biBitCount;   
    u_int32_t biCompression;   
    u_int32_t biSizeImage;   
    u_int32_t biXPelsPerMeter;   
    u_int32_t biYPelsPerMeter;   
    u_int32_t biClrUsed;   
    u_int32_t biClrImportant;   
}BITMAPINFODEADER;  

int img_convert(AVPicture *dst, enum AVPixelFormat dst_pix_fmt,
        const AVPicture *src, enum AVPixelFormat src_pix_fmt,
        int src_width, int src_height)
{
    int w;
    int h;
    SwsContext *pSwsCtx;
    w = src_width;
    h = src_height;
    pSwsCtx = sws_getContext(w, h, src_pix_fmt, w, h, dst_pix_fmt,SWS_BICUBIC, NULL, NULL, NULL);
    sws_scale(pSwsCtx, src->data, src->linesize,0, h, dst->data, dst->linesize);

    //这里释放掉pSwsCtx的内存

    return 0;
}
int CreateBmp(const char *filename, uint8_t *pRGBBuffer, int width, int height, int bpp)  
{  
    BITMAPFILEHEADER bmpheader;  
    BITMAPINFOHEADER bmpinfo;  
    FILE *fp = NULL;  

    fp = fopen(filename,"wb");  
    if( fp == NULL )  
    {  
        return -1;  
    }  

    bmpheader.bfType = ('M' <<8)|'B';  
    bmpheader.bfReserved1 = 0;  
    bmpheader.bfReserved2 = 0;  
    bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
    bmpheader.bfSize = bmpheader.bfOffBits + width*height*bpp/8;  

    bmpinfo.biSize = sizeof(BITMAPINFOHEADER);  
    bmpinfo.biWidth = width;  
    bmpinfo.biHeight = 0 - height;  
    bmpinfo.biPlanes = 1;  
    bmpinfo.biBitCount = bpp;  
    bmpinfo.biCompression = 0;  
    bmpinfo.biSizeImage = 0;  
    bmpinfo.biXPelsPerMeter = 100;  
    bmpinfo.biYPelsPerMeter = 100;  
    bmpinfo.biClrUsed = 0;  
    bmpinfo.biClrImportant = 0;  

    fwrite(&bmpheader,sizeof(BITMAPFILEHEADER),1,fp);  
    fwrite(&bmpinfo,sizeof(BITMAPINFOHEADER),1,fp);  
    fwrite(pRGBBuffer,width*height*bpp/8,1,fp);  
    fclose(fp);  
    fp = NULL;  

    return 0;  
}


int DecodeVideo()
{
    //打开视频文件
    AVFormatContext *pFormatCtx=NULL;
    const char *filename = "music.mp4";
    int videoStream=-1;
    int i;
    AVFrame *pFrameRGB=NULL;
    AVFrame *pFrame=NULL;


    uint8_t *buffer=NULL;
    int numBytes;

    AVCodec *pCodec=NULL;
    AVCodecContext *pCodecCtx=NULL;

    int frameFinished;
    AVPacket packet;

    unsigned int bmpi=0;
    char BmpName[40] ={0};
    //注册所有的编解码器

    av_register_all();

    while(1)
    {
        //打开文件
        if(avformat_open_input(&pFormatCtx,filename,NULL,NULL)!=0)
        {//这一步会用有效的信息把 AVFormatContext填满
            printf("####error: av_Open_input_file error \n");
            return 0;
        }
        // Retrieve stream information
        if(av_find_stream_info(pFormatCtx)<0)
        {
            printf("find stream info error\n");
            return -1; // Couldn't find stream information
        }


        videoStream=-1;
        for(i=0; i<pFormatCtx->nb_streams; i++)
        {
            if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)//CODEC_TYPE_VIDEO
            {
                videoStream=i;
                break;
            }
        }
        if(videoStream==-1)
            return -1; // Didn't find a video stream

        //找到解码器
        pCodecCtx=pFormatCtx->streams[videoStream]->codec;
        pCodec=avcodec_find_decoder(pCodecCtx->codec_id);

        if(pCodec==NULL) 
        {
            fprintf(stderr, "Unsupported codec!\n");
            return -1; // Codec not found
        }
        if(pCodec->capabilities&CODEC_CAP_TRUNCATED)
            pCodecCtx->flags|=CODEC_FLAG_TRUNCATED;
        // 打开解码器
        if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
            return -1; // Could not open codec






        // Allocate video frame
        pFrame=avcodec_alloc_frame();
        // Allocate an AVFrame structure
        pFrameRGB=avcodec_alloc_frame();
        if(pFrameRGB==NULL)
            return -1;
        // Determine required buffer size and allocate buffer
        numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height);
        buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

        // Assign appropriate parts of buffer to image planes in pFrameRGB
        // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
        // of AVPicture
        avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,pCodecCtx->width, pCodecCtx->height);

        //  CreateBmp("SaveBmp.bmp", buffer,pCodecCtx->width, pCodecCtx->height, 24);
        i=0;

        while(av_read_frame(pFormatCtx, &packet)>=0) 
        {
            //是视频帧吗
            if(packet.stream_index==videoStream) 
            {
                //ffmpeg中的avcodec_decode_video2()的作用是解码一帧视频数据。输入一个压缩编码的结构体AVPacket，输出一个解码后的结构体AVFrame
                avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,&packet);

                // Did we get a video frame?
                if(frameFinished) 
                {
                    //yuv数据
                    pFrame->data[0] += pFrame->linesize[0] * (pCodecCtx->height - 1);
                    pFrame->linesize[0] *= -1;
                    pFrame->data[1] += pFrame->linesize[1] * (pCodecCtx->height / 2 - 1);
                    pFrame->linesize[1] *= -1;
                    pFrame->data[2] += pFrame->linesize[2] * (pCodecCtx->height / 2 - 1);
                    pFrame->linesize[2] *= -1;

                    //   sws_scale (pSwsCtx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
                    // Convert the image from its native(YUV FMT) format to RGB
                    img_convert((AVPicture *)pFrameRGB, AV_PIX_FMT_BGR24,(AVPicture*)pFrame, pCodecCtx->pix_fmt,pCodecCtx->width, pCodecCtx->height);

                    //这个函数的使用本质上是为已经分配的空间的结构体AVPicture挂上一段用于保存数据的空间，这个结构体中有一个指针数组data[4]，挂在这个数组里
                    //          avpicture_fill((AVPicture *)pFrame, buffer, PIX_FMT_BGR24,pCodecCtx->width, pCodecCtx->height);

                    sprintf(BmpName,"%s_%d.bmp","SaveBmp",bmpi);
                    bmpi++;
                    printf("decode one frame \n");
                    CreateBmp(BmpName, buffer,pCodecCtx->width, pCodecCtx->height, 24);

                    //SaveVideoFile("SaveVideo.rgb24", buffer,pCodecCtx->width, pCodecCtx->height, 24);
                    // Save the frame to disk
                    // if(++i<=5)
                    //  SaveFrame(pFrameRGB, pCodecCtx->width,
                    //           pCodecCtx->height, i);
                }
            }

            // Free the packet that was allocated by av_read_frame
            av_free_packet(&packet);
        }
        avcodec_close(pCodecCtx);
        av_close_input_file(pFormatCtx);
        pCodecCtx = NULL;
        pFormatCtx = NULL;
        printf("the video file was decoded over...\n");
        break;
    }
    return 0;   
}

int main(int argc,char **argv)
{

    DecodeVideo();

    printf("kill");
    while(1);
    AVFormatContext *pFormatCtx=NULL;
    const char *filename = "20101013093728.avi";


    av_register_all();
    printf("run here");
    //打开文件
    int err_code;
    char buf[20];
    if(err_code=avformat_open_input(&pFormatCtx,filename,NULL,NULL))
    {//这一步会用有效的信息把 AVFormatContext填满
        av_strerror(err_code, buf, 1024);
        printf("Couldn't open file %s: %d(%s)", filename, err_code, buf);
        return 0;
    }
    else
        printf("open file success\n");

    return 0;
}
