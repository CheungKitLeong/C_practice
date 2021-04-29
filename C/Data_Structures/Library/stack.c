#include<stdio.h>

typedef int type;

typedef struct{
    type* data;
    int top;
    int size;
} Stack;

Stack* create(int size){
    Stack* stk = (Stack*)malloc(sizeof(Stack));
    stk->data = (type*)malloc(size*sizeof(type));
    stk->size = size;
    stk->top = -1;
    return stk;
}

void push(Stack* stk, type input){
    if(stk->top < stk->size - 1){
        stk->top++;
        stk->data[stk->top] = input;
    }else{
        exit(1); // full
    }
    return ;
}
type pop(Stack* stk){
    if(stk->top >= 0){
        stk->top--;
        return stk->data[stk->top+1];
    }
    return 0;
}

void freestk(Stack* stk){
    free(stk->data);
    free(stk);
}