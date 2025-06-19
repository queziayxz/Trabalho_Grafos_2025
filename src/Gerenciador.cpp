#include "Gerenciador.h"
#include <fstream>
#include <sstream>
#include <climits>
#include <map>

void Gerenciador::comandos(Grafo* grafo) {
    cout<<"Digite uma das opcoes abaixo e pressione enter:"<<endl<<endl;
    cout<<"(a) Fecho transitivo direto de um no;"<<endl;
    cout<<"(b) Fecho transitivo indireto de um no;"<<endl;
    cout<<"(c) Caminho minimo (Djikstra);"<<endl;
    cout<<"(d) Caminho minimo (Floyd);"<<endl;
    cout<<"(e) Arvore Geradora Minima (Algoritmo de Prim);"<<endl;
    cout<<"(f) Arvore Geradora Minima (Algoritmo de Kruskal);"<<endl;
    cout<<"(g) Arvore de caminhamento em profundidade;"<<endl;
    cout<<"(h) Raio, diametro, centro e periferia do grafo;"<<endl;
    cout<<"(0) Sair;"<<endl<<endl;

    char resp;
    cin >> resp;
    switch (resp) {
        case 'a': {

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl<<endl;
            }


            break;
        }

        case 'b':{

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

;
            break;
        }

        case 'c': {
            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();

            //Chama a função para buscar o caminho mínimo e aloca em um vetor simples
            vector<char> caminho = grafo->caminho_minimo_dijkstra(id_no_1, id_no_2);

            if (caminho.empty()) {
                cout << "Não existe caminho entre " << id_no_1 << " e " << id_no_2 << endl << endl;
            } else {
                // A string é montada para que se possa imprimir na tela o resultado do vetor do caminho encontrado
                stringstream caminho_str;
                int soma_pesos = 0;

                for (size_t i = 0; i < caminho.size(); i++) {
                    caminho_str << caminho[i];
                    if (i < caminho.size() - 1) caminho_str << " -> ";
                }

                // Procura os vértices e as arestas que se ligaram, para pegar o peso, e imprimir posteriormente
                for (size_t i = 0; i < caminho.size() - 1; i++) {
                    No* no = grafo->getNoForId(caminho[i]);
                    for (Aresta* aresta : no->arestas) {
                        if (aresta->id_no_alvo == caminho[i + 1]) {
                            soma_pesos += aresta->peso;
                            break;
                        }
                    }
                }

                cout << "Caminho mínimo (Dijkstra) de " << id_no_1 << " ate " << id_no_2 << ": " 
                    << caminho_str.str() << endl;
                cout << "Soma dos pesos das arestas no caminho: " << soma_pesos << endl << endl;
            }

            if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                cout << "Método de impressão em arquivo não implementado." << endl;
            }

            break;
        }


        case 'd': {
            //Como é caminho, precisa ser inserido os vértices de origem e o destino
            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();

            vector<char> caminho = grafo->caminho_minimo_floyd(id_no_1, id_no_2);

            if (caminho.empty()) {
                cout << "Não existe caminho entre " << id_no_1 << " e " << id_no_2 << endl << endl;
            } else {
                // String que será utilizada como impressão de resposta
                stringstream caminho_str;
                int soma_pesos = 0;

                for (size_t i = 0; i < caminho.size(); i++) {
                    caminho_str << caminho[i];
                    if (i < caminho.size() - 1) caminho_str << " -> ";
                }

                // Será calculada a soma dos pesos
                for (size_t i = 0; i < caminho.size() - 1; i++) {
                    No* no = grafo->getNoForId(caminho[i]);
                    for (Aresta* aresta : no->arestas) {
                        if (aresta->id_no_alvo == caminho[i + 1]) {
                            soma_pesos += aresta->peso;
                            break;
                        }
                    }
                }

                cout << "Caminho mínimo (Floyd) de " << id_no_1 << " ate " << id_no_2 << ": " 
                    << caminho_str.str() << endl;
                cout << "Soma dos pesos das arestas no caminho: " << soma_pesos << endl << endl;
            }

            // Imprime a matriz de distância feita com o algorritmo de Floyd
            cout << "Matriz de distâncias Floyd-Warshall:" << endl;

            map<char, int> mapa_id_para_indice;
            map<int, char> mapa_indice_para_id;
            int indice = 0;

            for (No* no : grafo->lista_adj) {
                mapa_id_para_indice[no->id] = indice;
                mapa_indice_para_id[indice] = no->id;
                indice++;
            }

            int tamanho = grafo->lista_adj.size();

            vector<vector<int>> distancia(tamanho, vector<int>(tamanho, INT_MAX));

            for (int i = 0; i < tamanho; i++) {
                distancia[i][i] = 0;
            }

            for (No* no : grafo->lista_adj) {
                int u = mapa_id_para_indice[no->id];
                for (Aresta* aresta : no->arestas) {
                    int v = mapa_id_para_indice[aresta->id_no_alvo];
                    int peso = grafo->in_ponderado_aresta ? aresta->peso : 1;
                    distancia[u][v] = peso;
                }
            }

            for (int k = 0; k < tamanho; k++) {
                for (int i = 0; i < tamanho; i++) {
                    for (int j = 0; j < tamanho; j++) {
                        if (distancia[i][k] + distancia[k][j] < distancia[i][j]) {
                            distancia[i][j] = distancia[i][k] + distancia[k][j];
                        }
                    }
                }
            }

            cout << "    ";
            for (int i = 0; i < tamanho; i++) {
                cout << mapa_indice_para_id[i] << "   ";
            }
            cout << endl;

            for (int i = 0; i < tamanho; i++) {
                cout << mapa_indice_para_id[i] << " | ";
                for (int j = 0; j < tamanho; j++) {
                    if (distancia[i][j] >= INT_MAX / 2) {
                        cout << "INF ";
                    } else {
                        cout << distancia[i][j] << "   ";
                    }
                }
                cout << endl;
            }

            cout << endl;

            if (pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                cout << "Método de impressão em arquivo não implementado." << endl;
            }

            break;
        }

        case 'e': {
            int tam;
            cout << endl << "Digite o tamanho do subconjunto: ";
            cin >> tam;

            if (tam > 0 && tam <= grafo->ordem) {
                vector<char> ids = get_conjunto_ids(grafo, tam);
                Grafo* agm_prim = grafo->arvore_geradora_minima_prim(ids);

                if (agm_prim != nullptr) {
                    grafo->imprimirAgm(agm_prim);
                } else {
                    cout << "Não foi possível gerar a AGM (Prim)." << endl;
                }

                delete agm_prim;
            } else {
                cout << "Valor inválido" << endl;
            }

            break;
        }

        case 'f': {
            //Bem simples, utiliza o mesmo modelo para pedir a Árvore Geradora Mínima Kruskal
            //e usa o mesmo algoritmo do Prim para imprimir a resposta, por isso virou uma função a parte
            int tam;
            cout << "Digite o tamanho do subconjunto: ";
            cin >> tam;

            if (tam > 0 && tam <= grafo->ordem) {
                vector<char> ids = get_conjunto_ids(grafo, tam);
                Grafo* agm_kruskal = grafo->arvore_geradora_minima_kruskal(ids);

                if (agm_kruskal != nullptr) {
                    grafo->imprimirAgm(agm_kruskal);
                } else {
                    cout << "Não foi possível gerar a AGM (Kruskal)." << endl;
                }

                delete agm_kruskal;
            } else {
                cout << "Valor inválido" << endl;
            }

            break;
        }

        case 'g': {

            char id_no = get_id_entrada();
            grafo->naoVisitado();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
            // cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            cout << "caminhamento em profundidade" << endl;
            for(No* no : arvore_caminhamento_profundidade->lista_adj) {
                cout << no->id << " ";
            }

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

            delete arvore_caminhamento_profundidade;
            break;
        }

        case 'h': {
            vector<char> articulacao = grafo->vertices_de_articulacao();
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

            break;
        }
        case 'i': {

            vector<char> articulacao = grafo->vertices_de_articulacao();
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

            break;
        }

        case '0': {
            exit(0);
        }
        default: {
            cout<<"Opção inválida"<<endl;
        }
    }

    comandos(grafo);

}

char Gerenciador::get_id_entrada() {
    cout<<"Digite o id de um no: ";
    char id;
    cin>>id;
    cout<<endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam) {
    vector<char> ids = {};
    while((int)ids.size() < tam) {
        char id_no =get_id_entrada();
        bool existe = false;
        for(No* no: grafo->lista_adj){
            if(no->id == id_no){
                existe = true;
                break;
            }
        }

        if(!existe){
            cout<<"Vertice nao existe"<<endl;
        }else{
            bool repetido = find(ids.begin(), ids.end(),id_no) != ids.end();
            if(repetido) {
                    cout<<"Valor repetido"<<endl;
            }else {
                ids.push_back(id_no);
            }
        }

    }

    return ids;
}


bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {

    cout<<"Imprimir em arquivo externo? ("<<nome_arquivo<<")"<<endl;
    cout<<"(1) Sim;"<<endl;
    cout<<"(2) Nao."<<endl;
    int resp;
    cin>>resp;
    cout<<endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout<<"Resposta invalida"<<endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}