#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Node Node;
struct Node{
    int ele;
    Node* left;
    Node* right;
    Node* next;
};

typedef struct{
    Node* head;
    Node* tail;
} Queue;

Queue* create(){
    Queue* que = (Queue*) malloc(sizeof(Queue));
    que->head = NULL;
    que->tail = NULL;
    return que;
}

bool isempty(Queue* que){
    if(que->head == NULL || que->tail == NULL){
        return true;
    }else return false;
}

void enter(Queue* que, Node* input){
    if(isempty(que)){
        que->head = input;
    }
    else{
        que->tail->next = input;
    }
    que->tail = input;
    return ;
}

void quit(Queue* que){
    if(isempty(que)){
        return ;
    }
    printf("%d ", que->head->ele);
    if(que->head->left != NULL){
        enter(que, que->head->left);
    }
    if(que->head->right != NULL){
        enter(que, que->head->right);
    }
    if(que->head->next == NULL){
        que->tail = NULL;
    }
    que->head = que->head->next;
    return ;
}

Node* grow(int* pre, int* post, int size, int* preindex, int start, int end){
    Node* root = (Node*) malloc(sizeof(Node));
    root->ele = pre[*preindex];
    root->left = NULL;
    root->right = NULL;
    root->next = NULL;
    (*preindex)++;
    if(start >= end){
        return root;
    }
    int current;
    for(int i=start; i<end; i++){
        if(pre[*preindex] == post[i]){
            current = i;
            break;
        }
    }
    root->left = grow(pre, post, size, preindex, start, current);
    root->right = grow(pre, post, size, preindex, current+1, end-1);
    return root;
}

Node* construct(int* pre, int* post, int size){
    int preindex = 0;
    Node* root = grow(pre, post, size, &preindex, 0, size-1);
    return root;
}

void levelorder(Node* root){
    Queue* que = create();
    enter(que, root);
    while (!isempty(que)){
        quit(que);
    }
    free(que);
}

int main(){
    int tc;
    scanf("%d", &tc);
    int* arr_size = (int*) malloc(tc*sizeof(int));
    int** pre = (int**) malloc(tc*sizeof(int*));
    int** post = (int**) malloc(tc*sizeof(int*));
    for(int i=0; i<tc; i++){
        scanf("%d",&arr_size[i]);
        pre[i] = (int*) malloc(arr_size[i]*sizeof(int));
        for(int j=0; j<arr_size[i]; j++){
            scanf("%d", &pre[i][j]);
        }
        post[i] = (int*) malloc(arr_size[i]*sizeof(int));
        for(int j=0; j<arr_size[i]; j++){
            scanf("%d", &post[i][j]);
        }
    }
    for(int i=0 ; i<tc; i++){
        levelorder(construct(pre[i], post[i], arr_size[i]));
        printf("\n");
    }
    return 0;
}