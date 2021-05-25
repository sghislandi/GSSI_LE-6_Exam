//c++ -o  Exercise1 Exercise1.cpp `root-config --glibs --cflags`


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "TH1F.h"
#include "TF1.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"

using namespace std;

int Iteration(int number){
    return number*663608941;
}

int MINSTD(int number){
    return number;
}

int main(){
    gStyle->SetOptStat(0000);
    TApplication * App = new TApplication("T",0,NULL);
    TCanvas * c = new TCanvas();

    //First simple generator part
    unsigned int seed = 987654321;
    unsigned int extractedNumber = seed;
    int iterationNumber = 0;
    unsigned int period = 4294967295;

    int nBin = 100;
    TH1F * h = new TH1F("Uniform Distribution (All events)", "Uniform Distribution (All events)", nBin, 0, 1);
    TH1F * h1 = new TH1F("Uniform Distribution (First 1000 events)", "Uniform Distribution (First 1000 events)", nBin, 0, 1);
    TH1F * h2 = new TH1F("Uniform Distribution (First 1000000 events)", "Uniform Distribution (First 1000000 events)", nBin, 0, 1);

    while(true){
        extractedNumber = Iteration(extractedNumber);
        iterationNumber ++;
        h->Fill((double) extractedNumber / period);
        if(iterationNumber < 1000){ h1->Fill((double) extractedNumber / period); }
        if(iterationNumber < 1000000){ h2->Fill((double) extractedNumber / period); }
        if(extractedNumber == seed) break;
    }

    cout << "You reobtained the seed after " << iterationNumber << " iterations" << endl;

    //Plots
    c->Divide(1,3);
    c->cd(1);
    h->SetLineColor(9);
    h->SetLineWidth(2);
    h->SetFillStyle(3003);
    h->SetFillColor(4);
    h->GetYaxis()->SetRangeUser(2683000, 2685000);
    h->GetYaxis()->SetTitle("Counts");
    h->Draw();

    c->cd(2);
    h1->SetLineColor(9);
    h1->SetLineWidth(2);
    h1->SetFillStyle(3003);
    h1->SetFillColor(4);
    h1->GetYaxis()->SetTitle("Counts");
    h1->Draw();

    c->cd(3);
    h2->SetLineColor(9);
    h2->SetLineWidth(2);
    h2->SetFillStyle(3003);
    h2->SetFillColor(4);
    h2->GetYaxis()->SetTitle("Counts");
    h2->Draw();

    c->SaveAs("SimpleGenerator.root");
    c->SaveAs("SimpleGenerator.pdf");

    
    
    //MINSTD generation part



    App->Run();

    return 0;
}