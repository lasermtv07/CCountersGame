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
    SDL_Rect plSz={w:40,h:70,x:150,y:460};
    int plC=1;

    SDL_Surface* gateS=IMG_Load("gfx/gates.png");
    SDL_Texture* gateZ=SDL_CreateTextureFromSurface(rend,gateS);
    SDL_FreeSurface(gateS);

    gate* a=newGate(6,true,14,false,100,false);
    addGate(a,2,false,11,false,300,false);

    TTF_Init();
    TTF_Font *font=TTF_OpenFont("comicsans.ttf",15);
    if (!font){
        printf("font load error: %s\n",TTF_GetError());
        return 1;
    }
    SDL_Surface* textS=TTF_RenderText_Solid(font,"uwu",(SDL_Color){255,0,0});
    SDL_Texture* text=SDL_CreateTextureFromSurface(rend,textS);
    while(1){
        int mX;
        SDL_GetMouseState(&mX,NULL);
        SDL_Event event;
        if(mX-2>plSz.x && plSz.x<580-50-150)
            plSz.x+=3;
        else if(mX+2<plSz.x && plSz.x>150)
            plSz.x-=3;
        SDL_Rect coords={x:100,y:100,w:30,h:20};
        SDL_RenderCopy(rend,text,NULL,&coords);
        SDL_RenderCopy(rend,pl,NULL,&plSz);
        SDL_Rect shifted={w:plSz.w,h:plSz.h,x:plSz.x,y:plSz.y};
        int tmp=plC-1;
        for(int j=1;j<3+1;j++){
            for(float i=0;i<2*M_PI;i+=M_PI/(3*j)){
                if(tmp>0){
                    shifted.x=(int)plSz.x+25*j*sin(2*M_PI-i);
                    shifted.y=(int)plSz.y+25*j*cos(2*M_PI-i);
                    SDL_RenderCopy(rend,pl,NULL,&shifted);
                    tmp-=1;
                }
            }
        }
        
        gate* b=a;
        while(b!=NULL){
            char* string1=malloc(1024);
            strncpy(string1,"",1024);
            if(b->leftMul)
                sprintf(string1,"x%d",b->leftMod);
            else
                sprintf(string1,"+%d",b->leftMod);
            SDL_Surface* tmp1S=TTF_RenderText_Solid(font,string1,(SDL_Color){255,255,255});
            SDL_Texture* tmp1=SDL_CreateTextureFromSurface(rend,tmp1S);
            char* string2=malloc(1024);
            strncpy(string2,"",1024);
            if(b->rightMul)
                sprintf(string2,"x%d",b->rightMod);
            else
                sprintf(string2,"+%d",b->rightMod);
            SDL_Surface* tmp2S=TTF_RenderText_Solid(font,string2,(SDL_Color){255,255,255});
            SDL_Texture* tmp2=SDL_CreateTextureFromSurface(rend,tmp2S);
            SDL_RenderCopy(rend,gateZ,NULL,&(SDL_Rect){x:150-40,y:b->y,w:350,h:30});
            SDL_RenderCopy(rend,tmp1,NULL,&(SDL_Rect){x:170,y:b->y+5,h:20,w:10*strlen(string1)});
            SDL_RenderCopy(rend,tmp2,NULL,&(SDL_Rect){x:170+180,y:b->y+5,h:20,w:18*(strlen(string1)-1)});
            free(string1);
            free(string2);
            SDL_FreeSurface(tmp1S);
            SDL_FreeSurface(tmp2S);
            SDL_DestroyTexture(tmp1);
            SDL_DestroyTexture(tmp2);

            b->y+=2;
            if(b->y>460 && !b->touched){
                if(plSz.x<110+175){
                    if(b->leftMul)
                        plC*=b->leftMod;
                    else
                        plC+=b->leftMod;
                }
                else {
                    printf("touched right\n");
                    if(b->rightMul)
                        plC*=b->rightMod;
                    else
                        plC+=b->rightMod;
                }
                b->touched=true;
            }
            b=b->next;
        }
        a=deleteUnderGate(a);
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