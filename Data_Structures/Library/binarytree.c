#include <stdio.h>
int* pre;
int* in;
int* post;

typedef struct Node Node;

struct Node{
    int ele;
    Node* left;
    Node* right;
};

Node* create(int ele){
    Node* nd = (Node*)malloc(sizeof(Node));
    nd->ele = ele;
    nd->left = NULL;
    nd->right = NULL;
    return nd;
}

void postorder(Node* root){
    if(root == NULL){
        return;
    }
    postorder(root->left);
    postorder(root->right);
    printf("%d", root->ele);
    return;
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

/*
int preindex = 0;
Node* root = grow(pre, in, &preindex, 0, size-1);
*/
Node* preingrow(int* pre, int* in, int* preindex, int start, int end){
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
    root->left = preingrow(pre, in, preindex, start, current-1);
    root->right = preingrow(pre, in, preindex, current+1, end);
    return root;
}

/*
int postindex = size-1;
Node* root = inpostgrow(in, post, &postindex, 0, postindex);
*/
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

// Only for full binary tree
/*
int preindex = 0;
Node* root = grow(pre, post, &preindex, 0, size-1);
*/
Node* prepostgrow(int* pre, int* post, int* preindex, int start, int end){
    Node* root = create(pre[*preindex]);
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
    root->left = prepostgrow(pre, post, preindex, start, current);
    root->right = prepostgrow(pre, post, preindex, current+1, end-1);
    return root;
}