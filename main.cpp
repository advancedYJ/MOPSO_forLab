#include "MOPSO.h"


int main(int argc, char *argv[])
//int main()
{
    //getArgv();                         // argv: [1]input address, [2]number of particles, [3]loop times
    multiplyNumber = 3;
    setTime();
    preDisposeInputParametersAndFiles(argv);
    applyVariable();
    multiplyBetterInput();
    inputParticles(particle);               // Input Data
    updateRep(particle, rep, 0);


    // MOPSO Main Loop
    for (int it = 1; it <= MaxIt; it++) {
        printCurrentId(it);
        for (int i = 0; i < nPop; i++) {
            //PSOAdaptionForXYZ(particle[i], rep, it - 1);
            PSOAdaptionForPhi(particle[i], rep, it - 1);            //  apply the PSO formula
            convertRotationToCoordinary(particle[i]);    //  convert rotation to coordinary
        }

        //checkAllParticleSimilarity(particle);
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
/home/advanced/MOPSO/data/input/TR760 2 2
 */