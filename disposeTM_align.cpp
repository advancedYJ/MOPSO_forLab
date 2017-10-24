//
// Created by advanced on 17-10-22.
//

#include "MOPSO.h"
/*
 * Position 0 and Position 1 refer to first AA while Position 1 is always 180
 * Position 2 ,Position 3, Position 4 refer to second[2] AA while Position 4 is always 180
 * Position 5 ,Position 6, Position 7 refer to third[3] AA while Position 7 is always 180
 * Position 8 ,Position 9, Position 10 refer to fourth[4] AA while Position 10 is always 180
 * ...  if there's only 5 AA, then Position 11 refer to the last AA
 * so for not first nor last AA, (Kth_AA - 2) * 3 + 2, (Kth_AA - 2) * 3 + 3, (Kth_AA - 2) * 3 + 4 is Mth_Atom
 */
void setVelMax(double *VelMax, const int &seq_AANum, const int &seqLen, const int &VelMax_Len) {
    if (seq_AANum == 0) {       // the first AA
        VelMax[0] = VelMax[1] = VEL_SMALL_RANGE;
    } else if (seq_AANum == seqLen - 1) {       //  the last AA
        VelMax[VelMax_Len - 1] = VEL_SMALL_RANGE;
    } else {    //  ordinary AA
        VelMax[(seq_AANum - 2) * 3 + 2] = VelMax[(seq_AANum - 2) * 3 + 3] = VelMax[(seq_AANum - 2) * 3 + 4] = VEL_SMALL_RANGE;
    }
}

void getVelMax_by_TMalign() {
    const char *firstPdb_File = catStrIntStr(inputAddress, 1, ".pdb");
    const char *secondPdb_File = catStrIntStr(inputAddress, 2, ".pdb");
    const char *TMalign_output = catStrIntStr(inputAddress, "align", 1, ".txt");
    const char *command = catStrStr("cd ", TM_alignAddress, " && ./TMalign ", firstPdb_File, " ", secondPdb_File, " > ", TMalign_output);
    system(command);
    //system("cd /home/advanced/实验环境/mopso/TMalign && ./TMalign /home/advanced/Data/TR829/1.pdb /home/advanced/Data/TR829Answer/rep2.pdb > /home/advanced/Data/TR829Answer/align.txt");


    int tmp = getLines(catStrStr(inputAddress, "particleO_1.txt"));
    int numAA = tmp/4;

    VelMax = new double [3 * numAA-3];
    int VelMax_Len = 3 * numAA - 3;
    ifstream inputFile(TMalign_output);
    if (!inputFile.is_open()){
        printf("can't open align.txt!\n");
        exit(0);
    }

    string str1;
    int cntLines = 0;
    while (getline(inputFile, str1)){
        cntLines++;
    }
    inputFile.clear();
    inputFile.seekg(0);             // back to start of the file
    int nowLine = 0;
    string seq1, seq2, referenceLine;
    while (getline(inputFile, str1)){
        nowLine++;
        if (nowLine == cntLines - 3)
            seq1 = str1;
        else if (nowLine == cntLines - 2)
            referenceLine = str1;
        else if (nowLine == cntLines -1)
            seq2 = str1;
    }

    for (int i = 0; i < VelMax_Len; i++)
        VelMax[i] = VEL_BIG_RANGE;

    int seqLen = static_cast<int>(seq1.length());
    int seq1_AANum = 0, seq2_AANum = 0;
    for (int i = 0; i < seqLen; i++){
        if (seq1[i] != '-')
            seq1_AANum++;
        if (seq2[i] != '-')
            seq2_AANum++;
        if (referenceLine[i] == ':')
            if ( (seq1[i] == seq2[i]) && (seq1_AANum == seq2_AANum) ) {
                setVelMax(VelMax, seq1_AANum, seqLen, VelMax_Len);
            }
    }
    //for (int i = 0; i < VelMax_Len; i++)
    //printf("i : %d v : %lf\n", i, VelMax[i]);
    inputFile.close();
}