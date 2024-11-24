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

int n, numero = 0, opcao, verifica = 0, final, fluxo_total = 0; 
double infinito = numeric_limits<double>::infinity();
vector <int> grau, caminho, pai, niveis, vertices_componente, componente_auxiliar1, componente_auxiliar2, distancia_auxiliar, grau_entrada, grau_saida, P;
vector <vector <double> > matriz_peso;
vector <vector <vector <double> > > matriz_DAG, matriz_residual;
vector <double> distancia_dijkstra;
int ** matriz;
vector <vector <int> > lista, componentes;
vector <vector <vector <double> > > lista_peso; 
vector <vector <vector <int> > > lista_DAG, lista_residual;

//Função para realizar exponenciação.
ll power(ll b, ll e, ll mod = numeric_limits<ll>::max()) {
 if (e == 0) return 1;
 b %= mod;
 ll a = power(b, e/2, mod);
 a = (a*a) % mod;
 if (e&1) return (a*b) % mod;
 return a; 
}

//Função para verificar utilizição de memória.
//void memoria_utilizada() {
    //struct rusage r_usage;
    //getrusage(RUSAGE_SELF, &r_usage);
    //cout << "Memória utilizada: " << r_usage.ru_maxrss << " KB" << endl;
//}

//Função para inicializar a estrutura de matriz de adjacência (utiliza array de array, cada um com tamanho n indicado por ponteiro).
void constroi_matriz (string nome_arquivo){
    ifstream arquivoEntrada(nome_arquivo);
    string linha;
    getline(arquivoEntrada, linha);
    n = stoi(linha); //Determina o total de vértices

    grau.resize(n, 0); //Preenche o vector grau, inicialmente, com n zeros.

        int rows = n, cols = n;
        matriz = new int*[rows];
        for (int i = 0; i < rows; i += 1){  //Cria a estrutura de matriz, utilizando ponteiros.
            matriz[i] = new int[cols];
        }   
        for (int i1 = 0; i1 < n; i1 += 1){     //Preenche a matriz com zeros.
            for (int i2 = 0; i2 < n; i2 += 1){
                matriz[i1][i2] = 0;
            }
        }

        while (getline (arquivoEntrada, linha)){ //Lê as arestas do grafo, a partir da linha 2.
            numero += 1;
            istringstream ss(linha);
            int n1, n2;
            ss >> n1 >> n2; 
            grau[n1 - 1] += 1;  //Para os vértices da aresta, aumenta seu grau em 1.
            grau[n2 - 1] += 1;
            matriz[n1 - 1][n2 - 1] = 1; //Preenche com 1 vértices que compõe a aresta.
            matriz[n2 - 1][n1 - 1] = 1;
        }
    arquivoEntrada.close();    
}

//Função para inicializar a estrutura de lista de adjacência (utiliza vector de vector de inteiros, o vector de i-ésima posição representa os vizinhos do vértice i + 1); recebe como parâmetro o nome do arquivo .txt a ser lido.
void constroi_lista(string nome_arquivo) {
    ifstream arquivoEntrada(nome_arquivo);
    string linha;
    getline(arquivoEntrada, linha);
    n = stoi(linha);  // Lê o número de vértices (n) do arquivo.

    // Inicializa o vetor grau com zeros.
    grau.resize(n, 0);

    // Inicializa a lista de adjacência (um vetor de vetores).
    vector<vector<int> > vertices(n);  // Cada vértice terá uma lista de vizinhos.

    while (getline(arquivoEntrada, linha)) {
        numero += 1;
        istringstream ss(linha);
        int n1, n2;

        // Lê os dois vértices da aresta.
        ss >> n1 >> n2;

        // Incrementa o grau de cada vértice.
        grau[n1 - 1] += 1;
        grau[n2 - 1] += 1;

        // Adiciona a aresta na lista de adjacência (grafo não direcionado).
        vertices[n1 - 1].push_back(n2);  // Adiciona n2 como vizinho de n1.
        vertices[n2 - 1].push_back(n1);  // Adiciona n1 como vizinho de n2.
        sort(vertices[n1 - 1].begin(), vertices[n1 - 1].end());    //Ordena os vizinhos dos vértices.
        sort(vertices[n2 - 1].begin(), vertices[n2 - 1].end());

    }
    lista = vertices;

    arquivoEntrada.close();
}

//Função que inicializa o grafo; recebe como parâmetros 0 (escolhe como representação a matriz) ou 1 (escolhe como representação a lista) e o nome do arquivo .txt a ser lido.
void constroi_grafo(int opcao1, string nome_arquivo){
    opcao = opcao1;

    // Representa o grafo conforme a escolha do usuário.
    if (opcao == 0){                
        constroi_matriz(nome_arquivo); 
    }
    else{
        constroi_lista(nome_arquivo);
    }
}

