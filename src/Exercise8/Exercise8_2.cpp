//c++ -o  Exercise8_2 Exercise8_2.cpp `root-config --glibs --cflags`

#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <random>

#include "TApplication.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TGraph2D.h"

using namespace std; 

double samplePhi(mt19937 &seed){
    uniform_real_distribution<double> randomPhi(0.0, 2*M_PI);
    return randomPhi(seed);
}

double sampleTheta(mt19937 &seed){
    uniform_real_distribution<double> randomCosTheta(-1.0, 1.0);
    return acos(randomCosTheta(seed));
}

int LShellExtraction(mt19937 &seed){
    discrete_distribution<int> LShellOccupacy{2,2,4};
    return LShellOccupacy(seed);
}

void sampleEmittedPhotons(mt19937 &seed, vector<double> &emittedPhotonEnergies, vector<vector<double>> &emittedPhotonDirections){
    vector<double> LEnergies = {15.86, 15.2, 13.04}; //keV
    double bindingEnergy = 88.01 ; //keV

    int sampledLShell = LShellExtraction(seed);
    emittedPhotonEnergies[0] = bindingEnergy - LEnergies[sampledLShell];
    emittedPhotonEnergies[1] = LEnergies[sampledLShell];

    emittedPhotonDirections[0][0] = samplePhi(seed);
    emittedPhotonDirections[0][1] = sampleTheta(seed);
    emittedPhotonDirections[1][0] = samplePhi(seed);
    emittedPhotonDirections[1][1] = sampleTheta(seed);
}

int main(){
    TApplication * App = new TApplication("T",0,NULL);

    random_device initSeedGenerator;
    std::mt19937 seed(initSeedGenerator());
    vector<double> emittedPhotonEnergies(2,0);
    vector<vector<double>> emittedPhotonDirections(2, vector<double>(2));
    int nExtraction = 1e4;
    int nBin = 1000;

    TCanvas * cEnergy = new TCanvas();
    TH1F* hEnergy = new TH1F("", "", nBin, 0, 100);
    TCanvas * cDirection = new TCanvas();
    TGraph2D * gDirection = new TGraph2D();
    for(int i=0; i<nExtraction; i++){
        sampleEmittedPhotons(seed, emittedPhotonEnergies, emittedPhotonDirections);
        hEnergy->Fill(emittedPhotonEnergies[0]);
        hEnergy->Fill(emittedPhotonEnergies[1]);
        gDirection->SetPoint(2*i, sin(emittedPhotonDirections[0][1])*cos(emittedPhotonDirections[0][0]),
                    sin(emittedPhotonDirections[0][1])*sin(emittedPhotonDirections[0][0]), cos(emittedPhotonDirections[0][1]));
        gDirection->SetPoint(2*i+1, sin(emittedPhotonDirections[1][1])*cos(emittedPhotonDirections[1][0]),
                    sin(emittedPhotonDirections[1][1])*sin(emittedPhotonDirections[1][0]), cos(emittedPhotonDirections[1][1]));
        //cout << "Photon1 : " << emittedPhotonEnergies[0] << "  (" << emittedPhotonDirections[0][0] << "," << emittedPhotonDirections[0][1]<<")" << endl
        //<< "Photon2 : " << emittedPhotonEnergies[1] << "  (" << emittedPhotonDirections[1][0] << "," << emittedPhotonDirections[1][1] << ")" << endl << endl;
    }

    cEnergy->cd();
    hEnergy->SetLineColor(9);
    hEnergy->SetLineWidth(2);
    hEnergy->SetFillStyle(3003);
    hEnergy->SetFillColor(4);
    hEnergy->GetXaxis()->SetTitle("Energy [keV]");
    hEnergy->GetXaxis()->SetTitleSize(0.045);
    hEnergy->GetXaxis()->SetTitleOffset(0.9);
    hEnergy->GetYaxis()->SetTitle("Counts");
    hEnergy->GetYaxis()->SetTitleSize(0.045);
    hEnergy->GetYaxis()->SetTitleOffset(1);
    hEnergy->GetYaxis()->SetRangeUser(0, 1.4*nExtraction/nBin);
    hEnergy->Draw();
    cEnergy->SaveAs("FluorescenceEnergy.pdf");

    cDirection->cd();
    gDirection->SetTitle("Fluorescence direction");
    gDirection->GetXaxis()->SetTitle("x");
    gDirection->GetYaxis()->SetTitle("y");
    gDirection->GetZaxis()->SetTitle("z");
    gDirection->SetMarkerStyle(8);
    gDirection->SetMarkerSize(0.3);
    gDirection->SetMarkerColor(kRed);
    gDirection->Draw("P");
    cDirection->SaveAs("FluorescenceDirection.pdf");

    App->Run();

    return 0;
}