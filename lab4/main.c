#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <gsl/gsl_blas.h>

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
    double naiveTimes[10] = {0};
    double betterTimes[10] = {0};
    double blasTimes[10] = {0};
    for(int size = 150; size <= 1500; size += 150){
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
        naiveTimes[size/150] = ((double)(naive_end - naive_start)) / CLOCKS_PER_SEC;;

        clock_t better_start = clock();
        better_multiply(size, mat1, mat2, res);
        clock_t better_end = clock();
        betterTimes[size/150] = ((double)(better_end - better_start)) / CLOCKS_PER_SEC;

        clock_t blas_start = clock();
        clock_t blas_end = clock();
        for(int i = 0; i < size; i++) {
            free(res[i]);
            free(mat1[i]);
            free(mat2[i]);
        }
        free(res);
        free(mat1);
        free(mat2);
    }
    for(int i = 0; i < 10; i++){
        printf("Naive %d: %f \n", i, naiveTimes[i]);
        printf("Better %d: %f \n", i, betterTimes[i]);
    }



//    double a[] = { 0.11, 0.12, 0.13,
//                   0.21, 0.22, 0.23 };
//
//    double b[] = { 1011, 1012,
//                   1021, 1022,
//                   1031, 1032 };
//
//    double c[] = { 0.00, 0.00,
//                   0.00, 0.00 };
//
//    gsl_matrix_view A = gsl_matrix_view_array(a, 2, 3);
//    gsl_matrix_view B = gsl_matrix_view_array(b, 3, 2);
//    gsl_matrix_view C = gsl_matrix_view_array(c, 2, 2);
//
//    /* Compute C = A B */
//
//    gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,
//                    1.0, &A.matrix, &B.matrix,
//                    0.0, &C.matrix);
//
//    printf ("[ %g, %g\n", c[0], c[1]);
//    printf ("  %g, %g ]\n", c[2], c[3]);


    return 0;
}