//Função que executa o BFS no vértice indicado, recebendo-o como parâmetro; armazena os pais de cada vértice i no vector pai e os níveis no vector niveis, nas posições i - 1. 
void BFS(int vertice){
    vector <int> marcados (n,0);    // Para marcar vértices visitados.
    int nivel;                     
    queue <int> fila;   //Fila para a BFS.
    pai.resize(n, 0);
    niveis.resize(n, -1);
    vertices_componente.resize(0);      

    marcados[vertice - 1] = 1; //Marca vértice inicial.
    fila.push(vertice);
    fila.push(0);
    niveis[vertice - 1] = 0;

    while (fila.size() != 0){
        vertice = fila.front();
        fila.pop(); //Pega o mais antigo da fila.
        vertices_componente.push_back(vertice); //Adiciona vértices para a componente conexa.
        nivel = fila.front();
        fila.pop();

        if (opcao == 0){    //Matriz
            for (int i = 0; i < n; i ++){
                if ((matriz[vertice - 1][i] == 1) && marcados[i] == 0){
                    marcados[i] = 1;
                    fila.push(i + 1);   //Adiciona na fila.
                    pai[i] = vertice;   //Atualiza pai.
                    niveis[i] = nivel + 1;  //Atualiza nível.
                    fila.push(nivel + 1);
                }
            }
        }
        else{   //Lista
            for (int i = 0; i < lista[vertice - 1].size(); i ++){
                if (marcados[lista[vertice - 1][i] - 1] == 0){
                    marcados[lista[vertice - 1][i] - 1] = 1;
                    fila.push(lista[vertice - 1][i]);   //Adiciona na fila.
                    pai[lista[vertice - 1][i] - 1] = vertice;   //Atualiza pai.
                    niveis[lista[vertice - 1][i] - 1] = nivel + 1;  //Atualiza nível.
                    fila.push(nivel + 1);
                }
            }
        }
    }
    componente_auxiliar1 = marcados;
}

//Função que executa o DFS no vértice indicado, recebendo-o como parâmetro; armazena os pais de cada vértice i no vector pai e os níveis no vector niveis, nas posições i - 1.
void DFS(int vertice) {
    vector<int> marcados(n, 0); // Para marcar vértices visitados.
    stack<int> pilha; // Pilha para a DFS.
    pai.resize(n, 0);
    niveis.resize(n, 0);

    // Marque o vértice inicial como visitado e empilhe.
    marcados[vertice - 1] = 1; 
    pilha.push(vertice);

    while (!pilha.empty()) {
        
        vertice = pilha.top();  // Obtenha o vértice do topo da pilha.
        pilha.pop();
        int nivel = niveis[vertice - 1]; // O nível é armazenado no vetor niveis.

        for (int i = 0; i < (opcao == 0 ? n : lista[vertice - 1].size()); i++) {    
            int adj = (opcao == 0) ? i : lista[vertice - 1][i] - 1;
            if (opcao == 0 && matriz[vertice - 1][adj] == 1) {  //Matriz
             
                if (marcados[adj] == 0) {       
                    marcados[adj] = 1; 
                    niveis[adj] = nivel + 1; // Atualiza o nível.
                    pai[adj] = vertice; // Atualiza o pai.
                    pilha.push(adj + 1); // Empilha o adjacente.
                }
            } else if (opcao == 1) {      //Lista
                
                if (marcados[adj] == 0) {
                    marcados[adj] = 1; 
                    niveis[adj] = nivel + 1; // Atualiza o nível.
                    pai[adj] = vertice; // Atualiza o pai.
                    pilha.push(lista[vertice - 1][i]); // Empilha o adjacente.
                }
            }
        }
    }
}

//Função para gerar um arquivo .txt contendo o pai e o nível de um vértice i na árvore de largura gerada; recebe como parâmetro o nome do arquivo a ser criado.
void constroi_arvore(string arquivo_saida){
    ofstream arquivo_Saida(arquivo_saida);

    arquivo_Saida << "Árvore gerada: " << endl;

    for (int i = 0; i < n; i++){
        arquivo_Saida << "Vértice: " << i + 1 <<  " Pai: " << pai[i] << " Nível: " << niveis[i] << endl; //Percorre os vectors pai e niveis.
    }
    arquivo_Saida.close();
}

//Função auxiliar para encontrar componentes conexas;
void componentes_conexas_auxiliar(){
    for (int i = 0; i < n; i++){
        if (componente_auxiliar1[i] == 1){ 
            componente_auxiliar2[i] = 1;
        }
    }
}

