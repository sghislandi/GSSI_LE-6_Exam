//c++ -o  Exercise5_2 Exercise5_2.cpp `root-config --glibs --cflags`

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <random>


#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h" 
#include "TAxis.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TLine.h"
#include "TPad.h"
#include "TRandom3.h"


using namespace std;


double exponential(double x){
    return exp(-x);
}

vector<double> Extraction2D(int dimensionality,double xMin, double xMax, double yMin, double yMax, TRandom3* randomArray, vector<double> &randomExtraction){
    //Fill x's
    for(int i=0; i<dimensionality; i++){
        randomExtraction[i] = randomArray[i].Uniform(xMin, xMax);
    }
    //Fill y's
    for(int i=0; i<dimensionality; i++){
        randomExtraction[i+dimensionality] = randomArray[i+dimensionality].Uniform(yMin, yMax);
    }
    return randomExtraction;
}

double HitOrMiss(int dimensionality, double xMin, double xMax, double yMin, double yMax, int nExtraction){
    vector<double> randomExtraction(2*dimensionality,0);
    vector<double> singleIntegrals(dimensionality,0);
    TRandom3 * randomArray;
    randomArray = new TRandom3[2*dimensionality];
    random_device initSeed;
    double integralProduct = 1.;

    //Setting the seed
    for(int i=0; i<2*dimensionality; i++){
        randomArray[i].SetSeed(initSeed());
    }
    //Extraction
    for(int i=0; i < nExtraction; i++){
        randomExtraction = Extraction2D(dimensionality, xMin, xMax, yMin, yMax, randomArray, randomExtraction);
        for(int j=0; j<dimensionality; j++){
            if(randomExtraction[j + dimensionality] < exponential(randomExtraction[j])){
                singleIntegrals[j] ++;
            }
        }
    }
    for(int i=0; i<dimensionality; i++){
        singleIntegrals[i] = (double) singleIntegrals[i] / nExtraction;
        singleIntegrals[i] *= 0.632120558;
        singleIntegrals[i] += 0.367879441;
        integralProduct *= singleIntegrals[i];
    }
    return integralProduct;
}

double MidPoint(int dimensionality, double xMin, double xMax, int nDivisions){
    vector<double> singleIntegrals(dimensionality,0);
    double dx = (double) (xMax - xMin) / nDivisions;
    double integralProduct = 1.;

    for(int i=0; i<dimensionality; i++){
        for(double cursor=xMin; cursor<xMax; cursor+=dx){
            singleIntegrals[i] += exponential(cursor + 0.5*dx) * dx;
        }
    }
    for(int i=0; i<dimensionality; i++){
        integralProduct*= singleIntegrals[i];
    }
    return integralProduct;
}

