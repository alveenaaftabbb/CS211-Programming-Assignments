#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct BSTNode {
	int data;
	struct BSTNode *left, *right;
} BSTNode;

BSTNode *getNewNode(int data){
	BSTNode *newNode = malloc(sizeof *newNode);
	if(newNode == NULL){
		return NULL;
	}
	newNode->data = data;
	newNode->left = NULL; 
	newNode->right = NULL;
	return newNode;
}

BSTNode *insert(BSTNode* root, int data){
	if(root == NULL){
		return getNewNode(data);
	} else if(data <= root->data){
		root->left = insert(root->left, data); 
	} else if(data > root->data){
		root->right = insert(root->right, data);
	} else{
	}
	return root;
}

bool search(BSTNode *root, int data){
	if(root == NULL) return false;
	if(root->data == data) return true;
	if(data < root->data) return search(root->left, data);
	return search(root->right, data);
}

void freeNodes(BSTNode *root){
	if(root == NULL){
		return;
	}
	freeNodes(root->left);
	freeNodes(root->right);
	free(root);
}

void printTree(BSTNode *root){
	if(root == NULL){
		return;
	}
	printf("(");
	if(root->left){
		 printTree(root->left);
	}
	printf("%d", root->data);
	if(root->right){
		printTree(root->right);
	}
	printf(")");
}

static BSTNode *getSuccessor(BSTNode *curr){
	if(curr == NULL || curr->right == NULL){
		return NULL;
	}
	curr = curr->right;
	while(curr->left != NULL){
		curr = curr->left;
	}
	return curr;
}

BSTNode *deleteNode(BSTNode *root, int key, int *deleted){
	if(root == NULL){
		*deleted = 0;
		return NULL;
	}

	if(key < root->data){
		root->left = deleteNode(root->left, key, deleted);
		return root;
	} else if(key > root->data){
		root->right = deleteNode(root->right, key, deleted);
		return root;
	} else{
		*deleted = 1;
		// No children
		if(root->left == NULL && root->right == NULL){
			free(root);
			return NULL;
		}
		// One child
		if(root->left == NULL){
			BSTNode *rightval = root->right;
			free(root);
			return rightval;
		}
		// One child
		if(root->right == NULL){
			BSTNode *leftval = root->left;
			free(root);
			return leftval;
		}
		BSTNode *succ = getSuccessor(root);
		root->data = succ->data;
		int ignored = 0;
		root->right = deleteNode(root->right, succ->data, &ignored);
		return root;
	}
}

int main(){
	// Made as global variable to be accessed by all 
	BSTNode* root = NULL; // Tree begins empty
	char cmnd;
	// While there is an input in the command line
	while(scanf(" %c",&cmnd) == 1){
	  switch(cmnd){
		case 'i': {
			int n;
			if(scanf("%d", &n) != 1){
				freeNodes(root);
				return 0;
			}
			if(search(root, n)){
				printf("not inserted\n");
			} else {
				root = insert(root, n);
				printf("inserted\n");
			}
			break;
		}
		case 's': {
			int n;
			if(scanf("%d", &n) != 1){
				freeNodes(root);
				return 0;
			}		
			if(search(root, n)){
				printf("present\n");
			} 
			else{
				printf("absent\n");
			}

			break;
		}
		case 'p':
			printTree(root);
			printf("\n");
			break;
		case 'd': {
			int n;
			if(scanf("%d", &n) != 1){
				freeNodes(root);
				return 0;
			}
			int del = 0;
			root = deleteNode(root, n, &del);
			if(del){
				printf("deleted\n");
			} else{
				printf("absent\n");
			}
		}
		default: {
			int c;
			while((c = getchar()) != '\n' && c != EOF) {}
			break;
		}
		}
	}
	freeNodes(root);
	return 0;
}