//Função para determinar as componentes conexas de um grafo (já inicializado), armazenando-as no vector componentes.
void componentes_conexas(){
    componentes.resize(0);

    BFS(1);  //Executa BFS no vértice 1 como ponto de partida.
    componentes.push_back(vertices_componente); //Registra a primeira componente.

    componente_auxiliar2 = componente_auxiliar1;

    for (int j = 0; j < n; j ++){   //Verifica se ainda há vértice não marcado.
        if (componente_auxiliar2[j] == 0){
            BFS(j + 1);     //Caso sim, executa BFS nesse vértice.
            componentes.push_back(vertices_componente); //Registra nova componente.
            componentes_conexas_auxiliar();
        }
    }
    cout << componentes.size(); //Total de componentes.
}

//Função que determina a distância entre dois vértices, inicial e final, recebendo-os como parâmetros; 
void distancia(int inicial, int final){
    BFS(inicial);   //Executa BFS no vértice inicial, para achar componente conexa.
    int pai_atual = final, distancia_atual = 0;

    while (pai_atual != 0){     //Executa enquanto não achar um pai = 0 (neste caso, ou não pertence a componente, ou é raiz, portanto, vértice inicial).
        final = pai_atual;
        pai_atual = pai[final - 1];
        distancia_atual += 1;   //Atualiza o valor da distância.
    }
    if (niveis[final - 1] == 0){    //Se nível = 0, é raiz da árvore;
        cout << "Distância: " << distancia_atual << endl;
    }
    else{                           //Caso contrário, não faz parte da árvore, portanto não tem conexão com o vértice inicial.
        cout << "Não há ligação entre os vértices." << endl;
    }
}

//Função que retorna uma aproximação do diâmetro para grafos com grande número de vértices;
void diametro_aproximado(){
    int maior = 0, idx;
    componentes_conexas();

    for (int i = 0; i < componentes.size(); i++){   //Acha a maior das componentes conexas.
        if (componentes[i].size() > maior){
            maior = componentes[i].size();
            idx = i;
        }
    }

    maior = 0;
    BFS(componentes[idx][0]);   //Executa BFS no primeiro vértice encontrado dessa componente.

    for (int i = 0; i < n; i++){    //Encontra o vértice com maior distância do incial.
        if (niveis[i] > maior){
            maior = niveis[i];
            idx = i + 1;
        }
    }

    maior = 0;
    BFS(idx);   //Executa BFS no vértice encontrado.

    for (int i = 0; i < n; i++){    //Encontra o vértice com maior distância.
        if (niveis[i] > maior){
            maior = niveis[i];
        }
    }
    cout << "Diâmetro aproximado: " << maior << endl;
}

//Função que retorna o diâmetro exato do grafo; ineficiente para um grande número de vértices.
void diametro(){
    int maior = 0, maior_atual;
    for (int i = 1; i < n + 1 ; i ++){
        BFS(i);
        maior_atual = *max_element(niveis.begin(), niveis.end());
        if (maior_atual > maior){
            maior = maior_atual;
        }
    }   
    cout << "Diâmetro: " << maior;
}

//Função que gera um arquivo .txt contendo o menor, maior e médio grau de um grafo, bem como a mediana de todos os graus, armazenados no vector grau; o arquivo também contém todas as componentes conexas do grafo, listando-as em ordem decrescente de tamanho; recebe como parâmetro o nome do arquivo a ser criado.
void saida(string arquivo_saida){
    vector <int> copia = grau;
    bool acabou = false;
    sort(copia.begin(), copia.end());   //Ordena o vector grau de forma crescente.
    int menor = copia[0];   //Menor grau.
    int maior = copia[n - 1];   //Maior grau.
    int mediana, maior_comp = 0, idx, atual = 1, grau_medio;

    ofstream arquivo_Saida(arquivo_saida);

    if (n%2 == 0){  //Verifica valor da mediana
        mediana = (copia[(n/2) - 1] + copia[n/2])/2; //Número de vértices ímpar. 
    }
    else{
        mediana = (copia[(n-1)/2]); //Número de vértices par. 
    }

    for (int i = 0; i < n; i++){    
        grau_medio += grau[i];
    }

    grau_medio = (grau_medio/n); //Calcula valor do grau médio.

    componentes_conexas();

    arquivo_Saida << "Número de vértices: " << n << endl;       //Gera arquivo .txt
    arquivo_Saida << "Número de arestas: " << numero << endl;
    arquivo_Saida << "Menor grau: " << menor << endl;
    arquivo_Saida << "Maior grau: " << maior << endl;
    arquivo_Saida << "Grau médio: " << grau_medio << endl;
    arquivo_Saida << "Mediana do grau: " << mediana << endl;

    while(acabou == false){                            //Ordena as componentes e lista cada vértice
        maior_comp = 0;
        for (int i = 0; i < componentes.size(); i++){
            if (componentes.size() == 1){
                acabou = true;
            }
            if (componentes[i].size() > maior_comp){
                maior_comp = componentes[i].size();
                idx = i;
            }
        }

        arquivo_Saida << "" << endl;
        arquivo_Saida << "Componente " << atual << endl;
        arquivo_Saida << "Tamanho:  " << componentes[idx].size() << endl;
        arquivo_Saida << "" << endl;
        
        for (int i = 0; i < componentes[idx].size(); i++){
            arquivo_Saida << componentes[idx][i] << endl;
        }

        componentes.erase(componentes.begin() + idx);
        atual += 1;
    }
    arquivo_Saida.close();
}

