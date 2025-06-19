#include <fstream>
#include <sstream>
#include <cstring>
#include <regex>
#include <climits>
#include <sstream>

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

void Grafo::novoNo(char id, int peso) {
    // Verifica se já existe
    if (getNoForId(id) != nullptr) {
        cout << "No " << id << " já existe no grafo." << endl;
        return;
    }

    No* novo = new No();
    novo->id = id;
    novo->peso = this->in_ponderado_vertice ? peso : 0;
    novo->visitado = false;
    this->lista_adj.push_back(novo);
    this->ordem++;
}

bool Grafo::ehConexo(vector<char>& vertices) {
    if (vertices.empty()) return false;

    vector<bool> visitado(vertices.size(), false);
    vector<char> fila;
    fila.push_back(vertices[0]);
    visitado[0] = true;

    while (!fila.empty()) {
        char atual = fila.front();
        fila.erase(fila.begin());

        No* noAtual = getNoForId(atual);
        if (noAtual == nullptr) continue;

        for (Aresta* aresta : noAtual->arestas) {
            char vizinho = aresta->id_no_alvo;

            // Verifica se o vizinho está no conjunto de vertices
            auto it = find(vertices.begin(), vertices.end(), vizinho);
            if (it != vertices.end()) {
                int index = distance(vertices.begin(), it);
                if (!visitado[index]) {
                    visitado[index] = true;
                    fila.push_back(vizinho);
                }
            }
        }
    }

    // Verifica se todos foram visitados
    for (bool v : visitado) {
        if (!v) return false;
    }

    return true;
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
    //Caminho mínimo entre dois pontos, é inicializado para melhor situar, categorizando os Ids a serem procurados como nós
    No* origem = getNoForId(id_no_a);
    No* destino = getNoForId(id_no_b);

    if (!origem || !destino) {
        cout << "No de origem ou destino nao encontrado." << endl;
        return {};
    }

    //Como não tem uma forma inclusa nos nós ou aresta para ir ao próximo, será mapeado
    map<char, int> distancia;
    map<char, char> precedente;
    map<char, bool> visitado;

    for (No* no : this->lista_adj) {
        distancia[no->id] = INT_MAX;
        precedente[no->id] = 0;
        visitado[no->id] = false;
    }

    //Começamos a percorrer, já visitando o nó de origem e colocando peso 0
    distancia[origem->id] = 0;

    while (true) {
        char vertice_menor = 0;
        int menor_distancia = INT_MAX;

        // Faz a  verificação S <- S - {j}, se encontrar menor é selecionado
        for (map<char, int>::iterator it = distancia.begin(); it != distancia.end(); ++it) {
            char id = it->first;
            int dist = it->second;
            if (!visitado[id] && dist < menor_distancia) {
                menor_distancia = dist;
                vertice_menor = id;
            }
        }

        if (vertice_menor == 0 || vertice_menor == destino->id) break;

        visitado[vertice_menor] = true;

        //mapeia o vértice como já visitado através do peso e vai para o próximo
        No* no_atual = getNoForId(vertice_menor);
        if (!no_atual) continue;

        for (Aresta* aresta : no_atual->arestas) {
            char vizinho = aresta->id_no_alvo;
            int peso = this->in_ponderado_aresta ? aresta->peso : 1;

            if (distancia[vertice_menor] + peso < distancia[vizinho]) {
                distancia[vizinho] = distancia[vertice_menor] + peso;
                precedente[vizinho] = vertice_menor;
            }
        }
    }

    // Reconstruindo o caminho
    vector<char> caminho;
    char atual = destino->id;

    if (distancia[atual] == INT_MAX) {
        cout << "Não existe caminho entre " << id_no_a << " e " << id_no_b << endl;
        return {};
    }

    while (atual != 0) {
        caminho.push_back(atual);
        if (atual == origem->id) break;
        atual = precedente[atual];
    }

    reverse(caminho.begin(), caminho.end());
    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(int id_no_a, int id_no_b) {
    int tamanho = this->lista_adj.size();

    // Mapeia char -> índice e índice -> char
    map<char, int> mapa_id_para_indice;
    map<int, char> mapa_indice_para_id;
    int indice = 0;
    for (No* no : this->lista_adj) {
        mapa_id_para_indice[no->id] = indice;
        mapa_indice_para_id[indice] = no->id;
        indice++;
    }

    // Matriz de distâncias
    vector<vector<int>> distancia(tamanho, vector<int>(tamanho, INT_MAX));

    // Matriz de predecessores
    vector<vector<int>> predecessores(tamanho, vector<int>(tamanho, -1));

    // Pesos das arestas para inicializar a matriz
    for (No* no : this->lista_adj) {
        int u = mapa_id_para_indice[no->id];
        distancia[u][u] = 0; // distância para ele mesmo é zero

        for (Aresta* aresta : no->arestas) {
            int v = mapa_id_para_indice[aresta->id_no_alvo];
            int peso = this->in_ponderado_aresta ? aresta->peso : 1;
            distancia[u][v] = peso;
            predecessores[u][v] = u;
        }
    }

    // Algoritmo de Floyd igual do slide
    for (int k = 0; k < tamanho; k++) {
        for (int i = 0; i < tamanho; i++) {
            for (int j = 0; j < tamanho; j++) {
                if (distancia[i][k] + distancia[k][j] < distancia[i][j]) {
                    distancia[i][j] = distancia[i][k] + distancia[k][j];
                    predecessores[i][j] = predecessores[k][j];
                }
            }
        }
    }

    // Verifica caminho
    int origem = mapa_id_para_indice[(char)id_no_a];
    int destino = mapa_id_para_indice[(char)id_no_b];

    if (distancia[origem][destino] == INT_MAX) {
        cout << "Não existe caminho entre " << (char)id_no_a << " e " << (char)id_no_b << endl;
        return {};
    }

    // Reconstrói o caminho
    vector<char> caminho;
    int atual = destino;
    caminho.push_back(mapa_indice_para_id[atual]);

    while (atual != origem) {
        atual = predecessores[origem][atual];
        if (atual == -1) {
            cout << "Caminho não encontrado corretamente." << endl;
            return {};
        }
        caminho.push_back(mapa_indice_para_id[atual]);
    }

    reverse(caminho.begin(), caminho.end());
    return caminho;
}

Grafo* Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    if (!ehConexo(ids_nos)) {
        cout << "Subgrafo nao eh conexo" << endl;
        return nullptr;
    }

    //Faz as verificações para facilitar o processo
    Grafo* agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = true;
    agm->in_ponderado_vertice = this->in_ponderado_vertice;

    // Adiciona os nós no novo grafo
    for (char id : ids_nos) {
        No* noOriginal = getNoForId(id);
        if (noOriginal) {
            agm->novoNo(noOriginal->id, noOriginal->peso);
        }
    }

    vector<char> vertices_agm;
    vertices_agm.push_back(ids_nos[0]);

    while (vertices_agm.size() < ids_nos.size()) {
        int menor_peso = INT_MAX;
        char origem, destino;

        for (char v : vertices_agm) {
            No* no = getNoForId(v);
            for (Aresta* aresta : no->arestas) {
                char vizinho = aresta->id_no_alvo;
                if (find(vertices_agm.begin(), vertices_agm.end(), vizinho) == vertices_agm.end()) {
                    if (aresta->peso < menor_peso) {
                        menor_peso = aresta->peso;
                        origem = v;
                        destino = vizinho;
                    }
                }
            }
        }

        if (menor_peso == INT_MAX) {
            cout << "Nao ha arestas disponiveis." << endl;
            delete agm;
            return nullptr;
        }

        // Adiciona a aresta ao novo grafo
        No* noOrigem = agm->getNoForId(origem);
        No* noDestino = agm->getNoForId(destino);

        if (noOrigem && noDestino) {
            Aresta* a1 = new Aresta();
            a1->id_no_alvo = destino;
            a1->peso = menor_peso;
            noOrigem->arestas.push_back(a1);

            if (!this->in_direcionado) {
                Aresta* a2 = new Aresta();
                a2->id_no_alvo = origem;
                a2->peso = menor_peso;
                noDestino->arestas.push_back(a2);
            }
        }

        vertices_agm.push_back(destino);
    }

    agm->ordem = agm->lista_adj.size();
    return agm;
}

