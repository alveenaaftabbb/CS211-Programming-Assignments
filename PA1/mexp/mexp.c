#include <stdio.h>
#include <stdlib.h>

// ** is pointer to  the 2D array representing the matrix
int **createMatrix(int *k, int *n, FILE *fp);
int **multiply(int **A, int **B, int k);
// k and n don't need to be modified here, so not passed by *
int **powerMatrix(int **M, int k, int n); 
void printMatrix(int **M, int k);
void freeMatrix(int **M, int k);

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("%s: Input a file\n", argv[0]);
		return 1;
	}
	
	FILE *fp = fopen(argv[1], "r");
	if(!fp){
		printf("Can't open %s\n", argv[1]);
		return 1;
	}

	int k, n;
	int **M = createMatrix(&k, &n, fp);
	fclose(fp);

	if(!M){
		printf("Allocation failed\n");
		return 1;
	}
	
	int **R = powerMatrix(M, k, n);
	if(!R){
		printf("Allocation failed\n");
		freeMatrix(M, k);
		return 1;
	}
	printMatrix(R, k);
	
	freeMatrix(M, k);
	freeMatrix(R, k);
	return 0;
}

int **createMatrix(int *k, int *n, FILE *fp){
	if(fscanf(fp, "%d", k)!=1){
		return NULL;
	}
	int **M = (int **)malloc((*k) * sizeof(int*));
	if(!M)
		return NULL;
	for(int i = 0; i < *k; i++){
		M[i] = (int *)malloc((*k) * sizeof *M[i]);
		if(!M[i]){
			for(int r = 0; r < i; r++)	free(M[r]);
			free(M);
			return NULL;
		}
		for(int j = 0; j < *k; j++) {
			if(fscanf(fp, "%d", &M[i][j]) !=1){
			for(int r = 0; r <= i; r++) free(M[r]);
			free(M);
			return NULL;
		}
		}
	}
	if(fscanf(fp, "%d", n) != 1) {
		for (int r = 0; r < *k; r++) free(M[r]);
		free(M);
		return NULL;
	}
	return M;
}

int **multiply(int **A, int **B, int k){
	int **C = malloc(k * sizeof *C);
	if (!C) return NULL;
	for(int i = 0; i < k; i++) {
		C[i] = (int *)malloc(k * sizeof *C[i]);
		if (!C[i]) {
			for (int r = 0; r < i; r++) free(C[r]);
			free(C);
			return NULL;
		}
		for(int j = 0; j < k; j++){
			int sum = 0; 
			for(int t = 0; t < k; t++){
				sum += A[i][t] * B[t][j];
			}
			C[i][j] = sum;
		}
	}
	return C;
}

int **powerMatrix(int **M, int k, int n){
	int **R = (int **)malloc(k * sizeof(int *));
	if(!R) return NULL;
	for(int i = 0; i < k; i++) {
		R[i] = (int *)malloc(k * sizeof(int));
		if(!R[i]) {
			for (int r = 0; r < i; r++) free(R[r]);
			free(R);
			return NULL;
		}
		for(int j = 0; j < k; j++) R[i][j] = (i == j) ? 1 : 0;
	}
	if(n == 0){
		return R;
	}
	for(int c = 0; c < n; c++) {
		int **next = multiply(R, M, k);
		if(!next){
			freeMatrix(R, k);
			return NULL;
		}
		freeMatrix(R, k);
		R = next;
	}
	return R;
}

void printMatrix(int **M, int k){
	for(int i = 0; i < k; ++i){
		for(int j = 0; j < k; ++j){
			if(j > 0) printf(" ");
			printf("%d", M[i][j]);;
		}
		printf("\n");
	}
}

void freeMatrix(int **M, int k){
	if(!M)
		return;
	for(int i = 0; i < k; i++){
		free(M[i]);
	}
	free(M);
}

