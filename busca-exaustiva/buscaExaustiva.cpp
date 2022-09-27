#include <iostream>
#include <math.h>  
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <random>

using namespace std;

struct ponto
{
    int id;
    double x;
    double y;
};

double calcDist(ponto a, ponto b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

vector<ponto> melhorGlobal;
double distMin = 10000000000;

vector<ponto> read_file(int numLines) {
    vector<ponto> pontos;
    string line;
    for(int i = 0; i < numLines; i++) {
        ponto novoItem;
        novoItem.id = i;
        cin >> line;
        novoItem.x = stof(line);
        cin >> line;
        novoItem.y = stof(line);
        pontos.push_back(novoItem);
    }
    return pontos;
}


long long int buscaExaustiva(vector<ponto> pontosRestantes ,vector<ponto> pontos, int numCidades) {
    if(pontosRestantes.size() == 0) {
        pontos.push_back(pontos[0]);
        double distLocal = 0;
        for(int e = 0; e < numCidades; e++){
            distLocal += calcDist(pontos[e], pontos[e+1]);
        }
        if(distLocal < distMin){
            distMin = distLocal;
            melhorGlobal = pontos;
        }
        
        return 1;
    }
    vector<ponto> permutacao;
    vector<ponto> pontosTemp;
    permutacao = pontos;
    pontosTemp = pontosRestantes;
    long long int numFolhasLevel = 0;
    for(long unsigned int i = 0; i < pontosRestantes.size(); i++){
        permutacao.push_back(pontosRestantes[i]);
        pontosTemp.erase(pontosTemp.begin()+i);

        numFolhasLevel += buscaExaustiva(pontosTemp, permutacao, numCidades);

        pontosTemp = pontosRestantes;
        permutacao = pontos;
    }

    return numFolhasLevel;
}

int main(){
    int numLines;
    cin >> numLines;

    vector<ponto> pontos = read_file(numLines);
    vector<ponto> listaLocal;
    long long int numFolhas = 0;

    listaLocal.push_back(pontos[0]);
    pontos.erase(pontos.begin()+0);

    numFolhas = buscaExaustiva(pontos, listaLocal, numLines);
    
    cerr << "num_leaf " << numFolhas << endl;

    cout << distMin << " 1" << endl;
    for(int i = 0; i < numLines; i++){
        cout << melhorGlobal[i].id << " ";
    } 
    cout << endl;  

} 