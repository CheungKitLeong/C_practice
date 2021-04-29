#include <stdio.h>
#include <stdlib.h>
typedef struct{
    char* data;
    int top;
    int size;
} Stack;
void create(Stack* stk, int insize){
    stk->data = (char*)malloc(insize*sizeof(char));
    stk->top = -1;
    stk->size = insize;
    return;
}
void push(Stack* stk, char input){
    if(stk->top <= stk->size-1){
        stk->top++;
        stk->data[stk->top] = input;
    }
    return ;
}
char pop(Stack* stk){
    if(stk->top >= 0){
        stk->top--;
        return stk->data[stk->top+1];
    }
    return 0;
}
int balance(char* str, int len){
    Stack* stk = (Stack*)malloc(sizeof(Stack));
    create(stk, len);
    int c = 0;
    for(int i = 0; i<len; i++){
        if(str[i] == '('){
            push(stk, str[i]);
        }
        else if (str[i] == ')'){
            if(pop(stk) != '('){
                c++;
            }
        }
    }
    c += stk->top+1;
    free(stk->data);
    free(stk);
    return c;
}
int main(){
    int test_count;
    scanf("%d", &test_count);
    int* str_size = (int*)malloc(test_count*sizeof(int));
    char** str = (char**)malloc(test_count*sizeof(char*));
    int* output = (int*)malloc(test_count*sizeof(int));
    for(int i = 0; i < test_count; i++){
        scanf("%d", &str_size[i]);
        str[i] = (char*)malloc((str_size[i]+1)*sizeof(char));
        if (str_size[i] == 0){
            output[i] = 0;
        }
        else{
            scanf("%s", str[i]);
        }
    }
    for(int i = 0; i < test_count; i++){
        //function
        output[i] = balance(str[i],str_size[i]);
        printf("%d\n", output[i]);
        free(str[i]);
    }
    free(str_size);
    free(str);
    free(output);
    return 0;
}