// Emerson Caneschi Coelho de Souza
// Emmanuel Gomes Nassif
// Quezia Emanuelly da Silva Oliveira

#include "Guloso.h"
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


using namespace std;

const double INF = numeric_limits<double>::infinity();

Guloso::Guloso()
{

}

Guloso::~Guloso()
{

}

pair<vector<char>, int> Guloso::conjunto_dominante_peso_minimo(Grafo *grafo)
{
    set<char> conjunto_dominante; // (1)
    int peso = 0;                 // (2)

    set<char> vertices_nao_dominados; // (2)
    for (No *no : grafo->lista_adj)
        vertices_nao_dominados.insert(no->id);

    // loop principal (3)
    while (!vertices_nao_dominados.empty())
    {
        double melhorRazao = INF;    // inicializa a melhor razão como infinito (4)
        No *melhorVertice = nullptr; // inicializa o melhor vértice como nulo (5)

        // //debug
        // cout << "Vertices nao dominados: ";
        // for (char c : vertices_nao_dominados)
        //     cout << c << " ";
        // cout << endl;
        // //fim do debug

        // para cada vertice, se ele nao estiver no conjunto dominante:
        for (No *v : grafo->lista_adj)
        {
            if (!vertices_nao_dominados.count(v->id))
                continue; // (6)

            // define vizinhanca fechada
            set<char> vizinhanca_fechada = grafo->calcula_vizinhanca_fechada(v); // (7)

            // pega os vertices que estao na vizinhanca fechada e ainda nao estao dominados
            set<char> novosDominados; // (8)

            for (char u : vizinhanca_fechada)
                if (vertices_nao_dominados.count(u))
                    novosDominados.insert(u);

            if (!novosDominados.empty()) //(9)
            {

                double razao =  static_cast<double>(v->peso) / novosDominados.size(); // (10)

                // cout << " - Vertice " << v->id << " domina " << novosDominados.size() << " nos, razao = " << razao << " | Dominados: ";
                // for (char c : novosDominados)
                //     cout << c << " ";
                // cout << endl;

                if (razao < melhorRazao) // (11)
                {
                    melhorRazao = razao; // (12)
                    melhorVertice = v;   // (13)
                }
            }
        }

        if (melhorVertice == nullptr) //(17)
            break;                    //(18)

        // cout << " --> Escolhido o vertice: " << melhorVertice->id << " com razao " << melhorRazao << endl << endl;

        // adiciona o vertice ao conjunto dominante
        conjunto_dominante.insert(melhorVertice->id); //(20)
        peso += melhorVertice->peso;

        // remove os vertices dominados do conjunto de vertices nao dominados
        for (char id : grafo->calcula_vizinhanca_fechada(melhorVertice)) //(21)
            vertices_nao_dominados.erase(id);
    }

    // converte o set para vector e retorna (precisa ser vector para manter compatibilidade com o resto do código do T1)
    return make_pair(vector<char>(conjunto_dominante.begin(), conjunto_dominante.end()), peso);
}


