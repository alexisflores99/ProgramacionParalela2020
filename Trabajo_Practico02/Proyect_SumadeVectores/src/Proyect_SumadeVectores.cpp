/*
 ============================================================================
 Name        : Proyect_SumadeVectores.c
 Author      : alexis Flores Solis
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
 */
#include <math.h> 
#include "mpi.h" 
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
	int n,rank, size;



	MPI::Init(argc, argv);
	size = MPI::COMM_WORLD.Get_size();
	rank = MPI::COMM_WORLD.Get_rank();

	double x[100],y[100],z[100];
		for(int i=0;i<100;i++){
			x[i]=rand()%100;
			y[i]=rand()%100;
		}

	double local_x[10],local_y[10],local_z[10];

	if(rank==0){
		n=100;
		for(int i=0;i<n/size;i++){
			MPI_Send(x+i*10,n/size,MPI_DOUBLE,0,i,MPI_COMM_WORLD);
			MPI_Send(y+i*10,n/size,MPI_DOUBLE,0,i,MPI_COMM_WORLD);

			z[i]=x[i]+y[i];

		}

		for(int i=1;i<n/size;i++){
			MPI_Recv(z+i*10, n/size, MPI_INT, i, i+1, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}

		for(int i=0;i<100;i++){
			cout<<z[i]<<endl;

		}


	}else{

		MPI_Recv(local_x, 10, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(local_y, 10, MPI_INT, 0, 1, MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		for(int i=0;i<10;i++){
			local_z[i]=local_x[i]+local_y[i];
		}
		MPI_Send(local_z, 10, MPI_INT, 0, rank+1, MPI_COMM_WORLD);
	}

	MPI::Finalize();
	return 0;

}

