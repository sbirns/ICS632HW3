#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <omp.h>

/* Convenient macro to access array element */
#define ARRAY(x,y) A[(x)*(N+2)+(y)]
/* Convenient macro to compute array element update */
#define UPDATE(x,y) ((ARRAY(x,y) + ARRAY(x-1,y) + ARRAY(x,y-1))/3.0);

int main(int argc, char **argv) {

    // Parse command-line arguments
    if (argc != 4) {
        fprintf(stderr,"Usage: %s <array size> <num threads> <num iterations>\n", argv[0]);
        exit(1);
    }

    int N = strtol(argv[1], NULL, 10);
    int p = strtol(argv[2], NULL, 10);
    int num_iterations = strtol(argv[3], NULL, 10);

    if (N <= 0 || p <= 0 || num_iterations <=0) {
        fprintf(stderr,"Invalid arguments\n");
        exit(1);
    }

    // set number of threads
    omp_set_num_threads(p);

    // Allocate (N+2)x(N+2) array
    double *A = (double *)malloc((N+2) * (N+2) * sizeof(double));

    // Array initialization
    fprintf(stderr,"Array initialization...\n");
    srand(42);
    for (int i=0; i < N+2; i++) {
        for (int j=0; j < N+2; j++) {
            ARRAY(i,j) = (double)rand();
        }
    }

    // Loop for num_iterations iterations
    fprintf(stderr,"Computing all iterations...\n");
    struct timeval begin, end;
    gettimeofday(&begin, NULL);

    for (int iter = 0; iter < num_iterations; iter++) {
        int i,j,k;
        for (i = 1; i < N+1; i++) {
            #pragma omp parallel for
            for (j = 1; j < i+1; j++) {
                ARRAY(i-j+1, j) = UPDATE(i-j+1, j);
            }
        }

        for (j = 2; j < N+1; j++) {
            #pragma omp parallel for
            for (k = i-1; k > j-1; k--) {
                ARRAY(k, j+N-k) = UPDATE(k, j+N-k);
            }
        }
    }

    gettimeofday(&end, NULL);

    double elapsed = (1.0E+6 * (end.tv_sec - begin.tv_sec) + end.tv_usec -  begin.tv_usec) / 1.0E+6;
    fprintf(stdout, "time: %.2lf\n", elapsed);

    // Compute and print the sum of elements for 
    // correctness checking (may overflow, whatever)
    double checksum =0;
    for (int i=1; i < N+1; i++) {
        for (int j=1; j < N+1; j++) {
            checksum += ARRAY(i,j);
        }
    }
    fprintf(stdout,"checksum: %.10f\n",checksum);

    exit(0);
}