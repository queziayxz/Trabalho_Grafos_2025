// Emerson Caneschi Coelho de Souza
// Emmanuel Gomes Nassif
// Quezia Emanuelly da Silva Oliveira

#include "Guloso.h"
#include "No.h"
#include "Aresta.h"
#include "Grafo.h"
#include <limits>
#include <iostream>
#include <vector>
#include <climits>
#include <set>
#include <algorithm>
using namespace std;

const double INF = numeric_limits<double>::infinity();

pair<vector<char>, int> Guloso::conjunto_dominante_peso_minimo(Grafo *grafo)
{
    set<char> conjunto_dominante; // (1)
    int peso = 0;                 // (2)

    set<char> vertices_nao_dominados; // (2)
    for (No *no : grafo->lista_adj)
        vertices_nao_dominados.insert(no->id);

    // loop principal (3)
    while (!vertices_nao_dominados.empty())
    {
        double melhorRazao = INF;    // inicializa a melhor razão como infinito (4)
        No *melhorVertice = nullptr; // inicializa o melhor vértice como nulo (5)

        // //debug
        // cout << "Vertices nao dominados: ";
        // for (char c : vertices_nao_dominados)
        //     cout << c << " ";
        // cout << endl;
        // //fim do debug

        // para cada vertice, se ele nao estiver no conjunto dominante:
        for (No *v : grafo->lista_adj)
        {
            if (!vertices_nao_dominados.count(v->id))
                continue; // (6)

            // define vizinhanca fechada
            set<char> vizinhanca_fechada = grafo->calcula_vizinhanca_fechada(v); // (7)

            // pega os vertices que estao na vizinhanca fechada e ainda nao estao dominados
            set<char> novosDominados; // (8)

            for (char u : vizinhanca_fechada)
                if (vertices_nao_dominados.count(u))
                    novosDominados.insert(u);

            if (!novosDominados.empty()) //(9)
            {
        
                double razao =  static_cast<double>(v->peso) / novosDominados.size(); // (10)

                // cout << " - Vertice " << v->id << " domina " << novosDominados.size() << " nos, razao = " << razao << " | Dominados: ";
                // for (char c : novosDominados)
                //     cout << c << " ";
                // cout << endl;

                if (razao < melhorRazao) // (11)
                {
                    melhorRazao = razao; // (12)
                    melhorVertice = v;   // (13)
                }
            }
        }

        if (melhorVertice == nullptr) //(17)
            break;                    //(18)

        // cout << " --> Escolhido o vertice: " << melhorVertice->id << " com razao " << melhorRazao << endl << endl;

        // adiciona o vertice ao conjunto dominante
        conjunto_dominante.insert(melhorVertice->id); //(20)
        peso += melhorVertice->peso;

        // remove os vertices dominados do conjunto de vertices nao dominados
        for (char id : grafo->calcula_vizinhanca_fechada(melhorVertice)) //(21)
            vertices_nao_dominados.erase(id);
    }

    // converte o set para vector e retorna (precisa ser vector para manter compatibilidade com o resto do código do T1)
    return make_pair(vector<char>(conjunto_dominante.begin(), conjunto_dominante.end()), peso);
}