#include<stdio.h>

typedef int type;

typedef struct{
    type* data;
    int head;
    int tail;
    int num; // number of elements
    int size; // capacity
} Queue;

Queue* create(int size){
    Queue* que = (Queue*)malloc(sizeof(Queue));
    que->data = (type*)malloc(size*sizeof(type));
    que->head = 0;
    que->tail = -1;
    que->num = 0;
    que-> size = size;
    return que;
}

void enter(Queue* que, type input){
    if(que->num<que->size){
        que->tail = (que->tail + 1) % que->size;
        que->num++;
        que->data[que->tail] = input;
    }else{
        exit(1); // full
    }
    return ;
}

type quit(Queue* que){
    if(que->num > 0){
        type output = que->data[que->head];
        que->head = (que->head + 1) % que->size;
        que->num--;
        return output;
    }
    return NULL; //empty
}

int isfull(Queue* que){
    return (que->num == que->size);
}

void freeque(Queue* que){
    free(que->data);
    free(que);
    return;
}