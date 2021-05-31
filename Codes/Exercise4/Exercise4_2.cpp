 //c++ -o  Exercise4_2 Exercise4_2.cpp `root-config --glibs --cflags`

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <random>

#include "TApplication.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TLine.h"

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
    gStyle->SetOptStat(0000);
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
    int nConquerAttempt = 10000;
    int nBin = nAttackerMax-nAttackerMin + 1;

    TH1F * h = new TH1F("", "", nBin, (double)nAttackerMin-0.5, (double)nAttackerMax+0.5);
    for(int i=nAttackerMin; i<=nAttackerMax; i++){
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
    h->GetXaxis()->SetTitle("Number of Attacker armies");
    h->GetXaxis()->SetTitleSize(0.045);
    h->GetXaxis()->SetTitleOffset(0.9);
    h->GetYaxis()->SetTitle("Win probability [%]");
    h->GetYaxis()->SetTitleSize(0.045);
    h->GetYaxis()->SetTitleOffset(1);
    h->Draw("histo");
        
    double probabilityThreshold = 80;
    TLine * probabilityThresholdLine = new TLine((double)nAttackerMin-0.5,probabilityThreshold,
                                                 (double)nAttackerMax+0.5,probabilityThreshold);
    probabilityThresholdLine->SetLineColor(2);
    probabilityThresholdLine->SetLineWidth(2);
    probabilityThresholdLine->SetLineStyle(9);
    probabilityThresholdLine->Draw("same");

    c->SaveAs("NAttackerDistribution.pdf");

    int minimumAttackerNumber = 0;
    for(int i=0; i<nBin; i++){
        if(h->GetBinContent(i) >= probabilityThreshold){
            minimumAttackerNumber = i;
            break;
        }
    }
    cout << "\nThe minimum numbers of attacker armies to conquer Kamchatka" <<
             " (in more than 80% of cases) is " << minimumAttackerNumber << endl;


    //4.2.2
    TCanvas * c1 = new TCanvas();
    int nBin1 = minimumAttackerNumber + 1;
    nConquerAttempt = 100000;
    int minimumRemainingAttackerNumber = 6; 
    double minimumRemainingAttackerNumberProb;


    TH1F * h1 = new TH1F("", "", nBin1, -0.5, minimumAttackerNumber+0.5);
    for(int i=0; i<nConquerAttempt; i++){
        nDefender = nDefenderStart;
        nAttacker = minimumAttackerNumber;
        conquerResult = conquerAttempt(nDice, nAttacker, nDefender);
        h1->Fill(nAttacker);
    }
    scaleFactor = (double) 100. / nConquerAttempt;
    h1->Scale((double) scaleFactor);

    c1->cd();
    h1->SetLineColor(9);
    h1->SetLineWidth(2);
    h1->SetFillStyle(3003);
    h1->SetFillColor(4);
    h1->GetXaxis()->SetTitle("Number of attacker army left");
    h1->GetXaxis()->SetTitleSize(0.045);
    h1->GetXaxis()->SetTitleOffset(0.9);
    h1->GetYaxis()->SetTitle("Probability [%]");
    h1->GetYaxis()->SetTitleSize(0.045);
    h1->GetYaxis()->SetTitleOffset(1);
    h1->Draw("histo");

    c1->SaveAs("RemainingAttackerArmyDistrib.pdf");


    for(int i=minimumRemainingAttackerNumber; i<nBin1; i++){
        minimumRemainingAttackerNumberProb += h1->GetBinContent(i);
    }
    cout << "\nAfter an attack to Kamchatka (defended by 3 armies) with " <<
            minimumAttackerNumber << " armies the probability that you remain" <<
            " with at least 6 armies is " << minimumRemainingAttackerNumberProb << "%" << endl;

    //4.3.3
    TCanvas * c2 = new TCanvas();
    nConquerAttempt = 10000;

    TH1F * h2 = new TH1F("", "", nBin, (double)nAttackerMin-0.5, (double)nAttackerMax+0.5);
    for(int i=nAttackerMin; i<=nAttackerMax; i++){
        nAttackerStart = i;
        for(int j=0; j<nConquerAttempt; j++){
            nDefender = nDefenderStart;
            nAttacker = nAttackerStart;
            conquerResult = 0;
            conquerResult = conquerAttempt(nDice, nAttacker, nDefender);
            if(conquerResult==2 && nAttacker >= 6){
                h2->Fill(nAttackerStart);
            }
        }
    }
    scaleFactor = (double) 100. / nConquerAttempt;
    h2->Scale((double) scaleFactor);

    c2->cd();
    h2->SetLineColor(9);
    h2->SetLineWidth(2);
    h2->SetFillStyle(3003);
    h2->SetFillColor(4);
    h2->GetXaxis()->SetTitle("Number of Attacker armies");
    h2->GetXaxis()->SetTitleSize(0.045);
    h2->GetXaxis()->SetTitleOffset(0.9);
    h2->GetYaxis()->SetTitle("Win probability (with 6 armies remaining) [%]");
    h2->GetYaxis()->SetTitleSize(0.045);
    h2->GetYaxis()->SetTitleOffset(1);
    h2->Draw("histo");

    probabilityThresholdLine->SetLineColor(2);
    probabilityThresholdLine->SetLineWidth(2);
    probabilityThresholdLine->SetLineStyle(9);
    probabilityThresholdLine->Draw("same");
    c2->SaveAs("ConquerAnd6Remaining.pdf");

    minimumAttackerNumber = 0;
    for(int i=0; i<nBin; i++){
        if(h2->GetBinContent(i) >= probabilityThreshold){
            minimumAttackerNumber = i;
            break;
        }
    }
    cout << "\nThe minimum numbers of attacker armies to conquer Kamchatka and" <<
             " remain with more than 6 armies (in more than 80% of cases) is " << 
             minimumAttackerNumber << endl;


    App->Run();
    return 0;
}