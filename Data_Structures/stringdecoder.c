#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct{
    char* data;
    int top;
    int size;
    int mark;
} Stack;
void create(Stack* stk, int insize){
    stk->data = (char*)malloc(insize*sizeof(char));
    stk->top = -1;
    stk->size = insize;
    stk->mark = -1;
    return;
}
int push(Stack* stk, char input){
    if(stk->top <= stk->size-1){
        stk->top++;
        stk->data[stk->top] = input;
        if(input == '('){
            stk->mark = stk->top;
        }
        else if(input == ')'){
            return 1;
        }
    }
    return 0 ;
}
void decode(Stack* stk){
    int i = stk->mark-2;
    while(i>=0){
       if(! isdigit(stk->data[i])){
           break;
       }
        i--;
    }
    char num[stk->mark-i];
    char copy[stk->top-stk->mark];
    memset(num, '\0', stk->mark-i);
    memset(copy, '\0', stk->top-stk->mark);
    strncpy(num, stk->data + i + 1, stk->mark - i - 1);
    strncpy(copy, stk->data + stk->mark + 1, stk->top - stk->mark - 1);
    stk->top = i;
    while(i>=0){
        if(stk->data[i]=='('){
            break;
        }
        i--;
    }
    stk->mark = i;
    stk->size = stk->size - 2 + atoi(num)*(strlen(copy))-strlen(num);
    stk->data = (char*)realloc(stk->data, stk->size);
    for (int i = 0; i < atoi(num); i++){
        for(int j = 0; j < strlen(copy); j++){
            push(stk, copy[j]);
        }
    }
    return ;
}
char* decoder(char* str){
    Stack* stk;
    stk = (Stack*)malloc(sizeof(Stack));
    int len = strlen(str);
    create(stk, len+1);
    for(int i = 0; i < len; i++){
        if(push(stk, str[i])){
            decode(stk);
        }
    }
    push(stk, '\0');
    char* output = stk->data;
    free(stk);
    return output ;
}
int main(){
    int test_count;
    scanf("%d", &test_count);
    char** test_arr = (char**)malloc(test_count*sizeof(char*));
    for (int i = 0; i<test_count; i++){
        test_arr[i] = (char*)malloc(10000*sizeof(char));
        scanf("%s", test_arr[i]);
        test_arr[i] = realloc(test_arr[i], (strlen(test_arr[i])+1)*sizeof(char));
    }
    char* output;
    for (int i = 0; i < test_count; i++){
        output = decoder(test_arr[i]);
        printf("%s\n", output);
    }
    free(output);
    return 0;
}