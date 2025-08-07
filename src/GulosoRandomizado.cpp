#include "GulosoRandomizado.h"
#include "No.h"
#include "Aresta.h"
#include "Grafo.h"
#include <limits>
#include <iostream>
#include <vector>
#include <climits>
#include <set>
#include <algorithm>
#include <random>
#include <numeric>
#include <unordered_map>
using namespace std;

const double INF = numeric_limits<double>::infinity();

pair<vector<char>, int> GulosoRandomizado::conjunto_dominante(Grafo *grafo, int numIter, double alfa) 
{
    // 1. Pré-computar vizinhança fechada (boa otimização, podemos manter)
    unordered_map<char, set<char>> vizinhancaFechada;
    for (No* no : grafo->lista_adj) {
        vizinhancaFechada[no->id] = grafo->calcula_vizinhanca_fechada(no);
    }

    // REMOVIDO: As estruturas reativas P, M, e contador.
    
    // 3. Melhor solução global (permanece igual)
    vector<char> melhor_solucao;
    int melhor_peso = INT_MAX;
    random_device rd;
    mt19937 gen(rd());

    // 4. Loop principal de iterações (permanece, mas simplificado)
    for (int iter = 0; iter < numIter; iter++) {
        
        // REMOVIDO: O bloco de atualização de probabilidades.
        // REMOVIDO: A escolha de alfa via roleta. O alfa já é fixo.
        
        // 4.3 Construção da solução randomizada (lógica central do GRASP)
        set<char> solucao_atual;
        int peso_atual = 0;
        set<char> nao_dominados;
        for (No* no : grafo->lista_adj) {
            nao_dominados.insert(no->id);
        }

        while (!nao_dominados.empty()) {
            vector<pair<No*, double>> candidatos;
            
            // 4.3.1 Avaliar candidatos viáveis (lógica idêntica)
            for (No* v : grafo->lista_adj) {
                if (solucao_atual.find(v->id) != solucao_atual.end()) 
                    continue;
                
                set<char> novos;
                set_intersection(
                    vizinhancaFechada[v->id].begin(), vizinhancaFechada[v->id].end(),
                    nao_dominados.begin(), nao_dominados.end(),
                    inserter(novos, novos.begin())
                );

                if (!novos.empty()) {
                    double score = static_cast<double>(novos.size()) / v->peso;
                    candidatos.push_back({v, score});
                }
            }

            if (candidatos.empty()) break;

            // 4.3.2 Ordenar candidatos (lógica idêntica)
            sort(candidatos.begin(), candidatos.end(), 
                 [](const auto& a, const auto& b) {
                     return a.second > b.second;
                 });

            // 4.3.3 Construir LRC (lógica idêntica, usando o alfa fixo)
            int k = max(1, static_cast<int>(alfa * candidatos.size()));
            vector<No*> LRC;
            for (int i = 0; i < k; i++) {
                LRC.push_back(candidatos[i].first);
            }

            // 4.3.4 Escolha aleatória da LRC (lógica idêntica)
            uniform_int_distribution<> dis(0, LRC.size() - 1);
            No* escolhido = LRC[dis(gen)];
            
            // 4.3.5 Atualizar solução (lógica idêntica)
            solucao_atual.insert(escolhido->id);
            peso_atual += escolhido->peso;
            
            set<char> dominados;
            set_intersection(
                vizinhancaFechada[escolhido->id].begin(), vizinhancaFechada[escolhido->id].end(),
                nao_dominados.begin(), nao_dominados.end(),
                inserter(dominados, dominados.begin())
            );
            
            for (char c : dominados) {
                nao_dominados.erase(c);
            }
        }

        // 4.5 Atualizar melhor solução global (permanece igual)
        if (peso_atual < melhor_peso) {
            melhor_peso = peso_atual;
            melhor_solucao = vector<char>(solucao_atual.begin(), solucao_atual.end());
        }
    }

    return {melhor_solucao, melhor_peso};
}