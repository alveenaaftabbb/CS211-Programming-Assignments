#include <stdio.h>

int main(int argc, char *argv[]){
	// User inputs string after ./rot13
	// argv is an array of char pointers
	if(argc > 1){
		char *word = argv[1];
		int i = 0; // First char of the word
		while(word[i] != '\0'){
		if((word[i] >= 'A' && word[i] <= 'M') || (word[i] >= 'a' && word[i] <= 'm')){
			word[i]+=13;
		}else if((word[i] >= 'N' && word[i] <= 'Z') || (word[i] >= 'n' && word[i] <= 'z')){
			word[i]-=13;
	  }	
	putchar(word[i]);
	// If non alphabetical, it will just automatically increment to the next char if there is one
	i++;		
	}
	putchar('\n');
    }		
}
