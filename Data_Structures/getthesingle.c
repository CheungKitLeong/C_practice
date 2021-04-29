#include <stdio.h>
#include <stdlib.h>
void getthesingle(int* arr, int* size){
    for (int i=1; i<*size; i++){
        arr[0] ^= arr[i];
    }
    printf("%d\n", arr[0]);
} 
int main(){
    int test_count;
    scanf("%d", &test_count);
    int* array_size = malloc(test_count*sizeof(int));
    int** t_arr = malloc(test_count*sizeof(int*));
    for (int i=0; i<test_count; i++){
        scanf("%d",&array_size[i]);
        t_arr[i] = malloc(array_size[i]*sizeof(int));
        for (int j=0; j<array_size[i]; j++){
            scanf("%d", &t_arr[i][j]);
        }
    }
    /* test*/
    /* function*/
    for (int i = 0; i < test_count; i++){
        getthesingle(t_arr[i], &array_size[i]);
    }
    /*free*/
    for (int i = 0; i < test_count; i++){
        free(t_arr[i]);
    }
    free(t_arr);
    free(array_size);
    return 0;
}