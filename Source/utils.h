#ifndef utils_h
#define utils_h

#include <string.h>
#include <stdio.h>


// Record per Green Pass
struct record_gp {
    char TesSan[8]; //ultime 8 cifre tessera sanitaria
    int durata; //Durata Green Pass
    int status; // Stato Green Pass, 0 valido, 1 scaduto
};

struct record_gp * create_record(char * TS, int durata, int status);

#endif