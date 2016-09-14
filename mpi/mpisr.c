#include "mpi.h"
#include <stdio.h>
#include <string.h>

#define BUFF 1024

int main(int argc, char *argv[])  {
int numtasks, rank, dest, source, rc, count, tag=1;  
char inmsg, outmsg='x';
char grande[BUFF];
char chico[BUFF];

strcpy(grande, "esto no es grande");

MPI_Status Stat;
MPI_Request reqs;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

if (rank == 0) {
  dest = 1;
  source = 1;

  rc = MPI_Send(grande, BUFF, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
strcpy(grande, "esto no es grande de nuevo");
  rc = MPI_Send(grande, BUFF, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

  rc = MPI_Recv(chico, BUFF, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
printf("chico vale %s\n",chico);
  rc = MPI_Recv(chico, BUFF, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
printf("chico vale %s\n",chico);

/*
  rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
  outmsg='y';
  rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
*/
//  rc = MPI_Isend(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD,&reqs);
//  rc = MPI_Ssend(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
printf("Envie desde 0\n");
//  rc = MPI_Recv(chico, 1000, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
/*
  rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
printf("Recibi desde 0 %c\n",inmsg);
  rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
printf("Recibi desde 0 %c\n",inmsg);
*/
  } 

else if (rank == 1) {
  dest = 0;
  source = 0;

  rc = MPI_Send(grande, BUFF, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
strcpy(grande, "esto no es grande de nuevo");
  rc = MPI_Send(grande, BUFF, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

  rc = MPI_Recv(chico, BUFF, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
printf("chico vale %s\n",chico);
  rc = MPI_Recv(chico, BUFF, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
printf("chico vale %s\n",chico);

//  rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
//  rc = MPI_Isend(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD,&reqs);
  //rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
  //outmsg='y';
  //rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
//  rc = MPI_Ssend(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
/*
printf("Envie desde 1\n");
  rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
printf("Recibi desde 1 %c\n",inmsg);
  rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
printf("Recibi desde 1 %c\n",inmsg);
*/
}
rc = MPI_Get_count(&Stat, MPI_CHAR, &count);
printf("Task %d: Received %d char(s) from task %d with tag %d \n",
       rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);

MPI_Finalize();
return 0;
}

