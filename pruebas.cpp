#include<iostream>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
//MUTACION SUBLISTA ALEATORIA
  srand (time(NULL));
  int n_unidades = 20;
  int tam_sublista = (int) n_unidades/4;
  int pos_i = rand() % (n_unidades-tam_sublista-1);

  int *sublista = new int [tam_sublista];
  int *lista = new int [n_unidades];

  for(int i=0; i<n_unidades; i++)
    lista[i] = i;

  int it = 0;
  for(int i=pos_i; i<tam_sublista+pos_i; i++){
    sublista[it] = lista[i];
    it++;
  }

  for(int i=0; i<tam_sublista; i++)
    std::cout << sublista[i] << " ";

  std::cout << std::endl;

  for(int i=0; i<n_unidades; i++)
    std::cout << lista[i] << " ";

  std::cout << std::endl << pos_i << std::endl;

  for(int i=pos_i; i<tam_sublista+pos_i;){
    int random_pos=rand()%(tam_sublista);
    std::cout << std::endl << "Random: " << random_pos << std::endl;
    if(sublista[random_pos]!=-1){
      lista[i]=sublista[random_pos];
      sublista[random_pos]=-1;
      i++;
    }

    for(int i=0; i<tam_sublista; i++)
      std::cout << sublista[i] << " ";

    std::cout << std::endl;

    for(int i=0; i<n_unidades; i++)
      std::cout << lista[i] << " ";
  }

  return 0;
}
