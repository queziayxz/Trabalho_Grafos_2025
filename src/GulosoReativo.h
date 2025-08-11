// Emerson Caneschi Coelho de Souza
// Emmanuel Gomes Nassif
// Quezia Emanuelly da Silva Oliveira

#ifndef GULOSOREATIVO_H
#define GULOSOREATIVO_H

#include "Grafo.h"
#include <vector>
#include <set>
#include <random>

class GulosoReativo {
public:
    static pair<vector<char>, int> conjunto_dominante_reativo(Grafo* grafo, int numIter, int bloco, const vector<double>& alfas);
    
private:
    static int escolherIndiceRoleta(const vector<double>& probabilidades, mt19937& gen);
    static vector<No*> calculaListaCandidatosOrdenada(Grafo* grafo, const set<char>& nao_dominados);
    static int getVizinhosNaoDominados(No* no, const set<char>& nao_dominados);
};

#endif // GULOSOREATIVO_H
