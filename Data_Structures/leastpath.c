#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node Node;

struct Node{
    int ele;
    Node* left;
    Node* right;
    Node* leastter;
};

Node* create(int ele){
    Node* nd = (Node*)malloc(sizeof(Node));
    nd->ele = ele;
    nd->left = NULL;
    nd->right = NULL;
    nd->leastter = NULL;
    return nd;
}

Node* inpostgrow(int* in, int* post, int* postindex, int start, int end){
    Node* root = create(post[*postindex]);
    if(start == end){
        (*postindex)--;
        return root;
    }
    if(start > end){
        return NULL;
    }
    int current = start;
    while(current <= end){
        if(post[*postindex] == in[current]){
            break;
        }
        current++;
    }
    (*postindex)--;
    root->right = inpostgrow(in, post, postindex, current+1, end);
    root->left = inpostgrow(in, post, postindex, start, current-1);
    return root;
}

void freetree(Node* root){
    if(root == NULL){
        return;
    }
    freetree(root->left);
    freetree(root->right);
    free(root);
    return;
}

int pathsum(Node* root){
    int a,b;
    if(root->left == NULL && root->right == NULL){
        root->leastter = root;
        return root->ele;
    }
    if(root->left == NULL){
        a = pathsum(root->right);
        root->leastter = root->right->leastter;
        return a + root->ele;
    }
    if(root->right == NULL){
        b = pathsum(root->left);
        root->leastter = root->left->leastter;
        return b + root->ele;
    }
    a = pathsum(root->left);
    b = pathsum(root->right);
    if(a < b){
        root->leastter = root->left->leastter;
        return a + root->ele;
    }else{
        root->leastter = root->right->leastter;
        return b + root->ele;
    }
}

int readintlines(int* arr){
    char str[20000];
    char buf[5];
    fgets(str, 20000, stdin);
    int c = 0;
    int i = 0;
    int bi = 0;
    while(str[i] != '\n'){
        if(str[i]!=' '){
            buf[bi] = str[i];
            bi++;
        }else{
            buf[bi] = '\0';
            arr[c] = atoi(buf);
            bi = 0;
            c++;
        }
        i++;
    }
    buf[bi] = '\0';
    arr[c] = atoi(buf);
    c++;
    arr = (int*)realloc(arr, c*sizeof(int));
    return c;
}

int main(){
    int* in = (int*)malloc(10000*sizeof(int));
    int* post = (int*)malloc(10000*sizeof(int));
    int c = readintlines(in);
    readintlines(post);
    int postindex = c-1;
    Node* root = inpostgrow(in, post, &postindex, 0, postindex);
    pathsum(root);
    printf("%d", root->leastter->ele);
    free(in);
    free(post);
    freetree(root);
    return 0;
}