#include <stdlib.h>
#include <stdio.h>

typedef struct Hash{
    int* keys;
    int hsize;
} Hash;

Hash* create(int hsize){
    Hash* ht = (Hash*)malloc(sizeof(Hash));
    ht->keys = (int*)calloc(hsize, sizeof(int));
    ht->hsize = hsize;
    return ht;
}

int hashing(Hash* ht, int key, int k){
    return (key + k) % ht->hsize;
}

void insert(Hash* ht, int key){
    int k = 0;
    int hv = hashing(ht, key, k);
    int i = hv;
    while(ht->keys[i] != 0){
        k++;
        i = hashing(ht, key, k);
        if(i == hv){
            exit(1);
        }
    }
    ht->keys[i] = key;
    return;
}

int find(Hash* ht, int key){
    int k = 0;
    int hv = hashing(ht, key, k);
    int i = hv;
    while(ht->keys[i] != 0){
        if (ht->keys[i] == key){
            return i;
        }
        k++;
        i = hashing(ht, key, k);
        if(i == hv){
            return -1;
        }
    }
    return -1;
}

int counttri(int* arr, int size, int p){
    if(p != 0){
        Hash* ht = create(size);
        for(int i = 0; i < size; i++){
            if((arr[i] != 0) && (p % arr[i] == 0)){
                insert(ht, arr[i]);
            }
        }
        int c = 0;
        for(int i = 0; i < size - 1; i++){
            if(ht->keys[i] == 0){
                continue;
            }
            for(int j = i + 1; j < size; j++){
                if(ht->keys[j] == 0){
                    continue;
                }
                int n = ht->keys[i] * ht->keys[j];
                if(p % n == 0){
                    n = find(ht, p / n);
                    if (n > j){
                        c++;
                    }
                }
            }
        }
        free(ht->keys);
        free(ht);
        return c;
    }else{
        for(int i = 0; i < size; i++){
            if(arr[i] == 0){
                return (size - 1) * (size - 2) / 2;
            }
        }
        return 0;
    }
}

int main(){
    int tc;
    scanf("%d", &tc);
    int arr_size[tc];
    int p[tc];
    int* arr[tc];
    for(int i = 0; i < tc; i++){
        scanf("%d %d", &arr_size[i], &p[i]);
        arr[i] = (int*) malloc(arr_size[i]*sizeof(int));
        for(int j = 0; j<arr_size[i]; j++){
            scanf("%d", &arr[i][j]);
        }
    }
    for(int i = 0; i < tc; i++){
        printf("%d\n", counttri(arr[i], arr_size[i], p[i]));
        free(arr[i]);
    }
    return 0;
}
