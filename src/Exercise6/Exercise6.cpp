// c++ -o  Exercise6 Exercise6.cpp `root-config --glibs --cflags`

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>

#include "TApplication.h"
#include "TCanvas.h"
#include "TMultiGraph.h" 
#include "TGraph.h"
#include "TAxis.h"

using namespace std;


float SumFloat(int nSum){
    float sum = 0;
    for(int i=1; i<= nSum; i++){sum += (float)1/nSum;}
    return sum;
}

double SumDouble(int nSum){
    double sum = 0;
    for(int i=1; i<= nSum; i++){sum += (double)1/nSum;}
    return sum;
}

int main(){
    TApplication * App = new TApplication("App",0,NULL);

    vector<int> nSums = {1000,10000,100000,1000000};
    vector<float> outcomeFloat;
    vector<float> outcomeDouble;
    for(int i=0; i<nSums.size(); i++){
        outcomeFloat.push_back(SumFloat(nSums[i]));
        outcomeDouble.push_back(SumDouble(nSums[i]));
    }


    //Print results
    cout << "*******RESULTS*******" << endl;
    cout << "N \t Float \t Double" << endl;
    for(int i=0; i<nSums.size(); i++){
        cout << nSums[i] << "\t" << outcomeFloat[i] << "\t" << outcomeDouble[i] << endl;
    }

    //Plot section 
    TCanvas * c = new TCanvas();
    TGraph * g = new TGraph();
    for(int i=0; i<nSums.size(); i++){
        g->SetPoint(i, nSums[i], abs(outcomeDouble[i]-outcomeFloat[i]));
    }
    c->cd();
    c->SetLogx();
    g->SetMarkerColor(2);
    g->SetMarkerStyle(8);
    g->SetMarkerSize(1.5);
    g->GetXaxis()->SetTitle("N");
    g->GetXaxis()->SetTitleSize(0.05);
    g->GetXaxis()->SetTitleOffset(0.85);
    g->GetXaxis()->SetLabelSize(0.04);
    g->GetYaxis()->SetTitle("Double - Float");
    g->GetYaxis()->SetTitleSize(0.05);
    g->GetYaxis()->SetTitleOffset(1.1);
    g->GetYaxis()->SetLabelSize(0.04);
    g->Draw("AP");

    c->SaveAs("DifferenceExercise6.pdf");
    App->Run();
    
    return 0;
}