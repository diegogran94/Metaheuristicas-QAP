//Problema de asignacion cuadratica

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "inout.h"
//#include "greedy.h"
#include"bus_loc.h"
#include "bmb.h"
#include "ils.h"
#include "AGG.h"
#include "random_ppio.h"
#include "och.h"

#include "funciones.h"

using namespace std;

#define N_PROBLEMAS 20



void imprimir_resultados(int coste, double tiempo, string fichero){
  cout << "-------------------------------------------------" << endl;
  cout << "Problema: " << fichero << endl;
  cout << "Coste: " << coste << endl;
  cout << fixed << "Tiempo ejecucion: " << tiempo << endl;
  cout << "-------------------------------------------------" << endl;
}


int main(int argc, char const *argv[]) {

    int** matriz_distancias;
    int** matriz_flujos;
    int tam_problema;
    int* solucion;
    unsigned long semilla = 123;
    double tiempo_total=0;

    Set_random(semilla);

    string vector_problemas[N_PROBLEMAS] = {"chr20a","chr20c","chr22b","chr25a","esc32a",
      "esc64a","esc128","kra32","lipa90a","sko42","sko49","sko81","sko90","sko100f","tai64c","tai80a",
      "tai100a","tai150b","tai256c","tho150"};


    if(argc < 2){
      std::cout << "Falta el argumento(algoritmo a ejecutar)" << std::endl;
      std::cout << "1. AM(10,1.0) | 2. AM(10,0.1) | 3. AM(10,0.1,mej)" << std::endl;
      exit(0);
    }


    for(int i=0; i<N_PROBLEMAS; i++){//<----------------------------

      string problema_actual = "../tablas/" + vector_problemas[i] + ".dat";

      double t_ejecucion;
      clock_t start_time;

      lee_fichero(problema_actual, tam_problema, matriz_flujos, matriz_distancias);

      solucion = new int[tam_problema];

      //Inicializacion de la solucion
      for(int j=0; j<tam_problema; j++)
        solucion[j]=j+1;


      start_time = clock();

      switch(atoi(argv[1])){
        case 1:
          memetico(solucion, matriz_flujos, matriz_distancias, tam_problema, semilla, 10,1.0,false);
          break;
        case 2:
          memetico(solucion, matriz_flujos, matriz_distancias, tam_problema, semilla, 10,0.1,false);
          break;
        case 3:
          memetico(solucion, matriz_flujos, matriz_distancias, tam_problema, semilla, 10,0.1,true);
          break;
        default:
          std::cout << "Ese argumento no corresponde a  ningun algoritmo" << std::endl;
      }


      t_ejecucion = clock()- start_time;
      t_ejecucion= t_ejecucion / CLOCKS_PER_SEC;

      int coste_sol = coste(solucion, matriz_flujos, matriz_distancias, tam_problema);

      imprimir_resultados(coste_sol,t_ejecucion,problema_actual);

      tiempo_total += t_ejecucion;


    }

    std::cout << fixed << "Tiempo total: " << tiempo_total << std::endl;

    delete [] matriz_flujos;
    delete [] matriz_distancias;
    delete [] solucion;

    return 0;
}
