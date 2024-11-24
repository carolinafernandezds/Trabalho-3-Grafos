#include "grafos.h"

int main (){
   constroi_grafo_peso(1 ,"grafo_rf_1.txt", 1); //Alternar entre matriz ou lista, DAG = 0 ou DAG = 1 e o grafo a ser utilizado
   
   for (int i = 0; i < 10; i++){
        clock_t start = clock();
        fluxo_maximo(1,2);    //Calcula o fluxo máximo entre os vértices 1 (fonte) e 2 (sumidouro)
        clock_t end = clock();
        cout <<  double(end - start) / CLOCKS_PER_SEC << endl;  // Retorna o tempo em segundos
    }

    imprime_fluxo("rede_1.txt"); //Gera arquivo .txt com fluxo total e capacidade/fluxo alocado de cada aresta
}