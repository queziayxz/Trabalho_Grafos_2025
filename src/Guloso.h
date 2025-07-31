// Emerson Caneschi Coelho de Souza
// Emmanuel Gomes Nassif
// Quezia Emanuelly da Silva Oliveira

#ifndef GULOSO_H
#define GULOSO_H

#include "Grafo.h"
#include <vector>
#include <set>

using namespace std;

class Guloso
{
public:
    static vector<char> conjunto_dominante_peso_minimo(Grafo *grafo);
    static set<char> calcula_vizinhanca_fechada(No *v);
    static set<char> dominados_por(No *v);
    static set<char> vizinhanca_fechada(No *v);
};

#endif // GULOSO_H
