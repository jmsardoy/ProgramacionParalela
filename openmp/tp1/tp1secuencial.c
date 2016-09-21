#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define VECLEN 100000000

int vector[VECLEN];

void main(){
    int i = 0;
    double tini, tfin;
    for(i=0;i<VECLEN;i++){
        vector[i] = 1;
    }
    tini = omp_get_wtime();
    for(i=1;i<VECLEN;i++){
        vector[i] += vector[i-1];
    }
    tfin = omp_get_wtime();
    printf("time: %f\n",tfin-tini);
}
