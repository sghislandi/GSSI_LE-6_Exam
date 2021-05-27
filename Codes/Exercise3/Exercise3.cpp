//c++ -o  Exercise3 Exercise3.cpp `root-config --glibs --cflags`

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TEllipse.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TF1.h"
#include "TRandom3.h"


using namespace std;


void extraction(vector<double> &point, double &PI){
    double random1, random2;
    random1 = (double) rand()/RAND_MAX;
    random2 = (double) rand()/RAND_MAX;
    point[0] = -1. + 2.*random1;
    point[1] = -1. + 2.*random2;
    if (point[0]*point[0] + point[1]*point[1] <= 1){PI ++;}
}

double PIMonteCarlo(int nExtraction, int seed){
    double PI = 0;
    double x, y;
    TRandom3 rand;
    rand.SetSeed(seed);

    for(int i=0; i<nExtraction; i++){
        x = rand.Uniform(-1,1);
        y = rand.Uniform(-1,1); 
        if(x*x + y*y <= 1){
            PI ++;
        }
    }
    PI = (double) 4. * PI / (double) nExtraction;
    return PI;
}


int main(){
    TApplication * App = new TApplication("T",0,NULL);
    TCanvas * c = new TCanvas();
    TCanvas * c1 = new TCanvas();
    TGraph * gToss = new TGraph();
    TGraph * gDifference = new TGraph();
    TEllipse *circle = new TEllipse(0,0,1);
    
    int nExtraction = 1e4;
    vector<double> point(2,0);
    double PI = 0;
    double difference;

    for(int i=0; i<nExtraction; i++){
        extraction(point, PI);
        gToss->SetPoint(i, point[0], point[1]);
        if(i % 10 == 0){
            gDifference->SetPoint(gDifference->GetN(), i, abs(((double) 4. * PI / nExtraction) - M_PI));
        }
    }
    PI = (double) 4. * PI / nExtraction;
    cout << "Your estimate of pi is: " << PI << endl;

    c->SetCanvasSize(700, 700);
    c->cd();
    gToss->SetMarkerStyle(8);
    gToss->SetMarkerSize(0.5);
    gToss->SetMarkerColor(2);
    gToss->GetXaxis()->SetTitle("x coordinate");
    gToss->GetXaxis()->SetTitleSize(0.045);
    gToss->GetXaxis()->SetTitleOffset(0.9);
    gToss->GetYaxis()->SetTitle("y coordinate");
    gToss->GetYaxis()->SetTitleSize(0.045);
    gToss->GetYaxis()->SetTitleOffset(1.1);
    gToss->Draw("AP");
    circle->SetLineWidth(3);
    circle->SetLineColor(kBlue);
    circle->SetFillStyle(0);
    circle->Draw("same");
    c->SaveAs("CircleToss.pdf");

    c1->cd();
    c1->SetLogy();
    gDifference->GetXaxis()->SetTitle("Number of extractions");
    gDifference->GetXaxis()->SetTitleSize(0.045);
    gDifference->GetXaxis()->SetTitleOffset(0.9);
    gDifference->GetYaxis()->SetTitle("|Deviation|");
    gDifference->GetYaxis()->SetTitleSize(0.045);
    gDifference->GetYaxis()->SetTitleOffset(0.8);
    gDifference->SetMarkerStyle(8);
    gDifference->SetMarkerSize(0.5);
    gDifference->Draw("AP");
    c1->SaveAs("Difference.pdf");



    //Evaluate the uncertainties on the PI evaluation
    nExtraction = 100;
    int nPiEvaluations = 10000;
    int nBin = 50;
    TRandom3 seed;

    TCanvas * cVariance = new TCanvas();
    TH1F * hVariance = new TH1F("Variance Histogram", "Variance Histogram", nBin, 2.4, 4);
    for(int i=0; i<nPiEvaluations; i++){
        hVariance->Fill(PIMonteCarlo(nExtraction, seed.Rndm()));
    }

    //Fitting section
    hVariance->Fit("gaus");
    TF1 *fit = (TF1*)hVariance->GetListOfFunctions()->FindObject("gaus");
    double variance100 = fit->GetParameter(2);
    double k = variance100*sqrt(nExtraction); 

    cVariance->cd();
    hVariance->GetXaxis()->SetTitle("#pi estimation");
    hVariance->GetXaxis()->SetTitleSize(0.045);
    hVariance->GetXaxis()->SetTitleOffset(0.8);
    hVariance->GetYaxis()->SetTitle("Counts");
    hVariance->GetYaxis()->SetTitleSize(0.045);
    hVariance->GetYaxis()->SetTitleOffset(1);
    hVariance->Draw();
    fit->Draw("same");
    cVariance->SaveAs("Variance.pdf");

    //Repeating for nExtraction = 1000 and 5000
    nExtraction = 1000;
    TH1F * hVariance1000 = new TH1F("Variance Histogram", "Variance Histogram", nBin, 2, 4);
    for(int i=0; i<nPiEvaluations; i++){
        hVariance1000->Fill(PIMonteCarlo(nExtraction, seed.Rndm()));
    }
    hVariance1000->Fit("gaus");
    TF1 *fit1000 = (TF1*)hVariance1000->GetListOfFunctions()->FindObject("gaus");
    double variance1000 = fit1000->GetParameter(2);


    nExtraction = 5000;
    TH1F * hVariance5000 = new TH1F("Variance Histogram", "Variance Histogram", nBin, 2, 4);
    for(int i=0; i<nPiEvaluations; i++){
        hVariance5000->Fill(PIMonteCarlo(nExtraction, seed.Rndm()));
    }
    hVariance5000->Fit("gaus");
    TF1 *fit5000 = (TF1*)hVariance5000->GetListOfFunctions()->FindObject("gaus");
    double variance5000 = fit5000->GetParameter(2);

    //Final graph
    TCanvas * cK = new TCanvas();
    TGraph * gK = new TGraph();
    TF1 *functionK = new TF1("function","[0]/sqrt(x)",10,6000);
    functionK->SetParameter(0, k);

    gK->SetPoint(0, 100, variance100);
    gK->SetPoint(1, 1000, variance1000);
    gK->SetPoint(2, 5000, variance5000);

    cK->cd();
    gK->GetXaxis()->SetTitle("Number of extractions");
    gK->GetXaxis()->SetTitleSize(0.045);
    gK->GetXaxis()->SetTitleOffset(0.8);
    gK->GetYaxis()->SetTitle("Variance");
    gK->GetYaxis()->SetTitleSize(0.045);
    gK->GetYaxis()->SetTitleOffset(1);
    gK->SetMarkerStyle(8);
    gK->SetMarkerSize(1);
    gK->Draw("AP");
    functionK->Draw("same");
    cK->SaveAs("K.pdf");


    App->Run();
    return 0;
}