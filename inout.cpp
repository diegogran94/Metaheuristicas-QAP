#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "inout.h"

using namespace std;

void lee_fichero(string nombre_fichero, int &dim_matrices,
  int** &flujos, int** &distancias){

    const char *cstr = nombre_fichero.c_str();

    ifstream fi;

    fi.open(cstr);
    if(fi.fail()){
      cerr << "ERROR: No se pudo abrir el fichero" << endl;
      exit(0);
    }

    fi >> dim_matrices;

    //Reservar memoria para las matrices

    flujos = new int *[dim_matrices];
    for(int i=0; i<dim_matrices; i++){
      flujos[i] = new int [dim_matrices];
    }

    distancias = new int *[dim_matrices];
    for(int i=0; i<dim_matrices; i++){
      distancias[i] = new int [dim_matrices];
    }

    //Leemos los datos del fichero

    for(int i=0; i<dim_matrices; i++){
      for(int j=0; j<dim_matrices; j++){
        fi >> flujos[i][j];
      }
    }

    for(int i=0; i<dim_matrices; i++){
      for(int j=0; j<dim_matrices; j++){
        fi >> distancias[i][j];
      }
    }

    fi.close();
}

/*
int main(int argc, char const *argv[]) {

    int** matriz_distancias;
    int** matriz_flujos;
    int dim_matrices;
    string nombre_fichero = "../tablas/esc64a.dat";

    lee_fichero(nombre_fichero, dim_matrices, matriz_flujos, matriz_distancias);

    for(int i=0; i<dim_matrices; i++){
      for(int j=0; j<dim_matrices; j++){
        cout << matriz_flujos[i][j] << " ";
      }
      cout << endl;
    }

    cout << endl;
    cout << endl;

    for(int i=0; i<dim_matrices; i++){
      for(int j=0; j<dim_matrices; j++){
        cout << matriz_distancias[i][j] << " ";
      }
      cout << endl;
    }

    return 0;
}
*/
