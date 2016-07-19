#ifndef BUS_LOC
#define BUS_LOC

#define MAX_ITERS 25000

void busqueda_local(int* &solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades);

void busqueda_local_och(int* &solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades);

void busqueda_local_mem(int* &solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades, int n_iters);

int coste(int* solucion, int** matriz_flujos, int** matriz_distancias, int n_unidades);

#endif
