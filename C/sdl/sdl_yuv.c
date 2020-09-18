#include <stdlib.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc , char* argv[])
{
    int w = 960;
    int h = 576;
    char c = 'n';

    FILE* fp;
    //char filename[64];
    unsigned char* pY;
    unsigned char* pU;
    unsigned char* pV;
    SDL_Rect rect;

    if (argc != 2) {
        printf("Usage: [file_name]\n");
        exit(1);
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "can not initialize SDL:%s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    SDL_Surface* screen = SDL_SetVideoMode(1920, 1080, 0, SDL_SWSURFACE);
    if (screen == NULL){
        fprintf(stderr, "create surface error: %s!\n", SDL_GetError());
        exit(1);
    }

    SDL_Overlay* overlay = SDL_CreateYUVOverlay(w, h, SDL_YV12_OVERLAY, screen);
    if (overlay == NULL){
        fprintf(stderr, "create overlay error!\n");
        exit(1);
    }

    printf("w:%d, h:%d, planes:%d\n", overlay->w, overlay->h, overlay->planes);
    printf("pitches:%d, %d, %d\n", overlay->pitches[0], overlay->pitches[1], overlay->pitches[2]);

    pY = (unsigned char*)malloc(w*h);
    pU = (unsigned char*)malloc(w*h/4);
    pV = (unsigned char*)malloc(w*h/4);

    SDL_LockSurface(screen);
    SDL_LockYUVOverlay(overlay);

    //sprintf(filename, "/home/ly/文档/testSDL/test2/3.yuv");

    fp = fopen(argv[1], "rb");
    if (fp == NULL){
        fprintf(stderr, "open file error!\n");
        exit(1);
    }

    int i;
    int ret;
    for (i=0; i<7; i++ ){
       ret =  fread(pY, 1, w*h, fp);
       if (ret == 0) {
            break;
       }
       ret =  fread(pU, 1, w*h/4, fp);
       if (ret == 0) {
            break;
       }
       ret =  fread(pV, 1, w*h/4, fp);
       if (ret == 0) {
            break;
       }
       sleep(1);
    }

    memcpy(overlay->pixels[0], pY, w*h);
    memcpy(overlay->pixels[1], pV, w*h/4);
    memcpy(overlay->pixels[2], pU, w*h/4);

    fclose(fp);

    SDL_UnlockYUVOverlay(overlay);
    SDL_UnlockSurface(screen);

    rect.w = w;
    rect.h = h;
    rect.x = rect.y = 0;
    SDL_DisplayYUVOverlay(overlay, &rect);

    SDL_Delay(2000);




    free(pY);
    free(pU);
    free(pV);

    getchar();
    getchar();
    getchar();


    SDL_FreeYUVOverlay(overlay);
    SDL_FreeSurface(screen);

    return 0;
}