int main(){
    TApplication * App = new TApplication("T",0,NULL);

    //Integration parameters
    int maxD = 8;
    vector<int> nCell = {65536, 256, 40, 16, 9, 6, 5, 4};
    vector<int> nExtraction = {65536, 65536, 64000, 65536, 59049, 46656, 78125, 65536};

    double xMin = 0.;
    double xMax = 1.;
    double e = 2.7182818284; 
    double yMin = (double) 1./e;
    double yMax = 1;
    vector<double> exactIntegral(maxD, 0);
    vector<double> integralHitOrMiss(maxD, 0);
    vector<double> integralMidPoint(maxD, 0);

    //Time measurement
    clock_t startTime;
    clock_t stopTime;

    //Filling the exactIntegralVector
    for(int i=0; i<maxD; i++){
        exactIntegral[i] = pow(((double) 1. - (double) 1./e),i+1);
    }

    //Filling the HitOrMiss
    for(int i=0; i<maxD; i++){
        integralHitOrMiss[i] = HitOrMiss(i+1, xMin, xMax, yMin, yMax, nExtraction[i]);
        integralMidPoint[i] = MidPoint(i+1, xMin, xMax, nCell[i]);
    }

    //Plot section
    TCanvas * c = new TCanvas();
    TCanvas * cTime = new TCanvas();
    TMultiGraph * graphIntegral = new TMultiGraph();
    TMultiGraph * graphTime = new TMultiGraph();
    TGraph * graphHitOrMiss = new TGraph();
    TGraph * graphMidPoint = new TGraph();
    TGraph * graphHitOrMissTime = new TGraph();
    TGraph * graphMidPointTime = new TGraph();

    for(int i=0; i<maxD; i++){
        startTime = clock();
        graphHitOrMiss->SetPoint(i, i+1, abs(integralHitOrMiss[i] - exactIntegral[i]));
        stopTime = clock();
        graphHitOrMissTime->SetPoint(i, i+1, (stopTime-startTime) / CLOCKS_PER_SEC); 
        startTime = clock();
        graphMidPoint->SetPoint(i, i+1, abs(integralMidPoint[i] - exactIntegral[i]));
        stopTime = clock();
        graphMidPointTime->SetPoint(i, i+1, (stopTime-startTime) / CLOCKS_PER_SEC); 
    }

    c->cd();
    c->SetLogy();
    graphIntegral->Add(graphHitOrMiss);
    graphIntegral->Add(graphMidPoint);
    graphHitOrMiss->SetMarkerColor(2);
    graphHitOrMiss->SetMarkerStyle(8);
    graphHitOrMiss->SetMarkerSize(1.5);
    graphHitOrMiss->SetTitle("Hit or Miss");
    graphMidPoint->SetMarkerColor(1);
    graphMidPoint->SetMarkerStyle(22);
    graphMidPoint->SetMarkerSize(1.5);
    graphMidPoint->SetTitle("Midpoint");
    graphIntegral->GetXaxis()->SetTitle("Dimensionality");
    graphIntegral->GetXaxis()->SetTitleSize(0.055);
    graphIntegral->GetXaxis()->SetTitleOffset(0.80);
    graphIntegral->GetXaxis()->SetLabelSize(0.04);
    graphIntegral->GetYaxis()->SetTitle("|I_{approx} - I_{exact}|");
    graphIntegral->GetYaxis()->SetTitleSize(0.055);
    graphIntegral->GetYaxis()->SetTitleOffset(0.85);
    graphIntegral->GetYaxis()->SetRangeUser(1e-13, 0.1);
    graphIntegral->GetYaxis()->SetLabelSize(0.04);
    graphIntegral->Draw("AP");
    c->BuildLegend(0.55, 0.3, 0.85, 0.15);

    cTime->cd();
    graphTime->Add(graphHitOrMiss);
    graphTime->Add(graphMidPoint);
    graphHitOrMissTime->SetMarkerColor(2);
    graphHitOrMissTime->SetMarkerStyle(8);
    graphHitOrMissTime->SetMarkerSize(0.9);
    graphHitOrMissTime->SetTitle("Hit or Miss");
    graphMidPointTime->SetMarkerColor(1);
    graphMidPointTime->SetMarkerStyle(8);
    graphMidPointTime->SetMarkerSize(0.9);
    graphMidPointTime->SetTitle("Midpoint");
    graphTime->GetXaxis()->SetTitle("Dimensionality");
    graphTime->GetXaxis()->SetTitleSize(0.055);
    graphTime->GetXaxis()->SetTitleOffset(0.80);
    graphTime->GetXaxis()->SetLabelSize(0.04);
    graphTime->GetYaxis()->SetTitle("Time [s]");
    graphTime->GetYaxis()->SetTitleSize(0.055);
    graphTime->GetYaxis()->SetTitleOffset(0.91);
    //graphTime->GetYaxis()->SetRangeUser(0.0005, 1);
    graphTime->GetYaxis()->SetLabelSize(0.04);
    graphTime->Draw("AP");
    cTime->BuildLegend();


    c->SaveAs("HitOrMissVSMidpoint.pdf");
    cTime->SaveAs("HitOrMissVSMidpointTime.pdf");

    App->Run();

    return 0;
}