Grafo* Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    if (this->in_direcionado) {
        cout << "Só funciona para grafos não direcionados" << endl;
        return nullptr;
    }

    if (!ehConexo(ids_nos)) {
        cout << "Subgrafo não é conexo" << endl;
        return nullptr;
    }

    Grafo* agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = true;
    agm->in_ponderado_vertice = this->in_ponderado_vertice;

    // Adiciona nós no grafo AGM
    for (char id : ids_nos) {
        No* noOriginal = getNoForId(id);
        if (noOriginal) {
            agm->novoNo(noOriginal->id, noOriginal->peso);
        }
    }

    // Cria lista de arestas
    vector<tuple<int, char, char>> arestas;

    for (char id : ids_nos) {
        No* no = getNoForId(id);
        if (no) {
            for (Aresta* a : no->arestas) {
                if (find(ids_nos.begin(), ids_nos.end(), a->id_no_alvo) != ids_nos.end()) {
                    if (id < a->id_no_alvo) { // evitar duplicatas
                        arestas.push_back(make_tuple(a->peso, id, a->id_no_alvo));
                    }
                }
            }
        }
    }

    // Ordena as arestas pelo peso
    sort(arestas.begin(), arestas.end());

    map<char, char> pai;
    for (char id : ids_nos) pai[id] = id;

    function<char(char)> encontrar_pai = [&](char vertice) -> char {
        if (pai[vertice] == vertice) return vertice;
        pai[vertice] = encontrar_pai(pai[vertice]);
        return pai[vertice];
    };

    // Une subárvores como pede o Algoritmo de Kruskal
    auto unir_conjuntos = [&](char u, char v) {
        pai[encontrar_pai(u)] = encontrar_pai(v);
    };

    // Executa Kruskal
    for (vector<tuple<int, char, char>>::iterator it = arestas.begin(); it != arestas.end(); ++it) {
        int peso = get<0>(*it);
        char u = get<1>(*it);
        char v = get<2>(*it);

        if (encontrar_pai(u) != encontrar_pai(v)) {
            // Adiciona a aresta no grafo AGM
            No* noU = agm->getNoForId(u);
            No* noV = agm->getNoForId(v);

            Aresta* a1 = new Aresta();
            a1->id_no_alvo = v;
            a1->peso = peso;
            noU->arestas.push_back(a1);

            Aresta* a2 = new Aresta();
            a2->id_no_alvo = u;
            a2->peso = peso;
            noV->arestas.push_back(a2);

            unir_conjuntos(u, v);
        }
    }

    agm->ordem = agm->lista_adj.size();
    return agm;
}

