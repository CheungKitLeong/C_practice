#include <stdio.h>

typedef int eletype;

typedef struct Heap{
    eletype* data;
    int size;
    int cap;
}Heap;

Heap* create(int cap){
    Heap* hp = (Heap*) malloc(sizeof(Heap));
    hp->data = (eletype*) malloc(cap* sizeof(eletype));
    hp->cap = cap;
    hp->size = 0;
    return hp;
}

// insert at last
void insert(Heap* hp, eletype item){
    if(hp->size == hp->cap){
        exit(1); // full
    }
    int i = hp->size;
    while( i != 0 && item < hp->data[(i-1)/2]){
        hp->data[i] = hp->data[(i-1)/2];
        i = (i-1)/2;
    }
    hp->data[i] = item;
    hp->size++;
    return;
}

eletype popmin(Heap* hp){
    if(hp->size > 0){
        eletype min;
        int parent = 0;
        int child = 1;
        min = hp->data[parent];
        eletype temp = hp->data[hp->size - 1];
        hp->size--;
        int size = hp->size;
        while(child < size){
            if((child + 1 < size) && (hp->data[child + 1] < hp->data[child])){
                child++;
            }
            if(temp < hp->data[child]){
                break;
            }else{
                hp->data[parent] = hp->data[child];
                parent = child;
                child = child * 2 + 1;
            }
        }
        hp->data[parent] = temp;
        return min;
    }else{
        exit(1);
    }
}

// build from array
eletype* hparr;
Heap* fastbuild(eletype* hparr, int size){
    Heap* hp = (Heap*) malloc(sizeof(Heap));
    hp->data = hparr;
    hp->size = size;
    hp->cap = size;
    int i = (size - 2) / 2;
    int child;
    eletype temp;
    while(i >= 0 ){
        child = 2*i + 1;
        if((child + 1 < size) && (hp->data[child+1]< hp->data[child])){
            child++;
        }
        if(hp->data[child] < hp->data[i]){
            temp = hp->data[child];
            hp->data[child] = hp->data[i];
            hp->data[i] = temp;
        }
        i--;
    }
    return hp;
}