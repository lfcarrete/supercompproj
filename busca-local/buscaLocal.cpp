#include <iostream>
#include <math.h>  
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <omp.h>

using namespace std;

struct ponto
{
    int id;
    double x;
    double y;
};

struct bestList
{
    double distMinGlobal;
    vector<ponto> pontosMinGlobal;
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

void calculaBuscaLocal(int numLines, default_random_engine generator, vector<ponto> pontos, bestList *best_list){
        int numCidades = numLines;
        int counter = numLines;
        double distMin = 0;

        vector<ponto> pontosShuffled;
        vector<ponto> pontosLocal = pontos;

        while(counter > 0){
            uniform_int_distribution<int> distribution(0,counter-1);
            int posOut = distribution(generator);
            pontosShuffled.push_back(pontosLocal[posOut]);
            pontosLocal.erase(pontosLocal.begin()+posOut);
            counter -= 1;
        }

        distMin += calcDist(pontosShuffled.back(), pontosShuffled[0]);
    
        for(int i = 0; i < numCidades - 1 ; i++){
            distMin += calcDist(pontosShuffled[i], pontosShuffled[i+1]);
        }
        for(int i = 0; i < numCidades; i++){
            pontosLocal.push_back(pontosShuffled[i]);
        }
        counter = numCidades;
         
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

        #pragma omp critical
        {
            if(distMin < best_list->distMinGlobal){
                best_list->distMinGlobal = distMin;
                best_list->pontosMinGlobal = pontosShuffled;
            }  
        }  
}

int main(){
    int numLines;
    int seed = 10;
    cin >> numLines;

    vector<ponto> pontos = read_file(numLines);

    default_random_engine generator(seed);

    bestList *best_list = new bestList;

    best_list->distMinGlobal = 100000000000;
    

    #pragma omp parallel
    {
        #pragma omp for
        for(int rounds = 0; rounds < 10*numLines; rounds ++){
            calculaBuscaLocal(numLines, generator, pontos, best_list);
        }
    }

    cout << best_list->distMinGlobal << " 0" << endl;
    for(int i = 0; i < numLines; i++){
        cout << best_list->pontosMinGlobal[i].id << " ";
    }  
    cout << endl;    

    return 0; 
}