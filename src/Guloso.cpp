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


pair<vector<char>, int> Guloso::adaptado_artigo(Grafo *grafo)
{
    set<char> conjunto_dominante;
    int peso_total = 0;

    set<char> vertices_nao_dominados;
    for (No *no : grafo->lista_adj)
        vertices_nao_dominados.insert(no->id);


    while (!vertices_nao_dominados.empty())
    {
        double melhorScore = -INF;
        
        No *melhorVertice = nullptr;
        set<char> dominados_por_melhorVertice;

        //debug
        // cout << endl <<"Vertices nao dominados: " << vertices_nao_dominados.size() << " | ";
        // for (char c : vertices_nao_dominados)
        //     cout << c << " ";
        // cout << endl;
        //fim do debug

        for (No *v : grafo->lista_adj)
        {
            if (conjunto_dominante.count(v->id)) continue;

            set<char> vizinhanca_fechada = grafo->calcula_vizinhanca_fechada(v);
            set<char> novos_dominados;

            for (char u : vizinhanca_fechada)
                if (vertices_nao_dominados.count(u))
                    novos_dominados.insert(u);

            if (!novos_dominados.empty())
            {
                // score = número de novos dominados / peso
                double score = static_cast<double>(novos_dominados.size()) / v->peso;

                // cout << " - Vertice " << v->id << " domina " << novos_dominados.size() << " nos, razao = " << score << " | Dominados: ";
                // for (char c : novos_dominados)
                //     cout << c << " ";
                // cout << endl;

                if (score > melhorScore)
                {
                    melhorScore = score;
                    melhorVertice = v;
                    dominados_por_melhorVertice = novos_dominados;
                }
            }
        }

        if (melhorVertice == nullptr) break; // Nenhum vértice pode dominar mais

        //cout << " --> Escolhido o vertice: " << melhorVertice->id << " com score " << melhorScore << endl;

        // Adiciona vértice com melhor score
        conjunto_dominante.insert(melhorVertice->id);
        peso_total += melhorVertice->peso;

        // Remove os vértices dominados por ele
        for (char u : dominados_por_melhorVertice)
            vertices_nao_dominados.erase(u);

        //vertices_nao_dominados.erase(melhorVertice->id);

        // cout << "  - Conjunto dominante atualizado: {";
        // for (char id : conjunto_dominante)
        // {
        //     if (id == *conjunto_dominante.rbegin())
        //         cout << id;
        //     else
        //         cout << id << ", ";
        // }
        // cout << "}" << endl;

        // cout << "  - Nao dominados restantes: " << vertices_nao_dominados.size() << endl;
        // for (char id : vertices_nao_dominados)
        // {
        //     if (id == *vertices_nao_dominados.rbegin())
        //         cout << id;
        //     else
        //         cout << id << ", ";
        // }
        // cout << endl;
    }

    return make_pair(vector<char>(conjunto_dominante.begin(), conjunto_dominante.end()), peso_total);
}
