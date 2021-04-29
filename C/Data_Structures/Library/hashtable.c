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