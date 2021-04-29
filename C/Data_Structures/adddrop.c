#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Single linked list
typedef struct CNode CNode;
struct CNode{
    int data;
    CNode* next;
};

typedef struct SNode SNode;
struct SNode{
    int data;
    SNode* next;
    CNode* course;
};

SNode* creates(int data){
    SNode* nd = (SNode*)malloc(sizeof(SNode));
    nd->data = data;
    nd->next = NULL;
    nd->course = NULL;
    return nd;
}

CNode* createc(int data){
    CNode* nd = (CNode*)malloc(sizeof(CNode));
    nd->data = data;
    nd->next = NULL;
    return nd;
}

SNode* insertnexts(SNode* prev, SNode* self){
    self->next = prev->next;
    prev->next = self;
    // ! IMPORTANT
    return self;
}

CNode* insertnextc(CNode* prev, CNode* self){
    self->next = prev->next;
    prev->next = self;
    // ! IMPORTANT
    return self;
}

SNode* inserts(SNode* head, SNode* self){
    SNode* tar;
    if(head == NULL){
        return self;
    }
    if(head->data > self->data){
        self->next = head;
        return self;
    }
    tar = head;
    while(tar->next){
        if(tar->next->data > self->data){
            insertnexts(tar, self);
            return head;
        }
        tar = tar->next;
    }
    insertnexts(tar, self);
    return head;
}

CNode* insertc(CNode* head, int self){
    CNode* tar;
    if(head == NULL){
        return createc(self);
    }
    if(head->data > self){
        tar = createc(self);
        tar->next = head;
        return tar;
    }
    tar = head;
    while(tar->next){
        if(tar->next->data > self){
            insertnextc(tar, createc(self));
            return head;
        }
        tar = tar->next;
    }
    insertnextc(tar, createc(self));
    return head;
}

// delete when prev known
void deletenextc(CNode* prev){
    if(prev->next){
        CNode* next = prev->next->next;
        free(prev->next);
        prev->next = next;
    }
    return;
}

CNode* delete(CNode* head, int self){
    CNode* tar;
    if(head->data == self){
        tar = head->next;
        free(head);
        return tar;
    }
    tar = head;
    while(tar->next){
        if(tar->next->data == self){
            deletenextc(tar);
            break;
        }
        tar = tar->next;
    }
    return head;
}

SNode* searchs(SNode* head, int key){
    SNode* tar = head;
    while(tar){
        if(tar->data == key){
            return tar;
        }
        tar = tar->next;
    }
    return NULL;    
}

/*
void printc(SNode* head){
    Node* tar = head;
    while(tar){
        printf("%d ",tar->data);
        tar = tar->next;
    }
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
*/
int main(){
    char cmd[7];
    int c,s;
    SNode* shead = 0;
    SNode* temp;
    CNode* ctp;
    scanf("%s", cmd);
    while(strcmp(cmd, "\0") != 0){
        if(strcmp(cmd, "ADD") == 0){
            scanf("%d %d", &s, &c);
            if(temp = searchs(shead, s)){
                temp->course = insertc(temp->course, c);
            }else{
                temp = creates(s);
                temp->course = createc(c);
                shead = inserts(shead, temp);
            }
        }else if(strcmp(cmd, "DROP") == 0){
            scanf("%d %d", &s, &c);
            if(temp = searchs(shead, s)){
                temp->course = delete(temp->course, c);
            }
        }else if(strcmp(cmd, "PRINTS") == 0){
            scanf("%d", &s);
            temp = searchs(shead, s);
            if(temp && temp->course){
                ctp = temp->course;
                printf("%d", ctp->data);
                ctp = ctp->next;
                while(ctp){
                    printf(" %d", ctp->data);
                    ctp = ctp->next;
                }
                printf("\n");
            }else{
                printf("NIL\n");
            }
        }else if(strcmp(cmd, "PRINTC") == 0){
            scanf("%d", &c);
            temp = shead;
            while(temp){
                ctp = temp->course;
                while(ctp){
                    if(ctp->data == c){
                        printf("%d ", temp->data);
                        break;
                    }
                    ctp = ctp->next;
                }
                temp = temp->next;
            }
            printf("\n");
        }
        scanf("%s", cmd);
    }
    return 0;
}