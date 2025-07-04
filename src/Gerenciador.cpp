#include "Gerenciador.h"
#include <fstream>
#include "Grafo.h"
#include <sstream>
#include <map>
#include <set>
#include <climits>
#include <cmath>
#include <algorithm>


void Gerenciador::comandos(Grafo* grafo) {
    cout << endl << "**************** MENU DE COMANDOS ****************" << endl << endl;
    cout<<"Digite uma das opcoes abaixo e pressione enter:"<<endl<<endl;
    cout<<"(a) Fecho transitivo direto de um no;"<<endl;
    cout<<"(b) Fecho transitivo indireto de um no;"<<endl;
    cout<<"(c) Caminho minimo (Djikstra);"<<endl;
    cout<<"(d) Caminho minimo (Floyd);"<<endl;
    cout<<"(e) Arvore Geradora Minima (Algoritmo de Prim);"<<endl;
    cout<<"(f) Arvore Geradora Minima (Algoritmo de Kruskal);"<<endl;
    cout<<"(g) Arvore de caminhamento em profundidade;"<<endl;
    cout<<"(h) Raio, diametro, centro e periferia do grafo;"<<endl;
    cout<<"(0) Sair."<<endl << endl;
    cout << "************************************************** " << endl;

    char resp;
    cout << "> Opcao desejada: ";
    cin >> resp;
    switch (resp) {
    case 'a':
    {
        cout << endl;
        cout << "**** OPCAO SELECIONADA: Fecho transitivo direto de um no. ****" << endl << endl;

        if (!grafo->in_direcionado)
        {
            cout << " Grafo nao direcionado, fecho transitivo direto nao se aplica!" << endl << endl;
            break;
        }

        char id_no = get_id_entrada();
        valida_entrada_id_no(id_no, grafo);

        vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);

        cout << "  Informacoes do fecho transitivo direto de '"<< id_no << "': " << endl;

        if (fecho_transitivo_direto.empty())
        {
            cout << "  - O no '" << id_no << "' nao possui fecho transitivo direto, pois seu grau de saida eh igual a 0!" << endl << endl;
            break;
        }
        else
        {
            cout << "  - Ha " << fecho_transitivo_direto.size() << " no(s) no fecho, dado por: F(" << id_no << ")" << " = {";

            for (char id : fecho_transitivo_direto)
            {
                if(id == fecho_transitivo_direto.back())
                    cout << id;
                else
                    cout << id << ", ";
            }

            cout << "}" << endl
                 << endl;
        }

        if (pergunta_imprimir_arquivo("fecho_trans_dir.txt"))
        {
            grafo->imprimir_fecho_em_arquivo(fecho_transitivo_direto, "fecho_trans_dir.txt");
            cout << endl << "> Impressao em arquivo realizada! (saidas/fecho_trans_dir.txt)" << endl << endl;
        }

        break;
    }

    case 'b':
    {
        cout << endl;
        cout << "**** OPCAO SELECIONADA: Fecho transitivo indireto de um no. ****" << endl << endl;

        if (!grafo->in_direcionado)
        {
            cout << endl << " Grafo nao direcionado, fecho transitivo indireto nao se aplica!" << endl << endl;
            break;
        }

        char id_no = get_id_entrada();
        valida_entrada_id_no(id_no, grafo);

        vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
        cout << "  Informacoes do fecho transitivo indireto de '"<< id_no << "': " << endl;
        
        if (fecho_transitivo_indireto.empty())
        {
            cout << "  - O no '" << id_no << "' nao possui fecho transitivo indireto, pois seu grau de entrada eh igual a 0!" << endl << endl;
            break;
        }
        else
        {
            cout << "  - Ha "<< fecho_transitivo_indireto.size() << " no(s) no fecho, dado por:  F(" << id_no << ")" << " = {";

            for (char id : fecho_transitivo_indireto)
            {
                if(id == fecho_transitivo_indireto.back())
                    cout << id;
                else
                    cout << id << ", ";
            }

            cout << "}" << endl
                 << endl;
        }

        if (pergunta_imprimir_arquivo("fecho_trans_indir.txt"))
        {
            grafo->imprimir_fecho_em_arquivo(fecho_transitivo_indireto, "fecho_trans_indir.txt");
            cout << endl << "> Impressao em arquivo realizada! (saidas/fecho_trans_indir.txt)" << endl << endl;
        }

        break;
    }

        case 'c': {

            cout << endl;
            cout << "**** OPCAO SELECIONADA: Caminho minimo (Djikstra). ****" << endl << endl;

            char id_no_1 = get_id_entrada();
            valida_entrada_id_no(id_no_1, grafo);

            char id_no_2 = get_id_entrada();
            valida_entrada_id_no(id_no_2, grafo);

            //Chama a função para buscar o caminho mínimo e aloca em um vetor simples
            vector<char> caminho = grafo->caminho_minimo_dijkstra(id_no_1, id_no_2);

            cout << "  Informacoes do caminho minimo (Djikstra) entre '" << id_no_1 << "' e '" << id_no_2 << "':" << endl;

            if (caminho.empty()) {
                cout << "  - Nao existe caminho possivel entre os nos '" << id_no_1 << "' e '" << id_no_2 << "'." << endl << endl;
                break;
            } else {
                // A string é montada para que se possa imprimir na tela o resultado do vetor do caminho encontrado
                stringstream caminho_str;
                int soma_pesos = 0;

                for (size_t i = 0; i < caminho.size(); i++) {
                    caminho_str << caminho[i];
                    if (i < caminho.size() - 1) caminho_str << ", ";
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

                cout << "  - Caminho dado por: " << caminho_str.str() << "; " << endl;
                cout << "  - Soma dos pesos das arestas no caminho minimo: " << soma_pesos << "." << endl << endl;
            }

            if (pergunta_imprimir_arquivo("caminho_minimo_djikstra.txt")) {
                grafo->imprimir_caminho_minimo(caminho, "caminho_minimo_djikstra.txt");
                cout << endl << endl << "> Impressao em arquivo realizada! (saidas/caminho_minimo_djikstra.txt)" << endl << endl;
            }


            break;
        }

        case 'd': {

            cout << endl;
            cout << "**** OPCAO SELECIONADA: Caminho minimo (Floyd). ****" << endl << endl;

            //Como é caminho, precisa ser inserido os vértices de origem e o destino
            char id_no_1 = get_id_entrada();
            valida_entrada_id_no(id_no_1, grafo);

            char id_no_2 = get_id_entrada();
            valida_entrada_id_no(id_no_2, grafo);

            try {
                vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1, id_no_2);
                cout << "  Informacoes do caminho minimo (Floyd) entre '" << id_no_1 << "' e '" << id_no_2 << "':" << endl;
    
                if (caminho_minimo_floyd.empty()) {
                    cout << "  - Nao existe um caminho possivel entre os nos '" << id_no_1 << "' e '" << id_no_2 << "'. "<< endl << endl;
                    break;
                } else {
                    
                    int soma_pesos = 0;
    
                    // Será calculada a soma dos pesos
                    for (size_t i = 1; i < caminho_minimo_floyd.size(); i++) {
                        No* no = grafo->getNoForId(caminho_minimo_floyd[i-1]);
                        for (Aresta* aresta : no->arestas) {
                            if (aresta->id_no_alvo == caminho_minimo_floyd[i]) {
                                soma_pesos += aresta->peso;
                                break;
                            }
                        }
                    }
    
                    cout << "  - Caminho dado por: ";
                    for(int i = 0; i < caminho_minimo_floyd.size(); i++) {
                        cout << caminho_minimo_floyd[i] << "; ";
                        if(i < caminho_minimo_floyd.size()-1) cout << ", ";
                    }
    
                    cout << endl <<  "  - Custo Total: " << soma_pesos << "." << endl;
    
                }
                cout << endl;
    
                if (pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                    grafo->imprimir_caminho_minimo(caminho_minimo_floyd, "caminho_minimo_floyd.txt");
                    cout << endl << endl << "> Impressao em arquivo realizada! (saidas/caminho_minimo_floyd.txt)" << endl << endl;
                }
            } catch(invalid_argument& e) {
                cout << "ERROR: " << e.what() << endl;
            }


            break;
        }
        case 'e': {

            cout << endl;
            cout << "**** OPCAO SELECIONADA: Arvore Geradora Minima (Algoritmo de Prim). ***" << endl;

            if(grafo->in_direcionado) {
                cout << endl << "  Informacoes da Arvore Geradora Minima (Prim): " << endl;
                cout << "  - O algoritmo nao funciona para grafos direcionados." << endl << endl;
                break;
            }

            int tam;
            cout << endl << "> Digite o tamanho do subconjunto: ";
            while (true) {
                cin >> tam;

                if (cin.fail()) {
                    cin.clear(); // limpa o erro
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // descarta entrada inválida
                    cout << "  Valor invalido. Digite um numero inteiro entre 1 e " << grafo->ordem << ": ";

                } 
                else if (tam > 0 && tam <= grafo->ordem) {
                    break;
                } 
                else {
                    cout << "  Valor invalido. Digite um numero inteiro entre 1 e " << grafo->ordem << ": ";
                }
            }

            cout << endl;

            if (tam > 0 && tam <= grafo->ordem) {
                vector<char> ids = get_conjunto_ids(grafo, tam);
                Grafo* agm_prim = grafo->arvore_geradora_minima_prim(ids);
                cout << endl << "  Informacoes da Arvore Geradora Minima (Prim): " << endl;

                if (agm_prim != nullptr) {
                    grafo->imprimirAgm(agm_prim);
     
                    if (pergunta_imprimir_arquivo("arvore_geradora_minima_prim.txt")) {
                        grafo->imprimir_grafo_arquivo(*agm_prim, "arvore_geradora_minima_prim.txt");
                        cout << endl << "> Impressao em arquivo realizada! (saidas/arvore_geradora_minima_prim.txt)" << endl << endl;
                    }
                } else {
                    cout << "  - Nao foi possível gerar a AGM (Prim)." << endl;
                }
                
               //delete agm_prim;

            } else {
                cout << "  - Valor invalido." << endl;
            }

            break;
        }

        case 'f': {

            //Bem simples, utiliza o mesmo modelo para pedir a Árvore Geradora Mínima Kruskal
            //e usa o mesmo algoritmo do Prim para imprimir a resposta, por isso virou uma função a parte
            cout << endl;
            cout << "**** OPCAO SELECIONADA: Arvore Geradora Minima (Algoritmo de Kruskal). ***" << endl << endl;

            if(grafo->in_direcionado) {
                cout << endl << "  Informacoes da Arvore Geradora Minima (Kruskal): " << endl;
                cout << "  - O algoritmo nao funciona para grafos direcionados." << endl << endl;
                break;
            }

            int tam;
            cout << "> Digite o tamanho do subconjunto: ";
            while (true) {
                cin >> tam;

                if (cin.fail()) {
                    cin.clear(); // limpa o erro
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // descarta entrada inválida
                    cout << "  Valor invalido. Digite um numero inteiro valido entre 1 e " << grafo->ordem << ": ";
                } 
                else if (tam > 0 && tam <= grafo->ordem) {
                    break;
                } 
                else {
                    cout << "  Valor invalido. Digite um numero inteiro entre 1 e " << grafo->ordem << ": ";
                }
            }

            cout << endl;

            if (tam > 0 && tam <= grafo->ordem) {
                vector<char> ids = get_conjunto_ids(grafo, tam);
                Grafo* agm_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                cout << endl << "  Informacoes da Arvore Geradora Minima (Kruskal): " << endl;


                if (agm_kruskal != nullptr) {
                    grafo->imprimirAgm(agm_kruskal);

                    if (pergunta_imprimir_arquivo("caminho_minimo_kruskal.txt")) {
                        grafo->imprimir_grafo_arquivo(*agm_kruskal, "arvore_geradora_minima_kruskal.txt");
                        cout << "> Impressao em arquivo realizada! (saidas/arvore_geradora_minima_kruskal.txt)" << endl << endl;
                    }
                } else {
                    cout << "  - Nao foi possivel gerar a AGM (Kruskal)." << endl;
                }

                delete agm_kruskal;

            } else {
                cout << "  - Valor invalido" << endl;
            }

            break;
        }

        case 'g': {

            cout << endl;
            cout << "**** OPCAO SELECIONADA: Arvore de caminhamento em profundidade. ***" << endl << endl;

            char id_no = get_id_entrada();
            valida_entrada_id_no(id_no, grafo);
            
            grafo->naoVisitado();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
            // cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            cout << "  Informacoes da arvore de caminhamento em profundidade: " << endl;
            cout << "   - Caminhamento em profundidade dado por: ";
            for(No* no : arvore_caminhamento_profundidade->lista_adj) {
                //verifica se eh o ultimo elemento
                if(no == arvore_caminhamento_profundidade->lista_adj.back())
                    cout << no->id << "; ";
                else
                    cout << no->id << ", ";
            }

            //imprime a arvore de caminhamento em profundidade
            cout << endl << "   - Representacao da lista de adjacencia da arvore: " << endl;
           
            //imprime a lista de adjacencia da arvore em ordem alfabetica
            sort(arvore_caminhamento_profundidade->lista_adj.begin(), arvore_caminhamento_profundidade->lista_adj.end(), [](No* a, No* b) {
                return a->id < b->id;
            });

            for(No* no : arvore_caminhamento_profundidade->lista_adj) {
                cout << "      " << no->id << ": ";
                
                if(no->arestas.empty()) {
                    cout << "null" << endl;
                    continue;
                }

                for(Aresta* aresta : no->arestas) {
                    //verifica se eh o ultimo elemento
                    if(aresta == no->arestas.back())
                        cout << aresta->id_no_alvo << " ";
                    else
                        cout << aresta->id_no_alvo << " -> ";
                }

                cout << endl;
            }

            cout << endl << endl;

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                grafo->imprimir_grafo_arquivo(*arvore_caminhamento_profundidade, "arvore_caminhamento_profundidade.txt");
                cout << endl << "> Impressao em arquivo realizada! (saidas/arvore_caminhamento_profundidade.txt)" << endl << endl;
            }

            delete arvore_caminhamento_profundidade;
            break;
        }

        case 'h': {
            
            cout << endl;
            cout << "**** OPCAO SELECIONADA: Raio, diametro, centro e periferia do grafo. ***" << endl << endl;
            cout << "  Informacoes do grafo: " << endl;

            int raio = grafo->raio();
            cout << "   - Raio do grafo: rad(G) = " << raio << ";" << endl;

            int diametro = grafo->diametro();
            cout << "   - Diametro do grafo: diam(G) = " << diametro << ";" << endl;

            vector<char> centro = grafo->centro();
            cout << "   - Centro do grafo: Z(G) = {";
            
            for (char id : centro){
                if(id == centro.back())
                    cout << id;
                else
                    cout << id << ", ";
            };
    
            cout << "};" << endl;

            vector<char> periferia = grafo->periferia();
            cout << "   - Periferia do grafo: Per(G) = {";

            for (char id : periferia){
                if(id == periferia.back())
                    cout << id;
                else
                    cout << id << ", ";
            };

            cout << "}." << endl;
            cout << endl;


            if (pergunta_imprimir_arquivo("raio_diametro_centro_periferia.txt"))
            {
                imprimir_raio_diametro_centro_periferia(raio, diametro, centro, periferia, "raio_diametro_centro_periferia.txt");
                cout << endl << "> Impressao em arquivo realizada! (saidas/raio_diametro_centro_periferia.txt)" << endl << endl;
            }

            break;
        }
        case 'i': {

            //vector<char> articulacao = grafo->vertices_de_articulacao();
            //cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            //if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
            //    cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            //}

            break;
        }

        case '0': {
            cout << endl << "  Saindo do programa..." << endl << endl;
            exit(0);
        }
        default: {
            cout << endl << "Opcao invalida! Digite uma opcao entre 'a' e 'h'." << endl << endl;
        }
    }

    comandos(grafo);

}

