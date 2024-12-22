#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#define M_PI 3.14159265358979323846
#include <math.h>
#include "gate.c"

int main(){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)){
        printf("error\n");
        return 1;
    }
    SDL_Window* win=SDL_CreateWindow("game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,580,640,0);
    if(!win){
        printf("window error\n");
        return 1;
    }
    SDL_Renderer* rend=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderClear(rend);
    if(!rend){
        printf("error creating renderer\n");
        return 1;
    }
    SDL_Surface* plS=IMG_Load("gfx/player.png");
    SDL_Texture* pl=SDL_CreateTextureFromSurface(rend,plS);
    SDL_FreeSurface(plS);
    SDL_Rect plSz={w:50,h:80,x:50,y:460};
    int plC=16;
    TTF_Init();
    TTF_Font *font=TTF_OpenFont("comicsans.ttf",40);
    if (!font){
        printf("font load error: %s\n",TTF_GetError());
        return 1;
    }
    SDL_Surface* textS=TTF_RenderText_Solid(font,"uwu",(SDL_Color){255,0,0});
    SDL_Texture* text=SDL_CreateTextureFromSurface(rend,textS);
    while(1){
        int mX;
        SDL_GetMouseState(&mX,NULL);
        printf("%d\n",mX);
        SDL_Event event;
        if(mX-2>plSz.x && plSz.x<580-50-150)
            plSz.x+=3;
        else if(mX+2<plSz.x && plSz.x>150)
            plSz.x-=3;
        SDL_Rect coords={x:100,y:100,w:100,h:100};
        SDL_RenderCopy(rend,text,NULL,&coords);
        SDL_RenderCopy(rend,pl,NULL,&plSz);
        SDL_Rect shifted={w:50,h:80,x:plSz.x,y:plSz.y};
        int tmp=plC-1;
        for(int j=1;j<3;j++){
            for(float i=0;i<2*M_PI;i+=M_PI/3*j){
                if(tmp>0){
                    shifted.x=(int)plSz.x+30*j*sin(i);
                    shifted.y=(int)plSz.y+30*j*cos(i);
                    SDL_RenderCopy(rend,pl,NULL,&shifted);
                    tmp-=1;
                }
            }
        }

        SDL_RenderPresent(rend);

        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                SDL_DestroyWindow(win);
                SDL_Quit();
                return 0;
            }
        }
        SDL_RenderClear(rend);
        SDL_Delay(1000/60);
    }

}