//c++ -o  Exercise5_1 Exercise5_1.cpp `root-config --glibs --cflags`

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <random>


#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TLine.h"
#include "TPad.h"

using namespace std;

double functionPower(double x, int degree){
  return (double) pow(x,degree);
}

double exactIntegral(int degree){
  return (double) 1. / (1. + degree);
}

vector<double> Extraction2D(double xMin, double xMax, double yMin, double yMax){
    vector<double> result(2,0);
    random_device initSeed;
    mt19937 randomSeed(initSeed());
    uniform_real_distribution<double> uniformExtractorX(xMin, xMax);
    uniform_real_distribution<double> uniformExtractorY(yMin, yMax);
    result[0] = uniformExtractorX(randomSeed);
    result[1] = uniformExtractorY(randomSeed);
    return result;
}

double HitOrMiss(double xMin, double xMax, double yMin, double yMax, int degree, int nExtraction){
  double integral = 0;
  vector<double> extractionResult(2,0);
  for(int i=0; i<nExtraction; i++){
    extractionResult = Extraction2D(xMin, xMax, yMin, yMax);
    if(extractionResult[1] < functionPower(extractionResult[0], degree)){
      integral ++;
    }
  }
  return (double) integral / nExtraction;
}


int main(){
  TApplication * App = new TApplication("T",0,NULL);
  TCanvas * c = new TCanvas();

  //Obtain I approx - I exact depending on nExtraction
  int nExtractionMin = 1e1;
  int nExtractionMax = 1e6;
  int degreeMax = 6;
  double exactIntegralValue;
  int counter = 0;
  int xMin = 0;
  int xMax = 1;
  int yMin = 0;
  int yMax = 1;

  TGraph ** graphError;
  graphError = new TGraph*[degreeMax];

  for(int i=0; i<degreeMax; i++){
    cout << "Doing the HitOrMiss for x^" << i+1 << endl;
    graphError[i] = new TGraph();
    exactIntegralValue = exactIntegral(i+1);
    counter = 0;
    for(int j=nExtractionMin; j<=nExtractionMax; j *= 2){
      graphError[i]->SetPoint(counter, j, abs(HitOrMiss(xMin, xMax, yMin, yMax, i+1, j) - exactIntegralValue));
      counter ++;
    }
  }


  //Plot
  c->SetLogy();
  c->SetLogx();
  c->Divide(2,3);
  

  TPad * pad = new TPad();
  for(int i=0; i<degreeMax; i++){
    c->cd(i+1);
    graphError[i]->SetTitle(Form("Hit or Miss discrepancy for x^{%i}", i+1));
    graphError[i]->SetMarkerColor(2);
    graphError[i]->SetMarkerStyle(8);
    graphError[i]->SetMarkerSize(0.9);
    pad = (TPad*) c->FindObject(Form("c1_%i", i+1));
    pad->SetLogy();
    pad->SetLogx();
    graphError[i]->GetXaxis()->SetTitle("N extractions");
    graphError[i]->GetXaxis()->SetTitleSize(0.055);
    graphError[i]->GetXaxis()->SetTitleOffset(0.85);
    graphError[i]->GetXaxis()->SetLabelSize(0.06);
    graphError[i]->GetYaxis()->SetTitle("Discrepancy");
    graphError[i]->GetYaxis()->SetTitleSize(0.055);
    graphError[i]->GetYaxis()->SetTitleOffset(0.6);
    graphError[i]->GetYaxis()->SetRangeUser(0.0005, 1);
    graphError[i]->GetYaxis()->SetLabelSize(0.06);

    graphError[i]->Draw("AP");
  }





  App->Run();


  return 0;
}