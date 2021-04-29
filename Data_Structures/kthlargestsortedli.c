#include <stdlib.h>
#include <stdio.h>
typedef struct element{
    int data;
    int idx;
}element;

typedef struct Heap{
    int size;
    element* ele;
}Heap;

Heap* buildempty(int size){
    Heap* hp = (Heap*) malloc(sizeof(Heap));
    hp->ele = (element*)malloc(size * sizeof(element));
    hp->size = size;
    return hp;
}

void replaceroot(Heap* hp, element new){
    int own = 0;
    int child = own * 2 + 1;
    int size = hp->size;
    while(child < size){
        if((child + 1 < size) && (hp->ele[child + 1].data > hp->ele[child].data)){
            child++;
        }
        if(new.data >= hp->ele[child].data){
            break;
        }else{
            hp->ele[own] = hp->ele[child];
            own = child;
            child = child * 2 + 1;
        }
    }
    hp->ele[own] = new;
    return;
}

int output(int* mat, int n, int k){
    Heap* hp = buildempty(n);
    int size = n*n;
    for(int i = 0; i< n; i++){
        hp->ele[i].data = mat[size - 1 - i];
        hp->ele[i].idx = size - 1 - i;
    }
    element kmax;
    for(int i = 0; i<k-1; i++){
        kmax = hp->ele[0];
        element new;
        if(kmax.idx - n < 0){
            new.data = -1;
            new.idx = -1;
        }else{
            new.idx = kmax.idx - n;
            new.data = mat[new.idx];
        }
        replaceroot(hp, new);
    }
    kmax = hp->ele[0];
    free(hp->ele);
    free(hp);
    return kmax.data;
}

int main(){
    int tc;
    scanf("%d",&tc);
    int n[tc];
    int k[tc];
    int* mat[tc];
    for(int i=0; i<tc; i++){
        scanf("%d %d", &n[i], &k[i]);
        int size = n[i]*n[i];
        mat[i] = (int*) malloc(size * sizeof(int));
        for(int j=0; j<size; j++){
            scanf("%d", &mat[i][j]);
        }
    }
    for(int i=0; i<tc; i++){
        //function
        printf("%d\n", output(mat[i], n[i], k[i]));
        free(mat[i]);
    }
    return 0;
}