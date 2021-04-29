#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool checkid(char* id){
    if(strlen(id) == 11){
        int w = 0 ;
        for (int i = 0; i < 11; i++){
            switch (i){
            case 0:
                if (id[i] >= 'A' && id[i] <= 'M'){
                    w = 8*((int) id[i]-64);
                    /*
                    printf("%d, w: %d\n",(int)id[i]-64,w);
                    */
                }else return false;
                break;
            
            case 1:
            case 5:
            case 9:
                if (id[i] != '-'){
                    return false;
                }
                break;

            case 10:
                if ((id[i]>='0'&&id[i]<='9') || id[i] == 'A'){
                    w = w % 11;
                    /*
                    printf("%d\n", w);
                    */
                    if (w == 1){
                        return (id[i]=='A')? true: false;
                    }else{
                        return (11-w == ((int)id[i]-48)) ? true: false;
                    }
                }else return false;
                break;
                
            default:
                if(id[i]>='0'&&id[i]<='9'){
                    if(i>=2&&i<=4){
                        w += ((int)id[i]-48)*(9-i);
                        /*
                        printf("%d * %d, w: %d\n",(int)id[i]-48,9-i,w);
                        */
                    }else{
                        w += ((int)id[i]-48)*(10-i);
                        /*
                        printf("%d * %d, w: %d\n",(int)id[i]-48, 10-i, w);
                        */
                    } 
                }else return false;
                break;
            }
        }
    }else return false;
    
}

int main(){
    int test_count;
    scanf("%d", &test_count);
    char** id_arr =(char**) malloc((test_count)*sizeof(char*));
    for (int i = 0; i < test_count; i++){
        id_arr[i] =(char*) malloc(12*sizeof(char));
        scanf("%s",id_arr[i]);
    }
    for (int i = 0; i < test_count; i++){
        printf("%s", checkid(id_arr[i]) ? "True\n" : "False\n");
    }
    return 0;
}