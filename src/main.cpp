#include <iostream>
#include "Gerenciador.h"

//Emerson Caneschi Coelho de Souza
//Emmanuel Gomes Nassif
//Quezia Emanuelly da Silva Oliveira

using namespace std;
int main(int argc, char *argv[])
{
    if(argc >= 2) {
        try {
            Grafo* grafo = new Grafo(argv[1]);
        
            Gerenciador::comandos(grafo);
        } catch(ios_base::failure& e) {
            cout << e.what() << endl; 
        }
    } else {
        cout << "por favor, informe o nome do arquivo com os dados do grafo" << endl;
    }
    
    return 0;
}
