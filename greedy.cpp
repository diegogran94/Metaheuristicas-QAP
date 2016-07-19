#include <iostream>
#include <limits>

#include "greedy.h"

using namespace std;

void greedy(int* &solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades){

	//Creamos los vectores de potencial de distancias y de flujos

	int* fi= new int[n_unidades];
	int* di= new int[n_unidades];

	//Inicializar los vectores a 0
	for(int i=0; i<n_unidades; i++){
			fi[i]=0;
			di[i]=0;
	}

	for(int i=0; i<n_unidades; i++){
		for(int j=0; j<n_unidades; j++){
			fi[i]+=matriz_flujos[i][j];
			di[i]+=matriz_distancias[i][j];
		}
	}

	//Creamos dos vectores que indican si ya se ha elegido cada valor de los vectores anteriores
	int* f_elegidos= new int[n_unidades];
	int* d_elegidos= new int[n_unidades];

	for(int i=0; i<n_unidades; i++){
		f_elegidos[i]=0;
		d_elegidos[i]=0;
	}

	int f_max;//Valor maximo en el vector de flujos
	int d_min;//Valor minimo en el vector de distancias
	int u=0;//Indice del maximo valor en el vector de flujos
	int l=0;//Indice del minimo valor en el vector de distancias

	for(int i=0; i<n_unidades; i++){

		f_max = std::numeric_limits<int>::min();
		d_min = std::numeric_limits<int>::max();

		//Calcular unidad con max flujo y localicacion con min dist
		for(int j=0; j<n_unidades; j++){
			if(fi[j] > f_max && f_elegidos[j]==0){
				f_max = fi[j];
				u=j;
			}

			if(di[j] < d_min && d_elegidos[j]==0){
				d_min = di[j];
				l=j;
			}
		}
		solucion[u] = l+1;//Deben de ser valores de 1-n_unidades
		f_elegidos[u]=1;
		d_elegidos[l]=1;

	}

	free(fi);
	free(di);
}