//Função para inicializar a estrutura da matriz de adjacência (utiliza vector de vector de double, visto que, para os vértices relacionados, é preenchida pelo peso da aresta; para os não relacionados, é preenchida por infinito).
void constroi_matriz_peso(string nome_arquivo){
    vector <vector <double> > exemplo2;
    vector <double> exemplo1;
    ifstream arquivoEntrada(nome_arquivo);
    string linha;
    getline(arquivoEntrada, linha);
    n = stoi(linha); //Determina o total de vértices

    grau.resize(n, 0); //Preenche o vector grau, inicialmente, com n zeros.

    exemplo1.resize(n, infinito); //Preenche a matriz com infinito.
    for (int i = 0; i < n; i++){
        exemplo2.push_back(exemplo1);
    }
    matriz_peso = exemplo2;

        while (getline (arquivoEntrada, linha)){ //Lê as arestas do grafo, a partir da linha 2.
            numero += 1;
            istringstream ss(linha);
            int n1, n2;
            double n3;
            ss >> n1 >> n2 >> n3; 
            if (n3 < 0){
                verifica = 1;
            }
            grau[n1 - 1] += 1;  //Para os vértices da aresta, aumenta seu grau em 1.
            grau[n2 - 1] += 1;
            matriz_peso[n1 - 1][n2 - 1] = n3; //Preenche com o peso da aresta.
            matriz_peso[n2 - 1][n1 - 1] = n3;
        }
    arquivoEntrada.close();    

}

//Função para inicializar a estrutura da lista de adjacência (utliza vector de vector de vector de double, visto que o vetor maior armazena n vetores; cada um armazena os vizinhos na forma de vetores, onde cada vetor armazena o vértice relacionado e o peso da aresta).
void constroi_lista_peso(string nome_arquivo){
    ifstream arquivoEntrada(nome_arquivo);
    string linha;
    getline(arquivoEntrada, linha);
    n = stoi(linha);  // Lê o número de vértices (n) do arquivo.

    // Inicializa o vetor grau com zeros.
    grau.resize(n, 0);

    // Inicializa a lista de adjacência (um vetor de vetores).
    vector <vector <vector <double> > > vertices(n);  // Cada vértice terá uma lista de vizinhos.
    vector <double> exemplo1, exemplo2; 

    while (getline(arquivoEntrada, linha)) {
        exemplo1.resize(0);
        exemplo2.resize(0);
        numero += 1;
        istringstream ss(linha);
        int n1, n2;
        double n3;

        // Lê os dois vértices da aresta.
        ss >> n1 >> n2 >> n3;

        if (n3 < 0){
            verifica = 1;
        }

        // Incrementa o grau de cada vértice.
        grau[n1 - 1] += 1;
        grau[n2 - 1] += 1;

        exemplo1.push_back(n1);
        exemplo1.push_back(n3);
        exemplo2.push_back(n2);
        exemplo2.push_back(n3);

        // Adiciona a aresta na lista de adjacência.
        vertices[n1 - 1].push_back(exemplo2);  // Adiciona n2 como vizinho de n1.
        vertices[n2 - 1].push_back(exemplo1);  // Adiciona n1 como vizinho de n2.
    }
    lista_peso = vertices;

    arquivoEntrada.close();
}

//Função que inicializa a estrutura de matriz de adjacência para DAG (utiliza vector de vector de vector de double, visto que, para os vértices relacionados, é preenchida pelo peso da aresta e pelo fluxo alocado; para os não relacionados, é preenchida por infinito e fluxo 0).
void constroi_matriz_DAG(string nome_arquivo){
    vector <double> exemplo(0);  
    vector <vector <double> > exemplo1(0) ; 
    vector < vector < vector <double> > > exemplo2(0);
    ifstream arquivoEntrada(nome_arquivo);
    string linha;
    getline(arquivoEntrada, linha);
    n = stoi(linha);  // Lê o número de vértices do arquivo

    matriz_DAG.resize(0);
    grau_entrada.resize(n, 0);
    grau_saida.resize(n, 0);

    exemplo.push_back(infinito);
    exemplo.push_back(0);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            exemplo1.push_back(exemplo);
        }
        exemplo2.push_back(exemplo1);
    }
    matriz_DAG = exemplo2;

    while (getline(arquivoEntrada, linha)) {  // Lê as arestas do grafo
        istringstream ss(linha);
        int n1, n2;
        double n3;
        ss >> n1 >> n2 >> n3;
        if (n3 < 0) {
            verifica = 1;
        }
        grau_saida[n1 - 1] += 1; //Aumenta o grau de saída de n1
        grau_entrada[n2 - 1] += 1; //Aumenta o grau de entrada de n2
        matriz_DAG[n1 - 1][n2 - 1][0] = n3;  // Preenche o peso/capacidade da aresta
    }
    matriz_residual = matriz_DAG; //Inicializa a estrutura da matriz residual
    arquivoEntrada.close();
}

