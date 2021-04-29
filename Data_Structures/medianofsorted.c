#include <stdlib.h>
#include <stdio.h>
double median(double* a, double* b, int asize, int bsize){
    int result = (asize+bsize) / 2;
    double merge[result + 1];
    int i = 0;
    int j = 0;
    while(i+j <= result){
        if(j == bsize){
            for(;i+j <= result; i++){
                merge[i+j] = a[i];
            }
            break;
        }
        if(i == asize){
            for(;i+j <= result; j++){
                merge[i+j] = b[j];
            }
            break;
        }
        if(a[i] >= b[j]){
            merge[i+j] = b[j];
            j++;
        }else{
            merge[i+j] = a[i];
            i++;
        }
    }
    if((asize+bsize) % 2){
        return merge[result];
    }else{
        return ((merge[result - 1] + merge[result]) / 2);
    }
}

int main(){
    int tc;
    scanf("%d", &tc);
    int asize;
    int bsize;
    double* a;
    double* b;
    for(int i = 0; i < tc; i++){
        scanf("%d %d", &asize, &bsize);
        a = (double*) malloc(asize*sizeof(double));
        b = (double*) malloc(bsize*sizeof(double));
        int temp;
        for(int j = 0; j < asize; j++){
            scanf("%d", &temp);
            a[j] = (double) temp;
        }
        for(int j = 0; j < bsize; j++){
            scanf("%d", &temp);
            b[j] = (double) temp;
        }
        printf("%.1lf\n", median(a, b, asize, bsize));
        free(a);
        free(b);
    }
    return 0;
}