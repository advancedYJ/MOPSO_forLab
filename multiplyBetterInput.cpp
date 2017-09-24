//
// Created by advanced on 17-1-16.
//

#include "MOPSO.h"

void multiplyBetterInput() {
    char *seq;
    seq = inputSeq();//input seq
    get_VelMax(seq);                       //  get VelMax for multiplyParticle
    for (int i=0; i < multiplyNumber; i++) {
        Particle particle;
        inputParticle(particle, 0, seq);
        initializeParticle(particle);
        multiplyParticle(particle, nPop - multiplyNumber + i);
    }
}

void multiplyParticle(Particle particle, int repNum){
    int len = particle.sizeOfPosition;
    cpyDoubleArray(particle.old_position, particle.Position, len);
    //  calculate Velocity, like PSO update
    for (int i=0; i<len; i++){
        double r = rand() *0.7/ double(RAND_MAX);
        particle.Velocity[i] = r;
    }
    checkV(particle.Velocity, particle.sizeOfVelocity);  

    //  calculate the new position
    for (int i=0; i<len; i++)  {
        particle.Position[i] += particle.Velocity[i];
    }

    checkP(particle.Position, particle.Velocity, particle.sizeOfPosition);
    
    convertRotationToCoordinary(particle);

    // generate particleO_21.txt ...
    char *filename;
    filename = catStrIntStr(inputAddress, "particleO_", repNum+1, ".txt");

    ofstream outFile(filename);
    len = particle.sizeOfAddO_origin;
    for (int i=0; i<len; i++){
        outFile << particle.addO_origin[i] << " ";
        if (i % 3 == 2) outFile << endl;
    }

    // generate phi21.txt ...
    filename = catStrIntStr(inputAddress, "phi", repNum+1, ".txt");
    //outFile.open(filename);
    ofstream outFileForPhi(filename);
    len = 3*particle.numAA-5;
    for (int j=0; j < len; j+=3){          // last : j=3*numAA-6; j+2=3*numAA-4
        outFileForPhi << particle.Position[j] << " ";
        outFileForPhi << particle.Position[j+2] << endl;
    }
}

void get_VelMax(char *seq) {
    int SeqLen = static_cast<int>(strlen(seq));
    if (SeqLen > 250)
        VelMax = 0.5;
    else if (SeqLen > 120)      // 120 - 250
        VelMax = 0.8;
    else
        VelMax =1.2;
}
