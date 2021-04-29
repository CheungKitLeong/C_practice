#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_L 256
void reverswords(char* sen){
    int save =0;
    for (int i = 0; i < strlen(sen)+1; i++){
       if (sen[i]==' ' || sen[i] =='\0'){
           int end = i-1;
           while(end> save){
               char temp = sen[end];
               sen[end] = sen[save];
               sen[save] = temp;
               save++;
               end--;
           }
           save = i+1;
       }
    }
    printf("%s\n", sen);
}
int main(){
    int test_count;
    scanf("%d",&test_count);
    char** sen_arr = malloc(test_count*sizeof(char*));
    while ((getchar()) != '\n');
    for (int i = 0; i < test_count; i++){
        sen_arr[i] = malloc(MAX_L*sizeof(char));
        gets(sen_arr[i]);
        sen_arr[i] = realloc(sen_arr[i],(strlen(sen_arr[i])+1)*sizeof(char));
    }
    /*printf("%s %d\n", sen[0], strlen(sen[0]));
    /*printf("%s %d\n", sen[1], strlen(sen[1]))*/
    /*function*/
    for (int i=0; i<test_count; i++){
        reverswords(sen_arr[i]);
    }
    for (int i = 0; i < test_count; i++){
        free(sen_arr[i]);
    }
    free(sen_arr);
    return 0;
}