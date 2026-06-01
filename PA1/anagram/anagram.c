#include <stdio.h>

int main(int argc, char* argv[]){
	// Needs to be declared outside if statement
	char *word;
	// Create new array for string without space/punctuation
	char new[256];
	// Index increment to compare with next letter
	int j = 0;
	if(argc > 1){
		word = argv[1];
		for(int i = 0; word[i] != '\0'; i++){
			if(word[i] >= 'A' && word[i] <= 'Z'){
				new[j++] = word[i] +32;
			} else if(word[i] >= 'a' && word[i] <= 'z'){
				new[j++] = word[i];
			}
		}
		new[j] = '\0'; // Strings need to end with this in the array
		for(int i = 0; new[i] != '\0'; i++){
			for(int k  = i + 1; new[k] != '\0'; k++){ 
				if(new[i] > new[k]){
					char temp = new[i];
					new[i] = new[k];
					new[k] = temp;
				}
			}		 
		}
	// %s tells printf to expect a char*
	printf("%s\n", new);
	} else {
		printf("Ex: ./anagram listen\n");
	}
	return 0;
}

