/*
  Búsqueda local reiterada
*/

#include <iostream>

#include "ils.h"
#include "random_ppio.h"
#include "bus_loc.h"


void mutacion_ils(int *solucion, int n_unidades){
  //Esta mutación consiste en mezclar los elementos de una sublista de tamaño n/4
  int tam_sublista = (int) n_unidades/4;
  int pos_i = Randint(0,n_unidades-tam_sublista-1);

  int *sublista = new int [tam_sublista];

  //Introducimos los valores correspondientes en la sublista
  int it = 0;
  for(int i=pos_i; i<tam_sublista+pos_i; i++){
    sublista[it] = solucion[i];
    it++;
  }

  //Mezclamos los elementos de la sublista
  int pos_rand, aux;
  for(int j=0; j<tam_sublista; j++){
    pos_rand=Randint(j,tam_sublista-1);
    aux=sublista[j];
    sublista[j]=sublista[pos_rand];
    sublista[pos_rand]=aux;
  }

  //Introducimos los nuevos valores de la sublista en el vector solucion
  int it2 = 0;
  for(int i=pos_i; i<tam_sublista+pos_i; i++){
    solucion[i] = sublista[it2];
    it++;
  }


}


void ils(int* &solucion, int** matriz_flujos, int** matriz_distancias,
  int n_unidades, unsigned long semilla){

    Set_random(semilla);

    int *mejor_solucion, *sol_act;
    int mejor_coste, coste_act;

    sol_act = new int[n_unidades];
    mejor_solucion = new int[n_unidades];

    //Generar solucion inicial aleatoria
    genera_sol_random(sol_act, n_unidades);
    //BL sobre la sol generada
    busqueda_local(sol_act, matriz_flujos, matriz_distancias, n_unidades);

    mejor_coste=coste(sol_act, matriz_flujos, matriz_distancias, n_unidades);

    for(int i=0; i<N_ITERS; i++){
      std::cout << "Iteracion: " << i << std::endl;
      //Mutacion
      mutacion_ils(sol_act, n_unidades);
      //Aplicar bl sobre ella
      busqueda_local(sol_act, matriz_flujos, matriz_distancias, n_unidades);

      coste_act=coste(sol_act, matriz_flujos, matriz_distancias, n_unidades);
      std::cout << "Coste actual: " << coste_act << std::endl;

      if(coste_act<mejor_coste){
        //Sustituir la mejo sol  por la actual
        mejor_coste=coste_act;
        for(int j=0; j<n_unidades; j++)
          mejor_solucion[j]=sol_act[j];

        std::cout << "Mejora encontrada con coste " << mejor_coste << " . Actualizando... " << std::endl;
      }
    }

    //Devolvemos la mejor solucion
    for(int k=0; k<n_unidades; k++){
      solucion[k] = mejor_solucion[k];
    }

}
