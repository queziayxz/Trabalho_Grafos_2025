#include "GulosoReativo.h"
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

// Função auxiliar para escolher índice via roleta
int escolherIndiceRoleta(const vector<double>& probabilidades) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    double r = dis(gen);
    double soma = 0.0;

    for (int i = 0; i < probabilidades.size(); i++) {
        soma += probabilidades[i];
        if (r <= soma) {
            return i;
        }
    }
    return probabilidades.size() - 1; // fallback
}

pair<vector<char>, int> GulosoReativo::conjunto_dominante(Grafo *grafo, int numIter, int bloco, const vector<double>& alfas) 
{
    // 1. Pré-computar vizinhança fechada para todos os vértices
    unordered_map<char, set<char>> vizinhancaFechada;
    for (No* no : grafo->lista_adj) {
        vizinhancaFechada[no->id] = grafo->calcula_vizinhanca_fechada(no);
    }

    // 2. Inicialização das estruturas reativas
    int m = alfas.size();
    vector<double> P(m, 1.0/m);   // Probabilidades uniformes
    vector<double> M(m, 0.0);     // Médias dos pesos
    vector<int> contador(m, 0);   // Contador de usos
    
    // 3. Melhor solução global
    vector<char> melhor_solucao;
    int melhor_peso = INT_MAX;
    random_device rd;
    mt19937 gen(rd());

    // 4. Loop principal de iterações
    for (int iter = 0; iter < numIter; iter++) {
        // 4.1 Atualizar probabilidades periodicamente
        if (iter > 0 && iter % bloco == 0) {
            vector<double> q(m, 0.0);
            double soma_q = 0.0;
            
            for (int i = 0; i < m; i++) {
                if (contador[i] > 0) {
                    q[i] = static_cast<double>(melhor_peso) / M[i];
                    soma_q += q[i];
                }
            }
            
            if (soma_q > 0) {
                for (int i = 0; i < m; i++) {
                    P[i] = q[i] / soma_q;
                }
            }
        }

        // 4.2 Escolher alfa para esta iteração
        int idx_alfa = escolherIndiceRoleta(P);
        double alfa = alfas[idx_alfa];
        
        // 4.3 Construção da solução randomizada
        set<char> solucao_atual;
        int peso_atual = 0;
        set<char> nao_dominados;
        for (No* no : grafo->lista_adj) {
            nao_dominados.insert(no->id);
        }

        while (!nao_dominados.empty()) {
            vector<pair<No*, double>> candidatos;
            double melhor_score = -INF;

            // 4.3.1 Avaliar candidatos viáveis
            for (No* v : grafo->lista_adj) {
                // Pular vértices já na solução
                if (solucao_atual.find(v->id) != solucao_atual.end()) 
                    continue;
                
                // Calcular novos dominados
                set<char> novos;
                set_intersection(
                    vizinhancaFechada[v->id].begin(), vizinhancaFechada[v->id].end(),
                    nao_dominados.begin(), nao_dominados.end(),
                    inserter(novos, novos.begin())
                );

                if (!novos.empty()) {
                    double score = static_cast<double>(novos.size()) / v->peso;
                    candidatos.push_back({v, score});
                    if (score > melhor_score) {
                        melhor_score = score;
                    }
                }
            }

            if (candidatos.empty()) break;

            // 4.3.2 Ordenar candidatos por score (decrescente)
            sort(candidatos.begin(), candidatos.end(), 
                [](const auto& a, const auto& b) {
                    return a.second > b.second;
                });

            // 4.3.3 Construir Lista Restrita de Candidatos (LRC)
            int k = max(1, static_cast<int>(alfa * candidatos.size()));
            vector<No*> LRC;
            for (int i = 0; i < k; i++) {
                LRC.push_back(candidatos[i].first);
            }

            // 4.3.4 Escolha aleatória da LRC
            uniform_int_distribution<> dis(0, LRC.size() - 1);
            No* escolhido = LRC[dis(gen)];
            
            // 4.3.5 Atualizar solução
            solucao_atual.insert(escolhido->id);
            peso_atual += escolhido->peso;
            
            // Remover vértices dominados
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

        // 4.4 Atualizar estatísticas reativas
        contador[idx_alfa]++;
        M[idx_alfa] += (peso_atual - M[idx_alfa]) / contador[idx_alfa];
        
        // 4.5 Atualizar melhor solução global
        if (peso_atual < melhor_peso) {
            melhor_peso = peso_atual;
            melhor_solucao = vector<char>(solucao_atual.begin(), solucao_atual.end());
        }
    }

    return {melhor_solucao, melhor_peso};
}