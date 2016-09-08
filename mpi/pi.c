#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void main(int argc, char *argv[]){
    
    

    MPI_Init(&argc,&argv);
    double npoints = 10000;
    double circle_count =0;

    int i;
    for (i=0;i<npoints;i++){
        double x = (double)rand()/(double)RAND_MAX;
        double y = (double)rand()/(double)RAND_MAX;
        double  distance = sqrt(pow(x,2)+pow(y,2));
        if(distance <=1){
            circle_count++;
        }
    }
    double pi = 4.0*circle_count/npoints;
    printf("pi: %f\n",pi);
    MPI_Finalize();
    
    
}

