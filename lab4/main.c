#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gsl/gsl_blas.h>

void naive_multiply(int N, int **mat1, int **mat2, int **res)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++)
                res[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
}

void better_multiply(int N, int **mat1, int **mat2, int **res)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++)
                res[i][j] += mat1[i][j] * mat2[j][k];
        }
    }
}

void fillArrayRandom(int N, int **arr){
    srand(0);
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            arr[i][j] = rand() % 100;
        }
    }
}

int main() {
    srand(0);
    double naiveTimes[10] = {0};
    double betterTimes[10] = {0};
    double blasTimes[10] = {0};
    int startSize = 100;
    for(int size = startSize; size <= startSize * 10; size += startSize){
        int **res = calloc(size, sizeof(int*));
        int **mat1 = calloc(size, sizeof(int*));
        int **mat2 = calloc(size, sizeof(int*));
        for(int i = 0; i < size; i++) {
            res[i] = calloc(size, sizeof(int));
            mat1[i] = calloc(size, sizeof(int));
            mat2[i] = calloc(size, sizeof(int));
        }
        fillArrayRandom(size, mat1);
        fillArrayRandom(size, mat2);
//        printf("%d \n", mat1[0][1]);

        clock_t naive_start = clock();
        naive_multiply(size, mat1, mat2, res);
        clock_t naive_end = clock();
        naiveTimes[size/startSize] = ((double)(naive_end - naive_start)) / CLOCKS_PER_SEC;;

        clock_t better_start = clock();
        better_multiply(size, mat1, mat2, res);
        clock_t better_end = clock();
        betterTimes[size/startSize] = ((double)(better_end - better_start)) / CLOCKS_PER_SEC;

        double *a = calloc(size*size, sizeof(double));
        double *b = calloc(size*size, sizeof(double));
        double *c = calloc(size*size, sizeof(double));

        for(int i=0; i < size*size; i++){
            a[i] = rand() % 100;
            b[i] = rand() % 100;
        }
        gsl_matrix_view A = gsl_matrix_view_array(a, size, size);
        gsl_matrix_view B = gsl_matrix_view_array(b, size, size);
        gsl_matrix_view C = gsl_matrix_view_array(c, size, size);

        clock_t blas_start = clock();
        gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,
                        1.0, &A.matrix, &B.matrix,
                        0.0, &C.matrix);
        clock_t blas_end = clock();
        blasTimes[size/startSize] = ((double)(blas_end - blas_start)) / CLOCKS_PER_SEC;
        free(a);
        free(b);
        free(c);
        for(int i = 0; i < size; i++) {
            free(res[i]);
            free(mat1[i]);
            free(mat2[i]);
        }
        free(res);
        free(mat1);
        free(mat2);
    }
    printf("[");
    for(int i = 0; i < 10; i++){
        printf("%f, ", naiveTimes[i]);
    }
    printf("]\n[");
    for(int i = 0; i < 10; i++){
        printf("%f, ", betterTimes[i]);
    }
    printf("]\n[");
    for(int i = 0; i < 10; i++){
        printf("%f, ", blasTimes[i]);
    }
    printf("]");
    return 0;
}