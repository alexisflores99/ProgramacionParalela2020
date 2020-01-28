/*
 ============================================================================
 Name        : Project_MPI_Reduc_Minimo.c
 Author      : alexis
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
 */
#include "mpi.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <time.h>
using namespace std;
double minimo(vector<double> vec, int n);
int main(int argc, char *argv[]) {
    int tama, rank, size;
    double local_min,total_min;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(NULL));
    if (argc < 2) {
        if (rank == 0) {
            cout << "No se ha especificado numero de elementos, multiplo "
            		"de la cantidad de entrada, por defecto sera " << size * 100;
            cout << "\nUso: <ejecutable> <cantidad>" << endl;
        }
        tama = size * 100;
    } else {
        tama = atoi(argv[1]);
        if (tama < size)
        	tama = size;
        else {
            int i = 1, num = size;
            while (tama > num) {
                ++i;
                num = size*i;
            }
            if (tama != num) {
                if (rank == 0)
                    cout << "Cantidad cambiada a " << num << endl;
                tama = num;
            }
        }
    }
    vector<double> VectorA, VectorALocal;
	VectorA.resize(tama, 0);
	VectorALocal.resize(tama/size, 0);
	if (rank == 0) {
		for (long i = 0; i < tama; ++i) {
			VectorA[i] = 100+rand()%900; // Generamos numeros aleatorios de 100 a 999
		}
	}
	MPI_Scatter(&VectorA[0], // Valores a compartir
	            tama/size, // Cantidad que se envia a cada proceso
	            MPI_DOUBLE, // Tipo del dato que se enviara
	            &VectorALocal[0], // Variable donde recibir los datos
	            tama/size, // Cantidad que recibe cada proceso
	            MPI_DOUBLE, // Tipo del dato que se recibira
	            0,  // proceso principal que reparte los datos
	            MPI_COMM_WORLD); // Comunicador (En este caso, el global)
	local_min=minimo(VectorALocal,tama/size);
	MPI_Reduce(&local_min, // Elemento a enviar
	            &total_min, // Variable donde se almacena la reunion de los datos
				1, // Cantidad de datos a reunir
	            MPI_DOUBLE, // Tipo del dato que se reunira
	            MPI_MIN, // Operacion aritmetica a aplicar
	            0, // Proceso que recibira los datos
	            MPI_COMM_WORLD); // Comunicador
	 if(rank==0){
		 cout<<"Vector:"<<endl;
		 for(int i=0;i<tama;i++){
			 cout<<VectorA[i]<<" ";
		 }
		 cout<<"Minimo de A: "<<total_min<<endl;
	 }
	 MPI_Finalize();
	return 0;
}

double minimo(vector<double> vec, int n){
	int min=vec[0];
	for (int i=0; i<n;i++){
		if (min > vec[i]){
			min=vec[i];
		}
	}
	return min;
}
