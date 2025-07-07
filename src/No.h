#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"

//Emerson Caneschi Coelho de Souza
//Emmanuel Gomes Nassif
//Quezia Emanuelly da Silva Oliveira

using namespace std;
class No {
public:
    No();
    ~No();

    char id;
    int peso;
    bool visitado;
    vector<Aresta*> arestas;
};



#endif //NO_H
