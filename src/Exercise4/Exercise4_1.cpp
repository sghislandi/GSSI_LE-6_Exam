//c++ -o  Exercise4_1 Exercise4_1.cpp `root-config --glibs --cflags`

#include <iostream>
#include <random>

#include "TApplication.h"
#include "TCanvas.h"
#include "TH1F.h"

using namespace std;

//Function that rolls a dice and extract a number between 1 and 6
int rollDice(){
    random_device random;
    return random() % 6 + 1;
}

//Function that plays a RisiKo round. 
// 1)Both attacker and defender roll 3 dices,
// 2)The results are sorted and compared
// 3)The function returns the attacker score in terms of winnings
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
    }
    return attackerScore;
}


int main(){
    TApplication * App = new TApplication("T",0,NULL);

    //Initializations
    int nDice = 3;
    int nRounds = 1e6;
    int attackerResult;
    double armyWonByAttacker = 0;
    int nBin = nDice + 1;
    double scoreMin = (double) -0.5;
    double scoreMax = (double) +0.5 + nDice; 

    TCanvas * c = new TCanvas();
    TH1F* hResults = new TH1F("", "", nBin, scoreMin, scoreMax);
    //Playing nRounds rounds
    for(int i=0; i<nRounds; i++){
        attackerResult = round(nDice);
        hResults->Fill(attackerResult);
        if(attackerResult>0){armyWonByAttacker += attackerResult;}
    }
    cout << "\n******ATTACKER STATISTICS******\n";
    cout << "Average number of army won (per round): " << (double) armyWonByAttacker / nRounds << endl;
    cout << "Average number of army won (per dice): " << (double) armyWonByAttacker / nRounds / nDice << endl;

    //Plot section
    c->cd();
    hResults->SetLineColor(9);
    hResults->SetLineWidth(2);
    hResults->SetFillStyle(3003);
    hResults->SetFillColor(4);
    hResults->GetXaxis()->SetTitle("Army won by the attacker");
    hResults->GetXaxis()->SetTitleSize(0.045);
    hResults->GetXaxis()->SetTitleOffset(0.9);
    hResults->GetYaxis()->SetTitle("Counts");
    hResults->GetYaxis()->SetTitleSize(0.045);
    hResults->GetYaxis()->SetTitleOffset(1);
    hResults->GetYaxis()->SetMaxDigits(1);
    hResults->GetYaxis()->SetRangeUser(0, hResults->GetMaximum() + nRounds/10);
    hResults->GetXaxis()->SetRangeUser(scoreMin-1, scoreMax+1);
    hResults->Draw();
    c->SaveAs("AttackerResults.pdf");


    App->Run();

    return 0;
}