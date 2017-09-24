#include "MOPSO.h"

void initializeParticles(Particle *particle){
    for (int j=0; j<Population; j++){
        //  initialize Velocity
        initializeParticle(particle[j]);
        
    }
}

void initializeParticle(Particle &particle){
    int lenP = particle.sizeOfPosition;
    int lenO = particle.sizeOfAddO_origin;

    particle.Velocity = new double[lenP];
    for (int i=0; i < lenP; i++)
        particle.Velocity[i] = 0;
    particle.sizeOfVelocity = lenP;

    //  old_position
    particle.old_position = new double[lenP];

    //  Best
    particle.Best.Position = new double [lenP];
    cpyDoubleArray(particle.Best.Position,particle.Position,lenP);
    particle.Best.addO_origin = new double [lenO];
    cpyDoubleArray(particle.Best.addO_origin,particle.addO_origin, lenO);
    particle.Best.Cost = new double[objectiveNumber];
    cpyDoubleArray(particle.Best.Cost, particle.Cost, objectiveNumber);
}