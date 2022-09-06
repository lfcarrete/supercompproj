#include <iostream>
#include <math.h>  
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

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

int main(){
    int numLines;
    string line;
    vector<ponto> pontos;
    vector<ponto> pontosOrdenados;
    cin >> numLines;

    for(int i = 0; i < numLines; i++) {
        ponto novoItem;
        novoItem.id = i;
        cin >> line;
        novoItem.x = stof(line);
        cin >> line;
        novoItem.y = stof(line);
        pontos.push_back(novoItem);
    }
    ponto pontoAtual = pontos[0];
    int index = 0;
    double distanciaMinima = sqrt(pow(pontos[0].x, 2) + pow(pontos[0].y, 2));
    for(int i = 1; i < numLines; i++) {
        double dist = sqrt(pow(pontos[0].x, 2) + pow(pontos[0].y, 2));
        if(dist < distanciaMinima) {
            distanciaMinima = dist;
            index = i;
            pontoAtual = pontos[i];
        }
    }
    
    pontos.erase(pontos.begin()+index);
    numLines --;
    pontosOrdenados.push_back(pontoAtual); 

    while(numLines > 0){
        pontoAtual = pontos[0];
        index = 0;
        double distancia = calcDist(pontosOrdenados.back(), pontos[0]);
        for(int i = 1; i < numLines; i++){
            double dist = calcDist(pontosOrdenados.back(), pontos[i]);
            if(dist < distancia){
                distancia = dist;
                pontoAtual = pontos[i];
                index = i;
            }
        }
        pontos.erase(pontos.begin()+index);
        numLines --;
        pontosOrdenados.push_back(pontoAtual); 
    }
    for(int i = 0; i < pontosOrdenados.size(); i++){
            cout << pontosOrdenados[i].id << endl;
    }
}