// Função que inicializa a estrutura de lista de adjacência para DAG (utliza vector de vector de vector de int, visto que o vetor maior armazena n vetores; cada um armazena os vizinhos na forma de vetores, onde cada vetor armazena o vértice relacionado, o peso da aresta e o fluxo alocado).
void constroi_lista_DAG(string nome_arquivo){
    ifstream arquivoEntrada(nome_arquivo);
    string linha;
    getline(arquivoEntrada, linha);
    n = stoi(linha);  // Lê o número de vértices
    vector <int> exemplo;

    grau_entrada.resize(n, 0);
    grau_saida.resize(n, 0);

    vector <vector < vector <int> > > vertices(n);  // Cada vértice terá uma lista de vizinhos

    while (getline(arquivoEntrada, linha)) {
        istringstream ss(linha);
        int n1, n2, n3;
        ss >> n1 >> n2 >> n3;
        if (n3 < 0) {
            verifica = 1;
        }
        grau_saida[n1 - 1] += 1; //Aumenta o grau de saída de n1
        grau_entrada[n2 - 1] += 1; //Aumenta o grau de entrada de n2

        exemplo.clear();
        exemplo.push_back(n2);
        exemplo.push_back(n3);
        exemplo.push_back(0);
        vertices[n1 - 1].push_back(exemplo);  //Adiciona na lista o peso/capacidade e o fluxo, inicialmente 0
    }

    lista_DAG = vertices;
    lista_residual = vertices; //Inicializa a estrutura da lista residual
    arquivoEntrada.close();
}

// Função para construir o grafo, seja usando matriz ou lista, com a variável DAG, para escolher entre um grafo com arestas direcionadas (DAG = 1) ou não (DAG = 0).
void constroi_grafo_peso(int opcao1, string nome_arquivo, int DAG) {
    opcao = opcao1;

    if (DAG == 0){
        if (opcao1 == 0){
            constroi_matriz_peso(nome_arquivo);
        }
        else{
            constroi_lista_peso(nome_arquivo);
        }
    }
    
    else{
        if (opcao1 == 0) {
            constroi_matriz_DAG(nome_arquivo);
        }
        else {
            constroi_lista_DAG(nome_arquivo);
        }
    }
}

//Função que fornece as distâncias de um vértice inicial s para todos os demais vértices do grafo, utilizando na implementação do algoritmo de Dijkstra um vetor auxiliar.
void distancia_peso(int s) {
    int minimo, idx;
    vector<double> distancia_copia(n);

    // Inicialização dos vetores
    distancia_dijkstra.resize(n, infinito);
    pai.resize(n, -1);
    niveis.resize(n, 0);

    distancia_dijkstra[s - 1] = 0;
    pai[s - 1] = 0;
    distancia_copia = distancia_dijkstra;

    for (int total = 0; total < n; total++) {
        // Encontrar o menor valor em distancia_copia.
        minimo = infinito;
        idx = -1;
        for (int i = 0; i < n; i++) {
            if (distancia_copia[i] < minimo) {
                minimo = distancia_copia[i];
                idx = i;
            }
        }

        if (idx == -1) break;  // Se não há mais nós para processar, sai do loop.
        distancia_copia[idx] = infinito;  // Marca como visitado.

        if (opcao == 0) {
            // Caso da matriz de adjacência
            for (int i = 0; i < n; i++) {
                if (matriz_peso[idx][i] != infinito) {
                    if (distancia_dijkstra[i] > distancia_dijkstra[idx] + matriz_peso[idx][i]) { 
                        distancia_dijkstra[i] = distancia_dijkstra[idx] + matriz_peso[idx][i]; //Atualiza com menor distância.
                        distancia_copia[i] = distancia_dijkstra[idx] + matriz_peso[idx][i];
                        pai[i] = idx + 1; //Constrói árvore de busca.
                        niveis[i] = niveis[idx] + 1;
                    }
                }
            }
        } 
        else {
            // Caso da lista de adjacência
            for (int i = 0; i < lista_peso[idx].size(); i++){
                if (distancia_dijkstra[lista_peso[idx][i][0] - 1] > distancia_dijkstra[idx] + lista_peso[idx][i][1]){
                    distancia_dijkstra[lista_peso[idx][i][0] - 1] = distancia_dijkstra[idx] + lista_peso[idx][i][1];  //Atualiza com menor distância.
                    distancia_copia[lista_peso[idx][i][0] - 1] = distancia_dijkstra[idx] + lista_peso[idx][i][1];
                    pai[lista_peso[idx][i][0] - 1] = idx + 1; //Constrói árvore de busca.
                    niveis[lista_peso[idx][i][0] - 1] = niveis[idx] + 1;
                }
            }
                
        }
    }
}

