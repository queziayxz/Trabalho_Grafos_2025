#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"
#include <algorithm>

//Emerson Caneschi Coelho de Souza
//Emmanuel Gomes Nassif
//Quezia Emanuelly da Silva Oliveira

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
    static void imprimir_raio_diametro_centro_periferia(int raio, int diametro, const vector<char>& centro, const vector<char>& periferia, const string& nome_arquivo);
    static void valida_entrada_id_no(char &id_no, Grafo *grafo);
};


#endif //GERENCIADOR_H
