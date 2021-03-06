#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

int main(int argc, char **argv) {
    int i,j,k;

    // Parse comand-line arguments
    if (argc != 3) {
        fprintf(stderr,"Usage: %s <array size> <num threads>\n", argv[0]);
        exit(1);
    }

    int N = strtol(argv[1], NULL, 10);
    int p = strtol(argv[2], NULL, 10);

    if (N <= 0 || p <= 0) {
        fprintf(stderr,"Invalid arguments\n");
        exit(1);
    }

    // set number of threads
    omp_set_num_threads(p);

    // Allocate and initialize array
    fprintf(stderr,"Array allocation and initialization...\n");
    int *A = (int *)malloc(N * N * sizeof(int));
    int *B = (int *)malloc(N * N * sizeof(int));
    int *C = (int *)calloc(N * N, sizeof(int));
    int *sequential_C;
    #ifdef CHECK
        sequential_C = (int *)calloc(N * N, sizeof(int));
    #endif

    for (i=0; i < N*N; i++) {
        A[i] = rand() % 1024;
        B[i] = rand() % 1024;
    }

    // Matrix multiplication
    fprintf(stderr,"Matrix multiplication...\n");
    struct timeval begin, end;
    gettimeofday(&begin, NULL);

    #ifdef PARALLEL_I
        #pragma omp parallel for private(j,k) shared(A,B,C)
        for (i=0; i < N; i++) {            
            for (k=0; k < N; k++) {                
                for (j=0; j < N; j++) {
                    C[i*N + j] += A[i*N + k] * B[k*N + j];
                }
            }
        }
        
        #ifdef CHECK
            printf("Checking enabled..\n");
            for (i=0; i < N; i++) {
                for (k=0; k < N; k++) {
                    for (j=0; j < N; j++) {
                        sequential_C[i*N + j] += A[i*N + k] * B[k*N + j];
                    }
                }
            }

            for (i=0; i < N; i++) {
                for (k=0; k < N; k++) {
                    for (j=0; j < N; j++) {
                        if (sequential_C[i*N + j] != C[i*N + j]) {
                            fprintf(stderr,"Parallel computation incorrect\n");
                            exit(1);
                        }
                    }
                }
            }                        
        #endif

    #elif PARALLEL_K
        for (i=0; i < N; i++) {  
            #pragma omp parallel for private(j) shared(A,B,C)      
            for (k=0; k < N; k++) {                
                for (j=0; j < N; j++) {
                    #pragma omp atomic
                    C[i*N + j] += A[i*N + k] * B[k*N + j];
                }
            }
        }
        
        #ifdef CHECK
            printf("Checking enabled..\n");
            for (i=0; i < N; i++) {
                for (k=0; k < N; k++) {
                    for (j=0; j < N; j++) {
                        sequential_C[i*N + j] += A[i*N + k] * B[k*N + j];
                    }
                }
            }

            for (i=0; i < N; i++) {
                for (k=0; k < N; k++) {
                    for (j=0; j < N; j++) {
                        if (sequential_C[i*N + j] != C[i*N + j]) {
                            fprintf(stderr,"Parallel computation incorrect\n");
                            exit(1);
                        }
                    }
                }
            }                        
        #endif

    #elif PARALLEL_J
        for (i=0; i < N; i++) {            
            for (k=0; k < N; k++) {      
                #pragma omp parallel for shared(A,B,C)          
                for (j=0; j < N; j++) {
                    C[i*N + j] += A[i*N + k] * B[k*N + j];
                }
            }
        }
        
        #ifdef CHECK
            printf("Checking enabled..\n");
            for (i=0; i < N; i++) {
                for (k=0; k < N; k++) {
                    for (j=0; j < N; j++) {
                        sequential_C[i*N + j] += A[i*N + k] * B[k*N + j];
                    }
                }
            }

            for (i=0; i < N; i++) {
                for (k=0; k < N; k++) {
                    for (j=0; j < N; j++) {
                        if (sequential_C[i*N + j] != C[i*N + j]) {
                            fprintf(stderr,"Parallel computation incorrect\n");
                            exit(1);
                        }
                    }
                }
            }                        
        #endif

    #endif
    gettimeofday(&end, NULL);

    double elapsed = (1.0E+6 * (end.tv_sec - begin.tv_sec) + end.tv_usec -  begin.tv_usec) / 1.0E+6;

    fprintf(stdout, "%.2lf\n", elapsed);

}