char Gerenciador::get_id_entrada() {
    cout<<"> Digite o id de um no: ";
    char id;
    cin>>id;
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
            cout << "  O ID informado nao se encontra no grafo. Tente novamente." << endl << endl;

        }else{
            bool repetido = find(ids.begin(), ids.end(),id_no) != ids.end();
            if(repetido) {
                    cout<<" O ID ja foi informado anteriormente. Tente um valor novo."<<endl;
            }else {
                ids.push_back(id_no);
            }
        }

    }

    return ids;
}


bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {
    //corrigindo erro de letra
    cout << "> Imprimir em arquivo externo? (" << nome_arquivo << ")" << endl;
    cout << "  (1) Sim;" << endl;
    cout << "  (2) Nao." << endl;
    int resp;

    while (true) {
        cout << "  Sua resposta: ";
        if (cin >> resp) {
            if (resp == 1) return true;
            if (resp == 2) return false;
            cout << "  Resposta invalida! Digite 1 ou 2." << endl;
        } else {
            cin.clear(); // limpa o failbit
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // descarta o que foi digitado
            cout << "  Resposta invalida! Digite 1 ou 2." << endl;
        }
    }

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

void Gerenciador::imprimir_raio_diametro_centro_periferia(int raio, int diametro, const vector<char>& centro, const vector<char>& periferia, const string& nome_arquivo) {
    
    string caminho = "saidas/" + nome_arquivo;
    ofstream arquivo(caminho);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo para escrita: " << caminho << endl;
        return;
    }

    arquivo << raio << endl;
    arquivo << diametro << endl;

    for(char id : centro) {
        if(id == centro.back())
            arquivo << id;
        else
            arquivo << id << ", ";
    }

    arquivo << endl;

    for(char id : periferia) {
        if(id == periferia.back())
            arquivo << id;
        else
            arquivo << id << ", ";
    }
    
    arquivo.close();

}

void Gerenciador::valida_entrada_id_no(char &id_no, Grafo *grafo) {
    while (!grafo->getNoForId(id_no)) {
        cout << "  O ID informado nao se encontra no grafo. Tente novamente." << endl << endl;
        id_no = Gerenciador::get_id_entrada();
    }
    cout << "  (ID '" << id_no << "' encontrado com sucesso!)." << endl << endl;
}