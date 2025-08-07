// Emerson Caneschi Coelho de Souza
// Emmanuel Gomes Nassif
// Quezia Emanuelly da Silva Oliveira

#ifndef GULOSOREATIVO_H
#define GULOSOREATIVO_H

#include "Grafo.h"
#include <vector>
#include <set>

using namespace std;

class GulosoReativo
{
public:
    static pair<vector<char>, int> conjunto_dominante(Grafo *grafo, int numIter, int bloco, const vector<double>& alfas);
};

#endif // GULOSOREATIVO_H
