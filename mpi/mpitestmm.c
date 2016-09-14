#include "mpi.h"
#include "omp.h"
#include <stdio.h>
#include <stdlib.h>

#define RANGE    4000
#define PROCS    4 
#define COLS     RANGE/PROCS
#define ROWS     RANGE/PROCS

int main (int argc, char *argv[])
{
int	numtasks,              /* number of tasks in partition */
	taskid,                /* a task identifier */
	source,                /* task id of message source */
	dest,                  /* task id of message destination */
	g,i, j, k, h,rc,          /* misc */
        n,alg;                 /* matrix size y frac = n /p */
        
double	a[ROWS][RANGE],           /* matrix A to be multiplied */
	b[RANGE][COLS],           /* matrix B to be multiplied */
	bw[RANGE][COLS],           /* matrix B to be multiplied */
	c[ROWS][RANGE];           /* result matrix C */

double inib,finb,inic,finc,inig,fing;

MPI_Status status;
MPI_Request reqs[PROCS];
MPI_Status statuss[PROCS];

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
/*
if (numtasks < PROCS ) {
  printf("Need at least %d MPI tasks. Quitting...\n",PROCS);
  MPI_Abort(MPI_COMM_WORLD, rc);
  exit(1);
}
*/ 
if (argc>1) {
   alg=atoi(argv[1]);
} else {
   alg=1;
}

  printf("Algoritmo %d .\n",alg);
    
   for (i=0;i<ROWS;i++) 
     for (j=0;j<RANGE;j++) {
         a[i][j]=1.0;
         c[i][j]=0.0;
   }

   for (i=0;i<RANGE;i++) 
     for (j=0;j<COLS;j++) 
         b[i][j]=1.0;

   inig=MPI_Wtime();
   switch (alg){
   case 1:
   for (h=0;h<numtasks;h++) {
       if (taskid==h) {
          for (i=0;i<RANGE;i++) 
             for (j=0;j<COLS;j++) 
                bw[i][j]=b[i][j];
       }
       inib=MPI_Wtime();
       MPI_Bcast(bw, RANGE*COLS, MPI_DOUBLE,h, MPI_COMM_WORLD);
       finb=MPI_Wtime();
       printf("Se hizo el BCAST en nodo %d master %d icon tiempo %f.\n",taskid,h,finb-inib);
       inic=MPI_Wtime();
       for (i=0;i<COLS;i++){
              for (k=0;k<RANGE;k++){
           for (j=0;j<COLS;j++){
                  c[i][j+h*COLS]=c[i][j+h*COLS]+a[i][k]*bw[k][j];
              }
           }
       }
       finc=MPI_Wtime();
       printf("Se hizo el comp en nodo %d icon tiempo %f.\n",taskid,finc-inic);
   }break;
   case 2:
   for (h=0;h<numtasks;h++) {
      if (h==taskid) {
         inib=MPI_Wtime();
         for(k=0;k<numtasks;k++){
            if (k!=h) {
               MPI_Send(&b, RANGE*COLS, MPI_DOUBLE, k, 1, MPI_COMM_WORLD);
            } else {
               for (i=0;i<RANGE;i++){
                 for (j=0;j<COLS;j++){
                  bw[i][j]=b[i][j];
                 }
               }
            }
         }
         finb=MPI_Wtime();
         printf("Hice los envios desde nodo %d hacia el resto con tiempo %f.\n",taskid,finb-inib);
      } else {
        inib=MPI_Wtime();
        MPI_Recv(&bw, RANGE*COLS, MPI_DOUBLE, h, 1, MPI_COMM_WORLD, &status);     
        finb=MPI_Wtime();
        printf("Recibi  en nodo %d desde  %d con tiempo %f.\n",taskid,h,finb-inib);
      }
      inic=MPI_Wtime();
      for (i=0;i<COLS;i++){
         for (k=0;k<RANGE;k++){
            for (j=0;j<COLS;j++){
                 c[i][j+h*COLS]=c[i][j+h*COLS]+a[i][k]*bw[k][j];
            }
         }
       }
       finc=MPI_Wtime();
       printf("Se hizo el comp en nodo %d icon tiempo %f.\n",taskid,finc-inic);
   }break;
   case 3:
   g=0;
   for (h=0;h<numtasks;h++) {
      if (h==taskid) {
         inib=MPI_Wtime();
         for(k=0;k<numtasks;k++){
            if (k!=h) {
               MPI_Isend(&b, RANGE*COLS, MPI_DOUBLE, k, 1, MPI_COMM_WORLD,&reqs[g]);
               g++;
            } else {
               for (i=0;i<RANGE;i++){
                  for (j=0;j<COLS;j++){
                     bw[i][j]=b[i][j];
                  }
               }
            }
         }
         finb=MPI_Wtime();
         printf("Hice los envios desde nodo %d hacia el resto con tiempo %f.\n",taskid,finb-inib);
//         MPI_Waitall(PROCS-1, reqs, statuss);
      } else {
         inib=MPI_Wtime();
         MPI_Recv(&bw, RANGE*COLS, MPI_DOUBLE, h, 1, MPI_COMM_WORLD, &status);     
         finb=MPI_Wtime();
         printf("Recibi  en nodo %d desde  %d con tiempo %f.\n",taskid,h,finb-inib);
      }
      inic=MPI_Wtime();
      for (i=0;i<COLS;i++){
         if (h==taskid) 
            MPI_Waitall(PROCS-1, reqs, statuss);
         for (k=0;k<RANGE;k++){
            for (j=0;j<COLS;j++){
               c[i][j+h*COLS]=c[i][j+h*COLS]+a[i][k]*bw[k][j];
            }
         }
       }
       finc=MPI_Wtime();
       printf("Se hizo el comp en nodo %d icon tiempo %f.\n",taskid,finc-inic);
//       if (h==taskid) 
//          MPI_Waitall(PROCS-1, reqs, statuss);
   }break;
   case 4:
   for (h=0;h<numtasks;h++) {
       inib=MPI_Wtime();
       MPI_Bcast(b, RANGE*COLS, MPI_DOUBLE,h, MPI_COMM_WORLD);
       finb=MPI_Wtime();
       printf("Se hizo el BCAST en nodo %d master %d icon tiempo %f.\n",taskid,h,finb-inib);
       inic=MPI_Wtime();
       omp_set_num_threads(4);
#pragma omp parallel for private(i,j,k) 
       for (i=0;i<COLS;i++){
              for (k=0;k<RANGE;k++){
           for (j=0;j<COLS;j++){
                  c[i][j+h*COLS]=c[i][j+h*COLS]+a[i][k]*b[k][j];
              }
           }
       }
       finc=MPI_Wtime();
       printf("Se hizo el comp en nodo %d icon tiempo %f.\n",taskid,finc-inic);
   }break;
   }

   MPI_Barrier(MPI_COMM_WORLD);
   if (taskid==0){
      fing=MPI_Wtime();
      printf("Se hizo el comp global con tiempo %f.\n",fing-inig);
   }
   MPI_Finalize();

   for (i=0;i<ROWS;i++) 
     for (j=0;j<RANGE;j++) 
         if (c[i][j]!=RANGE*1.0)
            printf("Error en proc %d fila %d col %d %f\n",taskid,i,j,c[i][j]);
}
