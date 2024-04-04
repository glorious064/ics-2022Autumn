#ifndef INSTR_H
#define INSTR_H
const int IHALT = 0;
const int INOP = 1;
const int IRRMOVQ = 2;
const int IIRMOVQ = 3;
const int IRMMOVQ = 4;
const int IMRMOVQ = 5;
const int IOPQ = 6;
const int IJXX = 7;
const int ICMOVXX = 2;
const int ICALL = 8;
const int IRET = 9;
const int IPUSHQ = 0xa;
const int IPOPQ = 0xb;
const int IIADDQ = 0xc;
#endif