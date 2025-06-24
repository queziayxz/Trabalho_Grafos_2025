#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"
#include <algorithm>

using namespace std;
class Gerenciador {
public:
    static void comandos(Grafo* grafo);
    static char get_id_entrada();
    static vector<char> get_conjunto_ids(Grafo* grafo, int tam);
    static bool pergunta_imprimir_arquivo(string nome_arquivo);
    void imprimir_base(const vector<char> &fecho, ofstream& arquivo);
    void imprimir_fecho_em_arquivo(const vector<char> &fecho, const vector<No *> &lista_adj);
    void imprimir_caminho_minimo(const vector<char>& agm, const string& nome_arquivo);
    void imprimir_grafo_arquivo(const Grafo& grafo, const string& nome_arquivo);
};


#endif //GERENCIADOR_H
