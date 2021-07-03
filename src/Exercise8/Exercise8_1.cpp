//c++ -o  Exercise8_1 Exercise8_1.cpp `root-config --glibs --cflags`

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <random>

#include "TApplication.h"
#include "TCanvas.h"
#include "TMultiGraph.h" 
#include "TH1F.h"
#include "TAxis.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TGraph2D.h"

using namespace std;

//Sauter formula
double SauterFormula(double theta){
    double eMassEnergy = 511.; //keV
    double photonEnergy = 1000.; //keV
    double bindingEnergy = 88.01; //keV
    double electronEnergy = photonEnergy - bindingEnergy;
    double beta = sqrt(electronEnergy * (electronEnergy + 2*eMassEnergy)) 
                    / (electronEnergy + eMassEnergy); 
    double gamma = 1 + electronEnergy / eMassEnergy;
    double numerator = sin(theta) * sin(theta);
    double denominator = pow(1-beta*cos(theta),4);
    double lastTerm = 1 + 0.5*gamma*(gamma-1)*(gamma-2)*(1-cos(theta));
    return numerator / denominator * lastTerm;
}

//Sauter formula used for the Fit (with parameters)
double SauterFormulaFit(double *x, double * par){
    return par[0]*(sin(x[0])*sin(x[0])) / (pow(1-par[1]*cos(x[0]),4)) * 
            (1 + 0.5*par[2]*(par[2] -1)*(par[2]-2)*(1-par[1]*cos(x[0])));
}

//Theta extraction through Hit or Miss applied to the Sauter formula
void thetaExtraction(TRandom3 * randomThetaX, TRandom3 * randomThetaY, vector<double> &direction){
    double x = randomThetaX->Uniform(0, M_PI);
    double y = randomThetaY->Uniform(0., 800.);
    if(y < SauterFormula(x)){
        direction[1] = x;
        return;
    }
    else{thetaExtraction(randomThetaX, randomThetaY, direction);}
}

//Extraction of theta and phi
void directionExtraction(vector<double> &direction, TRandom3 *randomPhi, TRandom3 * randomThetaX, TRandom3 * randomThetaY){
    direction[0] = randomPhi->Uniform(0, 2*M_PI);
    thetaExtraction(randomThetaX, randomThetaY, direction);
}

int main(){
    TApplication * App = new TApplication("T",0,NULL);

    //Initialization
    random_device initSeedGenerator;
    TRandom3 * randomPhi = new TRandom3(initSeedGenerator());
    TRandom3 * randomThetaX = new TRandom3(initSeedGenerator());
    TRandom3 * randomThetaY = new TRandom3(initSeedGenerator());
    vector<double> direction(2,0);
    int nExtraction = 1e6;
    int nBin = 200;

    //Canvas and graphs definition
    TCanvas * cPhi = new TCanvas();
    TH1F* hPhi = new TH1F("", "", nBin, 0, 2*M_PI);
    TCanvas * cTheta = new TCanvas();
    TH1F* hTheta = new TH1F("", "", nBin, 0, M_PI);
    TCanvas * cDirection = new TCanvas();
    TGraph2D * gDirection = new TGraph2D();
    TCanvas * cDirectionY = new TCanvas();
    TGraph2D * gDirectionY = new TGraph2D();

    //Space distribution sampling of the emitted photoelectron
    for(int i=0; i<nExtraction; i++){
        directionExtraction(direction, randomPhi, randomThetaX, randomThetaY);
        hPhi->Fill(direction[0]);
        hTheta->Fill(direction[1]);
        if(i%300 == 0){
            gDirection->SetPoint(i, sin(direction[1])*cos(direction[0]),
                    sin(direction[1])*sin(direction[0]), cos(direction[1]));
            gDirectionY->SetPoint(i, sin(direction[1])*sin(direction[0]),
                    cos(direction[1]), sin(direction[1])*cos(direction[0]));
        }
    }

    //Plot section
    cPhi->cd();
    hPhi->SetLineColor(9);
    hPhi->SetLineWidth(2);
    hPhi->SetFillStyle(3003);
    hPhi->SetFillColor(4);
    hPhi->GetXaxis()->SetTitle("#phi coordinate [rad]");
    hPhi->GetXaxis()->SetTitleSize(0.045);
    hPhi->GetXaxis()->SetTitleOffset(0.9);
    hPhi->GetYaxis()->SetTitle("Counts");
    hPhi->GetYaxis()->SetTitleSize(0.045);
    hPhi->GetYaxis()->SetTitleOffset(1);
    hPhi->GetYaxis()->SetRangeUser(0, 1.4*nExtraction/nBin);
    hPhi->Draw();
    cPhi->SaveAs("figs/Exercise8/Phi.pdf");

    cTheta->cd();
    hTheta->SetLineColor(9);
    hTheta->SetLineWidth(2);
    hTheta->SetFillStyle(3003);
    hTheta->SetFillColor(4);
    hTheta->GetXaxis()->SetTitle("#theta coordinate [rad]");
    hTheta->GetXaxis()->SetTitleSize(0.045);
    hTheta->GetXaxis()->SetTitleOffset(0.9);
    hTheta->GetYaxis()->SetTitle("Counts");
    hTheta->GetYaxis()->SetTitleSize(0.045);
    hTheta->GetYaxis()->SetTitleOffset(1);
    hTheta->Draw();

    //Fit section
    TF1 * sauterFit = new TF1("sauterFit", SauterFormulaFit, 0, M_PI, 3);
    sauterFit->SetParName(0,"Normalization");
    sauterFit->SetParName(1,"#beta");
    sauterFit->SetParName(2,"#gamma");
    sauterFit->SetParameters(50,0.93,2.78);
    hTheta->Fit("sauterFit");
    cTheta->SaveAs("figs/Exercise8/Theta.pdf");

    //Plot section
    cDirection->cd();
    gDirection->SetTitle("Photoelectron direction (0,0,1)");
    gDirection->GetXaxis()->SetTitle("x");
    gDirection->GetYaxis()->SetTitle("y");
    gDirection->GetZaxis()->SetTitle("z");
    gDirection->SetMarkerStyle(8);
    gDirection->SetMarkerSize(0.3);
    gDirection->SetMarkerColor(kRed);
    gDirection->Draw("P");
    cDirection->SaveAs("figs/Exercise8/Direction.pdf");

    cDirectionY->cd();
    gDirectionY->SetTitle("Photoelectron direction (0,1,0)");
    gDirectionY->GetXaxis()->SetTitle("x");
    gDirectionY->GetYaxis()->SetTitle("y");
    gDirectionY->GetZaxis()->SetTitle("z");
    gDirectionY->SetMarkerStyle(8);
    gDirectionY->SetMarkerSize(0.3);
    gDirectionY->SetMarkerColor(kRed);
    gDirectionY->Draw("P");
    cDirectionY->SaveAs("figs/Exercise8/DirectionY.pdf");


    App->Run();

    return 0;
}