//Função que fornece as distâncias de um vértice inicial s para todos os demais vértices do grafo, utilizando na implementação do algoritmo de Dijkstra um heap mínimo (MinHeap).
void distancia_peso_heap(int s){
    int minimo, idx;
    bool t = true;
    vector <int> marcados (n, 0);
    priority_queue< pair <double, double>, vector <pair <double, double> >, greater <pair <double, double> > > minHeap;


    //Inicialização dos vetores.
    distancia_dijkstra.resize(n, infinito);
    pai.resize(n, -1);
    niveis.resize(n, 0);

    distancia_dijkstra[s-1] = 0;
    pai[s-1] = 0;
    minHeap.push(make_pair(distancia_dijkstra[s-1], s - 1));

    for (int total = 0; total < n; total++){
        // Encontrar a menor chave/valor
        while (t != false){
            idx = minHeap.top().second;
            minHeap.pop();
            if (marcados[idx] != 1){
                t = false;
            }
        }
        t = true;
        if (opcao == 0){
            // Caso da matriz de adjacência
            for (int i = 0; i < n; i++){
                if (matriz_peso[idx][i] != infinito){
                    if (distancia_dijkstra[i] > distancia_dijkstra[idx] + matriz_peso[idx][i]){
                        distancia_dijkstra[i] = distancia_dijkstra[idx] + matriz_peso[idx][i]; //Atualiza com menor distância.
                        minHeap.push(make_pair(distancia_dijkstra[i], i));
                        pai[i] = idx + 1; //Constrói árvore de busca.
                        niveis[i] = niveis[idx] + 1;
                    }
                }
            }
        }

        else{
             // Caso da lista de adjacência
            for (int i = 0; i < lista_peso[idx].size(); i++){
                if (distancia_dijkstra[lista_peso[idx][i][0] - 1] > distancia_dijkstra[idx] + lista_peso[idx][i][1]){
                    distancia_dijkstra[lista_peso[idx][i][0] - 1] = distancia_dijkstra[idx] + lista_peso[idx][i][1]; //Atualiza com menor distância
                    minHeap.push(make_pair(distancia_dijkstra[lista_peso[idx][i][0] - 1], lista_peso[idx][i][0] - 1));
                    pai[lista_peso[idx][i][0] - 1] = idx + 1; //Constrói árvore de busca.
                    niveis[lista_peso[idx][i][0] - 1] = niveis[idx] + 1;
                }
            }
        }
    }

}

//Função que retorna o caminho mínimo entre o vértice inicial s e o vértice final f, após a execução das funções distancia_peso(s) ou distancia_peso_heap(s).
void caminho_minimo(int s, int f){
    //Inicialização de vetores.
    caminho.resize(0);

    //Recursão até achar raiz (vértice s) ou não ter caminho.
    while (niveis[f-1] != 0 && niveis[f-1] != -1){
        caminho.push_back(f);
        f = pai[f - 1];
    }

    caminho.push_back(f); //Inclui raiz.

    if (niveis[f-1] == -1){
        cout << "Não há caminho mínimo.";
    }

    else{
        cout << "Caminho: ";
        for (int i = caminho.size() - 1; i > -1; i--){
            cout << caminho[i] << " ";
        }
    }
}

//Função para construir o grafo residual, alocando fluxo 1 nas arestas originais e 0 nas reversas.
void constroi_grafo_residual(){
    vector <int> exemplo (0);
    if (opcao == 0){
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                if (matriz_DAG[i][j][0] != infinito){
                    matriz_residual[i][j][0] = matriz_DAG[i][j][0] - matriz_DAG[i][j][1]; 
                    matriz_residual[j][i][0] = matriz_DAG[i][j][1];
                    matriz_residual[i][j][1] = 1; //Aresta original
                    matriz_residual[j][i][1] = 0; //Aresta reversa
                }
            }
        }
    }
    else{
        for (int i = 0; i < n; i++){
            for (int j = 0; j < lista_DAG[i].size(); j++){
                exemplo.clear();
                int idx_vertice_vizinho = lista_DAG[i][j][0] - 1;
                lista_residual[i][j][1] = lista_DAG[i][j][1] - lista_DAG[i][j][2];
                lista_residual[i][j][2] = 1; //Aresta original
                exemplo.push_back(i + 1);
                exemplo.push_back(lista_DAG[i][j][2]);
                exemplo.push_back(0); //Aresta reversa
                lista_residual[idx_vertice_vizinho].push_back(exemplo); 
            }
        }
    }
}

