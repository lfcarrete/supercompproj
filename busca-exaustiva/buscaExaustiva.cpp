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

vector<vector<ponto>> todasOpcoes;

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


void buscaExaustiva(vector<ponto> pontosRestantes ,vector<ponto> pontos, int numCidades) {
    if(pontosRestantes.size() == 0) {
        pontos.push_back(pontos[0]);
        todasOpcoes.push_back(pontos);
        return;
    }
    vector<ponto> permutacao;
    vector<ponto> pontosTemp;
    permutacao = pontos;
    pontosTemp = pontosRestantes;
    for(int i = 0; i < pontosRestantes.size(); i++){
        permutacao.push_back(pontosRestantes[i]);
        pontosTemp.erase(pontosTemp.begin()+i);

        buscaExaustiva(pontosTemp, permutacao, numCidades);

        pontosTemp = pontosRestantes;
        permutacao = pontos;
    }

    return;
}

int main(){
    int numLines;
    cin >> numLines;

    vector<ponto> pontos = read_file(numLines);
    vector<ponto> listaLocal;
    buscaExaustiva(pontos, listaLocal, numLines);

    vector<ponto> melhorPermutacao;
    double distMin = 10000000000;

    for(int i = 0; i < todasOpcoes.size(); i++){
        double distLocal = 0;
        for(int e = 0; e < numLines; e++){
            distLocal += calcDist(todasOpcoes[i][e], todasOpcoes[i][e+1]);
        }
        if(distLocal < distMin){
            distMin = distLocal;
            melhorPermutacao = todasOpcoes[i];
        }
    }
    
    cout << distMin << " 0" << endl;
    for(int i = 0; i < numLines; i++){
        cout << melhorPermutacao[i].id << " ";
    }  
    cout << endl;  

}