//c++ -o  Exercise2 Exercise2.cpp `root-config --glibs --cflags`

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <ctime>
#include "TH1F.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TRandom3.h"

using namespace std;

double inverseCdf(double x){
    return sqrt((double)(x)/(1-x));
}

int main(){
    gStyle->SetOptStat(0000);
    TApplication * App = new TApplication("T",0,NULL);
    TCanvas * c = new TCanvas();

    int nExtraction = 10e6;
    int nBin = 10e3;
    double minAngle = 0;
    double maxAngle = 20;
    double extractedNumber;
    TRandom3 random_variable;

    TH1F * h = new TH1F("Rutherford Distribution", "Rutherford Distribution", nBin, minAngle, maxAngle);
    for(int i=0; i<nExtraction; i++){
        extractedNumber = random_variable.Rndm();
        h->Fill(inverseCdf(extractedNumber));
    }

    c->cd();
    h->SetLineColor(9);
    h->SetLineWidth(2);
    h->SetFillStyle(3003);
    h->SetFillColor(4);
    h->GetYaxis()->SetTitle("Counts");
    h->Draw();

    c->SaveAs("Rutherford.root");
    c->SaveAs("Rutherford.pdf");



    App->Run();

    return 0;
}