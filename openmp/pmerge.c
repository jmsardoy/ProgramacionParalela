#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define VECLEN 1000000

int a[VECLEN];
int ndv;

void SeqSort(int *A, int size) {
   int i,j,tmp,posmin;
   for(i=0;i<size;i++){
//      printf("en seq %d %d ",i,*(A+i));
      posmin=i;
      for(j=i+1;j<size;j++){
         if (*(A+j) < *(A+posmin)){
//            printf("%d %d %d %d ",j,posmin,*(A+j),*(A+posmin));
            posmin=j;
         }
      }
//swap
      tmp=*(A+i);
      *(A+i)=*(A+posmin);
      *(A+posmin)=tmp;
//      printf("\n%d %d %d %d\n",i,posmin,*(A+i),tmp);
   }
}

void Merge(int *A, int *B, int half, int size,int stg) {
   int i,ia,ib;
   int tmp[size];
   ia=0;
   ib=half;
   for (i=0;i<half;i++)
       tmp[i]=A[i];
   for (i=0;i<size-half;i++)
       tmp[half+i]=B[i];
   int *dato;
//printf("entro en merge con %d %d\n",size,stg);
   for (i=0;i<size;i++){
//      printf("%d %d\n",*(A+ia),*(B+ib));
      if (ia>=half){
         *(A+i)=*(tmp+ib);
         ib++;
      } else {
         if (ib>=size){
            *(A+i)=*(tmp+ia);
            ia++;
         } else {
           if ((*(tmp+ia)) < (*(tmp+ib))){
              *(A+i)=*(tmp+ia);
              ia++;
           } else {
              *(A+i)=*(tmp+ib);
              ib++;
           }
         }
      }
   }
}

void Sort(int *A, int size, int stg) {
   int half = size/2;
   int *B = A + half;
   int thid;
   thid=omp_get_thread_num();
//   printf("estoy en sort de hilo %d %d %d\n",thid,stg,ndv);

   if ( size<ndv ) {
      SeqSort (A,  size ) ;
   } else {
#pragma omp task firstprivate (A,half,stg)
{
      Sort (A, half, stg+1 ) ;
}
#pragma omp task firstprivate (B,half,size,stg)
{
      Sort (B, size-half, stg+1);
}
#pragma omp taskwait
      Merge (A, B, half, size, stg ) ;
   }
}

int main (int argc, char *argv[]) {
    int i,nth,stg;
    double tini,tfin;

    if (argc>1){
       nth=atoi(argv[1]);
    } else {
       nth=1;
    }
    if (argc>2){
       ndv=atoi(argv[2]);
    } else {
       ndv=128;
    }
    omp_set_num_threads(nth);

    for (i=0;i<VECLEN;i++)
       a[i]=rand();

    tini=omp_get_wtime();
#pragma omp parallel
{
    #pragma omp single 
    Sort(a,VECLEN,1);
}
    tfin=omp_get_wtime();

    printf("tiempo = %f\n",tfin-tini);
/*
    for (i=0;i<VECLEN;i++)
       printf("pos %d value %d\n",i,a[i]);
*/
    return 0;
}
