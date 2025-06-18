#include <fstream>
#include <sstream>
#include <cstring>
#include <regex>

#include "Grafo.h"

Grafo::Grafo()
{

}

Grafo::Grafo(char* fileName) {
    ifstream ifs;
    ifs.exceptions(std::ios::badbit);

    char relative[50] = "instancias/";
    std::string line;
    int i = 0;

    strcat(relative,fileName);

    try {
        ifs.open(relative, ios_base::in);
        if(ifs.is_open()) {
            getline(ifs,line); //le a primeira linha
            _tokenizationDAV(line); // guarda informacao de ser direcionado,poderado aresta ou vertice
            getline(ifs,line); // le segunda linha com a ordem do grafo
            _tokenizationOrdem(line); // transforma em inteiro a ordem do grafo
            _tokenizationVertices(ifs); // le os vertices do arquivo e armazena na lista de adjacencia
            _tokenizationArestas(ifs); // le as arestas e armazena na lista de adjacencia em cada no
        } else {
            ifs.close();
            throw ios_base::failure("erro ao abrir o arquivo, informe um arquivo dentro de /instancias");
        }
        ifs.close();
    } catch(const ios_base::failure& e) {
        throw ios_base::failure("erro ao abrir o arquivo");
        ifs.close();
    }
}

Grafo::~Grafo() {
}

vector<char> Grafo::fecho_transitivo_direto(int id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(int id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_dijkstra(int id_no_a, int id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(int id_no, int id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_caminhamento_profundidade(int id_no) {
    Grafo* grafo = new Grafo();
    grafo->ordem = 0;
    grafo->in_direcionado = this->in_direcionado;
    grafo->in_ponderado_aresta = this->in_ponderado_aresta;
    grafo->in_ponderado_vertice = this->in_ponderado_vertice;

    Grafo* H = new Grafo();
    H->ordem = 0;

    // cout << "size: inicio " << this->lista_adj.size() << endl;
    
    // cout << "lista adjacencia:" << endl;
    for(No* no : this->lista_adj) {
        // cout << no->id << " ";
        if(no->id == id_no) {
            no->visitado = true;
            // cout << no->id << " | ";
            No* noAux = new No();
            noAux->id = id_no;
            noAux->peso = no->peso;
            noAux->visitado = true;
            grafo->ordem++;
            grafo->lista_adj.push_back(noAux);

            // cout << "size arestas: " << no->arestas.size() << endl;
            for(Aresta* aresta : no->arestas) {
                No* vertice = getNoForId(aresta->id_no_alvo);
                if(vertice == nullptr) return nullptr;
                if(vertice->visitado) continue;

                // vertice->visitado = true;

                H = arvore_caminhamento_profundidade(vertice->id);
                
                int index = grafo->lista_adj.size();
                grafo->lista_adj.insert(grafo->lista_adj.end(), H->lista_adj.begin(), H->lista_adj.end());
                Aresta* are = new Aresta();
                are->id_no_alvo = grafo->lista_adj[index]->id;
                are->peso = 0;
                grafo->lista_adj[0]->arestas.push_back(are);
            }

            return grafo;

        }

        // cout << endl;
    }
    return nullptr;
}

int Grafo::raio() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

int Grafo::diametro() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

vector<char> Grafo::centro() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::periferia() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

void Grafo::_tokenizationDAV(string line)
{
    int i = 0;
    regex del(" ");
    sregex_token_iterator it(line.begin(), line.end(), del, -1);
    sregex_token_iterator end;
    while (it != end) {
        switch(i) {
            case 0:
                this->in_direcionado = *it == '0' ? false : true;
                break;
            case 1:
                this->in_ponderado_aresta = *it == '0' ? false : true;
                break;
            case 2:
                this->in_ponderado_vertice = *it == '0' ? false : true;
                break;
        }
        i++;
        it++;
    }
    cout << "direcionado: " << this->in_direcionado << endl;
    cout << "in_aresta: " << this->in_ponderado_aresta << endl;
    cout << "in_vertice: " << this->in_ponderado_vertice << endl;
}

void Grafo::_tokenizationOrdem(string line)
{
    regex reg("[^0-9]");
    string field = regex_replace(line,reg,"");
    this->ordem = field.empty() ? 0 : stoi(field);
    cout << "ordem: " << this->ordem << endl;

}

void Grafo::_tokenizationVertices(ifstream& ifs)
{
    string line;
    for(int i = 0; i < this->ordem; i++) {
        getline(ifs,line);
        No* vertice = new No();
        vertice->id = line[0];
        vertice->peso = this->in_ponderado_vertice ? stoi(line.substr(2)) : 0;
        vertice->visitado = false;
        this->lista_adj.push_back(vertice);
    }
    cout << "vertice 3 - id: " << this->lista_adj.at(2)->id << endl;
    cout << "vertice 3 - peso: " << this->lista_adj.at(2)->peso << endl;
}

/*
* * Matodo responsavel por realizar o token do arquivo e separar as arestas de cada no
* TODO: Realizar verificacoes para grafos direcionados
*/
void Grafo::_tokenizationArestas(ifstream& ifs)
{
    string line;
    while(getline(ifs,line)) {
        for(No* no : this->lista_adj) {
            if(no->id == line[0]) {
                Aresta* aresta = new Aresta();
                aresta->id_no_alvo = line[2];
                aresta->peso = this->in_ponderado_aresta ? stoi(line.substr(3)) : 0;
                no->arestas.push_back(aresta);
                if(!this->in_direcionado) {
                    for(No* n : this->lista_adj) {
                        if(n->id == line[2]) {
                            Aresta* ares = new Aresta();
                            ares->id_no_alvo = no->id;
                            ares->peso = this->in_ponderado_aresta ? stoi(line.substr(3)) : 0;
                            n->arestas.push_back(ares);
                        }
                    }
                }
            }
        }
    }

    cout << "vertices vizinhos de " << this->lista_adj[0]->id << ": ";
    for(Aresta* ares : this->lista_adj[0]->arestas) {
        cout << ares->id_no_alvo << ", ";
    }
    cout << endl;
}

No* Grafo::getNoForId(int id_no)
{
    for(No* no : this->lista_adj) {
        if(no->id == id_no) return no;
    }

    return nullptr;
}

void Grafo::naoVisitado()
{
    for(No* no : this->lista_adj) {
        no->visitado = false;
    }
}