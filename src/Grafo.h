//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>
#include <set>


using namespace std;
class Grafo {
    private:
        void _tokenizationDAV(string line);
        void _tokenizationOrdem(string line);
        void _tokenizationVertices(ifstream& ifs);
        void _tokenizationArestas(ifstream& ifs);



        No* getNoForId(int id_no);

public:
    Grafo(char* fileName);
    Grafo();
    ~Grafo();

    vector<char> fecho_transitivo_direto(int id_no); // a
    vector<char> fecho_transitivo_indireto(int id_no); // b
    vector<char> caminho_minimo_dijkstra(int id_no_a, int id_no_b); // c
    vector<char> caminho_minimo_floyd(int id_no, int id_no_b); // d
    Grafo* arvore_geradora_minima_prim(vector<char> ids_nos); // e
    Grafo* arvore_geradora_minima_kruskal(vector<char> ids_nos); // f
    Grafo* arvore_caminhamento_profundidade(int id_no); // g
    int raio(); // h 1
    int diametro(); // h 2
    vector<char> centro(); // h 3
    vector<char> periferia(); // h 4
    vector<char> vertices_de_articulacao(); // i

    vector<vector<int>> calcular_matriz_distancia();
    void fecho_busca_em_profundidade(char id, set<char> &visitados, vector<char> &resultado);
    void matriz_floyd_warshall(vector<vector<int>> &matriz, int n);
    int excentricidade(char id_no);
    Grafo *transpor_grafo();
    void fecho_busca_em_profundidade(Grafo *grafo, char id_no);
    void imprimir_fecho_em_arquivo(const vector<char> &fecho, const string &nome_arquivo);
    
    void imprimir_arvore_caminho_profundidade_em_arquivo(const string nome_arquivo);

    void naoVisitado();


    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No*> lista_adj;
    void salvar_fecho_em_arquivo(const vector<char> &fecho, const string &nome_arquivo);
};

#endif //GRAFO_H
