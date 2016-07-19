/*
  Búsqueda multiarranque básica
*/

#include <iostream>
#include <limits>

#include "bmb.h"
#include "random_ppio.h"
#include "bus_loc.h"
#include "funciones.h"


void busqueda_multi_basica(int* &solucion, int** matriz_flujos, int** matriz_distancias,
  int n_unidades, unsigned long semilla){

  int *sol_generada, *mejor_solucion;
  int coste_act, mejor_coste, iter_actual;

  mejor_coste = std::numeric_limits<int>::max(); //Para que en la primera iteracion mejor_coste=coste_act


  sol_generada = new int[n_unidades];
  mejor_solucion = new int[n_unidades];

  for(iter_actual=0; iter_actual<N_SOLUCIONES; iter_actual++){
    std::cout << "Iteracion " << iter_actual << std::endl;
    //Generar solucion aleatoria
    genera_sol_random(sol_generada, n_unidades);

    //Imprimir solucion
    for(int i=0; i<n_unidades; i++)
      std::cout << sol_generada[i] << " ";
    std::cout <<  std::endl;

    //Ejecutar bl con la sol generada
    busqueda_local(sol_generada, matriz_flujos, matriz_distancias, n_unidades);

    //Calcular coste de la solucion generada
    coste_act = coste(sol_generada, matriz_flujos, matriz_distancias, n_unidades);
    std::cout << "Coste actual: " << coste_act << ". Coste mejor: " << mejor_coste << std::endl;
    //Si es mejor que la solucion mejor en el momento, sustituir
    if(coste_act < mejor_coste){
      mejor_coste=coste_act;
      for(int i=0; i<n_unidades; i++)
        mejor_solucion[i] = sol_generada[i];

      std::cout << "Mejora encontrada con coste " << mejor_coste << std::endl;
    }

  }

  //Devolvemos la mejor solucion
  for(int i=0; i<n_unidades; i++)
    solucion[i] = mejor_solucion[i];

}
