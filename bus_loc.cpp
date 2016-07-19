#include <iostream>

#include "bus_loc.h"

void genera_vecino(int *sol_act, int *vecino, int n_unidades, int pos1, int pos2){
  int i, temp;

  for (i=0;i<n_unidades;i++)
   vecino[i]=sol_act[i];

  temp=vecino[pos1];
  vecino[pos1]=vecino[pos2];
  vecino[pos2]=temp;

}

void intercambia(int *sol_act, int pos1, int pos2){
  int temp=sol_act[pos1];
  sol_act[pos1]=sol_act[pos2];
  sol_act[pos2]=temp;
}



void busqueda_local(int* &solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades){

  int *vecino, *dlb;
  int coste_act, coste_vecino, iter_actual;
  bool improve_flag = true;
  bool fin_busqueda = false;

  vecino = new int[n_unidades];
  dlb = new int[n_unidades];

  //Inicializamos los valores de dlb a 0
  for(int i=0; i<n_unidades; i++)
    dlb[i]=0;

  coste_act=coste(solucion, matriz_flujos, matriz_distancias, n_unidades);

  //int coste_inicial=coste_act;

  iter_actual=0;
  while (iter_actual<100 && !fin_busqueda) {

    for(int i=0; i<n_unidades; i++){

      if(dlb[i] == 0){
        improve_flag = false;

        for(int j=0; j<n_unidades; j++){

          //Para que i y j sean diferentes
          if(i==j) continue;

          //check move i,j (mejorar con costes parciales)
          genera_vecino(solucion, vecino, n_unidades, i, j);
          coste_vecino=coste(vecino,matriz_flujos,matriz_distancias,n_unidades);
          if (coste_vecino<coste_act){
            //aplicar movimiento i,j
            //sol_actual=vecino
            coste_act=coste_vecino;
            intercambia(solucion,i,j);

            //std::cout << "\nMejora: " << coste_vecino;

            dlb[i]=0;
            dlb[j]=0;
            improve_flag=true;
            break;//Cogemos la primera solucion mejor a la actual
          }
        }
        if(!improve_flag){
          //std::cout << "No se ha encontrado mejora" << std::endl;
          dlb[i]=1;

          if(i==n_unidades){
            //std::cout << "Sin mejora en el entorno" << std::endl;
            fin_busqueda=true;
          }
        }
        else{//Si hemos encontrado mejora pasamos a otra iteracion para
          break;//generar otro vecindario
        }
      }
    }
    //Salida de info
    //std::cout << "\nIteracion: " << iter_actual;
    //std::cout << "\nCoste actual: " << coste_act << "\n";

    iter_actual++;
  }

  //std::cout << "\nSolucion inicial: " << coste_inicial <<"\n";
}

//Busqueda local de una sola iteracion para hormigas
void busqueda_local_och(int* &solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades){

  int *vecino, *dlb;
  int coste_act, coste_vecino;
  bool improve_flag = true;
  bool fin_busqueda = false;

  vecino = new int[n_unidades];
  dlb = new int[n_unidades];

  //Inicializamos los valores de dlb a 0
  for(int i=0; i<n_unidades; i++)
    dlb[i]=0;

  coste_act=coste(solucion, matriz_flujos, matriz_distancias, n_unidades);

  //int coste_inicial=coste_act;

  for(int i=0; i<n_unidades; i++){

    if(dlb[i] == 0){
      improve_flag = false;

      for(int j=0; j<n_unidades; j++){

        //Para que i y j sean diferentes
        if(i==j) continue;

        //check move i,j (mejorar con costes parciales)
        genera_vecino(solucion, vecino, n_unidades, i, j);
        coste_vecino=coste(vecino,matriz_flujos,matriz_distancias,n_unidades);
        if (coste_vecino<coste_act){
          //aplicar movimiento i,j
          //sol_actual=vecino
          coste_act=coste_vecino;
          intercambia(solucion,i,j);

          //std::cout << "\nMejora: " << coste_vecino;

          dlb[i]=0;
          dlb[j]=0;
          improve_flag=true;
          break;//Cogemos la primera solucion mejor a la actual
        }
      }
      if(!improve_flag){
        //std::cout << "No se ha encontrado mejora" << std::endl;
        dlb[i]=1;


        if(i==n_unidades){
          //std::cout << "Sin mejora en el entorno" << std::endl;
          fin_busqueda=true;
        }
      }
      else{//Si hemos encontrado mejora pasamos a otra iteracion para
        break;//generar otro vecindario
      }
    }
  }
    //Salida de info
    //std::cout << "\nIteracion: " << iter_actual;
    //std::cout << "\nCoste actual: " << coste_act << "\n";


  //std::cout << "\nSolucion inicial: " << coste_inicial <<"\n";
}

void busqueda_local_mem(int* &solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades, int n_iters){

  int *vecino, *dlb;
  int coste_act, coste_vecino, iter_actual;
  bool improve_flag = true;
  bool fin_busqueda = false;

  vecino = new int[n_unidades];
  dlb = new int[n_unidades];

  //Inicializamos los valores de dlb a 0
  for(int i=0; i<n_unidades; i++)
    dlb[i]=0;

  coste_act=coste(solucion, matriz_flujos, matriz_distancias, n_unidades);

  //int coste_inicial=coste_act;

  iter_actual=0;
  while (iter_actual<n_iters && !fin_busqueda) {
    for(int i=0; i<n_unidades; i++){

      if(dlb[i] == 0){
        improve_flag = false;

        for(int j=0; j<n_unidades; j++){

          //Para que i y j sean diferentes
          if(i==j) continue;

          //check move i,j (mejorar con costes parciales)
          genera_vecino(solucion, vecino, n_unidades, i, j);
          coste_vecino=coste(vecino,matriz_flujos,matriz_distancias,n_unidades);
          if (coste_vecino<coste_act){
            //aplicar movimiento i,j
            //sol_actual=vecino
            coste_act=coste_vecino;
            intercambia(solucion,i,j);

            //std::cout << "\nMejora: " << coste_vecino;

            dlb[i]=0;
            dlb[j]=0;
            improve_flag=true;
            break;//Cogemos la primera solucion mejor a la actual
          }
        }
        if(!improve_flag){
          //std::cout << "No se ha encontrado mejora" << std::endl;
          dlb[i]=1;

          if(i==n_unidades){
            //std::cout << "Sin mejora en el entorno" << std::endl;
            fin_busqueda=true;
          }
        }
        else{//Si hemos encontrado mejora pasamos a otra iteracion para
          break;//generar otro vecindario
        }
      }
    }
    //Salida de info
    //std::cout << "\nIteracion: " << iter_actual;
    //std::cout << "\nCoste actual: " << coste_act << "\n";

    iter_actual++;
  }

  //std::cout << "\nSolucion inicial: " << coste_inicial <<"\n";
}
