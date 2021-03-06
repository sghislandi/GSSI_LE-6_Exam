//c++ -o  Exercise3 Exercise3.cpp `root-config --glibs --cflags`

#include <stdlib.h>
#include <random>

#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TEllipse.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TF1.h"


using namespace std;

//Hit or Miss Monte Carlo (just to plot the points)
void extraction(vector<double> &point, double &PI){
    random_device initSeed;
    mt19937 randomSeed(initSeed());
    uniform_real_distribution<double> uniformExtractor(-1.0,1.0);
    point[0] = uniformExtractor(randomSeed);
    point[1] = uniformExtractor(randomSeed);
    if (point[0]*point[0] + point[1]*point[1] <= 1){
        PI ++;}
}

//Hit or Miss Monte Carlo returning the evaluated PI
double PIMonteCarlo(int nExtraction){
    random_device initSeed;
    mt19937 randomSeed(initSeed());
    uniform_real_distribution<double> uniformExtractor(-1.0,1.0);

    double Pi = 0;
    double x, y;
    for(int i=0; i<nExtraction; i++){
        x = uniformExtractor(randomSeed);
        y = uniformExtractor(randomSeed); 
        if(x*x + y*y <= 1){
            Pi ++;
        }
    }
    Pi = (double) 4. * Pi / (double) nExtraction;
    return Pi;
}


int main(){
    TApplication * App = new TApplication("T",0,NULL);
    gStyle->SetOptStat(11);
    gStyle->SetOptFit(1111);

    //Initializations
    TCanvas * c = new TCanvas();
    TCanvas * c1 = new TCanvas();
    TGraph * gToss = new TGraph();
    TGraph * gDifference = new TGraph();
    TEllipse *circle = new TEllipse(0,0,1);
    vector<double> point(2,0);
    int nExtraction = 1e5;
    double PI = 0;
    double difference;

    //First extraction to plot the extracted points and to compute PI_exp - PI_MC
    for(int i=0; i<nExtraction; i++){
        extraction(point, PI);
        gToss->SetPoint(i, point[0], point[1]);
        if(i % 100 == 0){
            gDifference->SetPoint(gDifference->GetN(), i, abs(((double) 4. * PI / nExtraction) - M_PI));
        }
    }
    PI = (double) 4. * PI / nExtraction;
    cout << "Your estimate of pi is: " << PI << endl;

    //Plot section
    c->SetCanvasSize(700, 700);
    c->cd();
    gToss->SetMarkerStyle(8);
    gToss->SetMarkerSize(0.5);
    gToss->SetMarkerColor(2);
    gToss->GetXaxis()->SetTitle("x coordinate");
    gToss->GetXaxis()->SetTitleSize(0.055);
    gToss->GetXaxis()->SetTitleOffset(0.9);
    gToss->GetYaxis()->SetTitle("y coordinate");
    gToss->GetYaxis()->SetTitleSize(0.055);
    gToss->GetYaxis()->SetTitleOffset(1.1);
    gToss->Draw("AP");
    circle->SetLineWidth(3);
    circle->SetLineColor(kBlue);
    circle->SetFillStyle(0);
    circle->Draw("same");
    c->SaveAs("figs/Exercise3/CircleToss.pdf");

    c1->cd();
    c1->SetLogy();
    gDifference->GetXaxis()->SetTitle("Number of extractions");
    gDifference->GetXaxis()->SetTitleSize(0.055);
    gDifference->GetXaxis()->SetTitleOffset(0.9);
    gDifference->GetYaxis()->SetTitle("|Deviation|");
    gDifference->GetYaxis()->SetTitleSize(0.055);
    gDifference->GetYaxis()->SetTitleOffset(0.8);
    gDifference->SetMarkerStyle(8);
    gDifference->SetMarkerSize(0.5);
    gDifference->Draw("AP");
    c1->SaveAs("figs/Exercise3/Difference.pdf");


    //Compute the MC uncertainties evaluating PI multiple times
    nExtraction = 100;
    int nPiEvaluations = 1e5;
    int nBin = 50;
    TCanvas * cVariance = new TCanvas();
    TH1F * hVariance = new TH1F("Variance Histogram", "Variance Histogram", nBin, -1, 1);
    for(int i=0; i<nPiEvaluations; i++){
        hVariance->Fill(PIMonteCarlo(nExtraction) - M_PI);
    }

    //Fitting section
    hVariance->Fit("gaus", "Q");
    TF1 *fit = (TF1*)hVariance->GetListOfFunctions()->FindObject("gaus");
    double variance100 = fit->GetParameter(2);
    double k = variance100*sqrt(nExtraction);
    cout << "\nThe value of k is " << k << endl << endl;

    //Plot section
    cVariance->cd();
    hVariance->SetLineColor(9);
    hVariance->SetLineWidth(2);
    hVariance->SetFillStyle(3003);
    hVariance->SetFillColor(4);
    hVariance->GetXaxis()->SetTitle("#pi_{est} - #pi_{exp}");
    hVariance->GetXaxis()->SetTitleSize(0.045);
    hVariance->GetXaxis()->SetTitleOffset(0.8);
    hVariance->GetYaxis()->SetTitle("Counts");
    hVariance->GetYaxis()->SetTitleSize(0.045);
    hVariance->GetYaxis()->SetTitleOffset(1);
    hVariance->Draw();
    //fit->Draw("same");
    cVariance->SaveAs("figs/Exercise3/Variance.pdf");


    //Repeating the multiple extraction for nExtraction = 1000 and 5000
    nExtraction = 1000;
    nBin = 500;
    TH1F * hVariance1000 = new TH1F("Variance Histogram", "Variance Histogram", nBin, -1, 1);
    for(int i=0; i<nPiEvaluations; i++){
        hVariance1000->Fill(PIMonteCarlo(nExtraction) - M_PI);
    }
    hVariance1000->Fit("gaus", "Q");
    TF1 *fit1000 = (TF1*)hVariance1000->GetListOfFunctions()->FindObject("gaus");
    double variance1000 = fit1000->GetParameter(2);

    nExtraction = 5000;
    TH1F * hVariance5000 = new TH1F("Variance Histogram", "Variance Histogram", nBin, -1, 1);
    for(int i=0; i<nPiEvaluations; i++){
        hVariance5000->Fill(PIMonteCarlo(nExtraction) - M_PI);
    }
    hVariance5000->Fit("gaus", "Q");
    TF1 *fit5000 = (TF1*)hVariance5000->GetListOfFunctions()->FindObject("gaus");
    double variance5000 = fit5000->GetParameter(2);

    //Plot section
    TCanvas * cK = new TCanvas();
    TGraph * gK = new TGraph();
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

    TF1 *functionK = new TF1("function","[0]/sqrt(x)",10,6000);
    functionK->SetParameter(0, k);
    gK->SetPoint(0, 100, variance100);
    gK->SetPoint(1, 1000, variance1000);
    gK->SetPoint(2, 5000, variance5000);
    functionK->Draw("same");
    cK->SaveAs("figs/Exercise3/K.pdf");


    App->Run();

    return 0;
}