//
// Created by advanced on 17-1-16.
//

#include "MOPSO.h"

int nPop;               // nPop = inputFilesNumber + multiplyFilesNumber
double VelMax;
char *inputAddress;
char *answerAddress;
char *logAddress;
int  MaxIt;
int multiplyNumber;
char **argv;

void preDisposeInputParametersAndFiles(char **argv) {
    // argv: [1]input address, [2]number of particles, [3]loop times
    getInputParameter(argv, nPop, MaxIt);
    inputAddress = catStrStr(argv[1], "/");
    answerAddress = catStrStr(argv[1], "Answer/");
    logAddress = catStrStr(answerAddress, "log.txt");
    createNewFold();
    disposePDB();
    freopen(logAddress, "w", stdout);
    nPop += multiplyNumber;
}

void disposePDB(){
    createSeqTxt();
    for (int i=0; i<nPop; i++){
        createParticleTxt(i+1);
        createPhi(i+1);
    }
    char *logAddress = catStrStr(answerAddress, "log.txt");
    freopen(logAddress, "w", stdout);
}

void createSeqTxt(){
    //char address[100] = "/home/ws/zzZyj/MOPSO/data/temp.pdb";
    char *address = catStrIntStr(inputAddress, 1, ".pdb");
    int cntLines = getLines(address);

    // input the data in pdb
    Atom *atom;
    int atomNum;
    atom = new Atom[cntLines+100];
    getAtom(atom, cntLines, atomNum, address);

    // create seq.txt
    char *seqAddress = catStrStr(inputAddress, "seq.txt");
    freopen(seqAddress, "w", stdout);
    printf(">TR233\n");
    int k=0;
    for (int i=0; i<atomNum; i++){
        if (atom[i].number != k){
            printf("%c", getAbbreviation(atom[i].group));
            k = atom[i].number;
        }
    }
    printf("\n");
}

void createParticleTxt(int k){
    char *address = catStrIntStr(inputAddress, k, ".pdb");
    int cntLines = getLines(address);

    // input the data in pdb
    Atom *atom;
    atom = new Atom[cntLines+100];
    int atomNum;
    getAtom(atom, cntLines, atomNum, address);

    // print the start number
    char *startNumberAddress = catStrStr(inputAddress, "startNumber");
    freopen(startNumberAddress, "w", stdout);
    printf("%d\n", atom[0].number);

    //  creat particle.txt
    char *particleAddress = catStrIntStr(inputAddress, "particleO_", k, ".txt");
    freopen(particleAddress, "w", stdout);
    for (int i=0; i<atomNum; i++){
        if (isImportantAtom(atom[i]))
            printf("%lf %lf %lf\n", atom[i].x-atom[0].x, atom[i].y-atom[0].y, atom[i].z-atom[0].z);
    }


}

void createPhi(int k){
    //system(" cd /home/ws/GL/stride && ./stride -mFile /home/ws/zzZyj/MOPSO/data/temp.pdb > /home/ws/zzZyj/MOPSO/data/tempPhi.txt");
    char *pdbAddress = catStrIntStr(inputAddress, k, ".pdb");
    char *prePhiAddress = catStrIntStr(inputAddress, "prePhi", k, ".txt");
    char *runStride = catStrStr("cd ", strideAddress, " && ./stride -mFile ",
                                pdbAddress, " > ", prePhiAddress);
    //  stride.exe
    system(runStride);
    // getLines
    int cntLines = getLines(prePhiAddress);

    //  create Phi.txt
    freopen(prePhiAddress, "r", stdin);
    char *phiAddress = catStrIntStr(inputAddress, "phi", k, ".txt");
    freopen(phiAddress, "w", stdout);
    char firstWord[100];
    int lineIt = 1;
    scanf("%s", firstWord);
    while (strcmp(firstWord, "ASG") != 0){
        char *buffer;
        buffer = new char[1000];
        scanf("%[^\n]", buffer);
        lineIt++;
        scanf("%s", firstWord);
    }
    // cout phi (ignore the first 360 and the last 360)
    double data1, data2;
    // first one
    scanf("%*s%*s%*s%*s%*s%*s%lf%lf%*f%*s", &data1, &data2);
    printf("%lf\n", data2);
    lineIt++;
    // ordinary ones
    for (int i = lineIt; i<cntLines; i++){
        scanf("%*s%*s%*s%*s%*s%*s%*s%lf%lf%*f%*s", &data1, &data2);
        printf("%lf %lf\n", data1, data2);
    }
    // last one
    scanf("%*s%*s%*s%*s%*s%*s%*s%lf%lf%*f%*s", &data1, &data2);
    printf("%lf\n", data1);
}

bool isImportantAtom(const Atom atom){
    const char *name = atom.name;
    if (strcmp(name, "N")==0 || strcmp(name, "CA")==0 || strcmp(name, "C")==0 || strcmp(name, "O")==0)
        return 1;
    else
        return 0;
}

char getAbbreviation(char *str){
    if (strcmp(str, "ALA") == 0) return 'A';
    if (strcmp(str, "ARG") == 0) return 'R';
    if (strcmp(str, "ASN") == 0) return 'N';
    if (strcmp(str, "ASP") == 0) return 'D';
    if (strcmp(str, "CYS") == 0) return 'C';
    if (strcmp(str, "GLN") == 0) return 'Q';
    if (strcmp(str, "GLU") == 0) return 'E';
    if (strcmp(str, "GLY") == 0) return 'G';
    if (strcmp(str, "HIS") == 0) return 'H';
    if (strcmp(str, "ILE") == 0) return 'I';
    if (strcmp(str, "LEU") == 0) return 'L';
    if (strcmp(str, "LYS") == 0) return 'K';
    if (strcmp(str, "MET") == 0) return 'M';
    if (strcmp(str, "PHE") == 0) return 'F';
    if (strcmp(str, "PRO") == 0) return 'P';
    if (strcmp(str, "SER") == 0) return 'S';
    if (strcmp(str, "THR") == 0) return 'T';
    if (strcmp(str, "TRP") == 0) return 'W';
    if (strcmp(str, "TYR") == 0) return 'Y';
    if (strcmp(str, "VAL") == 0) return 'V';
}

void getArgv(){
    argv = new char *[100];
    for (int i=0; i<100; i++)
        argv[i] = new char [100];
    for (int i=1; i<4; i++) {
        cin >> argv[i];
    }
}