//Função que atualiza o grafo residual, mais especificamente, as arestas do caminho P e suas capacidades residuais, reduzindo o tempo de execução.
void atualiza_grafo_residual(){
    int idx1 = 0, idx2 = 0;
    vector <int> exemplo;
    for (int i = 0; i < P.size() - 1; i++){
        int antes = P[i] - 1;
        int depois = P[i + 1] - 1;

        if (opcao == 0){
            matriz_residual[antes][depois][0] = matriz_DAG[antes][depois][0] - matriz_DAG[antes][depois][1]; //Atualiza capacidade residual da aresta original
            matriz_residual[depois][antes][0] = matriz_DAG[antes][depois][1]; //Atualiza capacidade residual da aresta reversa
        }

        else{
            for (int j = 0; j < lista_residual[antes].size(); j++){
                if (lista_residual[antes][j][0] - 1 == depois){
                    idx1 = j;
                    break;
                }
            }
            for (int j = 0; j < lista_DAG[antes].size(); j++){
                if (lista_DAG[antes][j][0] - 1 == depois){
                    idx2 = j;
                    break;
                }
            }

            lista_residual[antes][idx1][1] = lista_DAG[antes][idx2][1] - lista_DAG[antes][idx2][2]; //Atualiza capacidade residual da aresta original

            for (int j = 0; j < lista_residual[depois].size(); j++){
                if (lista_residual[depois][j][0] - 1 == antes){
                    idx1 = j;
                    break;
                }
            }
            lista_residual[depois][idx1][1] = lista_DAG[antes][idx2][2]; //Atualiza capacidade residual da aresta reversa
        }
    }
}

//Função BFS modificada, adaptada para ser utilizada em um DAG (arestas direcionadas e assimétricas).
void BFS_residual(int vertice) {
    vector <int> marcados(n, 0);  // Marca os vértices visitados
    queue <int> fila;  // Fila para BFS
    pai.resize(n, -1);
    niveis.resize(n, -1);
    vector <int> vertices_componente;

    marcados[vertice - 1] = 1;
    fila.push(vertice);
    niveis[vertice - 1] = 0;

    while (!fila.empty()) {
        vertice = fila.front();
        fila.pop();
        vertices_componente.push_back(vertice);

        if (opcao == 0) {  // Caso da matriz de adjacência
            for (int i = 0; i < n; i++) {
                if (matriz_residual[vertice - 1][i][0] != infinito && matriz_residual[vertice - 1][i][0] > 0 && marcados[i] == 0) {
                    marcados[i] = 1;
                    fila.push(i + 1);
                    pai[i] = vertice;
                    niveis[i] = niveis[vertice - 1] + 1;
                }
            }
        } else {  // Caso da lista de adjacência
            for (int i = 0; i < lista_residual[vertice - 1].size(); i++) {
                int vizinho = lista_residual[vertice - 1][i][0] - 1;
                if (lista_residual[vertice - 1][i][1] > 0 && marcados[vizinho] == 0) {
                    marcados[vizinho] = 1;
                    fila.push(lista_residual[vertice - 1][i][0]);
                    pai[vizinho] = vertice;
                    niveis[vizinho] = niveis[vertice - 1] + 1;
                }
            }
        }
    }
}

