#include <stdio.h>
//Needed for exit statements
#include <stdlib.h>
// Size that can give us flexibility to be changed later
#define SIZE 100
char stack[SIZE];
// Indication that the stack is empty
int top = -1; 

void push(char c) {
    if (top < SIZE - 1) {
        stack[++top] = c;
    }
}

char pop() {
    if (top == -1) {
        return '\0';  // Empty stack
    }
    return stack[top--];
}

char peek() {
    if (top == -1){ 
	return '\0';
    }
   	return stack[top];
}

int isEmpty() {
    return top == -1;
}

int isMatch(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}') ||
           (open == '<' && close == '>');
}

int main(int argc, char *argv[]){
	if(argc > 1){
		char *input = argv[1];
		for(int i = 0; input[i] != '\0'; i++){
			char c = input[i];
			// Each time a (, [, {, or < is encountered in the input, it will push that delimiter onto the stack
			if(c == '[' || c == '{' || c == '(' || c == '<'){
				push(c);
			}else if(c == ']' || c == '}' || c == ')' || c == '>'){
				char open = pop();  // pop exactly once
   				 if (!((open == '(' && c == ')') ||(open == '[' && c == ']') ||
          				(open == '{' && c == '}') || (open == '<' && c == '>'))) {
				//%d tells printf to expect an integer
				//%c tells printf to expect a char
				printf("Dad1 %d: %c\n", i, c);	
				exit(EXIT_FAILURE);
				} 
			}
		}
		// Now check if there’s still any open delimiters left
		if(isEmpty()){
			printf("Dad 2");
	
			exit(EXIT_SUCCESS);
		} else {
			printf("open: ");
			while(!isEmpty()){
				char open = pop();
				if (open == '(')
					printf(")");
   				if (open == '[') 
					printf("]");
    				if (open == '{') 
					printf("}");
    				if (open == '<') 
					printf(">");
			}
			printf("\n");
			exit(EXIT_FAILURE);
		}

return 0;
}
}
