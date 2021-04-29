#include <stdlib.h>
#include <stdio.h>

typedef unsigned int ui;

typedef struct Heap{
    int size;
    int cap;
    ui* ele;
} Heap;

Heap* buildempty(int cap){
    Heap* hp = (Heap*) malloc(sizeof(Heap));
    hp->cap = cap;
    hp->ele = (ui*)malloc(cap*sizeof(ui));
    hp->size = 0;
    return hp;
}

void insert(Heap* hp,ui in){
    if(hp-> size < hp->cap){
        int i = hp->size;
        while(i != 0 && in < hp->ele[(i - 1) / 2]){
            hp->ele[i] = hp->ele[(i - 1) / 2];
            i = (i - 1) / 2;
        }
        hp->ele[i] = in;
        hp->size++;
    }else{
        exit(1);
    }
    return;
}

ui popmin(Heap* hp){
    if(hp->size > 0){
        ui min;
        int parent = 0;
        int child = 1;
        min = hp->ele[parent];
        ui temp = hp->ele[hp->size - 1];
        hp->size--;
        int size = hp->size;
        while(child < size){
            if((child + 1 < size) && (hp->ele[child + 1] < hp->ele[child])){
                child++;
            }
            if(temp < hp->ele[child]){
                break;
            }else{
                hp->ele[parent] = hp->ele[child];
                parent = child;
                child = child * 2 + 1;
            }
        }
        hp->ele[parent] = temp;
        return min;
    }else{
        exit(1);
    }
}

ui output(Heap* hp, int k){
    ui* que = (ui*) malloc(k * sizeof(ui));
    for(int i = 0; i < k; i++){
        que[i] = popmin(hp);
    }
    ui min = que[k-1];
    for(int i = 0; i < k; i++){
        insert(hp, que[i]);
    }
    free(que);
    return min;
}

int main(){
    int k;
    scanf("%d", &k);
    char op;
    ui input;
    Heap* hp = buildempty(100000);
    while (1){
        scanf("%c", &op);
        if (op == 'I'){
            scanf("%d", &input);
            insert(hp, input);
        }else if (op == 'O'){
            printf("%d\n", output(hp, k));
        }else if (op == 'S'){
            break;
        }
    }
    free(hp->ele);
    free(hp);
    return 0;
}