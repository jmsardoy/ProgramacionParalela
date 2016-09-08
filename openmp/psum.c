#include <omp.h>
#include <stdio.h>
#define CHUNKSIZE 4096
#define N  10000000

int main (int argc, char *argv[]){

	int i,j, chunk,tid;
	float a[N];
    float suma = 0.0;
   	double tini,tfin;
    if(argc>1){
        omp_set_num_threads(atoi(argv[1]));
    }
    else{
        omp_set_num_threads(1);
    }
    /* Some initializations */
    for (i=0; i < N; i++){
       a[i]= 1.0;
    }
    chunk = CHUNKSIZE;
    tini=omp_get_wtime();
    #pragma omp parallel for shared(a) reduction(+:suma)
        for (i=0; i < N; i++){
        	suma += a[i];
        }
    tfin=omp_get_wtime();
    printf("suma = %f\n",suma);
    printf("demoro %f\n",tfin-tini);

}
