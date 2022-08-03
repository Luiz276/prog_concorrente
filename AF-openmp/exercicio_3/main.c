#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

double standard_deviation(double* data, int size) {
    double avg = 0;
    #pragma omp parallel for schedule(static) reduction(+:avg)
    for (int i = 0; i < size; ++i) 
        avg += data[i];

    avg /= size;

    double sd = 0;
    #pragma omp parallel for schedule(static) reduction(+:sd)
    for (int i = 0; i < size; ++i) 
        sd += pow(data[i] - avg, 2);

    sd = sqrt(sd / (size-1));

    return sd;
}
 
int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s tamanho\n", argv[0]);
        return 1;
    }
    int tamanho = atoi(argv[1]);
    omp_set_num_threads(tamanho);
    
    double* data = malloc(tamanho*sizeof(double));
    srand(time(NULL));
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < tamanho; ++i) 
        data[i] = 100000*(rand()/(double)RAND_MAX);
    
    printf("sd: %g\n", standard_deviation(data, tamanho));

    free(data);

    return 0;
}
