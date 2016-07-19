/*
  Algoritmo genético
*/

#include<iostream>
#include<vector>

#include"AGG.h"
#include "funciones.h"
#include"random_ppio.h"
#include "bus_loc.h"

using namespace std;

void iniciarPoblacion(vector<pair<int*, int> > &poblacion, int** matriz_flujos,
  int** matriz_distancias, int tam_problema){

  pair<int*,int> aux;
  int *una_sol;
  int un_coste;



  for(int i=0; i<TAM_POBLACION; i++){

    una_sol = new int[tam_problema];
    genera_sol_random(una_sol, tam_problema);
    un_coste=coste(una_sol, matriz_flujos, matriz_distancias, tam_problema);
    aux.first = una_sol;
    aux.second = un_coste;

    poblacion.push_back(aux);

  }
}

void cruce_pos(int *padre1 ,int *padre2, int* hijo, int n_unidades){

  vector<int> restos;

  for(int i=0; i<n_unidades; i++)
    hijo[i] = -1;
/*
  std::cout << "Padre1" << std::endl;
  for(int i=0; i<n_unidades; i++){
    std::cout << padre1[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "Padre2" << std::endl;
  for(int i=0; i<n_unidades; i++){
    std::cout << padre2[i] << " ";
  }
  std::cout << std::endl;
*/
  //Las posiciones que contengan el mismo valor en ambos padres se mantiene en el hijo
  for(int j=0; j<n_unidades; j++){
    if(padre1[j] == padre2[j]){//Si son iguales pasan al hijo
      hijo[j] = padre1[j];
    }
    else{//Si no los guardamos en restos
      restos.push_back(padre1[j]);
    }
  }
/*
  std::cout << "Restos" << std::endl;
  for(int i=0; i<restos.size(); i++){
    std::cout << restos[i] << " ";
  }
    std::cout << std::endl;
*/
  //Mezclamos el vector de restos
  int p, aux;
  for(unsigned int k=0; k<restos.size(); k++){
    do{
      p = Randint(k,restos.size()-1);//<------------------------------
    }while((unsigned int)p>restos.size()-1);
    aux = restos[k];
    restos[k] = restos[p];
    restos[p] = aux;
  }
/*
  std::cout << "Restos" << std::endl;
  for(int i=0; i<restos.size(); i++){
    std::cout << restos[i] << " ";
  }
    std::cout << std::endl;
*/
  for(int l=0; l<n_unidades; l++){
    if(hijo[l] == -1){//Si la posicion esta libre introducimos un elemento de restos
      hijo[l] = restos.front();
      restos.erase(restos.begin());
    }
  }
/*
  std::cout << "Hijo" << std::endl;
  for(int i=0; i<n_unidades; i++){
    std::cout << hijo[i] << " ";
  }
  std::cout << std::endl;
  std::cout << std::endl;
*/
}


