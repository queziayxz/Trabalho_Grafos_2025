#include <fstream>
#include <sstream>
#include <cstring>
#include <regex>
#include "Grafo.h"
#include <climits>

//Emerson Caneschi Coelho de Souza
//Emmanuel Gomes Nassif
//Quezia Emanuelly da Silva Oliveira

Grafo::Grafo()
{
}

Grafo::Grafo(char *fileName)
{
    ifstream ifs;
    ifs.exceptions(std::ios_base::badbit);

    this->filename = fileName;

    char relative[50] = "instancias/";
    std::string line;
    int i = 0;

    strcat(relative, fileName);

    try
    {
        ifs.open(relative, ios_base::in);
        if (ifs.is_open())
        {
            getline(ifs, line);         // le a primeira linha
            _tokenizationDAV(line);     // guarda informacao de ser direcionado,poderado aresta ou vertice
            getline(ifs, line);         // le segunda linha com a ordem do grafo
            _tokenizationOrdem(line);   // transforma em inteiro a ordem do grafo
            _tokenizationVertices(ifs); // le os vertices do arquivo e armazena na lista de adjacencia
            _tokenizationArestas(ifs);  // le as arestas e armazena na lista de adjacencia em cada no
        }
        else
        {
            ifs.close();
            throw ios_base::failure("erro ao abrir o arquivo, informe um arquivo dentro de /instancias");
        }
        ifs.close();
    }
    catch (const ios_base::failure &e)
    {
        throw ios_base::failure("erro ao abrir o arquivo");
        ifs.close();
    }
}

