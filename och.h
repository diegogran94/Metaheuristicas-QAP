#ifndef OCH
#define OCH

#define N_HORMIGAS 10
#define FEROM_INI 0.0000001
#define PARAM_EVAP 0.2
#define ALFA 1
#define BETA 2
#define N_EVALUACIONES 25000
#define RHO 0.2 //Parámetro de evaporacion
#define PROB_Q0 0.8 //probabilidad con la que se determinísticamente el arco más prometedor

typedef struct{
  int* lnv;//Lista nodos visitados (L)
  int* nnv;//Lista de nodos no visitados (J)
}hormiga;


void inicializar_feromona(double** &feromona, int tam_matriz);

void och (int* &solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades,
  unsigned long semilla, bool max_min);

#endif
