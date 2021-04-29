#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool check(int* path, int pathsize, int* tree, int treesize){
    int start;
    for(int i=0; i<treesize; i++){
        if(path[0] == tree[i]){
            start = i;
            break;
        }
        if(i == treesize-1){
            return false;            
        }
    }
    for(int i=1; i<pathsize; i++){
        start = start*2+1;
        if(start < treesize){
            if(path[i] == tree[start]){
                continue;
            }
            start++;
            if(start<treesize){
                if (path[i] == tree[start]){
                    continue;
                }
                
            }
        }
        return false;
    }
    return true;
}

int main(){
    int tc;
    scanf("%d", &tc);
    int* pathsize = (int*) malloc(tc*sizeof(int));
    int** paths = (int**)malloc(tc* sizeof(int*));
    int* treesize = (int*) malloc(tc*sizeof(int));
    int** trees = (int**)malloc(tc* sizeof(int*));
    for(int i=0; i<tc; i++){
        scanf("%d", &pathsize[i]);
        paths[i] = (int*) malloc(pathsize[i]*sizeof(int));
        for(int j=0; j<pathsize[i]; j++){
            scanf("%d", &paths[i][j]);
        }
        scanf("%d", &treesize[i]);
        trees[i] = (int*) malloc(treesize[i]*sizeof(int));
        for(int j=0; j<treesize[i]; j++){
            scanf("%d", &trees[i][j]);
        }
    }
    for(int i =0; i<tc; i++){
        if(check(paths[i], pathsize[i], trees[i], treesize[i])){
            printf("True\n");
        }
        else{
            printf("False\n");
        }
    }
    for(int i=0; i<tc; i++){
        free(paths[i]);
        free(trees[i]);
    }
    free(pathsize);
    free(treesize);
    free(paths);
    free(trees);
    return 0;
}