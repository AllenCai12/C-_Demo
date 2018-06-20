#include "SDL/SDL.h"
#include"SDL/SDL_ttf.h"
int main(int argc,char**argv)
{
    TTF_Font *font;
    SDL_Surface *text, *temp;
    if ( TTF_Init() < 0 ){
        fprintf(stderr, "Couldn't initialize TTF:%s\n",SDL_GetError());
        SDL_Quit();
        return(2);
    }
    font = TTF_OpenFont("zenhei.ttc", 48);
    if ( font == NULL ){
        fprintf(stderr, "Couldn't load %d pt font from %s:%s\n",
                "ptsize", 18,SDL_GetError());
    }

    SDL_Color forecol= { 0x00, 0x00, 0x00, 0 };
    char*string="你好啊";
    text = TTF_RenderUTF8_Solid(font, string,forecol);
    //SDL_LoadBMP
    SDL_SaveBMP(text, "1.bmp");
    SDL_FreeSurface(text);
    TTF_CloseFont(font);
    TTF_Quit();
}