void genetico(int* &solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades,
  unsigned long semilla){

  std::vector<pair<int*, int> > poblacion;
  vector<int*> padres;
  vector<pair<int*,int> > hijos;
  int pos_padre1, pos_padre2, it_padre1, it_padre2;
  int n_cruces = PROB_CRUCE*(TAM_POBLACION/2);
  int n_mutaciones = PROB_MUTAR*(TAM_POBLACION*n_unidades);
  int *hijo_aux1, *hijo_aux2;
  int coste_mejor_sol = 999999999;


  iniciarPoblacion(poblacion, matriz_flujos, matriz_distancias, n_unidades);
  /*
  for(int i=0; i<TAM_POBLACION; i++){
    for(int j=0; j<n_unidades; j++){
      std::cout << poblacion[i].first[j] << " ";
    }
    std::cout << std::endl;
  }
  */
  int it_actual=0;;
  while(it_actual<N_ITERS_AG){

    //Seleccion

    for(int i=0; i<TAM_POBLACION; i++){
      //Elegir dos padres diferentes
      pos_padre1 = Randint(0,TAM_POBLACION-1);
      do{
        pos_padre2 = Randint(0,TAM_POBLACION-1);
      }while(pos_padre1==pos_padre2);

      if(poblacion[pos_padre1].second > poblacion[pos_padre2].second){
        padres.push_back(poblacion[pos_padre2].first);
      }
      else{
        padres.push_back(poblacion[pos_padre1].first);
      }
    }
    /*
    std::cout << "Padres" << std::endl;
    for(int i=0; i<padres.size(); i++){
      for(int j=0; j<n_unidades; j++){
        std::cout << padres[i][j] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
    */

  //Cruce: cruzan las n_cruces primeras parejas
  it_padre1=0;
  it_padre2=1;

  //Cada pareja cruza dos veces en distinto orden
  pair<int*,int> h1, h2;
  for(int j=0; j<n_cruces; j++){
    hijo_aux1 = new int[n_unidades];//Reservar memoria para nuevo hijo
    hijo_aux2 = new int[n_unidades];
    cruce_pos(padres[it_padre1],padres[it_padre2],hijo_aux1,n_unidades);
    cruce_pos(padres[it_padre2],padres[it_padre1],hijo_aux2,n_unidades);
    h1.first = hijo_aux1;
    h2.first = hijo_aux2;
    hijos.push_back(h1);
    hijos.push_back(h2);
    it_padre1+=2;
    it_padre2+=2;
  }

  //Los padres que no cruzan permanecen en la poblacion
  pair<int*,int> p_aux;
  for(int i=2*n_cruces; i<TAM_POBLACION; i++){
    p_aux.first = padres[i];
    hijos.push_back(p_aux);
  }


  //Mutacion
  int mut_act = 0;
  int pos_i, pos_j, pos_rand, aux;
  while(mut_act < n_mutaciones){
    pos_i = Randint(0,TAM_POBLACION-1);
    pos_j = Randint(0,n_unidades-1);

    do{
      pos_rand = Randint(0,n_unidades-1);
    }while(pos_rand == pos_j);

    aux = hijos[pos_i].first[pos_rand];
    hijos[pos_i].first[pos_rand]=hijos[pos_i].first[pos_j];
    hijos[pos_i].first[pos_j] = aux;

    mut_act++;
  }

  //Reemplazamiento con elitismo
  //Buscamos el mejor cromosoma de la poblacion anterior
  int mejor_coste = 999999999;
  int pos_mejor_sol_elit = 0;

  for(unsigned int i=0; i<poblacion.size(); i++){
    if(poblacion[i].second < mejor_coste){
      mejor_coste = poblacion[i].second;
      pos_mejor_sol_elit = i;
    }
  }

  //std::cout << "Iteracion actual: " << it_actual << std::endl;
  /*
  for(unsigned int i=0; i<hijos.size(); i++){
    for(int j=0; j<n_unidades; j++){
      std::cout <<hijos[i].first[j] << " ";
    }
    std::cout << " - " << i <<std::endl;
  }
  std::cout << std::endl;
  */
  //Evaluar hijos
  int aaaa;
  for(unsigned int i=0; i<hijos.size(); i++){
    aaaa = coste(hijos[i].first, matriz_flujos, matriz_distancias, n_unidades);
    hijos[i].second = aaaa;
  }

  //Introducimos los nuevos cromosomas de la solucion
  /*
  int coste_aux;
  for(unsigned int i=0; i<poblacion.size(); i++){
    for(int j=0; j<n_unidades; j++){
      poblacion[i].first[j] = hijos[i].first[j];
    }
    coste_aux = coste(poblacion[i].first, matriz_flujos, matriz_distancias, n_unidades);
    poblacion[i].second = aux;
  }*/

  poblacion.clear();
  poblacion = hijos;


  //Guardamos la mejor solucion elitista
  int* mejor_sol_elit = new int[n_unidades];
  int coste_mejor_sol_elit;
  for(int i=0; i<n_unidades; i++){
    mejor_sol_elit[i] = poblacion[pos_mejor_sol_elit].first[i];
  }
  coste_mejor_sol_elit=poblacion[pos_mejor_sol_elit].second;



  //Introducimos en la nueva poblacion el mejor de la anterior

  for(int i=0; i<n_unidades; i++){
    poblacion[0].first[i] = mejor_sol_elit[i];
  }
  poblacion[0].second = coste_mejor_sol_elit;

  //Actualizar la mejor solucion.
  int pos_mejor_sol;
  for(unsigned i=0; i<poblacion.size(); i++){
    if(poblacion[i].second < coste_mejor_sol){
      coste_mejor_sol = poblacion[i].second;
      pos_mejor_sol=i;
    }
  }

  for(int i=0; i<n_unidades; i++){
    solucion[i] = poblacion[pos_mejor_sol].first[i];
  }
  /*
  std::cout << "Iteracion actual: " << it_actual << std::endl;

  for(unsigned int i=0; i<poblacion.size(); i++){
    for(int j=0; j<n_unidades; j++){
      std::cout <<poblacion[i].first[j] << " ";
    }
    std::cout << "- " << poblacion[i].second << " - " << i <<std::endl;
  }
  std::cout << std::endl;
  */
  //Borrar vectores auxiliares
  padres.clear();
  hijos.clear();


  it_actual++;
  }

}


