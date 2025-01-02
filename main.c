#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define M_PI 3.14159265358979323846
#include <math.h>
#include "gate.c"
#include "obstacle.c"
#include "players.c"

int main(){
    if(SDL_Init(SDL_INIT_VIDEO| SDL_INIT_TIMER | SDL_INIT_AUDIO)){
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
    SDL_Rect plSz={w:60,h:90,x:150,y:460};
    int plC=1;

    SDL_Surface* gateS=IMG_Load("gfx/gates.png");
    SDL_Texture* gateZ=SDL_CreateTextureFromSurface(rend,gateS);
    SDL_FreeSurface(gateS);

    SDL_Surface* obstacleS=IMG_Load("gfx/obstacle.png");
    SDL_Texture* obstacleZ=SDL_CreateTextureFromSurface(rend,obstacleS);

    gate* a=newGate(6,true,14,false,100,false);
    addGate(a,2,false,11,false,300,false);

    obstacle* o=newObstacle(false,100,-50);
    addObstacle(o,true,400,-150);

    player* z=newPlayer(0,0,true);
    TTF_Init();
    TTF_Font *font=TTF_OpenFont("comicsans.ttf",15);
    TTF_Font *font2=TTF_OpenFont("comicsans.ttf",48);
    if (!font){
        printf("font load error: %s\n",TTF_GetError());
        return 1;
    }
    SDL_Surface* textS=TTF_RenderText_Solid(font,"uwu",(SDL_Color){255,0,0});
    SDL_Texture* text=SDL_CreateTextureFromSurface(rend,textS);

    SDL_Surface* dedS=TTF_RenderText_Solid(font2,"u ded",(SDL_Color){255,0,0});
    SDL_Texture* ded=SDL_CreateTextureFromSurface(rend,dedS);

    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
    printf("%d\n",Mix_Volume(-1,2));
    Mix_Music* bg=Mix_LoadMUS("sfx/carefree.mp3");
    Mix_Chunk* hit=Mix_LoadWAV("sfx/hit.wav");
    Mix_Chunk* pass=Mix_LoadWAV("sfx/pass.wav");
    Mix_PlayMusic(bg,-1);

    int screenShift=0;
    int level=1;
    int progState=1;
    while(1){

        for(int i=0;i<9;i++){
            for(int j=0;j<6;j++){
                SDL_Rect sqer={w:116,h:116,x:116*j,y:640-116*i+screenShift};
                if(i%2==0){
                    if(j%2==0)
                        SDL_SetRenderDrawColor(rend,140,207,0,255);
                    else
                        SDL_SetRenderDrawColor(rend,92,189,0,255);
                } else {
                     if(j%2==1)
                        SDL_SetRenderDrawColor(rend,140,207,0,255);
                    else
                        SDL_SetRenderDrawColor(rend,92,189,0,255);                   
                }
                SDL_RenderFillRect(rend,&sqer);
            }
        }
        SDL_SetRenderDrawColor(rend,0,0,0,255);
        int mX;
        SDL_GetMouseState(&mX,NULL);
        SDL_Event event;
        if(mX-2>plSz.x && plSz.x<580-mostrightXPlayer(z))
            plSz.x+=3;
        else if(mX+2<plSz.x && plSz.x>mostleftXPlayer(z))
            plSz.x-=3;
        SDL_Rect coords={x:100,y:100,w:30,h:20};
        SDL_RenderCopy(rend,text,NULL,&coords);
        //SDL_RenderCopy(rend,pl,NULL,&plSz);
        SDL_Rect shifted={w:plSz.w,h:plSz.h,x:plSz.x,y:plSz.y};
        int tmp=plC-1;
        player* zz=z;
        sortPlayer(zz);

        player* firstPlayer=zz;
        while(zz!=NULL && progState==1){
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
                        if(pY+plSz.h>oY && pY<oY+50){
                            zz->d=false;
                            Mix_PlayChannel(-1,hit,0);
                        }
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
                    for(int i=0;i<30 && qq->y>0;i++){
                        SDL_Rect* l1=malloc(sizeof(SDL_Rect));
                        l1->w=2;
                        l1->h=2;
                        l1->x=orX+(150-i*5)*cos(deg);
                        l1->y=(int) orY+(150-i*5)*sin(deg);
                        //SDL_SetRenderDrawColor(rend,0,255,0,255);
                        //SDL_RenderDrawRect(rend,l1);
                        if(SDL_HasIntersection(l1,&pRect)){
                            zz->d=false;
                            Mix_PlayChannel(-1,hit,0);
                        }
                        free(l1);
                    }
                    for(int i=0;i<30 && qq->y>0;i++){
                        SDL_Rect* l1=malloc(sizeof(SDL_Rect));
                        l1->w=2;
                        l1->h=2;
                        l1->x=orX+(150-i*5)*cos(deg)-50*sin(deg);
                        l1->y=(int) orY+(150-i*5)*sin(deg)+50*cos(deg);
                        //SDL_SetRenderDrawColor(rend,0,255,0,255);
                        //SDL_RenderDrawRect(rend,l1);
                        if(SDL_HasIntersection(l1,&pRect))
                            zz->d=false;
                        free(l1);
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
            SDL_Surface* tmp1S=TTF_RenderText_Solid(font,string1,(SDL_Color){0,0,0});
            SDL_Texture* tmp1=SDL_CreateTextureFromSurface(rend,tmp1S);
            char* string2=malloc(1024);
            strncpy(string2,"",1024);
            if(b->rightMul)
                sprintf(string2,"x%d",b->rightMod);
            else
                sprintf(string2,"+%d",b->rightMod);
            SDL_Surface* tmp2S=TTF_RenderText_Solid(font,string2,(SDL_Color){0,0,0});
            SDL_Texture* tmp2=SDL_CreateTextureFromSurface(rend,tmp2S);
            SDL_RenderCopy(rend,gateZ,NULL,&(SDL_Rect){x:2,y:b->y,w:578,h:72});
            SDL_RenderCopy(rend,tmp1,NULL,&(SDL_Rect){x:140,y:b->y+16,h:30,w:15*strlen(string1)});
            SDL_RenderCopy(rend,tmp2,NULL,&(SDL_Rect){x:140+280,y:b->y+16,h:30,w:15*(strlen(string1))});
            free(string1);
            free(string2);
            SDL_FreeSurface(tmp1S);
            SDL_FreeSurface(tmp2S);
            SDL_DestroyTexture(tmp1);
            SDL_DestroyTexture(tmp2);
            b->y+=2;
            if(b->y>plSz.y+topYPlayer(z) && !b->touched){
                Mix_PlayChannel(-1,pass,0);
                redoPlayer(z);
                if(plSz.x+firstPlayer->offX<270){
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
        printf("%d\n",countPlayer(z));

        if(progState==1){
            SDL_SetRenderDrawColor(rend,0,0,0,180);
            SDL_SetRenderDrawBlendMode(rend,SDL_BLENDMODE_BLEND);
            SDL_RenderFillRect(rend,&(SDL_Rect){w:700,h:80,x:0,y:0});
            char* scoreText=malloc(1024);
            strncpy(scoreText,"",1024);
            sprintf(scoreText,"Level: %d",level);
            SDL_Surface* scoreS=TTF_RenderText_Solid(font,scoreText,(SDL_Color){255,255,255});
            SDL_Texture* scoreT=SDL_CreateTextureFromSurface(rend,scoreS);
            SDL_RenderCopy(rend,scoreT,NULL,&(SDL_Rect){x:20,y:20,h:32,w:15*strlen(scoreText)});
            free(scoreText);
            SDL_FreeSurface(scoreS);
            SDL_DestroyTexture(scoreT);

            char* plCText=malloc(1024);
            strncpy(plCText,"",1024);
            sprintf(plCText,"Players: %d",countPlayer(z));
            SDL_Surface* plCS=TTF_RenderText_Solid(font,plCText,(SDL_Color){255,255,255});
            SDL_Texture* plCT=SDL_CreateTextureFromSurface(rend,plCS);
            SDL_RenderCopy(rend,plCT,NULL,&(SDL_Rect){x:400,y:20,h:32,w:15*strlen(plCText)});
            free(plCText);
            SDL_FreeSurface(plCS);
            SDL_DestroyTexture(plCT);
        }

//TODO finish
        if(a==NULL && o==NULL){
            level++;
            int gateCount=((rand()%2)+1)*level;
            if(gateCount>100)
                gateCount=(int) 100+((gateCount-100)*0.5);
            int yS=-20;
            for(;gateCount>0;gateCount--){
                bool leftMul=rand()%2;
                bool rightMul=rand()%2;
                int leftMod;
                int rightMod;
                if(leftMul)
                    leftMod=(rand()%4)+1;
                else
                    leftMod=(rand()%14)+6;
                if(rightMul)
                    rightMod=(rand()%4)+1;
                else
                    rightMod=(rand()%14)+6;
                if(a==NULL)
                    a=newGate(leftMod,leftMul,rightMod,rightMul,yS,false);
                else
                    addGate(a,leftMod,leftMul,rightMod,rightMul,yS,false);

                int preset=rand()%6;
                if(preset==0){
                    if(o==NULL)
                        o=newObstacle(false,((rand()%400)),yS-100);
                    else
                        addObstacle(o,false,((rand()%400)),yS-100);
                }
                if(preset==1){
                    if(o==NULL)
                        o=newObstacle(true,(rand()%400),yS-100);
                    else
                        addObstacle(o,true,(rand()%400),yS-100);
                }
                if(preset==2){
                    if(o==NULL)
                        o=newObstacle(true,(rand()%110),yS-100);
                    else
                        addObstacle(o,true,rand()%110,yS-100);

                    addObstacle(o,false,290+rand()%110,yS-100);
                }
                if(preset==3){
                    if(o==NULL)
                        o=newObstacle(false,(rand()%110),yS-100);
                    else
                        addObstacle(o,false,rand()%110,yS-100);

                    addObstacle(o,true,290+rand()%110,yS-100);
                }
                if(preset==4){
                    if(o==NULL)
                        o=newObstacle(true,(rand()%110),yS-100);
                    else
                        addObstacle(o,true,rand()%110,yS-100);

                    addObstacle(o,true,290+rand()%110,yS-100);
                }
                if(preset==5){
                    if(o==NULL)
                        o=newObstacle(false,(rand()%110),yS-100);
                    else
                        addObstacle(o,false,rand()%110,yS-100);

                    addObstacle(o,false,290+rand()%110,yS-100);
                }
                yS-=225+(rand()%150);
            }
        }
        if(progState==2)
            SDL_RenderCopy(rend,ded,NULL,&(SDL_Rect){x:180,y:120,h:150,w:250});
        SDL_RenderPresent(rend);
        if(z->d==false && z->next==NULL)
            progState=2;

        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                SDL_DestroyWindow(win);
                Mix_CloseAudio();
                SDL_Quit();
                return 0;
            }
            if(event.type==SDL_MOUSEBUTTONDOWN && progState!=1){
                while(a!=NULL){
                    gate* aaqa=a->next;
                    free(a);
                    a=aaqa;
                }
                while(o!=NULL){
                    obstacle* on=o->next;
                    free(o);
                    o=on;
                }
                progState=1;
                level=1;
                player* zt=z->next;
                while(zt!=NULL){
                    player* ztt=zt->next;
                    free(zt);
                    zt=ztt;
                }
                z->d=true;
                a=newGate(6,true,14,false,100,false);
                addGate(a,2,false,11,false,300,false);
                o=newObstacle(false,100,-50);
                addObstacle(o,true,400,-150);
            }
        }
        screenShift+=2;
        screenShift=screenShift%(116*2);
        SDL_RenderClear(rend);
        SDL_Delay(1000/60);
    }

}