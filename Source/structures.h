#ifndef structures_h
#define structures_h

// Record per Green Pass
struct record_gp {
    char TesSan[8]; //ultime 8 cifre tessera sanitaria
    int durata; //Durata Green Pass
    int status; // Stato Green Pass, 0 valido, 1 scaduto
};

#endif