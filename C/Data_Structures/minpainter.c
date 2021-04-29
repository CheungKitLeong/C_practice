#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Node Node;
struct Node{
    int ele;
    Node* left;
    Node* right;
    bool painted;
    bool painter;
};

Node* createtree(int* tree, int size, int i){
    Node* root = (Node*) malloc(sizeof(Node));
    root->left = NULL;
    root->right = NULL;
    root->painted = false;
    root->painter = false;
    root->ele = tree[i];
    int tarindex = 2*i+1;
    if(tarindex < size){
        if(tree[tarindex] != 0){
            root->left = createtree(tree, size, tarindex);
        }
        tarindex++;
        if(tarindex < size){
            if(tree[tarindex] != 0){
                root->right = createtree(tree, size, tarindex);
            }
        }
    }
    return root;
}

void paint(Node* root, int* nop){
    if (root->left != NULL){
        paint(root->left, nop);
        if(root->left->painted == false){
            root->painter = true;
            root->painted = true;
            root->left->painted = true;
            (*nop)++;
        }else if (root->left->painter == true){
            root->painted = true;
        }
    }
    if (root->right != NULL){
        paint(root->right, nop);
        if(root->right->painted == false){
            if(root->painter == false){
                (*nop)++;
                root->painter = true;
                root->painted = true;
            }
            root->right->painted = true;
        }else if (root->right->painter == true){
            root->painted = true;
        }
    }
    return;
}

void freetree(Node* root){
    if (root->left != NULL){
        freetree(root->left);
    }
    if (root->right != NULL){
        freetree(root->right);
    }
    free(root);
}

int minpainters(int* tree, int size){
    Node* root = createtree(tree, size, 0);
    int nop = 0;
    paint(root, &nop);
    if(root->painted == false){
        nop++;
    }
    freetree(root);
    nop = (nop == 0)? 1: nop;
    return nop;
}

int main(){
    int tc;
    scanf("%d", &tc);
    int* treesize = (int*) malloc(tc*sizeof(int));
    int** trees = (int**)malloc(tc* sizeof(int*));
    for(int i=0; i<tc; i++){
        scanf("%d", &treesize[i]);
        trees[i] = (int*) malloc(treesize[i]*sizeof(int));
        for(int j=0; j<treesize[i]; j++){
            scanf("%d", &trees[i][j]);
        }
    }
    for(int i =0; i<tc; i++){
        printf("%d\n", minpainters(trees[i], treesize[i]));
    }
    for(int i=0; i<tc; i++){
        free(trees[i]);
    }
    free(treesize);
    free(trees);
    return 0;
}