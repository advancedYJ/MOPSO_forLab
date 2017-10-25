#include "MOPSO.h"


int main(int argc, char *argv[])
{
    setTime();
    preDisposeInputParametersAndFiles(argv);
    applyVariable();
    updateVelocityCheck(0);                 // initialize VelMax[]
    multiplyBetterInput();
    inputParticles(particle);               // Input Data
    updateRep(particle, rep, 0);


    // MOPSO Main Loop
    for (int it = 1; it <= MaxIt; it++) {
        printCurrentId(it);
        updateVelocityCheck(it-1);                    // update VelMax if necessary
        for (int i = 0; i < Population; i++) {
            PSOAdaptionForPhi(particle[i], rep, it - 1);            //  apply the PSO formula
            convertRotationToCoordinary(particle[i]);    //  convert rotation to coordinary
        }

        updateRep(particle, rep, it);
        updatePBest(particle);                       //      update pBest
        printTime(1, it);
    }
    outputAnswer(rep);
    printTime(0, 0);
    freeUpSpace(particle);
    return 0;
}

/*
In linux terminate:
      ./AIR /home/advanced/Data/TR829 7 3000 50 1 2
 /*
[argv]<0>       <1>                           <2>                     <3>                 <4>
     ./AIR /home/advanced/Data/TR829    7                            3000                 50
       <input address>                 <standard particle numbers>   <iteration times>    <total population>

 */