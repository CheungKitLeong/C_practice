#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node Node;
struct Node{
    char ele;
    Node* left;
    Node* right;
};

Node* create(char ele){
    Node* nd = (Node*)malloc(sizeof(Node));
    nd->ele = ele;
    nd->left = NULL;
    nd->right = NULL;
    return nd;
}

Node* grow(char* pre, char* in, int* preindex, int start, int end){
    int current = start;
    Node* root = create(pre[*preindex]);
    if(start == end){
        (*preindex)++;
        return root;
    }
    if(start > end){
        return NULL;
    }
    while(current <= end){
        if(pre[*preindex] == in[current]){
            break;
        }
        current++;
    }
    (*preindex)++;
    root->left = grow(pre, in, preindex, start, current-1);
    root->right = grow(pre, in, preindex, current+1, end);
    return root;
}

void post(Node* root){
    if(root == NULL){
        return;
    }
    post(root->left);
    post(root->right);
    printf("%c", root->ele);
    return;
}

void freelist(Node* root){
    if(root == NULL){
        return;
    }
    freelist(root->left);
    freelist(root->right);
    free(root);
    return;
}

int main(){
    char pre[27];
    char in[27];
    int preindex = 0;
    scanf("%s %s", pre, in);
    int size = strlen(pre);
    Node* root = grow(pre, in, &preindex, 0, size-1);
    post(root);
    freelist(root);
    return 0;
}