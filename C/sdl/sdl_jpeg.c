#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

int main(int argc, char **argv){

    if (argc != 2) {
        printf("Usage: [file_name]\n");
        exit(1);
    }

    char file[256] = {0};
    strcpy(file, argv[1]);

    //char *file = "/home/edemon/Pictures/sun.jpg";
    SDL_Surface *screen;
    SDL_Surface *picture;
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr,"SDL_INIT_VIDEO error %s\n",SDL_GetError());
        exit(1);
    }
    //extern DECLSPEC SDL_RWops * SDLCALL SDL_RWFromFile(const char *file, const char *mode);
    SDL_RWops *rwops = SDL_RWFromFile(file,"rb");
    //extern DECLSPEC SDL_Surface * SDLCALL IMG_LoadJPG_RW(SDL_RWops *src);
    if(rwops == NULL){
        fprintf(stderr," SDL_RWFromFile, error is %s\n",SDL_GetError());
        exit(1);
    }
    picture = IMG_LoadJPG_RW(rwops);
   // picture =  IMG_Load(file);
    if(picture == NULL){
        fprintf(stderr,"IMG_Load error %s\n",SDL_GetError());
        exit(1);
    }

    screen = SDL_SetVideoMode(1920,1080, 0,SDL_SWSURFACE|SDL_FULLSCREEN);
    if(screen == NULL){
        fprintf(stderr,"couldn't set 640*480*16 bits color model, error is %s\n",SDL_GetError());
        exit(1);
    }
    /* int SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect); */
    if(SDL_BlitSurface(picture,NULL,screen,NULL) < 0){
        fprintf(stderr,"SDL_BlitSurface error %s\n",SDL_GetError());
        exit(1);
    }
    SDL_UpdateRect(screen, 0, 0, 0, 0);
    atexit(SDL_Quit);
    //extern DECLSPEC void SDLCALL SDL_FreeRW(SDL_RWops *area);
    SDL_FreeRW(rwops);
    SDL_FreeSurface(picture);
    SDL_Delay(5000);
    SDL_FreeSurface(screen);

    return 0;

}
