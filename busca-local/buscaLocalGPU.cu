#include <iostream>
#include <math.h>  
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <random>
#include <algorithm>

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

struct saxpy {
    int *id;
    double *todasDists;
    int numCity;
    saxpy(int *id_, double *todasDists_, int numCity_) : id(id_), todasDists(todasDists_), numCity(numCity_) {};

    __host__ __device__
    int operator()(const int x) {
///////////// Dist inicial e preenche temp
        int index = x*numCity;
        double distTot = 0;
        int *temp = new int[numCity];
        int id_a;
        int id_b;

        for(int i = 0; i < numCity; i ++){
            if(i == numCity-1){
                id_a = id[index + numCity-1];
                id_b = id[index];
            } else {
                id_a = id[index + i];
                id_b = id[index + i + 1];
            }
            distTot += todasDists[id_a*numCity + id_b];
            
            temp[i] = id_a;
        }
///////////////////////



        bool madeChange = true;
        int *tempReset = new int[numCity];
        for(int v = 0; v < numCity; v++){
            tempReset[v] = temp[v];
        }

        int tempVal = 0;
        double distTemp = 0;
        while (madeChange){
            madeChange = false;
            for(int i = 0; i < numCity; i++){
                if(i == numCity-1){
                    tempVal = temp[i];
                    temp[i] = temp[0];
                    temp[0] = tempVal;
                } else {
                    tempVal   = temp[i];
                    temp[i]   = temp[i+1];
                    temp[i+1] = tempVal;
                }

                for(int c = 0; c < numCity; c ++){
                    if(c == numCity-1){
                        id_a = temp[c];
                        id_b = temp[0];
                    } else {
                        id_a = temp[c];
                        id_b = temp[c + 1];
                    }
                    distTemp += todasDists[id_a*numCity + id_b];
                }

                if(distTemp < distTot){
                    madeChange = true;
                    
                    distTot = distTemp;

                    for(int e = 0; e < numCity; e ++){
                        id[index + e] = temp[e];
                        tempReset[e] = temp[e];
                    }
                } else {
                    for(int v = 0; v < numCity; v++){
                        temp[v] = tempReset[v];
                    }
                }
                distTemp = 0;
            }
        }

        return distTot;
    }
};

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

    vector<ponto> pontos = read_file(numLines);

    thrust::host_vector<double> todasDist(numLines*numLines);

    for(int j = 0; j < numLines; j++){
        for(int i = 0; i < numLines; i++){
            int index = (i * numLines) + j;
            todasDist[index] = calcDist(pontos[j], pontos[i]);
            //cout << todasDist[index];
        }
    }

    thrust::device_vector<double> todasDistGPU(todasDist);    
    default_random_engine generator(seed);
    
    thrust::host_vector<int> todasPerm(10*numLines*numLines);
    vector<ponto> permut;
    for(int rounds = 0; rounds < 10*numLines; rounds ++){
        permut = pontos;
        shuffle(permut.begin(), permut.end(), generator);

        for(int i = 0; i < numLines; i++){
            int index = (rounds * numLines) + i;
            todasPerm[index] = permut[i].id;
        }
    }

    thrust::device_vector<int> todasPermGPU(todasPerm);    

    auto iter = thrust::make_counting_iterator(0);

    thrust::device_vector<int> result(10*numLines);

    thrust::transform(
        iter,
        iter + 10*numLines, 
        result.begin(), 
        saxpy(thrust::raw_pointer_cast(todasPermGPU.data()), thrust::raw_pointer_cast(todasDistGPU.data()), numLines)
    ); 
    
    cout << result.size() << endl;
    for(int rounds = 0; rounds < 10*numLines; rounds ++){
        for(int i = 0; i < numLines; i++){
            int index = (rounds * numLines) + i;
            cout << todasPermGPU[index] << " ";
        }
        cout << " : "<< result[rounds];
        cout << endl;
    }
    return 0; 
}

