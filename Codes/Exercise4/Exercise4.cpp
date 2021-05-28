//c++ -o  Exercise4 Exercise4.cpp `root-config --glibs --cflags`

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <random>

#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TF1.h"

using namespace std;

int rollDice(){
    random_device random;
    return random() % 6 + 1;
}

double round(){
    
    int nDice = 3;
    int attackerScore = 0;
    vector<int> outcomeA(nDice,0);
    vector<int> outcomeD(nDice,0);

    for(int i=0; i<nDice; i++){
        outcomeA[i] = rollDice();
        outcomeD[i] = rollDice();
    }
    sort(outcomeA.begin(), outcomeA.end(), greater<int>());
    sort(outcomeD.begin(), outcomeD.end(), greater<int>());

    for(int i=0; i<nDice; i++){
        cout << outcomeA[i] << "\t" << outcomeD[i] << endl;
        if(outcomeA[i] > outcomeD[i]){attackerScore++;}
        else{attackerScore--;}
    }
    return attackerScore;
}


int main(){

    int nRounds = 2;
    int attackerResult;
    double attackerWinAverage = 0;

    for(int i=0; i<nRounds; i++){
        cout << "***Round " << i << "***" << endl;
        attackerResult = round();
        attackerWinAverage += attackerResult;
        cout << "Result: " << attackerResult << endl; 
    }



    return 0;
}