//Função para encontrar o fluxo máximo em uma rede de fluxos, que tem o vértice s (fonte) e t (sumidouro) como parâmetros.
void fluxo_maximo(int s, int t) {
    int capacidade_s = 0;
    double delta = 0;

    // Calcula capacidade de saída
    if (opcao == 0) {
        for (int j = 0; j < n; j++) {
            if (matriz_DAG[s - 1][j][0] != infinito) {
                capacidade_s += matriz_DAG[s - 1][j][0];
            }
        }
    } else {
        for (int j = 0; j < lista_DAG[s - 1].size(); j++) {
            capacidade_s += lista_DAG[s - 1][j][1];
        }
    }

    // Encontra delta inicial, maior potência de 2 menor que capacidade de saída
    delta = power(2, floor(log2(capacidade_s)));

    constroi_grafo_residual(); //Constrói grafo residual

    while (delta >= 1) {
        BFS_residual(s); //Realiza BFS para encontrar caminho P entre s e t
        int final = t, idx, verif = 0;
        P.clear();

        while (final != s) {
            int pos_final = final - 1, pos_pai = pai[pos_final] - 1;

            if (niveis[pos_pai] == -1) { //Verifica se o caminho é válido
                break;
            }
            if (opcao == 0) {
                if (matriz_residual[pos_pai][pos_final][0] < delta) { //Interrompe se a aresta tem capacidade menor que delta
                    if (pos_pai == s - 1) {
                        verif = 1;
                    }
                    break;
                }
            } else {
                idx = -1;
                for (int j = 0; j < lista_residual[pos_pai].size(); j++) {
                    if (lista_residual[pos_pai][j][0] - 1 == pos_final) {
                        idx = j;
                        break;
                    }
                }
                if (idx != -1 && lista_residual[pos_pai][idx][1] < delta) { //Interrompe se a aresta tem capacidade menor que delta
                    if (pos_pai == s - 1) {
                        verif = 1;
                    }
                    break;
                }
            }

            P.push_back(final);  
            final = pai[pos_final];
        }

        if (final == s) {
            P.push_back(s); 
            reverse(P.begin(), P.end()); //Coloca os elementos do vetor na ordem correta
        }

        if (P.size() == 0 || P[0] != s || verif == 1) { //Verifica se o caminho encontrado é válido
            delta = delta / 2;
            continue;
        }

        double menor_capacidade = infinito;

        // Encontra a menor capacidade (gargalo)
        if (opcao == 0) {
            for (int i = 0; i < P.size() - 1; i++) {
                int antes = P[i] - 1;
                int depois = P[i + 1] - 1;
                if (matriz_residual[antes][depois][0] < menor_capacidade) {
                    menor_capacidade = matriz_residual[antes][depois][0];
                    break;
                }
            }
        } else {
            for (int i = 0; i < P.size() - 1; i++) {
                int antes = P[i] - 1;
                int depois = P[i + 1] - 1;
                int idx = -1;
                for (int j = 0; j < lista_residual[antes].size(); j++) {
                    if (lista_residual[antes][j][0] == P[i + 1]) {
                        idx = j;
                        break;
                    }
                }
                if (idx != -1 && lista_residual[antes][idx][1] < menor_capacidade) {
                    menor_capacidade = lista_residual[antes][idx][1];
                }
            }
        }

        // Atualiza fluxos
        for (int i = 0; i < P.size() - 1; i++) {
            int antes = P[i] - 1;
            int depois = P[i + 1] - 1;
            if (opcao == 0) {
                // Atualiza matriz original
                matriz_DAG[antes][depois][1] += menor_capacidade;
                matriz_DAG[depois][antes][1] -= menor_capacidade;

                // Atualiza matriz residual
                matriz_residual[antes][depois][0] -= menor_capacidade;
                matriz_residual[depois][antes][0] += menor_capacidade;
            } 
            else {
                // Atualiza lista original
                for (int j = 0; j < lista_DAG[antes].size(); j++) {
                    if (lista_DAG[antes][j][0] == P[i + 1]) {
                        lista_DAG[antes][j][2] += menor_capacidade;
                        continue;
                    }
                }
                for (int j = 0; j < lista_DAG[depois].size(); j++) {
                    if (lista_DAG[depois][j][0] == P[i]) {
                        lista_DAG[depois][j][2] -= menor_capacidade;
                        continue;
                    }
                }

                // Atualizar lista residual
                for (int j = 0; j < lista_residual[antes].size(); j++) {
                    if (lista_residual[antes][j][0] == P[i + 1]) {
                        lista_residual[antes][j][1] -= menor_capacidade;
                        continue;
                    }
                }
                for (int j = 0; j < lista_residual[depois].size(); j++) {
                    if (lista_residual[depois][j][0] == P[i]) {
                        lista_residual[depois][j][1] += menor_capacidade;
                        continue;
                    }
                }
            }
        }

        atualiza_grafo_residual(); //Atualiza as capacidades residuais do caminho
    }

    //Soma os fluxos de saída para determinar máximo
    if (opcao == 0) {
        for (int j = 0; j < n; j++) {
            fluxo_total += matriz_DAG[s - 1][j][1];
        }
    } else {
        for (int j = 0; j < lista_DAG[s - 1].size(); j++) {
            fluxo_total += lista_DAG[s - 1][j][2];
        }
    }
    cout << "Fluxo máximo: " << fluxo_total << endl;
}

//Função que armazena o fluxo total e o fluxo alocado de cada aresta em um arquivo .txt.
void imprime_fluxo(string arquivo_saida){
    ofstream arquivo_Saida (arquivo_saida);

    arquivo_Saida << "Fluxo total: " << fluxo_total << endl;

    if (opcao == 0){
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                if (matriz_DAG[i][j][0] != infinito){
                    arquivo_Saida << "Aresta: " << i + 1 << "-" << j + 1 << "  Capacidade total: " << matriz_DAG[i][j][0] << "  Fluxo alocado: " << matriz_DAG[i][j][1] << endl;
                }
            }
        }
    }
    else{
        for (int i = 0; i < n; i++){
            for (int j = 0; j < lista_DAG[i].size(); j++){
                arquivo_Saida << "Aresta: " << i + 1 << "-" << lista_DAG[i][j][0] << "  Capacidade total: " << lista_DAG[i][j][1] << "  Fluxo alocado: " << lista_DAG[i][j][2] << endl; 
            }
        }
    }
}
