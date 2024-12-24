#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct obstacle {
    bool type;
    float deg;
    int x;
    int y;
    bool touched;
    struct obstacle* next;
} obstacle;

obstacle* newObstacle(bool type, int x,int y){
    obstacle* a=malloc(sizeof(obstacle));
    if(a!=NULL){
        a->type=type;
        a->deg=0;
        a->x=x;
        a->y=y;
        a->touched=false;
        a->next=NULL;
    }
    return a;
}
void addObstacle(obstacle* a,bool type, int x, int y){
    while(a->next!=NULL){
        a=a->next;
    }
    a->next=newObstacle(type,x,y);
}
obstacle* deleteUnderObstacle(obstacle* a){
    if(a==NULL)
        return NULL;

    if(a->y>700){
        if(a->next==NULL){
            free(a);
            return NULL;
        }
        obstacle* b=a;
        a=a->next;
        free(b);
    }

    obstacle* b=a;
    while(a->next!=NULL){
        if((a->next)->y>700){
            obstacle* c=(a->next)->next;
            free(a->next);
            a->next=c;
        }
        else{
            a=a->next;
        }
    }
    return b;
}
