/*
  Optimización basada en colonia de hormigas
*/
#include<iostream>
#include<vector>
#include <limits>
#include <cmath>

#include"funciones.h"
#include"och.h"
#include"random_ppio.h"
#include"bus_loc.h"

using namespace std;

void inicializar_colonia(hormiga* &colonia, int n_unidades){

    //Inicializar valores

  for(int i=0; i<N_HORMIGAS; i++){
    for(int j=0; j<n_unidades; j++){
      colonia[i].lnv[j] = -1;
      colonia[i].nnv[j] = 0;
    }
  }
}

void inicializar_feromona(double** &feromona, int tam_matriz){

  for(int i=0; i<tam_matriz; i++){
    for(int j=0; j<tam_matriz; j++){
      feromona[i][j] = FEROM_INI;
    }
  }
}

void inicializar_feromona_mm(double** &feromona, int tam_matriz, double valor){

  for(int i=0; i<tam_matriz; i++){
    for(int j=0; j<tam_matriz; j++){
      feromona[i][j] = valor;
    }
  }
}


double transicion_sh(int origen, int destino, double** feromona,
  int* nnv, vector<int> pot_dis, int n_unidades){


  double numerador, denominador, aux;

  //Si el destino esta en la lista de nodos no visitados...
  if(nnv[destino]==0){

    numerador = pow(feromona[origen][destino],ALFA)*pow(1.0/pot_dis[destino],BETA);

    aux=0.0;
    for(int j=0; j<n_unidades; j++){
      if(nnv[j]==0){//Si no esta elegido
        aux+= pow(feromona[origen][j],ALFA) * pow(1.0/pot_dis[j],BETA);
      }
    }
    denominador = aux;

    return numerador/denominador;
  }
  else{//Si no devolver 0
    return 0.0;
  }

}

int transicion_sch(int origen, double** feromona,
  int* nnv, vector<int> pot_dis, int n_unidades){

  //En este caso se supone que q<= q0
  int unidad_elegida=0;
  double argmax = std::numeric_limits<int>::min();
  double aux = 0.0;
  for(int i=0; i<n_unidades; i++){
    if(nnv[i]==0){//Si no esta elegido
      aux = pow(feromona[origen][i],ALFA)*pow(1.0/pot_dis[i],BETA);
      if(argmax<aux){
        argmax = aux;
        unidad_elegida=i;
      }
    }
  }

  return unidad_elegida;


}

void actualizacion_local_feromona(double** &feromona, int r, int s){
  //r=origen, s=destino

  double nuevo_valor;
  nuevo_valor = (1-RHO)*feromona[r][s]+RHO*FEROM_INI;

  feromona[r][s] = nuevo_valor;
  feromona[s][r] = nuevo_valor;


}

void aporte_feromona(double** &feromona, int r, int s, double coste){

  double nuevo_valor;
  nuevo_valor = (1-PARAM_EVAP)*feromona[r][s]+PARAM_EVAP*(1/coste);

  feromona[r][s] = nuevo_valor;
  feromona[s][r] = nuevo_valor;
}



