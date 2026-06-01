#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


void transpose(const double *A, int m, int n, double *AT) {
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            AT[j*m + i] = A[i*n + j];
}
void direct_transpose(double *A, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j) {
            double tmp = A[i*n + j];
            A[i*n + j] = A[j*n + i];
            A[j*n + i] = tmp;
        }
}
void multiplication(const double *A, int m, int k,
            const double *B, int n,
            double *C)
{
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            for (int t = 0; t < k; ++t) {
                sum += A[i*k + t] * B[t*n + j];
            }
            C[i*n + j] = sum;
        }
    }
}
int invert(const double *M, double *N, int n) {
    
    double *A = (double *)malloc((size_t)n * n * sizeof(double));
    if (!A) return 0;

  
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i*n + j] = M[i*n + j];
            N[i*n + j] = (i == j) ? 1.0 : 0.0;
        }
    }
    for (int p = 0; p < n; ++p) {
        double pivot = A[p*n + p];
        if (pivot == 0.0) {          
            free(A);
            return 0;
        }


        for (int j = 0; j < n; ++j) {
            A[p*n + j] /= pivot;
            N[p*n + j] /= pivot;
        }
        for (int i = p + 1; i < n; ++i) {
            double f = A[i*n + p];
            if (f != 0.0) {
                for (int j = 0; j < n; ++j) {
                    A[i*n + j] -= f * A[p*n + j];
                    N[i*n + j] -= f * N[p*n + j];
                }
            }
        }
    }
    for (int p = n - 1; p >= 0; --p) {
        for (int i = p - 1; i >= 0; --i) {
            double f = A[i*n + p];
            if (f != 0.0) {
                for (int j = 0; j < n; ++j) {
                    A[i*n + j] -= f * A[p*n + j];
                    N[i*n + j] -= f * N[p*n + j];
                }
            }
        }
    }

    free(A);
    return 1;
}
int calculation_of_formula(const double *X, int m, int n, const double *Y, int p, double *W) {
    double *XT     = (double*)malloc((size_t)n * m * sizeof(double)); 
    double *XTX    = (double*)malloc((size_t)n * n * sizeof(double)); 
    double *XTXinv = (double*)malloc((size_t)n * n * sizeof(double)); 
    double *XTY    = (double*)malloc((size_t)n * p * sizeof(double)); 
    if (!XT || !XTX || !XTXinv || !XTY) { free(XT); free(XTX); free(XTXinv); free(XTY); return 0; }


    transpose(X, m, n, XT);                  

    multiplication(XT, n, m, X, n, XTX);         

  
    if (!invert(XTX, XTXinv, n)) {                
        free(XT); free(XTX); free(XTXinv); free(XTY);
        return 0;
    }


    multiplication(XT, n, m, Y, p, XTY);         

    multiplication(XTXinv, n, n, XTY, p, W);       

    free(XT); free(XTX); free(XTXinv); free(XTY);
    return 1;
}

int read_train(FILE *fp, double **X, int *n, int *k, double **Y) {
    char tag[32];
    if (fscanf(fp, "%31s", tag) != 1) 
        return 0;
    if (strcmp(tag, "train") != 0) 
        return 0;

    if (fscanf(fp, "%d", k) != 1) 
        return 0;
    if (fscanf(fp, "%d", n) != 1) 
        return 0;

    int d = *k + 1;  
    *X = (double*)malloc((size_t)(*n) * d * sizeof(double));
    *Y = (double*)malloc((size_t)(*n) * 1 * sizeof(double));
    if (!*X || !*Y) { 
        free(*X); free(*Y); 
        return 0; 
    }

    for (int i = 0; i < *n; ++i) {
        (*X)[i*d + 0] = 1.0;
        for (int j = 0; j < *k; ++j) {
            if (fscanf(fp, "%lf", &(*X)[i*d + 1 + j]) != 1) return 0;
        }
        if (fscanf(fp, "%lf", &(*Y)[i]) != 1) 
            return 0;
    }
    return 1;
}

int read_data(FILE *fp, double **Xp, int *m, int *k) {
    char tag[32];
    if (fscanf(fp, "%31s", tag) != 1) 
        return 0;
    if (strcmp(tag, "data") != 0) 
        return 0;

    if (fscanf(fp, "%d", k) != 1) 
        return 0;
    if (fscanf(fp, "%d", m) != 1) return 0;

    int d = *k + 1;  
    *Xp = (double*)malloc((size_t)(*m) * d * sizeof(double));
    if (!*Xp) return 0;

    for (int i = 0; i < *m; ++i) {
        (*Xp)[i*d + 0] = 1.0;           
        for (int j = 0; j < *k; ++j) {
            if (fscanf(fp, "%lf", &(*Xp)[i*d + 1 + j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void estimate_prices(const double *Xp, int m, int d, const double *W,  double *Yp) {
    multiplication(Xp, m, d, W, 1, Yp);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <train_file> <data_file>\n", argv[0]);
        return 1;
    }

    FILE *ftrain = fopen(argv[1], "r");
    if (!ftrain) { perror("open train"); return 1; }
    FILE *fdata  = fopen(argv[2], "r");
    if (!fdata)  { perror("open data"); fclose(ftrain); return 1; }

    double *X = NULL, *Y = NULL;
    int n = 0, k_train = 0;
    if (!read_train(ftrain, &X, &n, &k_train, &Y)) {
        fprintf(stderr, "bad train file format\n");
        fclose(ftrain); fclose(fdata);
        return 1;
    }
    fclose(ftrain);

    double *Xp = NULL;
    int m = 0, k_data = 0;
    if (!read_data(fdata, &Xp, &m, &k_data)) {
        fprintf(stderr, "bad data file format\n");
        free(X); free(Y);
        fclose(fdata);
        return 1;
    }
    fclose(fdata);

    if (k_train != k_data) {
        fprintf(stderr, "mismatched k: train=%d data=%d\n", k_train, k_data);
        free(X); free(Y); free(Xp);
        return 1;
    }

    int d = k_train + 1;  

    // Compute W
    double *W  = (double*)malloc((size_t)d * sizeof(double));
    double *Yp = (double*)malloc((size_t)m * sizeof(double));
    if (!W || !Yp) { 
        free(X); free(Y); free(Xp); free(W); free(Yp); 
        return 1; 
    }

    if (!calculation_of_formula(X, n, d, Y, 1, W)) {
        fprintf(stderr, "invert failed");
        free(X); free(Y); free(Xp); free(W); free(Yp);
        return 1;
    }
    estimate_prices(Xp, m, d, W, Yp);

    for (int i = 0; i < m; ++i) {
    printf("%.0f\n", Yp[i]);
    }


    free(X); free(Y); free(Xp); free(W); free(Yp);
    return 0;
}



// Formula: W = (X^T * X)^-1 *X^T *Y

