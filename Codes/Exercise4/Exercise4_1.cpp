//c++ -o  Exercise4_1 Exercise4_1.cpp `root-config --glibs --cflags`

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

double round(int nDice){
    
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
        //cout << outcomeA[i] << "\t" << outcomeD[i] << endl;
        if(outcomeA[i] > outcomeD[i]){attackerScore++;}
        else{attackerScore--;}
    }
    return attackerScore;
}


int main(){
    TApplication * App = new TApplication("T",0,NULL);
    TCanvas * c = new TCanvas();

    int nDice = 3;
    int nRounds = 1000;
    int attackerResult;
    double armyWonByAttacker = 0;
    double armyLostByAttacker = 0;

    int nBin = 2*nDice + 1;
    double scoreMin = (double) -0.5 - nDice;
    double scoreMax = (double) +0.5 + nDice; 

    TH1F* hResults = new TH1F("Results", "Results", nBin, scoreMin, scoreMax);
    for(int i=0; i<nRounds; i++){
        //cout << "***Round " << i << "***" << endl;
        attackerResult = round(nDice);
        hResults->Fill(attackerResult);
        if(attackerResult>0){armyWonByAttacker += attackerResult;}
        else{armyLostByAttacker -= attackerResult;}
        //cout << "Result: " << attackerResult << endl; 
    }

    cout << "\n******ATTACKER STATISTICS******\n";
    cout << "Average number of army won (per round): " << (double) armyWonByAttacker / nRounds << endl;
    cout << "Average number of army won (per dice): " << (double) armyWonByAttacker / nRounds / nDice << endl;
    cout << "Average number of army lost (per round): " << (double) armyLostByAttacker / nRounds << endl;
    cout << "Average number of army lost (per dice): " << (double) armyLostByAttacker / nRounds / nDice << endl;

    c->cd();
    hResults->SetLineColor(9);
    hResults->SetLineWidth(2);
    hResults->SetFillStyle(3003);
    hResults->SetFillColor(4);
    hResults->GetYaxis()->SetRangeUser(0, hResults->GetMaximum() + nRounds/10);
    hResults->GetXaxis()->SetRangeUser(scoreMin-1, scoreMax+1);
    hResults->Draw();
    hResults->SaveAs("AttackerResults.pdf");




    App->Run();
    return 0;
}