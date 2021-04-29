#include<stdio.h>

typedef int type;

// Double linked list
typedef struct Node Node;
struct Node{
    type data;
    Node* next;
    Node* prev;
};

Node* create(type data){
    Node* nd = (Node*)malloc(sizeof(Node));
    nd->data = data;
    nd->next = NULL;
    nd->prev = NULL;
    return nd;
}

Node* insertnext(Node* prev, Node* self){
    self->next = prev->next;
    prev->next = self;
    self->next->prev = self;
    self->prev = prev;
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
    self->prev = nd;
    index++;
    return index;
}

// add to the head
Node* push(Node* head, Node* self){
    self->next = head;
    head->prev = self;
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

// delete self
void delete(Node* self){
    if(self->prev){
        self->prev->next = self->next;

    }
    if(self->next){
        self->next->prev = self->prev;
    }
    free(self);
    return;
}

void deletekth(Node* head, int k){
    Node* nd = find(head, k);
    delete(nd);
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