//------------------------------MEMETICO--------------------------------

void memetico(int* &solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades,
  unsigned long semilla, int n_gen_bl, double prob_bl, bool mejores){

  std::vector<pair<int*, int> > poblacion;
  vector<int*> padres;
  vector<pair<int*,int> > hijos;
  int pos_padre1, pos_padre2, it_padre1, it_padre2;
  int n_cruces = PROB_CRUCE*(TAM_POBLACION/2);
  int n_mutaciones = PROB_MUTAR*(TAM_POBLACION*n_unidades);
  int *hijo_aux1, *hijo_aux2;
  int coste_mejor_sol = 999999999;


  iniciarPoblacion(poblacion, matriz_flujos, matriz_distancias, n_unidades);
  /*
  for(int i=0; i<TAM_POBLACION; i++){
    for(int j=0; j<n_unidades; j++){
      std::cout << poblacion[i].first[j] << " ";
    }
    std::cout << std::endl;
  }
  */
  int it_actual=0;
  int llamadas_por_ciclo = TAM_POBLACION+N_ITERS_BL;
  int iteraciones_totales = N_EVALS_F_OBJETIVO/llamadas_por_ciclo;

  while(it_actual<iteraciones_totales){
    //Seleccion

    for(int i=0; i<TAM_POBLACION; i++){
      //Elegir dos padres diferentes
      pos_padre1 = Randint(0,TAM_POBLACION-1);
      do{
        pos_padre2 = Randint(0,TAM_POBLACION-1);
      }while(pos_padre1==pos_padre2);

      if(poblacion[pos_padre1].second > poblacion[pos_padre2].second){
        padres.push_back(poblacion[pos_padre2].first);
      }
      else{
        padres.push_back(poblacion[pos_padre1].first);
      }
    }
    /*
    std::cout << "Padres" << std::endl;
    for(int i=0; i<padres.size(); i++){
      for(int j=0; j<n_unidades; j++){
        std::cout << padres[i][j] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
    */

  //Cruce: cruzan las n_cruces primeras parejas
  it_padre1=0;
  it_padre2=1;

  //Cada pareja cruza dos veces en distinto orden
  pair<int*,int> h1, h2;
  for(int j=0; j<n_cruces; j++){
    hijo_aux1 = new int[n_unidades];//Reservar memoria para nuevo hijo
    hijo_aux2 = new int[n_unidades];
    cruce_pos(padres[it_padre1],padres[it_padre2],hijo_aux1,n_unidades);
    cruce_pos(padres[it_padre2],padres[it_padre1],hijo_aux2,n_unidades);
    h1.first = hijo_aux1;
    h2.first = hijo_aux2;
    hijos.push_back(h1);
    hijos.push_back(h2);
    it_padre1+=2;
    it_padre2+=2;
  }

  //Los padres que no cruzan permanecen en la poblacion
  pair<int*,int> p_aux;
  for(int i=2*n_cruces; i<TAM_POBLACION; i++){
    p_aux.first = padres[i];
    hijos.push_back(p_aux);
  }


  //Mutacion
  int mut_act = 0;
  int pos_i, pos_j, pos_rand, aux;
  while(mut_act < n_mutaciones){
    pos_i = Randint(0,TAM_POBLACION-1);
    pos_j = Randint(0,n_unidades-1);

    do{
      pos_rand = Randint(0,n_unidades-1);
    }while(pos_rand == pos_j);

    aux = hijos[pos_i].first[pos_rand];
    hijos[pos_i].first[pos_rand]=hijos[pos_i].first[pos_j];
    hijos[pos_i].first[pos_j] = aux;

    mut_act++;
  }

  //Reemplazamiento con elitismo
  //Buscamos el mejor cromosoma de la poblacion anterior
  int mejor_coste = 999999999;
  int pos_mejor_sol_elit = 0;

  for(unsigned int i=0; i<poblacion.size(); i++){
    if(poblacion[i].second < mejor_coste){
      mejor_coste = poblacion[i].second;
      pos_mejor_sol_elit = i;
    }
  }

  //std::cout << "Iteracion actual: " << it_actual << std::endl;
  /*
  for(unsigned int i=0; i<hijos.size(); i++){
    for(int j=0; j<n_unidades; j++){
      std::cout <<hijos[i].first[j] << " ";
    }
    std::cout << " - " << i <<std::endl;
  }
  std::cout << std::endl;
  */
  //Evaluar hijos
  int c_aux;
  for(unsigned int i=0; i<hijos.size(); i++){
    c_aux = coste(hijos[i].first, matriz_flujos, matriz_distancias, n_unidades);
    hijos[i].second = c_aux;
  }

  //Introducimos los nuevos cromosomas de la solucion
  /*
  int coste_aux;
  for(unsigned int i=0; i<poblacion.size(); i++){
    for(int j=0; j<n_unidades; j++){
      poblacion[i].first[j] = hijos[i].first[j];
    }
    coste_aux = coste(poblacion[i].first, matriz_flujos, matriz_distancias, n_unidades);
    poblacion[i].second = aux;
  }*/

  poblacion.clear();
  poblacion = hijos;


  //Guardamos la mejor solucion elitista
  int* mejor_sol_elit = new int[n_unidades];
  int coste_mejor_sol_elit;
  for(int i=0; i<n_unidades; i++){
    mejor_sol_elit[i] = poblacion[pos_mejor_sol_elit].first[i];
  }
  coste_mejor_sol_elit=poblacion[pos_mejor_sol_elit].second;



  //Introducimos en la nueva poblacion el mejor de la anterior

  for(int i=0; i<n_unidades; i++){
    poblacion[0].first[i] = mejor_sol_elit[i];
  }
  poblacion[0].second = coste_mejor_sol_elit;

  //Obtener indice del mejor actual
  int pos_mejor_sol;
  for(unsigned i=0; i<poblacion.size(); i++){
    if(poblacion[i].second < coste_mejor_sol){
      coste_mejor_sol = poblacion[i].second;
      pos_mejor_sol=i;
    }
  }



  //------------------------OPTIMIZACION CON BL------------------------

  if(it_actual%n_gen_bl == 0 && it_actual != 0){//BL cada 10 generaciones
    if(prob_bl == 1.0){
      for(unsigned int i=0; i<poblacion.size(); i++){
        busqueda_local_mem(poblacion[i].first, matriz_flujos, matriz_distancias, n_unidades, N_ITERS_BL);
      }
    }
    else{
      int n_bus_loc = prob_bl*(TAM_POBLACION);
      if(mejores==false){
        for( int i=0; i<n_bus_loc; i++){
          busqueda_local_mem(poblacion[i].first, matriz_flujos, matriz_distancias, n_unidades, N_ITERS_BL);
        }
      }
      else{//Aplicar bl a los prob_bl*TAM_POBLACION mejores
        busqueda_local_mem(poblacion[pos_mejor_sol].first, matriz_flujos, matriz_distancias, n_unidades, N_ITERS_BL);

      }

    }

  }

  //------------------------------------------------------------------------

  for(int i=0; i<n_unidades; i++){
    solucion[i] = poblacion[pos_mejor_sol].first[i];
  }

  //Borrar vectores auxiliares
  padres.clear();
  hijos.clear();


  it_actual++;
  }

}
