#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct gate {
    int leftMod;
    bool leftMul;
    int rightMod;
    bool rightMul;
    bool touched;
    int y;
    struct gate* next;
} gate;

gate* newGate(int leftMod,bool leftMul,int rightMod,bool rightMul,int y,bool touched){
    gate* o=malloc(sizeof(gate));
    if(o!=NULL){
        o->leftMod=leftMod;
        o->leftMul=leftMul;
        o->rightMod=rightMod;
        o->rightMul=rightMul;
        o->y=y;
        o->touched=touched;
        o->next=NULL;
    }
    return o;
}
void addGate(gate* a,int leftMod,bool leftMul,int rightMod,bool rightMul,int y,bool touched){
    gate* tmp=a;
    while(tmp->next!=NULL){
        tmp=tmp->next;
    }
    tmp->next=newGate(leftMod,leftMul,rightMod,rightMul,y,touched);
}
gate* deleteUnderGate(gate* a){
    gate* b=a;
    gate* new=NULL;
    while(a!=NULL){
        if(a->y>700){
            a=a->next;
        }
        else{
            if(new==NULL)
                new=newGate(a->leftMod,a->leftMul,a->rightMod,a->rightMul,a->y,a->touched);
            else
                addGate(new,a->leftMod,a->leftMul,a->rightMod,a->rightMul,a->y,a->touched);
            a=a->next;
        }
    }
    while(b!=NULL){
        gate* q=b;
        b=b->next;
        free(q);
    }
    return new;
}