void Grafo::imprimirAgm(Grafo* grafo) {
    //Verifica se o grafo é nulo
    if (grafo == nullptr) {
        cout << "Grafo nulo." << endl;
        return;
    }

    // Pega os vértices
    cout << "Vertices: ";
    for (No* no : grafo->lista_adj) {
        cout << no->id << " ";
    }
    cout << endl;

    // Cria um vetor só para fazer as ligações de arestas
    vector<string> arestas_str;
    int soma_pesos = 0;

    for (No* no : grafo->lista_adj) {
        for (Aresta* aresta : no->arestas) {
            // Que a aresta de ida conte também como a de volta
            if (grafo->in_direcionado || no->id < aresta->id_no_alvo) {
                stringstream ss;
                ss << "(" << no->id << "," << aresta->id_no_alvo << ")";
                arestas_str.push_back(ss.str());

                soma_pesos += aresta->peso;
            }
        }
    }

    cout << "Arestas: [";
    for (size_t i = 0; i < arestas_str.size(); i++) {
        cout << arestas_str[i];
        if (i < arestas_str.size() - 1) cout << ", ";
    }
    cout << "]" << endl;

    // Aqui será feita a soma dos pesos detalhada
    cout << "Soma dos pesos: ";
    bool primeiro = true;

    for (No* no : grafo->lista_adj) {
        for (Aresta* aresta : no->arestas) {
            if (grafo->in_direcionado || no->id < aresta->id_no_alvo) {
                if (!primeiro) cout << " + ";
                cout << aresta->peso << " (" << no->id << "-" << aresta->id_no_alvo << ")";
                primeiro = false;
            }
        }
    }

    cout << " = " << soma_pesos << endl << endl;
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