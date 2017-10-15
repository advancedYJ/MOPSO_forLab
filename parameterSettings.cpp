#include "MOPSO.h"

//  the address of the input data
const char rootAddress[] = "/home/advanced/实验环境/mopso/";
const char *energyFileAddress = catStrStr(rootAddress, "data/energyFile/");
const char *tempFileAddress   = catStrStr(energyFileAddress, "tempFile/");
const char *defaultFileAddress = catStrStr(energyFileAddress, "defaultFile/");
const char *QUACKoutFileAddress= catStrStr(energyFileAddress, "QUACKoutFile/");
const char *charmmFileAddress = catStrStr(rootAddress, "charmm/exec/gnu/");
const char *refine_1Address   = catStrStr(rootAddress, "refine/");
const char *TM_scoreAddress   = catStrStr(rootAddress, "TM_score/");
const char *scoreAddress      = catStrStr(rootAddress, "rosetta/main/source/bin/score.linuxgccrelease");
const char *databaseAddress   = catStrStr(rootAddress, "rosetta/main/database/");
const char *mybinAddress      = catStrStr(rootAddress, "calRWplus/");
const char *strideAddress     = catStrStr(rootAddress, "stride/");

//const char *answerAddress     = catStrStr(rootAddress, "data/answer/newAnswer/");
//const char *charmmFileAddress = catStrStr(energyFileAddress, "charmmFile/");
//const char inputAddress[]="/home/ws/zzZyj/MOPSO/data/input/test_thread";
//const char *inputAddress      = catStrStr(rootAddress, "data/input/originpdb/");
//const char *mybinAddress      = catStrStr(rootAddress, "mybin/");

// Problem Definition
const int nVar = 183;
const double VarMax = 180;
const double VarMin = -180;
//const double VelMax = 0.7;             //  =20 without rama_map

// MOPSO Settings
const int tidSize =8;
const int objectiveNumber = 3; //  Multiple Obejectives settings, the size of objective function
const double TM_scoreThreshold = 0.13;
const int nRep = 50;                // Repository Size
const int Population = 50;
const double Criterion = 0.000001;
const int lambdaLoopTimes = 1000;

const int answerRepNumber = 10;
//const int  MaxIt = 1;          // Maximum Number of Iterations
//const int Population = 2;                  // Population Size


const double phi1 = 2.05;
const double phi2 = 2.05;
const double phi = phi1 + phi2;
const double chi = 2 / ( phi - 2 + sqrt( phi * phi - 4 * phi ) );    // 0.73
const int bufferLen = 2048;

const double wMin = chi;                        // =chi  Inertia Weight
const double wMax = 1.2;
const double wDamp=1;                       //  Inertia Weight Damping Ratio
//const double c1 = chi*phi1;                 //  Personal Learning Coefficient
//const double c2 = chi*phi2;                 //  Global Learning Coefficient
const double c1max = 0.7;
const double c1min = 0.4;
const double c2max = 1.3;
const double c2min = 0.8;


const double Alpha = 0.1;       //Grid Inflation Parameter
const int nGrid = 10;               //Number of Grids per each Dimension
const int Beta = 4;                   //Leader Selection Pressure Parameter
const int Gamma = 2;             // Extra (to be deleted) Repository Member Selection Pressure

const double PI = M_PI;
const double INF=100000000;
const double realMax = 100000000;
const double tiny = 0.0000000001;
