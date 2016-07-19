#ifndef AGG
#define AGG

#define TAM_POBLACION 10 //50 en agg ;10 en mem
#define PROB_CRUCE 0.7
#define PROB_MUTAR 0.001
#define N_ITERS_AG 25000

#define TAM_POB_MEM 10
#define N_EVALS_F_OBJETIVO 25000
#define N_ITERS_BL 400

void genetico(int* &solucion, int** matriz_flujos, int** matriz_distancias,
  int n_unidades, unsigned long semilla);

void memetico(int* &solucion, int** matriz_flujos, int** matriz_distancias,
  int n_unidades, unsigned long semilla, int n_gen_bl, double prob_bl, bool mejores);

#endif
