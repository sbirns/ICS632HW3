#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


int main(int argc, char **argv) {

    // Parse comand-line arguments
    if (argc != 2) {
        fprintf(stderr,"Usage: %s <array size>\n", argv[0]);
        exit(1);
    }

    int N = strtol(argv[1], NULL, 10);

    if (N <= 0) {
        fprintf(stderr,"Invalid arguments\n");
        exit(1);
    }

    // Allocate and initialize array
    fprintf(stderr,"Array allocation and initialization...\n");
    int *A = (int *)malloc(N * N * sizeof(int));
    int *B = (int *)malloc(N * N * sizeof(int));
    int *C = (int *)calloc(N * N, sizeof(int));
    for (int i=0; i < N*N; i++) {
        A[i] = rand() % 1024;
        B[i] = rand() % 1024;
    }

    // Matrix multiplication
    fprintf(stderr,"Matrix multiplication...\n");
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    int i,j,k;
    for (i=0; i < N; i++) {
      for (k=0; k < N; k++) {
        for (j=0; j < N; j++) {
          C[i*N + j] += A[i*N + k] * B[k*N + j];
        }
      }
    }
    gettimeofday(&end, NULL);

    double elapsed = (1.0E+6 * (end.tv_sec - begin.tv_sec) + end.tv_usec -  begin.tv_usec) / 1.0E+6;

    fprintf(stdout, "%.2lf\n", elapsed);

}