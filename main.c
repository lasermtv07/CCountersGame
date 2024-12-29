#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define M_PI 3.14159265358979323846
#include <math.h>
#include "gate.c"
#include "obstacle.c"
#include "players.c"

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

    SDL_Surface* obstacleS=IMG_Load("gfx/obstacle.png");
    SDL_Texture* obstacleZ=SDL_CreateTextureFromSurface(rend,obstacleS);

    gate* a=newGate(6,true,14,false,100,false);
    addGate(a,2,false,11,false,300,false);

    obstacle* o=newObstacle(true,100,-50);
    //addObstacle(o,true,300,-150);

    player* z=newPlayer(0,0,true);
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
        else if(mX+2<plSz.x && plSz.x>180)
            plSz.x-=3;
        SDL_Rect coords={x:100,y:100,w:30,h:20};
        SDL_RenderCopy(rend,text,NULL,&coords);
        //SDL_RenderCopy(rend,pl,NULL,&plSz);
        SDL_Rect shifted={w:plSz.w,h:plSz.h,x:plSz.x,y:plSz.y};
        int tmp=plC-1;
        player* zz=z;
        while(zz!=NULL){
            if(zz->d)
                SDL_RenderCopy(rend,pl,NULL,&(SDL_Rect){w:plSz.w,h:plSz.h,x:plSz.x+zz->offX,y:plSz.y+zz->offY});
            int pX=plSz.x+zz->offX;
            int pY=plSz.y+zz->offY;
            obstacle* qq=o;
            while(qq!=NULL){
                int oX=qq->x;
                int oY=qq->y;
                if(!qq->type || qq->deg==0 || qq->deg==270){
                    if(pX+plSz.w>oX && pX<oX+150){
                        if(pY+plSz.h>oY && pY<oY+50)
                            zz->d=false;
                    }
                }
                else {
                    float deg=((qq->deg)/180)*M_PI;
                    float orX=oX+75-75*cos(2*M_PI-deg);
                    float orY=oY+75*sin(2*M_PI-deg);
                    float farX=oX+75+75*cos(2*M_PI-deg);
                    float farY=oY-75*sin(2*M_PI-deg);
                    float bottomY=oY-75*sin(deg)+50*cos(deg);
                    float bottomY2=oY+75*sin(deg)+50*cos(deg);
                    float bottomX=oX+75-75*cos(deg)-50*sin(deg);
                    float bottomX2=oX+75+75*cos(deg)-50*sin(deg);

                    SDL_Rect pRect={x:pX,y:pY,w:plSz.w,h:plSz.h};
                    for(int i=0;i<30;i++){
                        SDL_Rect* l1=malloc(sizeof(SDL_Rect));
                        l1->w=2;
                        l1->h=2;
                        l1->x=orX+(150-i*5)*cos(deg);
                        l1->y=(int) orY+(150-i*5)*sin(deg);
                        SDL_SetRenderDrawColor(rend,0,255,0,255);
                        SDL_RenderDrawRect(rend,l1);
                        if(SDL_HasIntersection(l1,&pRect))
                            zz->d=false;
                        free(l1);
                    }
                    for(int i=0;i<30;i++){
                        SDL_Rect* l1=malloc(sizeof(SDL_Rect));
                        l1->w=2;
                        l1->h=2;
                        l1->x=orX+(150-i*5)*cos(deg)-50*sin(deg);
                        l1->y=(int) orY+(150-i*5)*sin(deg)+50*cos(deg);
                        SDL_SetRenderDrawColor(rend,0,255,0,255);
                        SDL_RenderDrawRect(rend,l1);
                        if(SDL_HasIntersection(l1,&pRect))
                            zz->d=false;
                    }

                    SDL_SetRenderDrawColor(rend,0,0,0,255);

                }
                qq=qq->next;
            }
            zz=zz->next;
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
                    if(b->leftMul){
                        int oTmp=countPlayer(z);
                        for(int i=0;i<b->leftMod-1;i++){
                            for(int j=0;j<oTmp;j++)
                                addPlayer(z);
                        }
                        printf("%d\n",countPlayer(z));
                    }
                    else {
                        for(int i=0;i<b->leftMod;i++)
                            addPlayer(z);
                    }
                }
                else {
                    printf("touched right\n");
                    if(b->rightMul){
                        int oTmp=countPlayer(z);
                        for(int i=0;i<b->rightMod-1;i++){
                            for(int j=0;j<oTmp;j++)
                                addPlayer(z);
                        }
                    }
                    else{
                        for(int i=0;i<b->rightMod;i++)
                        addPlayer(z);
                    }
                }
                b->touched=true;
            }
            b=b->next;
        }
        obstacle* p=o;
        while(p!=NULL){
            if(!p->type){
                SDL_RenderCopy(rend,obstacleZ,NULL,&(SDL_Rect){x:p->x,y:p->y,w:150,h:50});
            }
            else {
                SDL_RenderCopyEx(rend,obstacleZ,NULL,&(SDL_Rect){x:p->x,y:p->y,w:150,h:50},p->deg,&(SDL_Point){x:75,y:0},false);
                p->deg+=1;
                p->deg=(int)p->deg%360;

            }
            p->y+=2;
            p=p->next;
        }
        a=deleteUnderGate(a);
        o=deleteUnderObstacle(o);
        deletePlayer(z);
        SDL_RenderPresent(rend);
        if(z->d==false && z->next==NULL)
            printf("PLAYER LOST\n");

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