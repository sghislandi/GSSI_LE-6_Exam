 //c++ -o  Exercise4_2 Exercise4_2.cpp `root-config --glibs --cflags`

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <random>

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TH1F.h"

using namespace std;

int rollDice(){
    random_device random;
    return random() % 6 + 1;
}

void round(int nDice, int nAttacker, int nDefender, vector<int> &score){
    
    int nAttackerDices = 3;
    int nDefenderDices = 3;
    if(nAttacker<nDice){nAttackerDices = nAttacker;}
    if(nDefender<nDice){nDefenderDices = nDefender;}
    int attackerWin = 0;
    int attackerLost = 0;
    vector<int> outcomeA(nAttackerDices,0);
    vector<int> outcomeD(nDefenderDices,0);

    for(int i=0; i<nAttackerDices; i++){
        outcomeA[i] = rollDice();
    }

    for(int i=0; i<nDefenderDices; i++){
        outcomeD[i] = rollDice();
    }

    sort(outcomeA.begin(), outcomeA.end(), greater<int>());
    sort(outcomeD.begin(), outcomeD.end(), greater<int>());

    int comparisonNumber = min(nAttackerDices, nDefenderDices);
    for(int i=0; i<comparisonNumber; i++){
        if(outcomeA[i] > outcomeD[i]){attackerWin++;}
        else{attackerLost++;}
    }
    score[0] = attackerWin;
    score[1] = attackerLost;
}

int main(){

    int nDice = 3;
    int nDefender = 234;
    int nAttacker = 34;
    vector<int> score(2,0);

    round(nDice, nAttacker, nDefender, score);
    cout << score[0] << "-" << score[1] << endl;    

    return 0;
}