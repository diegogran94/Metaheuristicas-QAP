/*
  Funciones necesarias para los algoritmos
*/

#include<iostream>

#include"funciones.h"
#include "random_ppio.h"

//Primera aplroximación de la funcion que calcula el coste de una solucion dada
//Se puede mejorar teniendo en cuenta que las matrices dadas son simetricas
int coste(int* solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades){

    int result = 0;

    for(int i=0; i<n_unidades; i++){
      for(int j=0; j<n_unidades; j++){
        //Hay que controlar que no se acceda a la posicion n+1 de la matriz distancias
        //ya que en el vector solucion se encuentra el elemento n+1 (n = tamaño del problema)
        result += matriz_flujos[i][j] * matriz_distancias[solucion[i]-1][solucion[j]-1];
      }
    }

    return result;
}

void genera_sol_random(int *sol, int n_unidades){

  int pos, aux;

  for(int i=0; i<n_unidades; i++)
    sol[i] = i+1;

  for(int j=0; j<n_unidades; j++){
    pos = Randint(j,n_unidades-1);
    aux=sol[j];
    sol[j]=sol[pos];
    sol[pos]=aux;
  }

}
