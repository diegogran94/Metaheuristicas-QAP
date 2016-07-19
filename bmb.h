#ifndef BMB
#define BMB

#define N_SOLUCIONES 25

void busqueda_multi_basica(int* &solucion, int** matriz_flujos, int** matriz_distancias,
  int n_unidades, unsigned long semilla);

int coste(int* solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades);

#endif
