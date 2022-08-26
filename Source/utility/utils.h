#ifndef utils_h
#define utils_h

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct record_gp {
    char TesSan[8]; //ultime 8 cifre tessera sanitaria
    int From; // indentificatore per vedere da dove provenie 0 CentroVaccinale, 1 ClientS , 2 ClientT
    int durata; //Durata Green Pass
    int status; // Stato Green Pass, 0 valido, 1 scaduto
};

struct record_validate {
    char TesSan[8]; //ultime 8 cifre tessera sanitaria
    int From; // indentificatore per vedere da dove provenie 0 CentroVaccinale, 1 ClientS , 2 ClientT
    int status; // Stato Green Pass, 0 valido, 1 scaduto, 2 verifica
};

struct record_gp * create_record(char * TS, int durata, int status, int where);
struct record_validate * create_Vrecord(char * TS, int From, int status);
void SearchInto(struct record_gp * gp, int fp);

int SearchModifyRecord (struct record_gp * gp, FILE * fp, FILE *fg);

void printgp(struct record_gp * gp);
void checkHash(char * Buffer, int Bsize);

off_t whereisit(int fd, char * TS);
int SeeStatus(char * TS, int fp);

#endif