pair<vector<char>, int> Guloso::adaptado_artigo(Grafo *grafo)
{
    set<char> conjunto_dominante;
    int peso_total = 0;

    vector<char> dominados;

    set<char> vertices_nao_dominados;
    for (No *no : grafo->lista_adj)
        vertices_nao_dominados.insert(no->id);


    while (!vertices_nao_dominados.empty())
    {
        double melhorScore = -INF;

        No *melhorVertice = nullptr;
        set<char> dominados_por_melhorVertice;

        //debug
        // cout << endl <<"Vertices nao dominados: " << vertices_nao_dominados.size() << " | ";
        // for (char c : vertices_nao_dominados)
        //     cout << c << " ";
        // cout << endl;
        //fim do debug

        for (No *v : grafo->lista_adj)
        {
            if (conjunto_dominante.count(v->id)) continue;

            set<char> vizinhanca_fechada = grafo->calcula_vizinhanca_fechada(v);
            set<char> novos_dominados;

            for (char u : vizinhanca_fechada)
                if (vertices_nao_dominados.count(u))
                    novos_dominados.insert(u);

            if (!novos_dominados.empty())
            {
                // score = número de novos dominados / peso
                double score = static_cast<double>(novos_dominados.size()) / v->peso;

                cout << " - Vertice " << v->id << " domina " << novos_dominados.size() << " nos, razao = " << score << " | Dominados: ";
                for (char c : novos_dominados)
                    cout << c << " ";
                cout << endl;

                if (score > melhorScore)
                {
                    melhorScore = score;
                    melhorVertice = v;
                    dominados_por_melhorVertice = novos_dominados;
                }
            }
        }

        if (melhorVertice == nullptr) break; // Nenhum vértice pode dominar mais

        cout << " --> Escolhido o vertice: " << melhorVertice->id << " com score " << melhorScore << endl;

        // Adiciona vértice com melhor score
        conjunto_dominante.insert(melhorVertice->id);
        peso_total += melhorVertice->peso;

        // Remove os vértices dominados por ele
        for (char u : dominados_por_melhorVertice) {
            dominados.push_back(u);
            vertices_nao_dominados.erase(u);
        }

        cout << "conjunto dos dominados atualizado depois de retirar o no: " << melhorVertice->id << endl;
        for(char id : dominados) {
            cout << id << " ";
        }

        cout << endl;
        cout << endl;

        //vertices_nao_dominados.erase(melhorVertice->id);

        // cout << "  - Conjunto dominante atualizado: {";
        // for (char id : conjunto_dominante)
        // {
        //     if (id == *conjunto_dominante.rbegin())
        //         cout << id;
        //     else
        //         cout << id << ", ";
        // }
        // cout << "}" << endl;

        // cout << "  - Nao dominados restantes: " << vertices_nao_dominados.size() << endl;
        // for (char id : vertices_nao_dominados)
        // {
        //     if (id == *vertices_nao_dominados.rbegin())
        //         cout << id;
        //     else
        //         cout << id << ", ";
        // }
        // cout << endl;
    }

    return make_pair(vector<char>(conjunto_dominante.begin(), conjunto_dominante.end()), peso_total);
}

pair<vector<char>, vector<double>> Guloso::gulosoRandomizadoAdaptativo(Grafo *grafo, double alfa)
{
    set<char> conjunto_dominante;
    set<char> conjunto_dos_dominados;
    vector<No*> lista_cadidatos = calculaListaCandidatosOrdenada(grafo->lista_adj,conjunto_dos_dominados);
    // double alfa = 0.03;

    random_device rd;
    mt19937 gen(rd());

    int index = 0;
    int peso_total = 0;

    // cout << "lista inicial" << endl;
    // for(No* no : lista_cadidatos) {
    //     cout << no->id << " ";
    // }
    // cout << endl << endl;

    while(conjunto_dos_dominados.size() < grafo->lista_adj.size()) {
        uniform_int_distribution<> distr(0, alfa*(lista_cadidatos.size()-1));
        index = distr(gen);

        // cout << "mukt: " << (alfa*(lista_cadidatos.size()-1)) << endl;

        if(conjunto_dominante.count(lista_cadidatos[index]->id)) break;

        conjunto_dominante.insert(lista_cadidatos[index]->id);
        peso_total += lista_cadidatos[index]->peso;

        No* noEscolhido = new No();
        noEscolhido = lista_cadidatos[index];
        // lista_cadidatos[index] = lista_cadidatos[lista_cadidatos.size()-1];
        // lista_cadidatos[lista_cadidatos.size()-1] = noEscolhido;
        // lista_cadidatos.pop_back();

        lista_cadidatos = atualizaListaCandidatos(lista_cadidatos, noEscolhido , conjunto_dos_dominados);

        // cout << "lista depois de ordenar dominar o no: " << noEscolhido->id << endl;
        // for(No* no : lista_cadidatos) {
        //     cout << no->id << " ";
        // }
        // cout << endl;

        // cout << "consjunto dos dominados depois de dominar o no: " << noEscolhido->id << endl;
        // for(char id : conjunto_dos_dominados) {
        //     cout << id << " ";
        // }
        // cout << endl;
    }

    // cout << "consjunto dominante" << endl;
    // for(char id : conjunto_dominante) {
    //     cout << id << " ";
    // }
    // cout << endl;
    // cout << "peso total: " << peso_total << endl;

    vector<double> resultado;
    resultado.push_back(peso_total);
    // resultado.push_back(alfa);

    return make_pair(vector<char>(conjunto_dominante.begin(), conjunto_dominante.end()), resultado);
}

