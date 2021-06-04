//c++ -o  Exercise5_3 Exercise5_3.cpp `root-config --glibs --cflags`

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
#include "TRandom3.h"


using namespace std; 

double integrandFunctionMidPoint(vector<double> position, double dx, double xMin){
    double squaredSum = 0;
    for(int i=0; i<position.size(); i++){
        squaredSum += pow(xMin + position[i]*dx + 0.5*dx ,2);
    }
    return squaredSum;
}

void nDimensionLoop(int nDivisions, double dx, double xMin, vector<double> &position, double &integral, int flag){
    for(int i=0; i<nDivisions; i++){
        position[flag-1] = i;
        if(flag>1){
            nDimensionLoop(nDivisions, dx, xMin, position, integral, flag-1);
        }
        if(flag==1){
            integral += integrandFunctionMidPoint(position, dx, xMin);
        }
    }
    position[flag-1] = 0;
}

double integrandFunctionHitOrMiss(int dimensionality, vector<double> x){
    double squaredSum = 0;
    for(int i=0; i<dimensionality; i++){
        squaredSum += pow(x[i],2);
    }
    return squaredSum;
}

void Extraction(int dimensionality,double xMin, double xMax, double yMin, double yMax, TRandom3* randomArray, vector<double> &randomExtraction){
    //Fill x's
    for(int i=0; i<dimensionality; i++){
        randomExtraction[i] = randomArray[i].Uniform(xMin, xMax);
    }
    //Fill y
    randomExtraction[dimensionality] = randomArray[dimensionality-1].Uniform(yMin, yMax);
}

double HitOrMiss(int dimensionality, double xMin, double xMax, double yMin, double yMax, int nExtraction){
    vector<double> randomExtraction(dimensionality+1,0);
    TRandom3 * randomArray;
    randomArray = new TRandom3[dimensionality+1];
    random_device initSeed;
    double integral = 0.;

    //Setting the seed
    for(int i=0; i<dimensionality+1; i++){
        randomArray[i].SetSeed(initSeed());
    }
    //Extraction
    for(int i=0; i < nExtraction; i++){
        Extraction(dimensionality, xMin, xMax, yMin, yMax, randomArray, randomExtraction);
        if(randomExtraction[dimensionality] < integrandFunctionHitOrMiss(dimensionality, randomExtraction)){
            integral += 1.;
        }
    }
    return (double) integral / nExtraction * dimensionality;
}

double MidPoint(int dimensionality, double xMin, double xMax, int nDivisions){
    double dx = (double) (xMax - xMin) / nDivisions;
    double integral = 0;
    vector<double> position(dimensionality,0);
    int flag = dimensionality;
    nDimensionLoop(nDivisions, dx, xMin, position, integral, flag);
    return integral * pow(dx, dimensionality);
}

int main(){
    TApplication * App = new TApplication("T",0,NULL);

    //Integration parameters
    int maxD = 8;
    vector<int> nCell = {65536, 256, 40, 16, 9, 6, 5, 4};
    vector<int> nExtraction = {65536, 65536, 64000, 65536, 59049, 46656, 78125, 65536};

    double xMin = 0.;
    double xMax = 1.;
    double yMin = 0.;
    double yMax;
    int currentDimension;
    vector<double> exactIntegral(maxD, 0);
    vector<double> integralHitOrMiss(maxD, 0);
    vector<double> integralMidPoint(maxD, 0);

    //Filling the exactIntegralVector
    for(int i=0; i<maxD; i++){
        exactIntegral[i] = (double) (i+1)/3.;
    }

    //Filling the HitOrMiss
    for(int i=0; i<maxD; i++){
        currentDimension = i+1;
        integralHitOrMiss[i] = HitOrMiss(currentDimension, xMin, xMax, yMin, currentDimension, nExtraction[i]);
        integralMidPoint[i] = MidPoint(currentDimension, xMin, xMax, nCell[i]);
        cout << integralHitOrMiss[i] << "\t" << integralMidPoint[i] << "\t" << exactIntegral[i] << endl;
    }

    //Plot section
    TCanvas * c = new TCanvas();
    TMultiGraph * graphIntegral = new TMultiGraph();
    TGraph * graphHitOrMiss = new TGraph();
    TGraph * graphMidPoint = new TGraph();
    for(int i=0; i<maxD; i++){
        graphHitOrMiss->SetPoint(i, i+1, abs(integralHitOrMiss[i] - exactIntegral[i]));
        graphMidPoint->SetPoint(i, i+1, abs(integralMidPoint[i] - exactIntegral[i]));
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

    c->SaveAs("HitOrMissVSMidpoint2.pdf");



    return 0;
}