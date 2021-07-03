//c++ -o  Exercise5_1 Exercise5_1.cpp `root-config --glibs --cflags`

#include <iostream>
#include <math.h>
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

//Function power 
double functionPower(double x, int degree){
  return (double) pow(x,degree);
}

//Exact result from integral, depending on the degree of the power
double exactIntegral(int degree){
  return (double) 1. / (1. + degree);
}

//2D extraction for the Hit or Miss Monte Carlo
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

//Hit or Miss Monte Carlo returning the integral value
double HitOrMiss(double xMin, double xMax, double yMin, double yMax, int degree, int nExtraction){
  double integral = 0;
  vector<double> extractionResult(2,0);
  for(int i=0; i<nExtraction; i++){
    extractionResult = Extraction2D(xMin, xMax, yMin, yMax);
    if(extractionResult[1] <= functionPower(extractionResult[0], degree)){
      integral ++;
    }
  }
  return (double) integral / nExtraction;
}

double AritmethicMean(double xMin, double xMax, int degree, int nExtraction){
  double integral = 0;
  random_device initSeed;
  mt19937 randomSeed(initSeed());    
  uniform_real_distribution<double> uniformExtractorX(xMin, xMax);
  for(int i=0; i<nExtraction; i++){
    integral += functionPower(uniformExtractorX(randomSeed), degree);
  }
  return (double) integral / nExtraction;
}


int main(){
  TApplication * App = new TApplication("T",0,NULL);

  //Initialization
  int nExtractionMin = 1e1;
  int nExtractionMax = 1e5;
  int degreeMax = 6;
  double exactIntegralValue;
  int counter = 0;
  int xMin = 0;
  int xMax = 1;
  int yMin = 0;
  int yMax = 1;

  //Definition of TGraphError array
  TCanvas * c = new TCanvas();
  TGraph ** graphError;
  graphError = new TGraph*[degreeMax];

  TCanvas * cMean = new TCanvas();
  TGraph ** graphErrorMean;
  graphErrorMean = new TGraph*[degreeMax];

  //MC looping over degrees and nExtraction
  for(int i=0; i<degreeMax; i++){
    cout << "Doing x^" << i+1 << endl;
    graphError[i] = new TGraph();
    graphErrorMean[i] = new TGraph();
    exactIntegralValue = exactIntegral(i+1);
    counter = 0;
    clock_t cstart1 = clock();
    for(int j=nExtractionMin; j<=nExtractionMax; j *= 2){
      graphError[i]->SetPoint(counter, j, abs(HitOrMiss(xMin, xMax, yMin, yMax, i+1, j) - exactIntegralValue));
      counter ++;
    }
    clock_t cstop1 = clock();
    clock_t cstart2 = clock();
    for(int j=nExtractionMin; j<=nExtractionMax; j *= 2){
      graphErrorMean[i]->SetPoint(counter, j, abs(AritmethicMean(xMin, xMax, i+1, j) - exactIntegralValue));
      counter ++;
    }
    clock_t cstop2 = clock();
    cout << "H or M: " << (double) (cstop1-cstart1) / CLOCKS_PER_SEC << "seconds   Arithmetic mean: " << (double) (cstop2-cstart2) / CLOCKS_PER_SEC << " seconds" << endl;
  }

  //Plot section
  c->Divide(2,3);
  TPad * pad = new TPad();
  for(int i=0; i<degreeMax; i++){
    c->cd(i+1);
    graphError[i]->SetTitle(Form("Hit or Miss discrepancy for x^{%i}", i+1));
    gStyle->SetTitleFontSize(0.08);
    graphError[i]->SetMarkerColor(1);
    graphError[i]->SetMarkerStyle(8);
    graphError[i]->SetMarkerSize(0.9);
    graphErrorMean[i]->SetMarkerColor(2);
    graphErrorMean[i]->SetMarkerStyle(8);
    graphErrorMean[i]->SetMarkerSize(0.9);
    pad = (TPad*) c->FindObject(Form("c1_%i", i+1));
    pad->SetLogy();
    pad->SetLogx();
    graphError[i]->GetXaxis()->SetTitle("N extractions");
    graphError[i]->GetXaxis()->SetTitleSize(0.075);
    graphError[i]->GetXaxis()->SetTitleOffset(0.6);
    graphError[i]->GetXaxis()->SetLabelSize(0.06);
    graphError[i]->GetYaxis()->SetTitle("Discrepancy");
    graphError[i]->GetYaxis()->SetTitleSize(0.075);
    graphError[i]->GetYaxis()->SetTitleOffset(0.6);
    graphError[i]->GetYaxis()->SetRangeUser(1e-4, 1);
    graphError[i]->GetYaxis()->SetLabelSize(0.06);
    graphError[i]->Draw("AP");
    graphErrorMean[i]->Draw("same P");
  }
  c->SaveAs("figs/Exercise5/Discrepancies.pdf");

  //------------------------------//
  //************5.1.2*************//
  //------------------------------//
  //Initialization
  TCanvas * cVariance = new TCanvas();  
  TH1F ** histoVariance;
  histoVariance = new TH1F*[degreeMax];
  double nExtraction = 1e5;
  int nRepetitions = 1e2;
  int nBin = 50;

  //MC repeated nRepetitions times to get the variance
  for(int i=0; i<degreeMax; i++){
    cout << "Doing the variance computation for x^" << i+1 << endl;
    histoVariance[i] = new TH1F("","", nBin, -0.02, 0.02);
    exactIntegralValue = exactIntegral(i+1);
    for(int j=0; j<nRepetitions; j++){
      histoVariance[i]->Fill(HitOrMiss(xMin, xMax, yMin, yMax, i+1, nExtraction) - exactIntegralValue);
    }
  }

  //Plot section
  cVariance->Divide(2,3);
  for(int i=0; i<degreeMax; i++){
    cVariance->cd(i+1);
    histoVariance[i]->SetTitle(Form("Variance for x^{%i}", i+1));
    histoVariance[i]->SetLineColor(9);
    histoVariance[i]->SetFillStyle(3003);
    histoVariance[i]->SetFillColor(4);
    histoVariance[i]->SetLineWidth(2);
    histoVariance[i]->GetXaxis()->SetTitle("|I_{approx} - I_{exact}|");
    histoVariance[i]->GetXaxis()->SetTitleSize(0.055);
    histoVariance[i]->GetXaxis()->SetTitleOffset(0.85);
    histoVariance[i]->GetXaxis()->SetLabelSize(0.06);
    histoVariance[i]->GetYaxis()->SetTitle("Counts");
    histoVariance[i]->GetYaxis()->SetTitleSize(0.055);
    histoVariance[i]->GetYaxis()->SetTitleOffset(0.6);
    histoVariance[i]->GetYaxis()->SetLabelSize(0.06);
    histoVariance[i]->Draw("");
  }
  cVariance->SaveAs("figs/Exercise5/VariancePlot.pdf");


  App->Run();

  return 0;
}