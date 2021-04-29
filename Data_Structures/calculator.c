#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    double* data;
    int top;
    int size;
} NumStack;

NumStack* createnumstk(int size){
    NumStack* stk = (NumStack*)malloc(sizeof(NumStack));
    stk->data = (double*)malloc(size*sizeof(double));
    stk->size = size;
    stk->top = -1;
    return stk;
}

void pushnum(NumStack* stk, double input){
    if(stk->top < stk->size - 1){
        stk->top++;
        stk->data[stk->top] = input;
    }else{
        exit(1); // full
    }
    return ;
}

double popnum(NumStack* stk){
    if(stk->top >= 0){
        stk->top--;
        return stk->data[stk->top+1];
    }
    return 0;
}

typedef struct{
    char* data;
    int top;
    int size;
} CharStack;

CharStack* createcharstk(int size){
    CharStack* stk = (CharStack*)malloc(sizeof(CharStack));
    stk->data = (char*)malloc(size*sizeof(char));
    stk->size = size;
    stk->top = -1;
    return stk;
}

void pushchar(CharStack* stk, char input){
    if(stk->top < stk->size - 1){
        stk->top++;
        stk->data[stk->top] = input;
    }else{
        exit(1); // full
    }
    return;
}
char popchar(CharStack* stk){
    if(stk->top >= 0){
        stk->top--;
        return stk->data[stk->top+1];
    }
    return 0;
}

#define SECONDPIO(a) ( a=='+' || a=='-' || a=='*' || a=='/')
#define FIRSTPIO(a) ( a=='*' || a=='/' )

void binaryop(NumStack* nstk, CharStack* cstk){
    char op = popchar(cstk);
    double b = popnum(nstk);
    double a = popnum(nstk);
    switch (op){
    case '+':
        pushnum(nstk, a+b);
        break;
    case '-':
        pushnum(nstk, a-b);
        break;
    case '*':
        pushnum(nstk, a*b);
        break;
    case '/':
        pushnum(nstk, a/b);
        break;
    default:
        exit(1);
        break;
    }
    return;
}

void clearbuf(char* buffer, int* bi, NumStack* nstk){
    buffer[*bi] = '\0';
    if(*bi > 0){
        pushnum(nstk, atof(buffer));
        buffer[0] = '\0';
        *bi = 0;
    }    
    return;
}

double cal(char* str){
    int i = 0;
    NumStack* nstk = createnumstk(100);
    CharStack* cstk = createcharstk(100);
    char buffer[10];
    int bi = 0;
    while(str[i] != '\n'){
        switch (str[i]){
        case '+':
        case '-':
            if(str[i-1] != 'e'){
                clearbuf(buffer, &bi, nstk);
                while(cstk->top != -1){
                    if(SECONDPIO(cstk->data[cstk->top])){
                        binaryop(nstk, cstk);
                    }else{
                        break;
                    }
                }
                pushchar(cstk, str[i]);
            }else{
                buffer[bi] = str[i];
                bi++;
            }
            break;
        case '*':
        case '/':
            clearbuf(buffer, &bi, nstk);
            while(cstk->top != -1){
                if(FIRSTPIO(cstk->data[cstk->top])){
                    binaryop(nstk, cstk);
                }else{
                    break;
                }
            }
            pushchar(cstk, str[i]);
            break;            
        case '(':
            clearbuf(buffer, &bi, nstk);
            pushchar(cstk, str[i]);
            break;
        case ')':
            clearbuf(buffer, &bi, nstk);
            while (cstk->data[cstk->top] != '('){
                binaryop(nstk, cstk);
            }
            popchar(cstk);
            break;
        case ' ':
            break;
        default:
            buffer[bi] = str[i];
            bi++;
            break;
        }
        i++;
    }
    clearbuf(buffer, &bi, nstk);
    while(cstk->top != -1){
        binaryop(nstk, cstk);
    }
    double result = popnum(nstk);
    free(cstk->data);
    free(nstk->data);
    free(cstk);
    free(nstk);
    return result;
}

int main(){
    char str[100];
    fgets(str, 100, stdin);
    while(strcmp(str,"\n") != 0){
        printf("%.4f\n", cal(str));
        fgets(str, 100, stdin);
    }
    return 0;
}