#ifndef ILS
#define ILS

#define N_ITERS 25

void ils(int* &solucion, int** matriz_flujos, int** matriz_distancias,
  int n_unidades, unsigned long semilla);

void genera_sol_random(int *sol, int n_unidades);

int coste(int* solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades);


#endif