Grafo::~Grafo()
{
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

vector<char> Grafo::caminho_minimo_dijkstra(int id_no_a, int id_no_b)
{
    // Caminho mínimo entre dois pontos, é inicializado para melhor situar, categorizando os Ids a serem procurados como nós
    No *origem = getNoForId(id_no_a);
    No *destino = getNoForId(id_no_b);

    if (!origem || !destino)
    {
        cout << "  No de origem ou destino nao encontrado." << endl;
        return {};
    }

    // Como não tem uma forma inclusa nos nós ou aresta para ir ao próximo, será mapeado
    map<char, int> distancia;
    map<char, char> precedente;
    map<char, bool> visitado;

    for (No *no : this->lista_adj)
    {
        distancia[no->id] = INT_MAX;
        precedente[no->id] = 0;
        visitado[no->id] = false;
    }

    // Começamos a percorrer, já visitando o nó de origem e colocando peso 0
    distancia[origem->id] = 0;

    while (true)
    {
        char vertice_menor = 0;
        int menor_distancia = INT_MAX;

        // Faz a  verificação S <- S - {j}, se encontrar menor é selecionado
        for (map<char, int>::iterator it = distancia.begin(); it != distancia.end(); ++it)
        {
            char id = it->first;
            int dist = it->second;
            if (!visitado[id] && dist < menor_distancia)
            {
                menor_distancia = dist;
                vertice_menor = id;
            }
        }

        if (vertice_menor == 0 || vertice_menor == destino->id)
            break;

        visitado[vertice_menor] = true;

        // mapeia o vértice como já visitado através do peso e vai para o próximo
        No *no_atual = getNoForId(vertice_menor);
        if (!no_atual)
            continue;

        for (Aresta *aresta : no_atual->arestas)
        {
            char vizinho = aresta->id_no_alvo;
            int peso = this->in_ponderado_aresta ? aresta->peso : 1;

            if (distancia[vertice_menor] + peso < distancia[vizinho])
            {
                distancia[vizinho] = distancia[vertice_menor] + peso;
                precedente[vizinho] = vertice_menor;
            }
        }
    }

    // Reconstruindo o caminho
    vector<char> caminho;
    char atual = destino->id;

    if (distancia[atual] == INT_MAX)
    {
        // cout << "Não existe caminho entre " << id_no_a << " e " << id_no_b << endl;
        return {};
    }

    while (atual != 0)
    {
        caminho.push_back(atual);
        if (atual == origem->id)
            break;
        atual = precedente[atual];
    }

    reverse(caminho.begin(), caminho.end());
    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) 
{
    if(!this->in_ponderado_aresta) throw invalid_argument("Informe um Grafo Ponderado!!");

    vector<vector<int>> caminho_minimo (this->ordem, vector<int>(this->ordem));
    vector<vector<char>> predecessores (this->ordem, vector<char>(this->ordem));
    vector<char> caminho;

    for(int k = 0; k < this->lista_adj.size(); k++) {

        for(int i = 0; i < this->lista_adj.size(); i++) {
            for(int j = 0; j < this->lista_adj.size(); j++) {
                if(this->lista_adj[i]->id == this->lista_adj[j]->id) {
                    caminho_minimo[i][j] = 0;
                    predecessores[i][j] = '-';
                } else {
                    int peso = getPesoAresta(this->lista_adj[i], this->lista_adj[j]->id);
                    if(peso != 0) {
                        caminho_minimo[i][j] = peso;
                        predecessores[i][j] = this->lista_adj[i]->id;
                    } else {
                        caminho_minimo[i][j] = 800000000;
                        predecessores[i][j] = '-';
    
                    }
                }
            }
        }
        
    }

    matriz_floyd(caminho_minimo,predecessores);

    caminho = getCaminho(predecessores,id_no,id_no_b);
    reverse(caminho.begin(),caminho.end());
    if(caminho.size() > 0) caminho.push_back(id_no_b);

    return caminho;

}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    if (!ehConexo(ids_nos))
    {
        cout << "  - Subgrafo nao eh conexo." << endl;
        return nullptr;
    }

    Grafo *agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = true;
    agm->in_ponderado_vertice = this->in_ponderado_vertice;

    // Adiciona os nós no novo grafo
    for (char id : ids_nos)
    {
        No *noOriginal = getNoForId(id);
        if (noOriginal)
        {
            agm->novoNo(noOriginal->id, noOriginal->peso);
        }
    }

    vector<char> vertices_agm;
    vertices_agm.push_back(ids_nos[0]);

    while (vertices_agm.size() < ids_nos.size())
    {
        int menor_peso = INT_MAX;
        char origem, destino;

        for (char v : vertices_agm)
        {
            No *no = getNoForId(v);
            for (Aresta *aresta : no->arestas)
            {
                char vizinho = aresta->id_no_alvo;

                // Condição para corrigir problema de: Aceitar vértices fora da agm
                bool vizinho_nao_inserido = find(vertices_agm.begin(), vertices_agm.end(), vizinho) == vertices_agm.end();
                bool vizinho_esta_no_subgrafo = find(ids_nos.begin(), ids_nos.end(), vizinho) != ids_nos.end();

                if (vizinho_nao_inserido && vizinho_esta_no_subgrafo)
                {
                    if (aresta->peso < menor_peso)
                    {
                        menor_peso = aresta->peso;
                        origem = v;
                        destino = vizinho;
                    }
                }
            }
        }

        if (menor_peso == INT_MAX)
        {
            cout << "  - Nao ha arestas disponiveis." << endl;
            delete agm;
            return nullptr;
        }

        No *noOrigem = agm->getNoForId(origem);
        No *noDestino = agm->getNoForId(destino);

        if (noOrigem && noDestino)
        {
            Aresta *a1 = new Aresta();
            a1->id_no_alvo = destino;
            a1->peso = menor_peso;
            noOrigem->arestas.push_back(a1);

            if (!this->in_direcionado)
            {
                Aresta *a2 = new Aresta();
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


Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{
    if (this->in_direcionado)
    {
        cout << "  - Apenas funciona para grafos nao direcionados." << endl;
        return nullptr;
    }

    if (!ehConexo(ids_nos))
    {
        cout << "  - Subgrafo não eh conexo." << endl;
        return nullptr;
    }

    Grafo *agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = true;
    agm->in_ponderado_vertice = this->in_ponderado_vertice;

    // Adiciona nós no grafo AGM
    for (char id : ids_nos)
    {
        No *noOriginal = getNoForId(id);
        if (noOriginal)
        {
            agm->novoNo(noOriginal->id, noOriginal->peso);
        }
    }

    // Cria lista de arestas
    vector<tuple<int, char, char>> arestas;

    for (char id : ids_nos)
    {
        No *no = getNoForId(id);
        if (no)
        {
            for (Aresta *a : no->arestas)
            {
                if (find(ids_nos.begin(), ids_nos.end(), a->id_no_alvo) != ids_nos.end())
                {
                    if (id < a->id_no_alvo)
                    { // evitar duplicatas
                        arestas.push_back(make_tuple(a->peso, id, a->id_no_alvo));
                    }
                }
            }
        }
    }

    // Ordena as arestas pelo peso
    sort(arestas.begin(), arestas.end());

    map<char, char> pai;
    for (char id : ids_nos)
        pai[id] = id;

    function<char(char)> encontrar_pai = [&](char vertice) -> char
    {
        if (pai[vertice] == vertice)
            return vertice;
        pai[vertice] = encontrar_pai(pai[vertice]);
        return pai[vertice];
    };

    // Une subárvores como pede o Algoritmo de Kruskal
    auto unir_conjuntos = [&](char u, char v)
    {
        pai[encontrar_pai(u)] = encontrar_pai(v);
    };

    // Executa Kruskal
    for (vector<tuple<int, char, char>>::iterator it = arestas.begin(); it != arestas.end(); ++it)
    {
        int peso = get<0>(*it);
        char u = get<1>(*it);
        char v = get<2>(*it);

        if (encontrar_pai(u) != encontrar_pai(v))
        {
            // Adiciona a aresta no grafo AGM
            No *noU = agm->getNoForId(u);
            No *noV = agm->getNoForId(v);

            Aresta *a1 = new Aresta();
            a1->id_no_alvo = v;
            a1->peso = peso;
            noU->arestas.push_back(a1);

            Aresta *a2 = new Aresta();
            a2->id_no_alvo = u;
            a2->peso = peso;
            noV->arestas.push_back(a2);

            unir_conjuntos(u, v);
        }
    }

    agm->ordem = agm->lista_adj.size();
    return agm;
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
    int raio = INT_MAX / 2;

    for (No *no : lista_adj)
    {
        int excentricidade = this->excentricidade(no->id);

        raio = min(raio, excentricidade);
    }

    if (raio == INT_MAX / 2)
    {
        cout << "  - Grafo desconexo, raio indefinido." << endl;
        return -1;
    }

    return raio;
}

int Grafo::diametro()
{
    int diametro = 0;

    for (No *no : lista_adj)
    {
        int excentricidade = this->excentricidade(no->id);
        diametro = max(diametro, excentricidade);
    }

    if (diametro == 0)
    {
        cout << "  - Grafo desconexo, diâmetro indefinido." << endl;
        return -1;
    }

    return diametro;
}

vector<char> Grafo::centro()
{
    int raio = this->raio();

    if (raio == -1)
    {
        cout << "  - Grafo desconexo, centro indefinido." << endl;
        return {};
    }

    vector<char> centro;

    for (No *no : lista_adj)
    {
        int excentricidade = this->excentricidade(no->id);

        if (excentricidade == raio)
            centro.push_back(no->id);
    }

    return centro;
}

vector<char> Grafo::periferia()
{
    int diametro = this->diametro();

    if (diametro == -1)
    {
        cout << "  - Grafo desconexo, periferia indefinida." << endl;
        return {};
    }

    vector<char> periferia;

    for (No *no : lista_adj)
    {
        int excentricidade = this->excentricidade(no->id);

        if (excentricidade == diametro)
            periferia.push_back(no->id);
    }

    return periferia;
}

vector<char> Grafo::vertices_de_articulacao()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}



void Grafo::_tokenizationDAV(string line)
{
    int i = 0;
    regex del(" ");
    sregex_token_iterator it(line.begin(), line.end(), del, -1);
    sregex_token_iterator end;
    while (it != end)
    {
        switch (i)
        {
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
}

void Grafo::_tokenizationOrdem(string line)
{
    regex reg("[^0-9]");
    string field = regex_replace(line, reg, "");
    this->ordem = field.empty() ? 0 : stoi(field);
}

void Grafo::_tokenizationVertices(ifstream &ifs)
{
    string line;
    for (int i = 0; i < this->ordem; i++)
    {
        getline(ifs, line);
        No *vertice = new No();
        vertice->id = line[0];
        vertice->peso = this->in_ponderado_vertice ? stoi(line.substr(2)) : 0;
        vertice->visitado = false;
        this->lista_adj.push_back(vertice);
    }
}

/*
 * * Matodo responsavel por realizar o token do arquivo e separar as arestas de cada no
 * TODO: Realizar verificacoes para grafos direcionados
 */

void Grafo::_tokenizationArestas(ifstream &ifs)
{
    string line;
    while (getline(ifs, line))
    {
        for (No *no : this->lista_adj)
        {
            if (no->id == line[0])
            {
                Aresta *aresta = new Aresta();
                aresta->id_no_alvo = line[2];
                aresta->peso = this->in_ponderado_aresta ? stoi(line.substr(3)) : 0;
                no->arestas.push_back(aresta);
                if (!this->in_direcionado)
                {
                    for (No *n : this->lista_adj)
                    {
                        if (n->id == line[2])
                        {
                            Aresta *ares = new Aresta();
                            ares->id_no_alvo = no->id;
                            ares->peso = this->in_ponderado_aresta ? stoi(line.substr(3)) : 0;
                            n->arestas.push_back(ares);
                        }
                    }
                }
            }
        }
    }
}

No *Grafo::getNoForId(int id_no)
{
    for (No *no : this->lista_adj)
    {
        if (no->id == id_no)
            return no;
    }

    return nullptr;
}

int Grafo::getPesoAresta(No* no, char id_alvo)
{
    for(Aresta* aresta : no->arestas) {
        if(aresta->id_no_alvo == id_alvo) {
            return aresta->peso;
        }
    }

    return 0;
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
    for (No *no : this->lista_adj)
    {
        no->visitado = false;
    }
}

vector<vector<int>> Grafo::calcular_matriz_distancia()
{

    const int INF = INT_MAX / 2;

    int n = this->ordem;
    map<char, int> id_para_indice;
    vector<char> indice_para_id(n);

    // trata os ids dos nós que são caracteres para inteiros, assim pode usar como índice na matriz
    for (int i = 0; i < n; i++)
    {
        id_para_indice[lista_adj[i]->id] = i;
        indice_para_id[i] = lista_adj[i]->id;
    }

    vector<vector<int>> matriz_distancias(n, vector<int>(n, INF)); // inicializa a matriz de distâncias com "infinito"

    for (int i = 0; i < n; ++i) // inicializa a matriz de distâncias com 0 para a diagonal
        matriz_distancias[i][i] = 0;

    for (No *no : lista_adj)
    {
        int u = id_para_indice[no->id];

        for (Aresta *aresta : no->arestas)
        {
            int v = id_para_indice[aresta->id_no_alvo];
            int peso = this->in_ponderado_aresta ? aresta->peso : 1;
            matriz_distancias[u][v] = peso;

            if (!this->in_direcionado) // se o grafo não for direcionado, coloca a aresta na matriz de forma simétrica
                matriz_distancias[v][u] = peso;
        }
    }

    matriz_floyd_warshall(matriz_distancias, n);

    // imprime a matriz de distâncias (teste para visualização, ta dando certo)
    //  cout << "Matriz de distâncias:" << endl;
    //  for (int i = 0; i < n; ++i) {
    //      for (int j = 0; j < n; ++j) {
    //          if (matriz_distancias[i][j] == INF)
    //              cout << "INF ";
    //          else
    //              cout << matriz_distancias[i][j] << " ";
    //      }
    //      cout << endl;
    //  }
    //  cout << endl;

    return matriz_distancias;
}

void Grafo::matriz_floyd_warshall(vector<vector<int>> &matriz_distancias, int n)
{

    const int INF = INT_MAX / 2;
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (matriz_distancias[i][k] < INF && matriz_distancias[k][j] < INF)
                    matriz_distancias[i][j] = min(matriz_distancias[i][j], matriz_distancias[i][k] + matriz_distancias[k][j]);
                    
}

void Grafo::matriz_floyd(vector<vector<int>> &matriz_distancias, vector<vector<char>> &predecessores) 
{
    for (int k = 0; k < this->lista_adj.size(); ++k) {
        for (int i = 0; i < this->lista_adj.size(); ++i) {
            for (int j = 0; j < this->lista_adj.size(); ++j) {
                if(matriz_distancias[i][j] > matriz_distancias[i][k] + matriz_distancias[k][j]) {
                    matriz_distancias[i][j] = matriz_distancias[i][k] + matriz_distancias[k][j];
                    if(predecessores[i][k] == this->lista_adj[i]->id) {
                        predecessores[i][j] = this->lista_adj[k]->id;
                    } else {
                        predecessores[i][j] = predecessores[i][k];
                    }
                }
            }
        }
    }
}

vector<char> Grafo::getCaminho(vector<vector<char>> predecessores, char id_no_a, char id_no_b)
{
    vector<char> caminho;

    int index_a = getIndice(id_no_a);
    int index_b = getIndice(id_no_b);
  
    if(predecessores[index_a][index_b] == id_no_a) {
        caminho.push_back(predecessores[index_a][index_b]);
        return caminho;
    } 
    if(predecessores[index_a][index_b] == '-') {
        return caminho;
    } 
    else {
        caminho = getCaminho(predecessores,predecessores[index_a][index_b],id_no_b);
        caminho.push_back(id_no_a);

    }
    return caminho;
}

int Grafo::getIndice(char id_no)
{
    for(int i = 0; i < this->lista_adj.size(); i++) {
        if(this->lista_adj[i]->id == id_no) {
            return i;
        }
    }

    return -1;
}

int Grafo::excentricidade(char id_no)
{
    vector<vector<int>> matriz_distancias = calcular_matriz_distancia();
    int indice = -1;
    int ordem = this->ordem;

    for (int i = 0; i < ordem; i++)
    { // procura o índice do nó com o id fornecido
        if (lista_adj[i]->id == id_no)
        {
            indice = i;
            break;
        }
    }

    if (indice == -1) // se o nó não for encontrado, retorna -1
        return -1;

    int excentricidade = 0; // inicializa a excentricidade como infinito

    for (int j = 0; j < this->ordem; j++)
    {
        if (matriz_distancias[indice][j] < INT_MAX / 2 && matriz_distancias[indice][j] > 0)
            excentricidade = max(excentricidade, matriz_distancias[indice][j]);
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

    for (No *no : this->lista_adj)
    {
        No *novo_no = new No();
        novo_no->id = no->id;
        novo_no->peso = no->peso;
        novo_no->visitado = false;
        grafo_transposto->lista_adj.push_back(novo_no);
    }

    for (No *no : this->lista_adj)
    {
        for (Aresta *aresta : no->arestas)
        {
            No *alvo = grafo_transposto->getNoForId(aresta->id_no_alvo);
            if (alvo)
            {
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

/////

set<char> Grafo::calcula_vizinhanca_fechada(No *v)
{
    set<char> vizinhanca;
    vizinhanca.insert(v->id);

    for (Aresta *a : v->arestas)
        vizinhanca.insert(a->id_no_alvo);

    return vizinhanca;
}

void Grafo::novoNo(char id, int peso)
{
    // Verifica se já existe
    if (getNoForId(id) != nullptr)
    {
        cout << "No " << id << " já existe no grafo." << endl;
        return;
    }

    No *novo = new No();
    novo->id = id;
    novo->peso = this->in_ponderado_vertice ? peso : 0;
    novo->visitado = false;
    this->lista_adj.push_back(novo);
    this->ordem++;
}

bool Grafo::ehConexo(vector<char> &vertices)
{

    if (vertices.empty())
        return false;

    vector<bool> visitado(vertices.size(), false);
    vector<char> fila;
    fila.push_back(vertices[0]);
    visitado[0] = true;

    while (!fila.empty())
    {
        char atual = fila.front();
        fila.erase(fila.begin());

        No *noAtual = getNoForId(atual);
        if (noAtual == nullptr)
            continue;

        for (Aresta *aresta : noAtual->arestas)
        {
            char vizinho = aresta->id_no_alvo;

            // Verifica se o vizinho está no conjunto de vertices
            auto it = find(vertices.begin(), vertices.end(), vizinho);
            if (it != vertices.end())
            {
                int index = distance(vertices.begin(), it);
                if (!visitado[index])
                {
                    visitado[index] = true;
                    fila.push_back(vizinho);
                }
            }
        }
    }

    // Verifica se todos foram visitados
    for (bool v : visitado)
    {
        if (!v)
            return false;
    }

    return true;
}

void Grafo::imprimirAgm(Grafo *grafo)
{
    // Verifica se o grafo é nulo
    if (grafo == nullptr)
    {
        cout << "  - Grafo nulo." << endl;
        return;
    }

    // Pega os vértices
    cout << "  - Vertices selecionados: ";
    for (No *no : grafo->lista_adj)
    {
        if (no == grafo->lista_adj.back())
            cout << no->id << ". ";
        else
            cout << no->id << ", ";
    }
    cout << endl;

    //imprime lista de adjacência no formato a: b,c,d
    cout << "  - Lista de adjacencia da AGM: " << endl;
    for (No *no : grafo->lista_adj)
    {
        cout << "     " << no->id << ": ";
        //se for vazio, imprime vazio
        if (no->arestas.empty())
        {
            cout << "null" << endl;
            continue;
        }
        for (Aresta *aresta : no->arestas)
        {
            if (aresta == no->arestas.back())
                cout << aresta->id_no_alvo;
            else
                cout << aresta->id_no_alvo << " -> ";
        }
        cout << endl;
    }


    // Cria um vetor só para fazer as ligações de arestas
    vector<string> arestas_str;
    int soma_pesos = 0;

    for (No *no : grafo->lista_adj)
    {
        for (Aresta *aresta : no->arestas)
        {
            // Que a aresta de ida conte também como a de volta
            if (grafo->in_direcionado || no->id < aresta->id_no_alvo)
            {
                stringstream ss;
                ss << "(" << no->id << "," << aresta->id_no_alvo << ")";
                arestas_str.push_back(ss.str());

                soma_pesos += aresta->peso;
            }
        }
    }

    // // cout << "Arestas: [";
    // for (size_t i = 0; i < arestas_str.size(); i++)
    // {
    //     //cout << arestas_str[i];
    //     //if (i < arestas_str.size() - 1)
    //         //cout << ", ";
    // }
    // // cout << "]" << endl;

    // Aqui será feita a soma dos pesos detalhada
    cout << "  - Soma dos pesos: ";
    bool primeiro = true;

    // for (No *no : grafo->lista_adj)
    // {
    //     for (Aresta *aresta : no->arestas)
    //     {
    //         if (grafo->in_direcionado || no->id < aresta->id_no_alvo)
    //         {
    //             if (!primeiro)
    //                 cout << " + ";
    //             cout << aresta->peso << " (" << no->id << "-" << aresta->id_no_alvo << ")";
    //             primeiro = false;
    //         }
    //     }
    // }

    cout << soma_pesos << endl << endl;
}


//Teste imprimir agm com base imprimir grafo
void Grafo::imprimir_grafo_arquivo(const Grafo& grafo, const string& nome_arquivo) {
    string caminho = "saidas/" + nome_arquivo;
    ofstream arquivo(caminho);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo para escrita: " << caminho << endl;
        return;
    }

    // Cabeçalho do arquivo (direcionado, ponderado_aresta, ponderado_vertice)
    arquivo << grafo.in_direcionado << " " << grafo.in_ponderado_aresta << " " << grafo.in_ponderado_vertice << endl;

    // Número de nós
    arquivo << grafo.lista_adj.size() << endl;

    // Impressão dos nós
    for (No* no : grafo.lista_adj) {
        if (grafo.in_ponderado_vertice) {
            arquivo << no->id << " " << no->peso << endl;
        } else {
            arquivo << no->id << endl;
        }
    }

    // Impressão das arestas
    for (No* no : grafo.lista_adj) {
        for (Aresta* aresta : no->arestas) {
            // Evita duplicatas se for não direcionado
            if (grafo.in_direcionado || no->id < aresta->id_no_alvo) {
                if (grafo.in_ponderado_aresta) {
                    arquivo << no->id << " " << aresta->id_no_alvo << " " << aresta->peso << endl;
                } else {
                    arquivo << no->id << " " << aresta->id_no_alvo << endl;
                }
            }
        }
    }

    arquivo.close();
}


//Função para criar o arquivo, inicialização padrão
void Grafo::imprimir_base(const vector<char>& vetor, ofstream& arquivo) {
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo.txt" << endl;
        return;
    }

    // cabeçalho
    arquivo << this->in_direcionado << " " << this->in_ponderado_aresta << " " << this->in_ponderado_vertice << endl;
    arquivo << vetor.size() << endl;

    // imprime os nós
    for (char id : vetor){
        if(this->in_ponderado_vertice) { // se o grafo for ponderado
            // Procura o nó correspondente em lista_adj
            No* no = nullptr;
            for (No* n : lista_adj) {
                if (n->id == id) {
                    no = n;
                    break;
                }
            }
            if (no)
                arquivo << id << " " << no->peso << endl;
            else
                arquivo << id << " " << 0 << endl; // ou outro valor padrão
        } else {
            arquivo << id << endl;
        }
    }
}


//verificar se precisa incluir ou nao o proprio id no fecho (aqui nao inclui)
void Grafo::imprimir_fecho_em_arquivo(const vector<char>& fecho, const string& nome_arquivo) {
    
    string caminho = "saidas/" + nome_arquivo;
    ofstream arquivo(caminho);
    
    this->imprimir_base(fecho, arquivo);

    for (char id : fecho) {

        No* no = getNoForId(id);

        for (Aresta* aresta : no->arestas) {
            
            bool destino_no_fecho = false;
            
            for (char outro : fecho) { //verifica se o destino da aresta está no fecho
                
                if (aresta->id_no_alvo == outro) {
                    destino_no_fecho = true;
                    break;
                }
            }

            if (destino_no_fecho) { //se o destino da aresta está no fecho, imprime a aresta
                
                if (this->in_ponderado_aresta)
                    arquivo << id << " " << aresta->id_no_alvo << " " << aresta->peso << endl;
                else
                    arquivo << id << " " << aresta->id_no_alvo << endl;
            }
        }
    }

    arquivo.close();
}


//
void Grafo::imprimir_caminho_minimo(const vector<char>& caminhoMin, const string& nome_arquivo) {
    
    string caminho = "saidas/" + nome_arquivo;
    ofstream arquivo(caminho);
    
    this->imprimir_base(caminhoMin, arquivo);

    for (char id : caminhoMin) {

        No* no = getNoForId(id);

        for (Aresta* aresta : no->arestas) {
            
            bool destino_no_caminhoMin = false;
            
            for (char outro : caminhoMin) { //verifica se o destino da aresta está no fecho
                
                if (aresta->id_no_alvo == outro) {
                    destino_no_caminhoMin = true;
                    break;
                }
            }

            if (destino_no_caminhoMin) { //se o destino da aresta está no fecho, imprime a aresta
                
                if (this->in_ponderado_aresta)
                    arquivo << id << " " << aresta->id_no_alvo << " " << aresta->peso << endl;
                else
                    arquivo << id << " " << aresta->id_no_alvo << endl;
            }
        }
    }

    arquivo.close();
}

No* Grafo::getNo(char id) {
    return getNoForId(id);
}

//imprimir conjunto guloso
void Grafo::imprimir_conjunto_guloso(const vector<char>& conjunto, double peso_total, const string& nome_arquivo) {
    
    string caminho = "saidas/" + nome_arquivo;
    ofstream arquivo(caminho);

    arquivo << peso_total << endl;

    for(char id : conjunto) {
        arquivo << id << endl;
    }

    arquivo.close();
}

//imprimir conjunto guloso randomizado
void Grafo::imprimir_conjunto_guloso_randomizado(ofstream&arquivo, pair<vector<char>, vector<double>> dados, int iteracao) {

    arquivo << "  --> Iteracao: " << iteracao+1 << endl;
    
    arquivo << "     Conjunto Dominante de Peso Minimo:" << endl;
    arquivo << "     D = { ";
    for(char id : dados.first) {
        if (id == dados.first.back()) {
            arquivo << id << " ";
        } else {
            arquivo << id << ", ";
        }
    }
    
    arquivo << "}" << endl;
    
    arquivo << "     Peso Total: " << dados.second[0] << endl;
    arquivo << "     Alfa Utilizado: " << dados.second[1] << endl;
    arquivo << "     Duracao do Algoritmo: " << dados.second[2] << " micros" << endl; //duracao
    
    arquivo << endl;
}