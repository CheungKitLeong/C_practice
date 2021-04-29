#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct{
    char* data;
    int head;
    int tail;
    int num;
    int size;
} Queue;

void create(Queue* que, int insize){
    que->data = (char*)malloc(insize*sizeof(char));
    que->head = 0;
    que->tail = -1;
    que->num = 0;
    que-> size = insize;
    return ;
}

void enter(Queue* que, char input){
    if(que->num<que->size){
        que->tail = (que->tail + 1) % que->size;
        que->num++;
        que->data[que->tail] = input;
    }
    return ;
}
void quit(Queue* que){
    if(que->num > 0){
        que->head = (que->head + 1) % que->size;
        que->num--;
    }
    return ;
}
int max_ind(Queue* que){
    int mind = que->head;
    int end = que->head > que->tail ? que->tail + que->size : que->tail;
    int t;
    for(int i = que->head+1; i <= end; i++){
        t = i >= que->size ? i-que->size : i;
        mind = que->data[t] > que->data[mind] ? t : mind;
    }
    return mind;
}
char* largest(char* str, int delc){
    int i = 0;
    int j = 0;
    int temp;
    int len = strlen(str);
    char* output = (char*)calloc(len-delc+1,sizeof(char));
    int flag = 0;
    memset(output, '\0', len-delc);
    Queue* que = (Queue*)malloc(sizeof(Queue));
    create(que, delc + 1);
    while (delc > 0  && i < len){
        enter(que, str[i]);
        if(que->num == delc + 1){
            temp =  max_ind(que);
            output[j] = que->data[temp];
            if (flag == 1){
                j++;
            }
            else if (output[j] != '0' && flag == 0){
                flag = 1;
                j++;
            }
            while(que->head != temp){
                quit(que); 
                delc--;
            }
            quit(que); 
        }
        i++;
    }
    while (i<len){
        output[j] = str[i];
        if (flag == 1){
            j++;
        }
        else if (output[j] != '0' && flag == 0){
            flag = 1;
            j++;
        }
        i++;
    }
    if(output[0] == '\0'){
        output[0] = '0';
    }
    free(que->data);
    free(que);
    return output;
}
int main(){
    int t_count;
    scanf("%d", &t_count);
    char**str = (char**)malloc(t_count*sizeof(char*));
    int* delc = (int*)malloc(t_count*sizeof(int));
    for (int i = 0; i < t_count; i++){
        str[i] = (char*)malloc(10000*sizeof(char));
        scanf("%s", str[i]);
        scanf("%d", &delc[i]);
    }
    char* output;
    for (int i = 0; i < t_count; i++){
        output = largest(str[i], delc[i]);
        printf("%s\n", output);
        free(str[i]);
        free(output);
    }
    free(str);
    free(delc);
    return 0;
}