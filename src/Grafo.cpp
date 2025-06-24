#include <fstream>
#include <sstream>
#include <cstring>
#include <regex>
#include <cmath>
#include <climits>

#include "Grafo.h"

Grafo::Grafo()
{

}

Grafo::Grafo(char* fileName) {
    ifstream ifs;
    ifs.exceptions(std::ios_base::badbit);

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

vector<char> Grafo::fecho_transitivo_direto(int id_no)
{
    set<char> visitados;
    vector<char> resultado;
    
    fecho_busca_em_profundidade(id_no, visitados, resultado);
    resultado.erase(remove(resultado.begin(), resultado.end(), id_no), resultado.end());

    return resultado;
}

vector<char> Grafo::fecho_transitivo_indireto(int id_no) 
{
    Grafo *grafo_transposto = this->transpor_grafo();

    set<char> visitados;
    vector<char> resultado;

    grafo_transposto->fecho_busca_em_profundidade(id_no, visitados, resultado);
    resultado.erase(remove(resultado.begin(), resultado.end(), id_no), resultado.end());

    delete grafo_transposto;

    return resultado;
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

/**
* * Metodo responsavel por retornar a arvore de caminhamento em profundidade a partir de um no
* TODO: Arrumar pesos quando eh ponderado (aresta ou vertice) */
Grafo * Grafo::arvore_caminhamento_profundidade(int id_no) {
    Grafo* grafo = new Grafo();
    grafo->ordem = this->ordem;
    grafo->in_direcionado = this->in_direcionado;
    grafo->in_ponderado_aresta = this->in_ponderado_aresta;
    grafo->in_ponderado_vertice = this->in_ponderado_vertice;

    Grafo* H = new Grafo();
    H->ordem = 0;
    
    for(No* no : this->lista_adj) {
        if(no->id == id_no) {
            no->visitado = true;
            No* noAux = new No();
            noAux->id = id_no;
            noAux->peso = no->peso;
            noAux->visitado = true;
            grafo->lista_adj.push_back(noAux);

            for(Aresta* aresta : no->arestas) {
                No* vertice = getNoForId(aresta->id_no_alvo);
                if(vertice == nullptr) return nullptr;
                if(vertice->visitado) continue;

                H = arvore_caminhamento_profundidade(vertice->id);
                
                int index = grafo->lista_adj.size();
                grafo->lista_adj.insert(grafo->lista_adj.end(), H->lista_adj.begin(), H->lista_adj.end());
                Aresta* are = getArestaForIdAlvo(grafo->lista_adj[0],grafo->lista_adj[index]->id);
                grafo->lista_adj[0]->arestas.push_back(are);
            }
            return grafo;
        }
    }
    return nullptr;
}

int Grafo::raio() 
{
    int raio = INT_MAX/2;

    for (No* no : lista_adj){
        int excentricidade = this->excentricidade(no->id);
        raio = min(raio, excentricidade);
    }
    
    if (raio == INT_MAX/2) {
        cout << "Grafo desconexo, raio indefinido." << endl;
        return -1;
    }

    return raio;
}

int Grafo::diametro()
{
    int diametro = 0;

    for (No* no : lista_adj) {
        int excentricidade = this->excentricidade(no->id);
        diametro = max(diametro, excentricidade);
    }

    if(diametro == 0) {
        cout << "Grafo desconexo, diâmetro indefinido." << endl;
        return -1;
    }

    return diametro;
}

vector<char> Grafo::centro()
{
    int raio = this->raio();

    if (raio == -1) {
        cout << "Grafo desconexo, centro indefinido." << endl;
        return {};
    }

    vector<char> centro;

    for(No* no : lista_adj) {
        int excentricidade = this->excentricidade(no->id);

        if (excentricidade == raio) 
            centro.push_back(no->id);
        
    }

    return centro;
}

vector<char> Grafo::periferia()
{
    int diametro = this->diametro();

    if (diametro == -1) {
        cout << "Grafo desconexo, periferia indefinida." << endl;
        return {};
    }

    vector<char> periferia;

    for(No* no : lista_adj) {
        int excentricidade = this->excentricidade(no->id);

        if (excentricidade == diametro) 
            periferia.push_back(no->id);
        
    }

    return periferia;
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

Aresta* Grafo::getArestaForIdAlvo(No* no, char id_alvo)
{
    Aresta* ares = new Aresta();
    for(No* noAux : this->lista_adj) {
        if(noAux->id == no->id) {
            for(Aresta* aresta : noAux->arestas) {
                if(aresta->id_no_alvo == id_alvo) {
                    ares = aresta;
                    break;
                }
            }
        }
    }
    return ares;
}

void Grafo::naoVisitado()
{
    for(No* no : this->lista_adj) {
        no->visitado = false;
    }
}

void Grafo::imprimir_arvore_caminho_profundidade_em_arquivo(const string nome_arquivo)
{
    string path = "instancias/"+nome_arquivo;

    ofstream ofs;
    ofs.exceptions(ios_base::badbit | ios_base::failbit);
    
    try {
        ofs.open(path,ios_base::trunc | ios_base::binary);

        ofs << this->in_direcionado << " " << this->in_ponderado_aresta << " " << this->in_ponderado_vertice << "\n";
        ofs << this->ordem << "\n";

        for(No* no : this->lista_adj) {
            ofs << no->id;
            if(this->in_ponderado_vertice) {
                ofs << " " << no->peso;
            }

            ofs << "\n";
        }

        for(No* no : this->lista_adj) {
            for(Aresta* aresta : no->arestas) {
                ofs << no->id << " " << aresta->id_no_alvo;
                if(this->in_ponderado_aresta) {
                    ofs << " " << aresta->peso;
                }

                ofs << "\n";
            }
        }

        ofs.close();
    } catch(ios_base::failure& e) {
        cout << "error: " << e.what() << endl;
    }
}

vector<vector<int>> Grafo::calcular_matriz_distancia() {
    
    const int INF = INT_MAX/2;

    int n = this->ordem;
    map<char, int> id_para_indice; 
    vector<char> indice_para_id(n); 

    //trata os ids dos nós que são caracteres para inteiros, assim pode usar como índice na matriz
    for (int i = 0; i < n; i++) { 
        id_para_indice[lista_adj[i]->id] = i; 
        indice_para_id[i] = lista_adj[i]->id; 
    }

    vector<vector<int>> matriz_distancias(n, vector<int>(n, INF)); //inicializa a matriz de distâncias com "infinito"
    
    for (int i = 0; i < n; ++i) //inicializa a matriz de distâncias com 0 para a diagonal 
        matriz_distancias[i][i] = 0;

    for (No* no : lista_adj) {
        int u = id_para_indice[no->id];

        for (Aresta* aresta : no->arestas) {
            int v = id_para_indice[aresta->id_no_alvo];
            int peso = this->in_ponderado_aresta ? aresta->peso : 1;
            matriz_distancias[u][v] = peso;
            
            if (!this->in_direcionado) //se o grafo não for direcionado, coloca a aresta na matriz de forma simétrica
                matriz_distancias[v][u] = peso;
        }

    }

    matriz_floyd_warshall(matriz_distancias, n);

    //imprime a matriz de distâncias (teste para visualização, ta dando certo)
    // cout << "Matriz de distâncias:" << endl;
    // for (int i = 0; i < n; ++i) {
    //     for (int j = 0; j < n; ++j) {
    //         if (matriz_distancias[i][j] == INF)
    //             cout << "INF ";
    //         else
    //             cout << matriz_distancias[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;


    return matriz_distancias;
}

void Grafo::matriz_floyd_warshall(vector<vector<int>> &matriz_distancias, int n) {

    const int INF = INT_MAX/2;

    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (matriz_distancias[i][k] < INF && matriz_distancias[k][j] < INF)
                    matriz_distancias[i][j] = min(matriz_distancias[i][j], matriz_distancias[i][k] + matriz_distancias[k][j]);

}

int Grafo::excentricidade(char id_no)
{
    vector<vector<int>> matriz_distancias = calcular_matriz_distancia();
    int indice = -1;
    int ordem = this->ordem;

    for (int i = 0; i < ordem; i++) { //procura o índice do nó com o id fornecido
        if (lista_adj[i]->id == id_no) {
            indice = i;
            break;
        }
    }

    if (indice == -1) //se o nó não for encontrado, retorna -1
        return -1; 

    int excentricidade = 0;

    for (int j = 0; j < this->ordem; j++) {
        if (matriz_distancias[indice][j] < INT_MAX/2 && matriz_distancias[indice][j] > 0) 
            excentricidade = max(excentricidade, matriz_distancias[indice][j]);
            // cout << "distancia de " << id_no << " para " << lista_adj[j]->id << ": " << matriz_distancias[indice][j] << endl;
            // cout << "excentricidade: " << excentricidade << endl;
    }

    return excentricidade;
}

Grafo *Grafo::transpor_grafo()
{
    Grafo *grafo_transposto = new Grafo();
    grafo_transposto->ordem = this->ordem;
    grafo_transposto->in_direcionado = this->in_direcionado;
    grafo_transposto->in_ponderado_aresta = this->in_ponderado_aresta;
    grafo_transposto->in_ponderado_vertice = this->in_ponderado_vertice;

    for (No *no : this->lista_adj){
        No *novo_no = new No();
        novo_no->id = no->id;
        novo_no->peso = no->peso;
        novo_no->visitado = false;
        grafo_transposto->lista_adj.push_back(novo_no);
    }

    for (No *no : this->lista_adj){
        for (Aresta *aresta : no->arestas){
            No *alvo = grafo_transposto->getNoForId(aresta->id_no_alvo);
            if (alvo){
                Aresta *nova_aresta = new Aresta();
                nova_aresta->id_no_alvo = no->id;
                nova_aresta->peso = aresta->peso;
                alvo->arestas.push_back(nova_aresta);
            }
        }
    }

    return grafo_transposto;
}

void Grafo::fecho_busca_em_profundidade(char id, set<char> &visitados, vector<char> &resultado)
{
    if (visitados.count(id))
        return;

    visitados.insert(id);
    resultado.push_back(id);
    No *no = getNoForId(id);

    if (!no)
        return;

    for (Aresta *aresta : no->arestas)
        fecho_busca_em_profundidade(aresta->id_no_alvo, visitados, resultado);
    
}
