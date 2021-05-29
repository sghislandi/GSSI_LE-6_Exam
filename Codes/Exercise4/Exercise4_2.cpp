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

void printResults(int nAttacker, int nDefender, int attackerWin, int attackerLost){
    cout << attackerWin << "-" << attackerLost << endl;
    cout << "Attacker: " << nAttacker << "\t Defender: " << nDefender << endl;
}

void fillOutcome(int nAttDefDices, vector<int> &outcome){
    for(int i=0; i<nAttDefDices; i++){
        outcome[i] = rollDice();
    }
}


int round(int nDice, int &nAttacker, int &nDefender){
    
    int nAttackerDices = 3;
    int nDefenderDices = 3;
    if(nAttacker<nDice){nAttackerDices = nAttacker;}
    if(nDefender<nDice){nDefenderDices = nDefender;}
    int attackerWin = 0;
    int attackerLost = 0;
    vector<int> outcomeA(nAttackerDices,0);
    vector<int> outcomeD(nDefenderDices,0);

    fillOutcome(nAttackerDices, outcomeA);
    fillOutcome(nDefenderDices, outcomeD);
    sort(outcomeA.begin(), outcomeA.end(), greater<int>());
    sort(outcomeD.begin(), outcomeD.end(), greater<int>());

    int comparisonNumber = min(nAttackerDices, nDefenderDices);
    for(int i=0; i<comparisonNumber; i++){
        if(outcomeA[i] > outcomeD[i]){
            attackerWin++;
            nDefender --;
        }
        else{
            attackerLost++;
            nAttacker --;
        }
        if(nAttacker == 0){
            cout << "The attacker has no more army :(" << endl;
            printResults(nAttacker,  nDefender,  attackerWin,  attackerLost);
            return 1;
        }
        if(nDefender == 0){
            cout << "You conquered the Kamchatka!!!" << endl;
            printResults(nAttacker,  nDefender,  attackerWin,  attackerLost);
            return 2;
        }
    }
    printResults(nAttacker, nDefender, attackerWin, attackerLost);
    return 0;
}


int conquerAttempt(int nDice, int &nAttacker, int &nDefender){
    int roundCounter = 1;
    int flag = 0;
    while(flag == 0){
        cout << "\n***Round " << roundCounter << "***\n";
        flag = round(nDice, nAttacker, nDefender);
        roundCounter ++;
    }
    cout << "\n*********FINAL RESULT*********\n" << flag << endl;
    return flag;
}

int main(){

    int nDice = 3;
    int nDefender = 23;
    int nAttacker = 456;
    int conquerResult = 0;

    cout << "Starting Point" << endl;
    cout << "Attacker: " << nAttacker << "\t Defender: " << nDefender << endl;

    conquerResult = conquerAttempt(nDice, nAttacker, nDefender);
    return 0;
}