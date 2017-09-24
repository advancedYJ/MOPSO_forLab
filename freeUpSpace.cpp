//
// Created by advanced on 17-2-7.
//

#include "MOPSO.h"

void freeUpSpace(Particle *& particle){
    delete [] particle;
}