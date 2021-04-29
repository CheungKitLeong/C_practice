#include <stdio.h>

typedef int type;

// Single linked list
typedef struct Node Node;
struct Node{
    type data;
    Node* next;
};

Node* create(type data){
    Node* nd = (Node*)malloc(sizeof(Node));
    nd->data = data;
    nd->next = NULL;
    return nd;
}

Node* insert(Node* prev, Node* self){
    self->next = prev->next;
    prev->next = self;
    // ! IMPORTANT
    return self;
}

// linear time append (return the index)
int append(Node* head, Node* self){
    Node* nd = head;
    int index = 0;
    while(nd->next != NULL){
        nd = nd->next;
        index++;
    }
    nd->next = self;
    index++;
    return index;
}

// add to the head
Node* push(Node* head, Node* self){
    self->next = head;
    return self;
}

// find by index
Node* find(Node* head, int k){
    Node* tar = head;
    for(int i = 0; i < k; i++){
        tar = tar->next;
    }
    return tar;
}

// delete when prev known
void deletenext(Node* prev){
    if(prev->next){
        Node* next = prev->next->next;
        free(prev->next);
        prev->next = next;
    }
    return;
}

void deletekth(Node* head, int k){
    Node* nd = find(head, k-1);
    deletenext(nd);
    return;
}

void freelist(Node* head){
    Node* nd = head;
    while(nd->next != NULL){
        head = nd;
        free(head);
        nd = nd->next;
    }
    free(nd);
}
