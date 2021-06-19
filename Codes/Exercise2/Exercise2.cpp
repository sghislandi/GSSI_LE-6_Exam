//c++ -o  Exercise2 Exercise2.cpp `root-config --glibs --cflags`

#include <stdio.h>

#include "TH1F.h"
#include "TF1.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TRandom3.h"

using namespace std;

//Inverse cumulative function
double inverseCdf(double x){
    return sqrt((double)(x)/(1-x));
}

//Small angle Rutherford distribution
double RutherfordFormula(double *x, double *p){
    return p[0]*2*x[0]/pow((1+x[0]*x[0]),2);
}


int main(){
    gStyle->SetOptStat(0000);
    TApplication * App = new TApplication("T",0,NULL);
    
    //Initialization
    int nExtraction = 10e6;
    int nBin = 1e3;
    double minX = 0;
    double maxX = 20;
    double extractedNumber;
    TRandom3 random_variable;

    TCanvas * c = new TCanvas();
    TH1F * h = new TH1F("Rutherford Distribution", "Rutherford Distribution", nBin, minX, maxX);
    //Extraction
    for(int i=0; i<nExtraction; i++){
        extractedNumber = random_variable.Rndm();
        h->Fill(inverseCdf(extractedNumber));
    }

    //Plot section
    c->cd();
    c->SetLogy();
    h->SetLineColor(9);
    h->SetLineWidth(2);
    h->SetFillStyle(3003);
    h->SetFillColor(4);
    h->GetYaxis()->SetTitle("Counts");
    h->GetYaxis()->SetTitleSize(0.045);
    h->GetYaxis()->SetTitleOffset(1);
    h->GetXaxis()->SetTitle("x");
    h->GetXaxis()->SetTitleSize(0.045);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->Draw();

    //Fitting the histogram
    TF1 * Rutherford = new TF1("Rutherford", RutherfordFormula, minX, maxX, 1);
    Rutherford->SetLineWidth(3);
    h->Fit("Rutherford");
    c->SaveAs("Rutherford.pdf");


    App->Run();

    return 0;
}