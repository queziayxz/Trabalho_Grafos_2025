//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>
#include <set>

//Emerson Caneschi Coelho de Souza
//Emmanuel Gomes Nassif
//Quezia Emanuelly da Silva Oliveira

using namespace std;
class Grafo {
    private:
        void _tokenizationDAV(string line);
        void _tokenizationOrdem(string line);
        void _tokenizationVertices(ifstream& ifs);
        void _tokenizationArestas(ifstream& ifs);

        //No* getNoForId(int id_no);
        Aresta* getArestaForIdAlvo(No* no, char id_alvo);

public:
    Grafo(char* fileName);
    Grafo();
    ~Grafo();

    vector<char> fecho_transitivo_direto(int id_no); // a
    vector<char> fecho_transitivo_indireto(int id_no); // b
    vector<char> caminho_minimo_dijkstra(int id_no_a, int id_no_b); // c
    vector<char> caminho_minimo_floyd(char id_no, char id_no_b); // d
    Grafo* arvore_geradora_minima_prim(vector<char> ids_nos); // e
    Grafo* arvore_geradora_minima_kruskal(vector<char> ids_nos); // f
    Grafo* arvore_caminhamento_profundidade(int id_no); // g
    int raio(); // h 1
    int diametro(); // h 2
    vector<char> centro(); // h 3
    vector<char> periferia(); // h 4
    vector<char> vertices_de_articulacao(); // i

    Grafo *transpor_grafo();

    vector<vector<int>> calcular_matriz_distancia();
    vector<No*> lista_adj;

    //No* getNoForId(char id);
    No* getNoForId(int id_no);
    int getPesoAresta(No* no, char id_alvo);
    vector<char> getCaminho(vector<vector<char>> predecessores, char id_no_a, char id_no_b);

    void fecho_busca_em_profundidade(char id, set<char> &visitados, vector<char> &resultado);
    void matriz_floyd_warshall(vector<vector<int>> &matriz_distancias, int n);
    void matriz_floyd(vector<vector<int>> &matriz_distancias, vector<vector<char>> &predecessores);
    int getIndice(char id_no);
    
    void fecho_busca_em_profundidade(Grafo *grafo, char id_no);
    void imprimir_base(const vector<char> &fecho, ofstream& arquivo);
    void imprimir_fecho_em_arquivo(const vector<char> &fecho, const string &nome_arquivo);
    void imprimir_arvore_caminho_profundidade_em_arquivo(const string nome_arquivo);
    void imprimir_caminho_minimo(const vector<char>& agm, const string& nome_arquivo);
    void imprimir_grafo_arquivo(const Grafo& grafo, const string& nome_arquivo);
    void naoVisitado();
    void novoNo(char id, int peso);
    void imprimirAgm(Grafo *grafo);
    void salvar_fecho_em_arquivo(const vector<char> &fecho, const string &nome_arquivo);
    int excentricidade(char id_no);
    bool ehConexo(vector<char> &vertices);
    set<char> calcula_vizinhanca_fechada(No *v);

    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;

};

#endif //GRAFO_H
