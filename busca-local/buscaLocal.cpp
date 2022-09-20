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

int main(){
    int numLines;
    int seed = 10;
    cin >> numLines;
    int numCidades = numLines;

    vector<ponto> pontos = read_file(numLines);

    default_random_engine generator(seed);

    double distMinGlobal = 100000000000;
    vector<ponto> pontosMinGlobal;

    for(int rounds = 0; rounds < 10*numCidades; rounds ++){
        double distMin = 0;
        vector<ponto> pontosShuffled;
        while(numLines > 0){
            uniform_int_distribution<int> distribution(0,numLines-1);
            int posOut = distribution(generator);
            pontosShuffled.push_back(pontos[posOut]);
            pontos.erase(pontos.begin()+posOut);
            numLines -= 1;
        }

        distMin += calcDist(pontosShuffled.back(), pontosShuffled[0]);
    
        for(int i = 0; i < numCidades - 1 ; i++){
            distMin += calcDist(pontosShuffled[i], pontosShuffled[i+1]);
        }
        for(int i = 0; i < numCidades; i++){
            pontos.push_back(pontosShuffled[i]);
        }
        numLines = numCidades;
         
        vector<ponto> permutacao;

        for(int swapLocal = 0; swapLocal < numCidades; swapLocal++){
            permutacao = pontosShuffled;
            if(swapLocal < numCidades-1){
                swap(permutacao[swapLocal], permutacao[swapLocal+1]);
            } else {
                swap(permutacao.back(), permutacao[0]);
            }

            double distLocal = 0;

            distLocal += calcDist(permutacao.back(), permutacao[0]);
            for(int pont = 0; pont < numCidades - 1 ; pont++){
                distLocal += calcDist(permutacao[pont], permutacao[pont+1]);
            }

            if(distLocal < distMin){
                distMin = distLocal;
                pontosShuffled = permutacao;
            }
        }

        cerr << "local: " << distMin << " " ;
        for(int e = 0; e < numCidades; e++){
            cerr << pontosShuffled[e].id << " ";
        }  
        cerr << endl;

 
        if(distMin < distMinGlobal){
            distMinGlobal = distMin;
            pontosMinGlobal = pontosShuffled;
        }    
    }
    cout << distMinGlobal << " 0" << endl;
    for(int i = 0; i < numCidades; i++){
        cout << pontosMinGlobal[i].id << " ";
    }  
    cout << endl;  
}