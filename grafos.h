#ifndef GRAFOS_H
#define GRAFOS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <stack>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <random>
#include <limits>
using namespace std;
using ll = long long;

extern int n, numero = 0, opcao, verifica = 0, final, fluxo_total = 0; 
extern double infinito = numeric_limits<double>::infinity();
extern vector <int> grau, caminho, pai, niveis, vertices_componente, componente_auxiliar1, componente_auxiliar2, distancia_auxiliar, grau_entrada, grau_saida, P;
extern vector <vector <double> > matriz_peso;
extern vector <vector <vector <double> > > matriz_DAG, matriz_residual;
extern vector <double> distancia_dijkstra;
extern int ** matriz;
extern vector <vector <int> > lista, componentes;
extern vector <vector <vector <double> > > lista_peso; 
extern vector <vector <vector <int> > > lista_DAG, lista_residual;

//Função para realizar exponenciação.
ll power(ll b, ll e, ll mod = numeric_limits<ll>::max());

// Função para verificar a utilização de memória
void memoria_utilizada();

// Função para inicializar a matriz de adjacência
void constroi_matriz(string nome_arquivo);

// Função para inicializar a lista de adjacência
void constroi_lista(string nome_arquivo);

// Função para inicializar o grafo (0 para matriz e 1 para lista)
void constroi_grafo(int opcao1, string nome_arquivo);

// Função para executar o BFS a partir de um vértice
void BFS(int vertice);

// Função para executar o DFS a partir de um vértice
void DFS(int vertice);

// Função para gerar um arquivo de saída contendo a árvore gerada (pai e nível de cada vértice)
void constroi_arvore(string arquivo_saida);

// Função auxiliar para encontrar componentes conexas
void componentes_conexas_auxiliar();

// Função para determinar as componentes conexas do grafo
void componentes_conexas();

// Função para determinar a distância entre dois vértices
void distancia(int inicial, int final);

// Função para calcular uma aproximação do diâmetro do grafo
void diametro_aproximado();

//Função que retorna o diâmetro exato do grafo; ineficiente para um grande número de vértices.
void diametro();

// Função para gerar um arquivo de saída com informações sobre o grau do grafo e componentes conexas
void saida(string arquivo_saida);

//Função para inicializar a estrutura da matriz de adjacência (utiliza vector de vector de double, visto que, para os vértices relacionados, é preenchida pelo peso da aresta; para os não relacionados, é preenchida por infinito).
void constroi_matriz_peso(string nome_arquivo);

//Função para inicializar a estrutura da lista de adjacência (utliza vector de vector de vector de double, visto que o vetor maior armazena n vetores; cada um armazena os vizinhos na forma de vetores, onde cada vetor armazena o vértice relacionado e o peso da aresta).
void constroi_lista_peso(string nome_arquivo);

//Função que inicializa a estrutura de matriz de adjacência para DAG (utiliza vector de vector de vector de double, visto que, para os vértices relacionados, é preenchida pelo peso da aresta e pelo fluxo alocado; para os não relacionados, é preenchida por infinito e fluxo 0).
void constroi_matriz_DAG(string nome_arquivo);

// Função que inicializa a estrutura de lista de adjacência para DAG (utliza vector de vector de vector de int, visto que o vetor maior armazena n vetores; cada um armazena os vizinhos na forma de vetores, onde cada vetor armazena o vértice relacionado, o peso da aresta e o fluxo alocado).
void constroi_lista_DAG(string nome_arquivo);

//Função que inicializa o grafo com peso; recebe como parâmetros 0 (escolhe como representação a matriz) ou 1 (escolhe como representação a lista) e o nome do arquivo .txt a ser lido.
void constroi_grafo_peso(int opcao1, string nome_arquivo, int DAG);

//Função que fornece as distâncias de um vértice inicial s para todos os demais vértices do grafo, utilizando na implementação do algoritmo de Dijkstra um vetor auxiliar.
void distancia_peso(int s);

//Função que fornece as distâncias de um vértice inicial s para todos os demais vértices do grafo, utilizando na implementação do algoritmo de Dijkstra um heap mínimo (MinHeap).
void distancia_peso_heap(int s);

//Função que retorna o caminho mínimo entre o vértice inicial s e o vértice final f, após a execução das funções distancia_peso(s) ou distancia_peso_heap(s).
void caminho_minimo(int s, int f);

//Função para construir o grafo residual, alocando fluxo 1 nas arestas originais e 0 nas reversas.
void constroi_grafo_residual();

//Função que atualiza o grafo residual, mais especificamente, as arestas do caminho P e suas capacidades residuais, reduzindo o tempo de execução.
void atualiza_grafo_residual();

//Função BFS modificada, adaptada para ser utilizada em um DAG (arestas direcionadas e assimétricas).
void BFS_residual(int vertice);

//Função para encontrar o fluxo máximo em uma rede de fluxos, que tem o vértice s (fonte) e t (sumidouro) como parâmetros.
void fluxo_maximo(int s, int t);

//Função que armazena o fluxo total e o fluxo alocado de cada aresta em um arquivo .txt.
void imprime_fluxo(string arquivo_saida);

#endif 
