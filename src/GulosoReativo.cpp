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

using namespace std;

int GulosoReativo::escolherIndiceRoleta(const vector<double>& probabilidades, mt19937& gen) {
    uniform_real_distribution<> dis(0.0, 1.0);
    double r = dis(gen);
    double soma = 0.0;

    for (int i = 0; i < probabilidades.size(); i++) {
        soma += probabilidades[i];
        if (r <= soma) {
            return i;
        }
    }
    return probabilidades.size() - 1;
}

int GulosoReativo::getVizinhosNaoDominados(No* no, const set<char>& nao_dominados) {
    int count = 0;
    for (Aresta* aresta : no->arestas) {
        if (nao_dominados.find(aresta->id_no_alvo) != nao_dominados.end()) {
            count++;
        }
    }
    if (nao_dominados.find(no->id) != nao_dominados.end()) {
        count++;
    }
    return count;
}

vector<No*> GulosoReativo::calculaListaCandidatosOrdenada(Grafo* grafo, const set<char>& nao_dominados) {
    vector<No*> lista = grafo->lista_adj;
    
    // Ordenação por inserção mais eficiente para listas parcialmente ordenadas
    for (int i = 1; i < lista.size(); i++) {
        No* chave = lista[i];
        double razao_chave = static_cast<double>(getVizinhosNaoDominados(chave, nao_dominados)) / chave->peso;
        
        int j = i - 1;
        while (j >= 0) {
            double razao_j = static_cast<double>(getVizinhosNaoDominados(lista[j], nao_dominados)) / lista[j]->peso;
            if (razao_j >= razao_chave) break;
            
            lista[j + 1] = lista[j];
            j--;
        }
        lista[j + 1] = chave;
    }
    
    return lista;
}

pair<vector<char>, int> GulosoReativo::conjunto_dominante_reativo(
    Grafo* grafo, int numIter, int bloco, const vector<double>& alfas) 
{
    // 1. Inicialização das estruturas reativas
    int m = alfas.size();
    vector<double> P(m, 1.0/m);
    vector<double> M(m, 0.0);
    vector<int> contador(m, 0);
    
    // 2. Melhor solução global e RNG
    vector<char> melhor_solucao;
    int melhor_peso = INT_MAX;
    random_device rd;
    mt19937 gen(rd());

    // 3. Loop principal
    for (int iter = 0; iter < numIter; iter++) {
        // 3.1 Atualização periódica de probabilidades
        if (iter > 0 && iter % bloco == 0) {
            vector<double> q(m, 0.0);
            double soma_q = 0.0;
            
            for (int i = 0; i < m; i++) {
                if (contador[i] > 0 && M[i] > 1e-6) {
                    q[i] = static_cast<double>(melhor_peso) / M[i];
                    soma_q += q[i];
                }
            }
            
            if (soma_q > 1e-6) {
                for (int i = 0; i < m; i++) {
                    P[i] = q[i] / soma_q;
                }
            } else {
                fill(P.begin(), P.end(), 1.0/m);
            }
        }

        // 3.2 Escolha de alfa
        int idx_alfa = escolherIndiceRoleta(P, gen);
        double alfa = alfas[idx_alfa];
        
        // 3.3 Construção da solução
        set<char> solucao_atual;
        set<char> nao_dominados;
        for (No* no : grafo->lista_adj) {
            nao_dominados.insert(no->id);
        }
        
        int peso_atual = 0;
        vector<No*> lista_candidatos = calculaListaCandidatosOrdenada(grafo, nao_dominados);

        while (!nao_dominados.empty() && !lista_candidatos.empty()) {
            // 3.3.1 Seleção randomizada
            int k = max(1, static_cast<int>(alfa * (lista_candidatos.size() - 1)));
            uniform_int_distribution<> dis(0, k);
            int index = dis(gen);
            
            No* escolhido = lista_candidatos[index];
            
            // 3.3.2 Atualização da solução
            solucao_atual.insert(escolhido->id);
            peso_atual += escolhido->peso;
            
            // 3.3.3 Atualizar não dominados
            nao_dominados.erase(escolhido->id);
            for (Aresta* a : escolhido->arestas) {
                nao_dominados.erase(a->id_no_alvo);
            }
            
            // 3.3.4 Atualizar lista de candidatos
            vector<No*> nova_lista;
            for (No* no : lista_candidatos) {
                if (solucao_atual.find(no->id) == solucao_atual.end()) {
                    // Verificar se ainda domina algum vértice
                    if (getVizinhosNaoDominados(no, nao_dominados) > 0) {
                        nova_lista.push_back(no);
                    }
                }
            }
            
            // 3.3.5 Reordenar parcialmente (mais eficiente que ordenação completa)
            if (!nova_lista.empty()) {
                // Ordenação parcial - apenas os primeiros elementos
                for (int i = 1; i < nova_lista.size(); i++) {
                    No* chave = nova_lista[i];
                    double razao_chave = static_cast<double>(getVizinhosNaoDominados(chave, nao_dominados)) / chave->peso;
                    
                    int j = i - 1;
                    while (j >= 0) {
                        double razao_j = static_cast<double>(getVizinhosNaoDominados(nova_lista[j], nao_dominados)) / nova_lista[j]->peso;
                        if (razao_j >= razao_chave) break;
                        
                        nova_lista[j + 1] = nova_lista[j];
                        j--;
                    }
                    nova_lista[j + 1] = chave;
                }
            }
            lista_candidatos = move(nova_lista);
        }

        // 3.4 Tratar vértices isolados não dominados
        for (char id : nao_dominados) {
            solucao_atual.insert(id);
            No* no = grafo->getNo(id);
            if (no != nullptr) {
                peso_atual += no->peso;
            }
            else {
                cerr << "Erro: No " << id << " nao encontrado!" << endl;
            }
        }
        // 3.5 Atualizar estatísticas
        contador[idx_alfa]++;
        M[idx_alfa] += (peso_atual - M[idx_alfa]) / contador[idx_alfa];
        
        // 3.6 Atualizar melhor solução
        if (peso_atual < melhor_peso) {
            melhor_peso = peso_atual;
            melhor_solucao.assign(solucao_atual.begin(), solucao_atual.end());
        }
    }

    return {melhor_solucao, melhor_peso};
}