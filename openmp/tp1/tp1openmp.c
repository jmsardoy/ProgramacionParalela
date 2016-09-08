#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define	VECLEN	100000000

int vector[VECLEN];
int ndiv;

void join(int *A, int *B, int half, int size){
	int i;
	for(i=0; i<size-half;i++){
		*(B+i) += *(A+half-1);
	}
}
void sumarVectorSeq(int *A, int size){
	int i;
	for(i = 1; i<size; i++){
		*(A+i) += *(A+i-1);
	}


}
void sumarVector(int *A, int size){
	int half = size/2;
	int *B = A + half;
	
	if (size<=ndiv){
		sumarVectorSeq(A,size);
	}
	else{

		#pragma omp task firstprivate(A,half)
		sumarVector(A, half);
		#pragma omp task firstprivate(B,half,size)
		sumarVector(B, size-half);
		#pragma omp taskwait
		join(A,B,half,size);

	}

}

int main(int argc, char const *argv[])
{
	int i, nthreads;
	double tini,tfin;
	if (argc>1){
		nthreads = atoi(argv[1]);
	}else{
		nthreads=1;
	}
	if (argc>2){
		ndiv = atoi(argv[2]);
	}else{
		ndiv = 1000;
	}

	omp_set_num_threads(nthreads);
	//lleno el vector de unos

	for (i = 0; i < VECLEN; i++)
	{	
		vector[i] = 1;
	}
	tini = omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp single
		sumarVector(vector,VECLEN);
	}
	tfin = omp_get_wtime();
	/*for (i = 0; i < VECLEN; i+=100000)
	{	
		printf("%i\n", vector[i]);
	}*/
	printf("Tiempo: %f\n", tfin-tini);
	
	return 0;
}