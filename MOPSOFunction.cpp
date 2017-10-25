#include "MOPSO.h"

void PSOAdaptionForPhi(Particle &particle, myRep &rep, int it) {
    double w = getInertiaWeight(it-1, MaxIt);       //  get inertia wight
    int len = particle.sizeOfPosition;
    // int h = rouletteWheel(rep);
    int h = getGBest(particle, rep);
    cpyDoubleArray(particle.old_position, particle.Position, len);
    myRep ::iterator a = rep.begin();
    for (int i=0; i<h; i++) a++;
    //  calculate the new Velocity
    double tmp = acos(-2.0 * currentIteration_Times / times_for_each_play + 1) / PI;    // double tmp = acos(-2.0 * it / MaxIt + 1) / PI;
    double c1 = c1max - (c1max - c1min) * (1 - tmp);
    double c2 = c2min + (c2max - c2min) * (1 - tmp);
    printf("it : %d  c1 : %lf  c2 : %lf\n", it, c1, c2);
    for (int i=0; i<len; i++){
        double r1 = rand() / double(RAND_MAX) , r2 = rand() / double(RAND_MAX) ;
        if(it==0 && i%3 != 1) particle.Velocity[i] = rand() / double(RAND_MAX);
        particle.Velocity[i] = w * particle.Velocity[i] +
                               c1 * r1* (particle.Best.Position[i] - particle.Position[i]) +
                               c2 * r2 * (a->Position[i] - particle.Position[i]);
    }
    //  check velocity range
    checkV(particle.Velocity, len);

    cout << "it : " << it << "print V:" <<   endl;
    for (int i = 0; i < len; i++){
        printf("V: %lf VelMax: %lf\n", particle.Velocity[i], VelMax[i]);
    }
    printf("\n");

    //  calculate the new position
    for (int i=0; i<len; i++)   particle.Position[i] += particle.Velocity[i];
    //  check position range
    checkP(particle.Position, particle.Velocity, len);

}

void getAllParticleCost(Particle * particle){
    pthread_t tids[tidSize];
    int haveRun=0;
    while (haveRun < Population){
        for (int i=0; i<tidSize && haveRun+i<Population; i++){
            pthread_create(&tids[i], NULL, getAParticleCost, (void *) &particle[haveRun+i]);
        }
        for (int i=0; i<tidSize && haveRun+i<Population; i++){
            pthread_join(tids[i], NULL);
        }
        haveRun += tidSize;
    }
}

void *getAParticleCost(void *p){
    Particle *particle = (Particle *) p;
    printPdb(particle);
    runScwrl(particle->index);

    particle->Cost[0] = getCost0(particle->index);

    particle->Cost[1] = getCost1(particle->index);

    particle->Cost[2] = getCost2(particle->index);

    removeTmp(particle->index);
}

/*void getAllParticleCostForTest(Particle * particle){                   // calculate f(x)
    for (int i=0; i<Population; i++)
    {
        int len = particle[i].sizeOfAddO_origin;
        double ans1 = 0, ans2=0;
        for (int j=0; j<len; j+=3){
            double x= particle[i].addO_origin[j];
            double y= particle[i].addO_origin[j+1];
            double z= particle[i].addO_origin[j+2];
            ans1 += dis1(x,y,z);
            ans2 += dis2(x, y, z);
        }
        particle[i].Cost[0] = ans1;
        particle[i].Cost[1] = ans2;
    }
}
*/
bool isDominated(double *cost1, double *cost2){
    bool flag = 1;                                          //  if flag is true then  cost1 is dominated by cost2
    //  which means  cost1 >= cost2, for all the f(x)
    bool equal = 1;
    for (int i=0; i<objectiveNumber; i++){
        if (cost1[i] < cost2[i]){   flag = 0;  equal = 0;   break;      }
        if ( !doubleEqual(cost1[i], cost2[i]) )  equal = 0;
    }
    if (equal)  flag  =  1;                             //   if cost1 = cost2 ,for all the f(x), then cost1 is not
    return flag;                                          //dominated by cost2, which means 2 is not better than 1
}                                                       // but it will generate many same paticles in the rep
                                                        // so I change it to true
void decideDominated(Particle * particle){
    for (int i=0; i<Population; i++)
        particle[i].dominated = 0;

    for (int i=0; i<Population; i++)
        for (int j=0; j<Population; j++) {
            if (i==j) continue;
            if (isDominated(particle[i].Cost, particle[j].Cost))
                particle[i].dominated = 1;
        }
}


void updatePBest(Particle * particle){
    for (int i=0; i<Population; i++){
        int update=0;               //  update=-1  :  pre pBest is better;  update=1: new particle is better
        if (isDominated(particle[i].Cost, particle[i].Best.Cost))  update = -1;
        if (isDominated(particle[i].Best.Cost, particle[i].Cost))  update = 1;

        if (update==0)
            if (rand() % 2 ==1)
                update=1;
        if (update == 1){
            cpyDoubleArray(particle[i].Best.addO_origin, particle[i].addO_origin, particle[i].sizeOfAddO_origin);
            cpyDoubleArray(particle[i].Best.Position, particle[i].Position, particle[i].sizeOfPosition);
            cpyDoubleArray(particle[i].Best.Cost, particle[i].Cost, objectiveNumber);
        }
    }
}