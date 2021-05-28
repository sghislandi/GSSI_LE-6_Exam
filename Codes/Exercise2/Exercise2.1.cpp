//c++ -o  Exercise2.1 Exercise2.1.cpp
//c++ -Ofast -o  Exercise2.1 Exercise2.1.cpp (optimized)

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>


using namespace std;
    
void analyticMethod(int nExtraction){
    srand(time(NULL));
    double random1, random2, x, y;
    int counter = 0;
    while(counter<nExtraction){
        random1 = (double) rand()/RAND_MAX;
        random2 = (double) rand()/RAND_MAX;
        x = sqrt(random1) * cos(2*M_PI*random2);
        y = sqrt(random1) * sin(2*M_PI*random2);
        counter++;
    }
}

void rejectionMethod(int nExtraction){
    srand(time(NULL));
    double random1, random2, x, y;
    int counter = 0;
    while(counter<nExtraction){
        random1 = (double) rand()/RAND_MAX;
        random2 = (double) rand()/RAND_MAX;
        x = -1 + 2*random1;
        y = -1 + 2*random2;
        if(x*x + y*y < 1){counter++;}
    }
}


int main(){
 
    int nExtraction = 1e8;

    clock_t cStartAnalytic = clock();
    analyticMethod(nExtraction);
    clock_t cStopAnalytic = clock();
    cout << "The analytic method takes " << (double) (cStopAnalytic-cStartAnalytic) / CLOCKS_PER_SEC << " seconds" << endl;

    clock_t cStartRejection = clock();
    rejectionMethod(nExtraction);
    clock_t cStopRejection = clock();
    cout << "The rejection method takes " << (double) (cStopRejection-cStartRejection) / CLOCKS_PER_SEC << " seconds" << endl;



    return 0;
}