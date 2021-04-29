#include <stdio.h>
// An entry in dict
typedef int eletype;

// AVL binary search tree
typedef struct Node{
	eletype* ele;
	Node* left;
	Node* right;
	int height;
}Node ;

Node* create(eletype* ety){
	Node* root = (Node*) malloc(sizeof(Node));
	root->ele = ety;
	root->left = NULL;
	root->right =NULL;
	root->height = 0;
	return root;
}

int getheight(Node* root){
	if(root == NULL){
		return -1;
	}else{
		return root->height;
	}
}

int setheight(Node* root){
	return max(getheight(root->left), getheight(root->right)) + 1;
}

Node* rightrotate(Node* root){
	Node* temp = root;
	root = root->left;
	temp->left = root->right;
	root->right = temp;
	root->right->height = setheight(root->right);
	root->height = setheight(root);
	return root;
}

Node* leftrotate(Node* root){
	Node* temp = root;
	root = root->right;
	temp->right = root->left;
	root->left = temp;
	root->left->height = setheight(root->left);
	root->height = setheight(root);
	return root;
}

Node* insert(Node* root, eletype* ety){
	if(root == NULL){
		return create(ety);
	}
	int cmp ;
	if(ety < root->ele){
		root->left = insert(root->left, ety);
	}else if(ety > root->ele){
		root->right = insert(root->right, ety);
	}else{
		exit(1); // equal entries
	}
	root->height = setheight(root);
	cmp = getheight(root->left) - getheight(root->right);
	if(cmp > 1){
		if(ety > root->left->ele){
			root->left = leftrotate(root->left);
		}
		return rightrotate(root);
	}else if (cmp < -1){
		if(ety < root->right->ele){
			root->right = rightrotate(root->right);
		}
		return leftrotate(root);
	}
	return root;
}

eletype* search(Node* root, eletype tar){
	if(root == NULL){
		return NULL;
	}else{
		if(tar == root->ele){
			return root->ele;
		}else if(tar > root->ele){
			return search(root->right, tar);
		}else{
			return search(root->left, tar);
		}
	}
}

void deldict(Node* dict){
	if(dict == NULL){
		return;
	}else{
		deldict(dict->left);
		deldict(dict->right);
		free(dict->ele);
		free(dict);
		return;
	}
}