// Emerson Caneschi Coelho de Souza
// Emmanuel Gomes Nassif
// Quezia Emanuelly da Silva Oliveira

#ifndef GULOSORANDOMIZADO_H
#define GULOSORANDOMIZADO_H

#include "Grafo.h"
#include <vector>
#include <set>

using namespace std;

class GulosoRandomizado
{
public:
    static pair<vector<char>, int> conjunto_dominante(Grafo *grafo, int numIter, double alfa);
};

#endif // GULOSORANDOMIZADO_H