void och (int* &solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades,
  unsigned long semilla, bool max_min){

    Set_random(semilla);

    std::vector<int> pot_flujos(n_unidades,0);
    std::vector<int> pot_dis(n_unidades,0);
    std::vector<int> pot_flujos_ind;
    std::vector<int> elegidos(n_unidades,0);
    std::vector<double> prob_elegir_local(n_unidades,0);

    bool sol_actualizada;
    int* random_sol;
    double max_valor_ferom, min_valor_ferom, coste_rand_sol;

    double** feromona;

    feromona = new double *[n_unidades];
    for(int i=0; i<n_unidades; i++){
      feromona[i] = new double [n_unidades];
    }

    if(max_min){
      //Generar solucion aleatoria
      random_sol = new int [n_unidades];
      genera_sol_random(random_sol, n_unidades);
      max_valor_ferom = coste(random_sol, matriz_flujos, matriz_distancias, n_unidades);
      inicializar_feromona_mm(feromona,n_unidades,coste_rand_sol);
      max_valor_ferom = 1/(PARAM_EVAP*coste_rand_sol);
      min_valor_ferom = max_valor_ferom/500;
    }
    else{

      inicializar_feromona(feromona,n_unidades);

    }
    hormiga* colonia;
    colonia = new hormiga [N_HORMIGAS];

    colonia = new hormiga[N_HORMIGAS];
    for(int i=0; i<N_HORMIGAS; i++){
      colonia[i].lnv = new int [n_unidades];
      colonia[i].nnv = new int [n_unidades];
    }

    inicializar_colonia(colonia,n_unidades);


    //Calcular el potencial de flujo y distancias
    for(int i=0; i<n_unidades; i++){
      for(int j=0; j<n_unidades; j++){
        pot_flujos[i]+=matriz_flujos[i][j];
        pot_dis[i]+=matriz_distancias[i][j];
      }
    }

    //ordenar unidades de menor a mayor potencial de flujo
    int p_min, id_min;
    for(int i=0; i<n_unidades; i++){
      p_min = std::numeric_limits<int>::max();
      id_min=0;
      for(int j=0; j<n_unidades; j++){
        if(pot_flujos[j]<p_min && elegidos[j]!=1){
          p_min=pot_flujos[j];
          id_min=j;
        }
      }
      pot_flujos_ind.push_back(id_min);
      elegidos[id_min]=1;
    }


    //Bucle principal

    int mejor_hormiga;
    float q;
    int local_actual;
    double coste_actual, coste_mejor;
    float n_rand;

    //LLamadas a la funcion objetivo por ciclo
    //La BL realiza n_unidades*n_unidades ejecuciones
    //Este programa realiza una llamada por cada hormiga al elegir la mejor hormiga.
    int llamadas_por_ciclo = (n_unidades*n_unidades)+N_HORMIGAS;
    int n_evals = N_EVALUACIONES/llamadas_por_ciclo;

    for(int e=0; e<n_evals; e++){

      inicializar_colonia(colonia,n_unidades);

      for(int i=0; i<n_unidades; i++){
        for(int k=0; k<N_HORMIGAS; k++){

        q = Rand();

        if(q<=PROB_Q0){

          local_actual = transicion_sch(i,feromona,colonia[k].nnv,pot_dis,n_unidades);

        }
        else{
          //Vector de probabilidades
          for(int j=0; j<n_unidades; j++){
            prob_elegir_local[j] = transicion_sh(i, j,feromona, colonia[k].nnv, pot_dis, n_unidades);
          }

          //Elegir localizacion segun probabilidades
          n_rand = Rand();
          for(int m=0; m<n_unidades; m++){
            if(colonia[k].nnv[m]==0){
              if(prob_elegir_local[m]>n_rand){
                local_actual = m;
              }
              else{
                n_rand = n_rand - prob_elegir_local[m];
              }
            }
          }



        }

        colonia[k].lnv[i] = local_actual+1;
        colonia[k].nnv[local_actual]=1;


        //Actualizacion local feromona

        actualizacion_local_feromona(feromona,i,local_actual);
        }

      }


      //Busqueda local
      for(int k=0; k<N_HORMIGAS; k++){
        busqueda_local_och(colonia[k].lnv, matriz_flujos, matriz_distancias, n_unidades);
      }


    //Obtener mejor hormiga

    coste_mejor = coste(colonia[0].lnv,matriz_flujos,matriz_distancias,n_unidades);
    mejor_hormiga=0;
    sol_actualizada = false;
    for(int k=1; k<N_HORMIGAS;k++){
      coste_actual = coste(colonia[k].lnv,matriz_flujos,matriz_distancias,n_unidades);
      if(coste_actual<coste_mejor){
        coste_mejor=coste_actual;
        mejor_hormiga=k;
        sol_actualizada = true;
      }
    }

    //Actualizar valore max y min para SHMM_BL

    if(max_min && sol_actualizada){
      max_valor_ferom = 1/(PARAM_EVAP*coste_mejor);
      min_valor_ferom = max_valor_ferom/500;
      //Truncar los valores de la matriz de feromonas

      for(int i=0; i<n_unidades; i++){
        for(int j=0; j<n_unidades; j++){

          if(feromona[i][j]>max_valor_ferom){
            feromona[i][j] = max_valor_ferom;
          }
          if(feromona[i][j]<min_valor_ferom){
            feromona[i][j] = min_valor_ferom;
          }
        }
      }
    }

    //Aporte de feromona por la mejor hormiga

    int loc_mejor_hormiga;
    for(int i=0; i<n_unidades; i++){
      loc_mejor_hormiga = colonia[mejor_hormiga].lnv[i];
      aporte_feromona(feromona, i,loc_mejor_hormiga-1,coste_mejor);
    }

    //Guardamos mejor solucion global
    for(int i=0; i<n_unidades; i++){
      solucion[i] = colonia[mejor_hormiga].lnv[i];
    }

  }




  for(int i=0; i<N_HORMIGAS; i++){
    delete [] colonia[i].nnv;
    delete [] colonia[i].lnv;

  }

    delete [] feromona;
    delete [] colonia;

}
