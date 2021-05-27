//c++ -o  Exercise3 Exercise3.cpp `root-config --glibs --cflags`

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TEllipse.h"

using namespace std;


void PIMonteCarlo(int nExtraction, vector<double> &point, double &PI){
    double random1, random2;
    random1 = (double) rand()/RAND_MAX;
    random2 = (double) rand()/RAND_MAX;
    point[0] = -1 + 2*random1;
    point[1] = -1 + 2*random2;
    if (point[0]*point[0] + point[1]*point[1] <= 1){PI ++;}
}


int main(){
    TApplication * App = new TApplication("T",0,NULL);
    TCanvas * c = new TCanvas();
    TGraph * g = new TGraph();
    TEllipse *circle = new TEllipse(0,0,1);
    
    int nExtraction = 1e5;
    vector<double> point(2,0);
    double PI = 0;
    double difference;

    for(int i=0; i<nExtraction; i++){
        PIMonteCarlo(nExtraction, point, PI);
        g->SetPoint(i, point[0], point[1]);
    }
    PI = (double) 4. * PI / nExtraction;
    cout << "Your estimate of pi is: " << PI << endl;

    c->cd();
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.5);
    g->SetMarkerColor(2);
    g->Draw("AP");
    circle->SetLineWidth(2);
    circle->SetFillStyle(0);
    circle->Draw();
    App->Run();
    return 0;
}