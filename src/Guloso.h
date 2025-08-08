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
    private:
        vector<No*> calculaListaCandidatosOrdenada(vector<No*> listaVertices, set<char> conjunto_dos_dominados);
        vector<No*> atualizaListaCandidatos(vector<No*>listaVertices, No* no, set<char>&conjunto_dos_dominados);
        int getVizinhosNaoDominados(No* no, set<char> conjunto_dos_dominados);
    public:
        Guloso();
        ~Guloso();

        static pair<vector<char>, int> conjunto_dominante_peso_minimo(Grafo *grafo);
        static pair<vector<char>, int> adaptado_artigo(Grafo *grafo);
        pair<vector<char>, vector<double>> gulosoRandomizadoAdaptativo(Grafo *grafo);
        static set<char> calcula_vizinhanca_fechada(No *v);
        static set<char> dominados_por(No *v);
        static set<char> vizinhanca_fechada(No *v);
};

#endif // GULOSO_H
