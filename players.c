#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
typedef struct player {
    int offX;
    int offY;
    bool d;
    struct player* next;
} player;
player* newPlayer(int offX,int offY,bool d){
    player* n=malloc(sizeof(player));
    if(n!=NULL){
        n->offX=offX;
        n->offY=offY;
        n->d=d;
        n->next=NULL;
    }
    return n;
}
int countPlayer(player* a){
    int count=0;
    while(a!=NULL){
        count++;
        a=a->next;
    }
    return count;
}
void addPlayer(player* a){

    int count=countPlayer(a);
    while(a->next!=NULL){
        a=a->next;
    }
    if(count<=6){
        a->next=newPlayer((int) 25*sin(count*((2*3.1415926)/6)),(int) 25*cos(count*((2*3.1415926)/6)),true);
        return;
    }
    int ring=((count-6)/12)+2;
    if(ring<5){
        int newC=(count-6)-(ring-2)*12;
        a->next=newPlayer((int) ring*25*sin(newC*((2*3.1415926)/12)),(int) ring*25*cos(newC*((2*3.1415926)/12)),true);
        return;
    }
    int ring2=((count-6)/24)+2;
    int newC2=(count-6)-(ring-2)*24;
    a->next=newPlayer((int) ring*18*sin(newC2*((2*3.1415926)/24)),(int) ring*18*cos(newC2*((2*3.1415926)/24)),true);
    return;
}
