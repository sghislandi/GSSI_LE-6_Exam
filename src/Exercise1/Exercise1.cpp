//c++ -o  Exercise1 Exercise1.cpp `root-config --glibs --cflags`

#include <iostream>
#include <ctime>

#include "TH1F.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <Math/ProbFuncMathCore.h>

using namespace std;

//Implementation of the LCG generator with multiplicative term
int Iteration(unsigned int number){
    return number*663608941;
}

//MINSTD generator
int MINSTD(int a, int periodMINSTD, long int number){
    return (a * number) % periodMINSTD;
}

//Function that. given graph and the number of bins
//compute the chi square
double getChiSquare(TH1F * graph, int nBin){
    double nEntries = graph->GetEntries();
    double chiSquare = 0;
    double expectedCounts = (double) nEntries / nBin;
    for(int i=1; i<=nBin; i++){
        chiSquare += (double) pow(graph->GetBinContent(i) - expectedCounts ,2) / expectedCounts;
    }
    return chiSquare;
}


int main(){
    gStyle->SetOptStat("e");
    TApplication * App = new TApplication("T",0,NULL);
    TCanvas * c = new TCanvas();
    TCanvas * c1 = new TCanvas();

    //Initialization of the first enerator
    unsigned int seed = 987654321;
    unsigned int extractedNumber = seed;
    int iterationNumber = 1;
    unsigned int period = 4294967295;

    int nBin1 = 100;
    int nBin2 = 50;
    TH1F * h = new TH1F("Uniform Distribution (All events)", "Uniform Distribution (All events)", nBin1, 0, 1);
    TH1F * h1 = new TH1F("Uniform Distribution (First 1000 events)", "Uniform Distribution (First 1000 events)", nBin2, 0, 1);
    TH1F * h2 = new TH1F("Uniform Distribution (First 1000000 events)", "Uniform Distribution (First 1000000 events)", nBin2, 0, 1);

    //Compute the CPU time
    clock_t c_start = clock();
    while(true){
        extractedNumber = Iteration(extractedNumber);
        if(extractedNumber == seed) break;
        iterationNumber ++;
    }
    clock_t c_end = clock();
    cout << "Time needed to complete the whole extraction period: " << (double) (c_end-c_start) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << "You reobtained the seed after " << iterationNumber << " iterations" << endl;

    //Computation of the period and histogram filling
    extractedNumber = seed;
    iterationNumber = 0;
    while(true){
        extractedNumber = Iteration(extractedNumber);
        iterationNumber ++;
        h->Fill((double) extractedNumber / period);
        if(iterationNumber <= 1000){ h1->Fill((double) extractedNumber / period); }
        if(iterationNumber <= 1000000){ h2->Fill((double) extractedNumber / period); }
        if(extractedNumber == seed) break;
    }

    //Plots
    c->cd();
    h->SetLineColor(9);
    h->SetLineWidth(2);
    h->SetFillStyle(3003);
    h->SetFillColor(4);
    h->GetXaxis()->SetTitle("Extracted number");
    h->GetXaxis()->SetTitleSize(0.045);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitle("Counts");
    h->GetYaxis()->SetTitleSize(0.045);
    h->GetYaxis()->SetTitleOffset(1.1);
    h->Draw();

    c1->SetCanvasSize(900,400);
    c1->Divide(2,1);
    c1->cd(1);
    h1->SetLineColor(9);
    h1->SetLineWidth(2);
    h1->SetFillStyle(3003);
    h1->SetFillColor(4);
    h1->GetXaxis()->SetTitle("Extracted number");
    h1->GetXaxis()->SetTitleSize(0.045);
    h1->GetXaxis()->SetTitleOffset(0.8);
    h1->GetYaxis()->SetTitle("Counts");
    h1->GetYaxis()->SetTitleSize(0.045);
    h1->GetYaxis()->SetTitleOffset(0.9);
    h1->Draw();

    c1->cd(2);
    h2->SetLineColor(9);
    h2->SetLineWidth(2);
    h2->SetFillStyle(3003);
    h2->SetFillColor(4);
    h2->GetXaxis()->SetTitle("Extracted number");
    h2->GetXaxis()->SetTitleSize(0.045);
    h2->GetXaxis()->SetTitleOffset(0.8);
    h2->GetYaxis()->SetTitle("Counts");
    h2->GetYaxis()->SetTitleSize(0.045);
    h2->GetYaxis()->SetTitleOffset(0.9);
    h2->Draw();

    c->SaveAs("figs/Exercise1/SimpleGenerator.pdf");
    c1->SaveAs("figs/Exercise1/SimpleGenerator1.pdf");

    //Chi square test
    double chiSquare, chiSquare1, chiSquare2;
    double pValue, pValue1, pValue2;

    //Chi square for All events graph
    chiSquare = getChiSquare(h, nBin1);
    pValue = 1-ROOT::Math::chisquared_cdf(chiSquare, nBin1-1);
    cout << "***FIT RESULTS (All events)*************\n"
         << "Chi square: " << chiSquare << endl
         << "d.o.f.: " << nBin1-1 << endl
         << "p-value: " << pValue << endl << endl;
    
    //Chi square for 1e3 events graph
    chiSquare1 = getChiSquare(h1, nBin2);
    pValue1 = 1-ROOT::Math::chisquared_cdf(chiSquare1, nBin2-1);
    cout << "***FIT RESULTS (1000 entries)*************\n"
         << "Chi square: " << chiSquare1 << endl
         << "d.o.f.: " << nBin2-1 << endl
         << "p-value: " << pValue1 << endl << endl;
    
    //Chi square for 1e6 events graph
    chiSquare2 = getChiSquare(h2, nBin2);
    pValue2 = 1-ROOT::Math::chisquared_cdf(chiSquare2, nBin2-1);
    cout << "***FIT RESULTS (1000000 entries)*************\n"
         << "Chi square: " << chiSquare2 << endl
         << "d.o.f.: " << nBin2-1 << endl
         << "p-value: " << pValue2 << endl << endl;


    //MINSTD generation initialization
    int a = 16807;      //pow(7,5)
    long int periodMINSTD = 2147483647;      //pow(2,31) - 1
    vector<long int> seedMINSTD = {23353, 3432, 43543821};
    long int extractedNumberMINSTD;
    long int extractionLength;

    TCanvas * c2 = new TCanvas();
    TH1F * h3 = new TH1F("MINSTD", "MINSTD", nBin1, 0, 1);

    //MINSTD complete extraction with different seeds
    cout << "****************MINSTD RESULTS****************\n";
    cout << "The m value is: " << periodMINSTD << endl;  
    for(int seedIndex=0; seedIndex < seedMINSTD.size(); seedIndex++){
        extractedNumberMINSTD = seedMINSTD[seedIndex];
        extractionLength = 0;
        while(true){
            extractedNumberMINSTD =  MINSTD(a, periodMINSTD, extractedNumberMINSTD);
            if(seedIndex == 0){
                h3->Fill((double) extractedNumberMINSTD / periodMINSTD);
            }
            extractionLength ++;
            if(extractedNumberMINSTD == seedMINSTD[seedIndex]){break;}
        }        
        cout << "Length with seed = " << seedMINSTD[seedIndex] << " is: " << extractionLength << endl;  
    }

    c2->cd(2);
    h3->SetLineColor(9);
    h3->SetLineWidth(2);
    h3->SetFillStyle(3003);
    h3->SetFillColor(4);
    h3->GetXaxis()->SetTitle("Extracted number");
    h3->GetXaxis()->SetTitleSize(0.045);
    h3->GetXaxis()->SetTitleOffset(0.8);
    h3->GetYaxis()->SetTitle("Counts");
    h3->GetYaxis()->SetTitleSize(0.045);
    h3->GetYaxis()->SetTitleOffset(0.9);
    h3->Draw();
    c2->SaveAs("figs/Exercise1/Minstd.pdf");


    App->Run();

    return 0;
}