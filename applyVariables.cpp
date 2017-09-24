//
// Created by advanced on 17-2-16.
//

#include "MOPSO.h"

Particle *particle;
ANGLE *angle;
myRep rep;
int *sortAns;

void applyVariable(){
    particle = new Particle[nPop];                       // nPop = inputFilesNumber + multiplyFilesNumber
    angle = new ANGLE[nRep + 100];
    sortAns = new int[nRep + 100];
    angle[0].id = -1;
}

void releaseSpace(double **p, const int &n){
    for (int i = 0; i < n; i++){
        delete [] p[i];
    }
    delete [] p;
}

void releaseSpace(double *p){
    delete [] p;
}