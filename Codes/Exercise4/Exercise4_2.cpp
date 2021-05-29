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
            //cout << "The attacker has no more army :(" << endl;
            //printResults(nAttacker,  nDefender,  attackerWin,  attackerLost);
            return 1;
        }
        if(nDefender == 0){
            //cout << "You conquered the Kamchatka!!!" << endl;
            //printResults(nAttacker,  nDefender,  attackerWin,  attackerLost);
            return 2;
        }
    }
    //printResults(nAttacker, nDefender, attackerWin, attackerLost);
    return 0;
}


int conquerAttempt(int nDice, int &nAttacker, int &nDefender){
    int roundCounter = 1;
    int flag = 0;
    while(flag == 0){
        //cout << "\n***Round " << roundCounter << "***\n";
        flag = round(nDice, nAttacker, nDefender);
        //cout << flag << endl;
        roundCounter ++;
    }
    //cout << "\n*********FINAL RESULT*********\n" << flag << endl;
    return flag;
}

int main(){
    TApplication * App = new TApplication("T",0,NULL);
    TCanvas * c = new TCanvas();

    int nDice = 3;
    int nDefenderStart = 3;
    int conquerResult;

    //4.2.1
    int nAttackerStart;
    int nAttackerMin = 1;
    int nAttackerMax = 25;
    int nAttacker, nDefender;
    int nConquerAttempt = 500;
    int nBin = nAttackerMax-nAttackerMin + 1;

    TH1F * h = new TH1F("", "", nBin, (double)nAttackerMin-0.5, (double)nAttackerMax+0.5);
    for(int i=nAttackerMin; i<nAttackerMax; i++){
        nAttackerStart = i;
        for(int j=0; j<nConquerAttempt; j++){
            nDefender = nDefenderStart;
            nAttacker = nAttackerStart;
            conquerResult = 0;
            conquerResult = conquerAttempt(nDice, nAttacker, nDefender);
            if(conquerResult==2){
                h->Fill(nAttackerStart);
            }
        }
    }
    
    double scaleFactor = (double) 100. / nConquerAttempt;
    h->Scale((double) scaleFactor);

    c->cd();
    h->SetLineColor(9);
    h->SetLineWidth(2);
    h->SetFillStyle(3003);
    h->SetFillColor(4);
    h->Draw("histo");
        
    
    App->Run();
    return 0;
}