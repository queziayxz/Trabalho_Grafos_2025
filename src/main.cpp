#include <iostream>
#include "Gerenciador.h"

using namespace std;
int main(int argc, char *argv[])
{
    if(argc >= 2) {
        Grafo* grafo = new Grafo();
    
        Gerenciador::comandos(grafo);
    } else {
        cout << "por favor, informe o nome do arquivo com os dados do grafo" << endl;
    }
    
    return 0;
}
