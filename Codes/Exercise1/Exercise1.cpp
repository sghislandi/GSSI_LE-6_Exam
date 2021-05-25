//c++ -o  Exercise1 Exercise1.cpp `root-config --glibs --cflags`


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "TH1F.h"
#include "TF1.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"

using namespace std;

int Iteration(int number){
    return number*663608941;
}

int main(){
    TApplication * App = new TApplication("T",0,NULL);
    TCanvas * c = new TCanvas();

    int seed = 987654321;
    int extractedNumber = seed;
    int iterationNumber = 0;
    int period = 1073741823;

    int nBin = 100;
    TH1F * h = new TH1F("Uniform Distribution", "Uniform Distribution", nBin, 0, 1);

    while(true){
        extractedNumber = Iteration(extractedNumber);
        iterationNumber ++;
        h->Fill((double) extractedNumber / period);
        if(extractedNumber == seed) break;
    }

    cout << "You reobtained the seed after " << iterationNumber << " iterations" << endl;

    //Plots
    c->cd();
    h->SetLineColor(9);
    h->SetLineWidth(2);
    h->SetFillStyle(3003);
    h->SetFillColor(4);
    h->GetYaxis()->SetRangeUser(2684000, 2685000);
    h->Draw();
    h->SaveAs("SimpleGenerator.root");
    h->SaveAs("SimpleGenerator.pdf");
    


    App->Run();

    return 0;
}