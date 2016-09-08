#include <omp.h>
#include <stdio.h>
#define CHUNKSIZE 4096
#define N    1000000000

main ()  
{

int i,j, chunk,tid;
float a[N], b[N], c[N];
double tini,tfin;

/* Some initializations */
for (i=0; i < N; i++){
  a[i] = b[i] = i * 1.0;
  c[i] = 0.0;
}
//j=0;
chunk = CHUNKSIZE;
tini=omp_get_wtime();
//#pragma omp parallel shared(a,b,c,chunk) private(i)
#pragma omp parallel shared(a,b,c) private(tid)
  {
//#pragma omp for schedule(dynamic,chunk) 
#pragma omp for  
  for (i=0; i < N; i++){
    c[i] = c[i] + a[i] + b[i];
//    printf("en c[%d]=%f\n",j,c[j]);
//    tid=omp_get_thread_num();
//    printf("estoy en %d en iteracion %d\n",tid,i);
  }
  }  /* end of parallel section */
tfin=omp_get_wtime();

  for (i=0;i<N; i+=10000){
    printf("en c[%d]=%f\n",i,c[i]);
  }
printf("demoro %f\n",tfin-tini);

}
