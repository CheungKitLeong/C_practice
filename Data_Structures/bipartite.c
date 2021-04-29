#include <stdlib.h>
#include <stdio.h>
typedef struct Node Node;
struct Node{
    int data;
    Node* next;
};

Node* create(int data){
    Node* nd = malloc(sizeof(Node));
    nd->data = data;
    nd->next = NULL;
    return nd;
}

Node* setnbr(Node* root, int newdnode){
    Node* nd = create(newdnode);
    if(!root){
        return nd;
    }
    Node* iter = root;
    while(iter->next && newdnode != iter->data){
        iter = iter->next;
    }
    if(newdnode == root->data){
        free(nd);
        return root;
    }
    iter->next = nd;
    return root;
}

int coloring(Node* root, char* color, char nowcolor){
    while(root){
        if(color[root->data] != nowcolor && color[root->data] != 0){
            return 1;
        }else if (color[root->data] == 0){
            color[root->data] = nowcolor;
        }
        root = root->next;
    }
    return 0;
}

void freeroot(Node* root){
    if(!root){
        return;
    }
    freeroot(root->next);
    free(root);
    return;
}

int main(){
    int tc;
    scanf("%d", &tc);
    int nodenum;
    Node** adjli;
    char* color;
    for(int i = 0; i < tc; i++){
        scanf("%d", &nodenum);
        adjli = (Node**) calloc(nodenum, sizeof(Node*));
        color = (char*) calloc(nodenum, sizeof(char));
        int input = 0;
        int que[2];
        int count = 0;
        while(input >= 0){
            scanf("%d", &input);
            if(input == nodenum){
                input = 0;
            }
            que[count] = input;
            count++;
            if(count == 2){
                adjli[que[0]] = setnbr(adjli[que[0]], que[1]);
                adjli[que[1]] = setnbr(adjli[que[1]], que[0]);
                count = 0;
            }
        }
        int qhead = 0;
        int qtail = 1;
        char nowcolor;
        int queue[nodenum];
        queue[0] = 0;
        color[0] = 1;
        while(qhead < nodenum){
            int v = queue[qhead];
            qhead++;
            nowcolor = color[v];
            if(nowcolor == 1){
                nowcolor = 2;
            }else{
                nowcolor = 1;
            }
            Node* root = adjli[v];
            while(root){
                if(color[root->data] != nowcolor && color[root->data] != 0){
                    printf("NO\n");
                    qhead = nodenum + 1;
                    break;
                }else if (color[root->data] == 0){
                    color[root->data] = nowcolor;
                    queue[qtail] = root->data;
                    qtail++;
                }
                root = root->next;
            }
            if((qtail == qhead)&&(qhead<nodenum)){
                for(int n = 0; n<nodenum; n++){
                    if(color[n]==0){
                        queue[qtail] = n;
                        color[n] = 1;
                        qtail++;
                        break;
                    }
                }
            }
        }
        if(qhead == nodenum){
            printf("YES\n");
        }
        free(color);
        for(int j = 0; j<nodenum; j++){
            freeroot(adjli[j]);
        }
        free(adjli);
    }
    return 0;
}