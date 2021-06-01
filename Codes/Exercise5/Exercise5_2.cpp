//c++ -o  Exercise5_2 Exercise5_2.cpp `root-config --glibs --cflags`

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <random>


#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TLine.h"
#include "TPad.h"
#include "TRandom3.h"


using namespace std;


double exponential(double x){
    return exp(-x);
}

vector<double> Extraction2D(int dimensionality,double xMin, double xMax, double yMin, double yMax, TRandom3* randomArray, vector<double> &randomExtraction){
    //Fill x's
    for(int i=0; i<dimensionality; i++){
        randomExtraction[i] = randomArray[i].Uniform(xMin, xMax);
    }
    //Fill y's
    for(int i=0; i<dimensionality; i++){
        randomExtraction[i+dimensionality] = randomArray[i+dimensionality].Uniform(yMin, yMax);
    }
    return randomExtraction;
}

double HitOrMiss(int dimensionality, double xMin, double xMax, double yMin, double yMax, int nExtraction){
    vector<double> randomExtraction(2*dimensionality,0);
    vector<double> singleIntegrals(dimensionality,0);
    TRandom3 * randomArray;
    randomArray = new TRandom3[2*dimensionality];
    random_device initSeed;
    double integralProduct = 1;

    //Setting the seed
    for(int i=0; i<2*dimensionality; i++){
        randomArray[i].SetSeed(initSeed());
    }
    //Extraction
    for(int i=0; i < nExtraction; i++){
        randomExtraction = Extraction2D(dimensionality, xMin, xMax, yMin, yMax, randomArray, randomExtraction);
        for(int j=0; j<dimensionality; j++){
            if(randomExtraction[j + dimensionality] < exponential(randomExtraction[j])){
                singleIntegrals[j] ++;
            }
        }
    }
    for(int i=0; i<dimensionality; i++){
        singleIntegrals[i] = (double) singleIntegrals[i] / nExtraction;
        singleIntegrals[i] *= 0.632120558;
        singleIntegrals[i] += 0.367879441;
        integralProduct *= singleIntegrals[i];
    }
    return integralProduct;
}

int main(){

    vector<int> nExtraction = {65536, 65536, 64000, 65536, 59049, 46656, 78125, 65536};
    double xMin = 0.;
    double xMax = 1.;
    double e = 2.7182818284; 
    double yMin = (double) 1./e;
    double yMax = 1;
    int maxD = 8;
    vector<double> integrals(maxD, 0);

    for(int dimensionality = 1; dimensionality<=maxD; dimensionality++){
         cout << HitOrMiss(dimensionality, xMin, xMax, yMin, yMax, nExtraction[dimensionality-1]) << endl;
    }

    return 0;
}
