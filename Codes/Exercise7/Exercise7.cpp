 //c++ -o  Exercise7 Exercise7.cpp `root-config --glibs --cflags`

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <random>

#include "TApplication.h"
#include "TCanvas.h"
#include "TH1F.h" 
#include "TAxis.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TStyle.h"

using namespace std;

double inverseCDF(double x, double mu){
    return -1./mu * log(1-x);
}

double PDFExtraction(double mu, TRandom3* random){
    double extractedNumber;
    extractedNumber = random->Uniform(0.,1.);
    return inverseCDF(extractedNumber, mu);
}

double minimumSelection(double number1, double number2, int &counter){
    if(number1<=number2){
        counter ++;
        return number1;
    }
    else{return number2;}
}

TH1F* Geant4Sampling(int nExtraction, TRandom3* random1, TRandom3* random2, int nBin, int xMin, int xMax, int &counter){
    double s1, s2;
    double mu1 = 1.;
    double mu2 = 2.;

    TH1F * h = new TH1F("","", nBin, xMin, xMax);
    for(int i=0; i<nExtraction; i++){
        s1 = PDFExtraction(mu1, random1);
        s2 = PDFExtraction(mu2, random2);
        h->Fill(minimumSelection(s1,s2, counter));
    }
    return h;
}

TH1F* PenelopeSampling(int nExtraction, TRandom3* random, int nBin, int xMin, int xMax){
    double s;
    double mu = 3; 

    TH1F * h = new TH1F("","", nBin, xMin, xMax);
    for(int i=0; i<nExtraction; i++){
        s = PDFExtraction(mu, random);
        h->Fill(s);
    }
    return h;
}

int main(){
    TApplication * App = new TApplication("T",0,NULL);
    gStyle->SetOptFit(111);
    gStyle->SetOptStat(0000);

    random_device initSeed;
    TRandom3 * randomS1 = new TRandom3;
    TRandom3 * randomS2 = new TRandom3;
    TRandom3 * randomS = new TRandom3;
    randomS1->SetSeed(initSeed());
    randomS2->SetSeed(initSeed());
    randomS->SetSeed(initSeed());

    int nExtraction = 1e5;
    int nBin = 100;
    int xMin = 0;
    int xMax = 3;
    int counter = 0;

    TCanvas * c = new TCanvas();
    TH1F * hGeant4 = new TH1F();
    TH1F * hPenelope = new TH1F();

    hGeant4 = Geant4Sampling(nExtraction, randomS1, randomS2, nBin, xMin, xMax, counter);
    //hPenelope = PenelopeSampling(nExtraction, randomS);

    cout << "****Sampling Fraction Results****\n"
         << " * mu1=1 \t mu1 fraction = " << (double) counter / nExtraction << endl
         << " * mu1=2 \t mu2 fraction = " << (double) (nExtraction - counter) / nExtraction << endl << endl;


    //Plot section 
    c->cd();
    hGeant4->SetLineColor(9);
    hGeant4->SetLineWidth(2);
    hGeant4->SetFillStyle(3003);
    hGeant4->SetFillColor(4);
    hGeant4->GetYaxis()->SetTitle("Counts");
    hGeant4->GetYaxis()->SetTitleSize(0.05);
    hGeant4->GetYaxis()->SetTitleOffset(1);
    hGeant4->GetXaxis()->SetTitle("x");
    hGeant4->GetXaxis()->SetTitleSize(0.05);
    hGeant4->GetXaxis()->SetTitleOffset(0.8);
    hGeant4->Draw();
    /*hPenelope->SetLineColor(2);
    hPenelope->SetLineWidth(2);
    hPenelope->SetFillStyle(3003);
    hPenelope->SetFillColor(2);
    hPenelope->Draw("Same");*/

    //Fit section
    TF1 * fitFunction = new TF1("fitFunction", "[0]*exp(-[1]*x)", xMin, xMax);
    fitFunction->SetParName(0, "Normalization");
    fitFunction->SetParName(1, "#mu");
    hGeant4->Fit(fitFunction, "Q");
    fitFunction->Draw("Same");

    c->SaveAs("Geant4Sampling.pdf");

    App->Run();

    return 0;
}