//ordena usando o algoritmo insertion sort
vector<No*> Guloso::calculaListaCandidatosOrdenada(vector<No*> listaVertices, set<char> conjunto_dos_dominados)
{
    No* aux = new No();

    double razaoAtual = 0;
    double razaoAnterior = 0;

    // cout << "size vertices: " << listaVertices.size() << endl;

    for(int i = 0; i < listaVertices.size()-1; i++) {
        for(int j = i+1; j > 0; j--) {
            // int dominados = getVizinhosNaoDominados(listaVertices[j],conjunto_dos_dominados);
            // cout << "index: " << i << endl;
            // cout << "vertice " << listaVertices[j]->id << " domina " << dominados << " razao: " << static_cast<double>(dominados)/listaVertices[j]->peso << endl;

            razaoAtual = static_cast<double>(getVizinhosNaoDominados(listaVertices[j],conjunto_dos_dominados))/listaVertices[j]->peso;
            razaoAnterior = static_cast<double>(getVizinhosNaoDominados(listaVertices[j-1],conjunto_dos_dominados))/listaVertices[j-1]->peso;

            // cout << "razao do vertice " << listaVertices[j]->id << ": " << razaoAtual << endl;
            // cout << "razao do vertice " << listaVertices[j-1]->id << ": " << razaoAnterior << endl;

            if(razaoAtual > razaoAnterior) {
                //faz o shift da chave
                aux = listaVertices[j];
                listaVertices[j] = listaVertices[j-1];
                listaVertices[j-1] = aux;
            } else {
                break;
            }
        }
    }

    return listaVertices;
}

vector<No*> Guloso::atualizaListaCandidatos(vector<No*>listaVertices, No* no, set<char>&conjunto_dos_dominados)
{
    // cout << "size dominados antes: " << conjunto_dos_dominados.size() << endl;
    // conjunto_dos_dominados.insert(no->id);

    for(Aresta* aresta : no->arestas) {
        // cout << "domina: " << aresta->id_no_alvo << " ";
        if(!conjunto_dos_dominados.count(aresta->id_no_alvo)) {
            conjunto_dos_dominados.insert(aresta->id_no_alvo);
        }
    }

    if(!conjunto_dos_dominados.count(no->id)) {
        conjunto_dos_dominados.insert(no->id);
    }

    // cout << endl;

    // cout << "size dominados depois: " << conjunto_dos_dominados.size() << endl;

    return calculaListaCandidatosOrdenada(listaVertices,conjunto_dos_dominados);
}

int Guloso::getVizinhosNaoDominados(No* no, set<char> conjunto_dos_dominados)
{
    int count = 0;

    // cout << "vertices do no: " << no->id << endl;
    for(Aresta* aresta : no->arestas) {
        // cout << aresta->id_no_alvo << " ";
        if(!conjunto_dos_dominados.count(aresta->id_no_alvo)) {
            count++;
        }
    }

    if(!conjunto_dos_dominados.count(no->id)) {
        // cout << "proprio vertice ainda nao dominado: " << no->id << endl;
        count++;
    }

    return count;
}
