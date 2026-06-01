// Lets use printf and scanf
#include <stdio.h>
// Needed for malloc() and free()
#include <stdlib.h>
// For string related functions
// Will help interpret commands from input 
#include <string.h>


void insert(int n);
void delete(int n);
void printlist(void); 

// Defines node like shown in class with a pointer to the next
typedef struct Node{
	struct Node *next;
	// Data each node stores like the integer n 
	int value;
} Node;

// Initializes the first node using the struct above
// Initializing the list to be emepty - clean slate
// head never changes, only next is followed
Node *head = NULL;

// n is temporary variable that will be stored in value
void insert(int n){
	// Asks OS for enough memory for one node
	// Returns a pointer to that memory aka points to the new node
	Node *new  = malloc(sizeof(Node));
	if (!new)
		exit(EXIT_FAILURE);
	// Go to the memory new points to and look at the value field
	// Store n in that value field
	new->value = n;
	// Has to connect to another node even if that node is unknown
	new->next = NULL;
	if(head == NULL){
		/* If there are no nodes yet, point head to the node 
		just created */
		head = new;
		// Don't need to link anything else
		return;
	}
	// Keep the list sorted
	Node *current = head;
	Node *previous = NULL;
	// Remmemmber n is the value given by input
	while (current != NULL && current->value < n) {
	    previous = current;
	    current = current->next;
	}
	// Prevent duplicates
	if (current != NULL && current->value == n) {
	    free(new);
            return;
    	}
	if (previous == NULL) {
    		new->next = head;
   		head = new;
	} else {
   		new->next = current;
    		previous->next = new;
	}

}

void delete(int n){
    Node *current = head;
    Node *previous = NULL;

    while (current != NULL && current->value != n) {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
	 return; 

    if (previous == NULL) {
        head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
}

void printlist(){
	int count = 0;
	Node *temp = head;
	while (temp != NULL) {
		count++;
		temp = temp->next;
	}
	printf("%d :", count);
	temp = head;
	while (temp != NULL) {
		printf(" %d", temp->value);
		temp = temp->next;
	}
	printf("\n");
}

int main(){
    char command;
    int n;

    while (scanf(" %c %d", &command, &n) == 2) {
        if (command == 'i') {
            insert(n);
        } else if (command == 'd') {
            delete(n);
        }
        printlist();
    }

    return 0;
}
