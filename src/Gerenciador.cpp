#include "Gerenciador.h"
#include <fstream>
#include "Grafo.h"


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
    case 'a':
    {
        cout << endl;

        if (!grafo->in_direcionado)
        {
            cout << "Grafo nao direcionado, fecho transitivo direto nao faz sentido!" << endl
                 << endl;
            break;
        }

        char id_no = get_id_entrada();
        vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);

        if (fecho_transitivo_direto.empty())
        {
            cout << "O no '" << id_no << "' nao possui fecho transitivo direto!" << endl
                 << endl;
            break;
        }
        else
        {
            cout << "Ha " << fecho_transitivo_direto.size() << " no(s) no fecho transitivo direto em questao." << endl;
            cout << "-> O fecho eh dado por: F(" << id_no << ")" << " = {";

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
            cout << "Fecho transitivo direto impresso!" << endl << endl;
        }

        break;
    }

    case 'b':
    {
        cout << endl;

        if (!grafo->in_direcionado)
        {
            cout << "Grafo nao direcionado, fecho transitivo indireto nao faz sentido!" << endl
                 << endl;
            break;
        }

        char id_no = get_id_entrada();

        vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
        
        if (fecho_transitivo_indireto.empty())
        {
            cout << "O no '" << id_no << "' nao possui fecho transitivo indireto!" << endl
                 << endl;
            break;
        }
        else
        {
            cout << "Ha "<< fecho_transitivo_indireto.size() << " no(s) no fecho transitivo indireto em questao." << endl;
            cout << "-> O fecho eh dado por: F(" << id_no << ")" << " = {";

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
            cout << "Fecho transitivo indireto impresso!" << endl << endl;
        }

        break;
    }

        case 'c': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1,id_no_2);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }


            break;
        }

        case 'd': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }


            break;
        }
        case 'e': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->ordem) {

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);
                cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

                if(pergunta_imprimir_arquivo("agm_prim.txt")) {
                    cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
                }

                delete arvore_geradora_minima_prim;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'f': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->ordem) {

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

                if(pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                    cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
                }

                delete arvore_geradora_minima_kruskal;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'g': {

            char id_no = get_id_entrada();
            grafo->naoVisitado();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
            // cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            cout << "caminhamento em profuncidade" << endl;
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
            
            cout << endl;

            int raio = grafo->raio();
            cout << "Raio do grafo: R(G) = " << raio << ";" << endl;

            int diametro = grafo->diametro();
            cout << "Diametro do grafo: D(G) = " << diametro << ";" << endl;

            vector<char> centro = grafo->centro();
            cout << "Centro do grafo: C(G) = {";
            
            for (char id : centro){
                if(id == centro.back())
                    cout << id;
                else
                    cout << id << ", ";
            };
    
            cout << "};" << endl;

            vector<char> periferia = grafo->periferia();
            cout << "Periferia do grafo: P(G) = {";

            for (char id : periferia){
                if(id == periferia.back())
                    cout << id;
                else
                    cout << id << ", ";
            };

            cout << "}." << endl;
            cout << endl;


            if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt"))
            {
                cout << "Metodo de impressao em arquivo nao implementado" << endl;
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

//verificar se precisa incluir ou nao o proprio id no fecho (aqui nao inclui)
void Grafo::imprimir_fecho_em_arquivo(const vector<char>& fecho, const string& nome_arquivo) {
    
    string caminho = "instancias/" + nome_arquivo;
    ofstream arquivo(caminho);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
        return;
    }

    //cabeçalho
    arquivo << this->in_direcionado << " " << this->in_ponderado_aresta << " " << this->in_ponderado_vertice << endl;
    arquivo << fecho.size() << endl;

    // imprime os nós do fecho
    for (char id : fecho){
        
        if(this->in_ponderado_vertice) { // se o grafo for ponderado
            No* no = getNoForId(id);
            arquivo << id << " " << no->peso << endl;
        } else {
            arquivo << id << endl;
        }

    }

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