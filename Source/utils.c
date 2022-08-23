#include "utils.h"

struct record_gp * create_record(char * TS, int durata, int status){
    //Allocazione dinamica dell' output 
    struct record_gp * out = (struct record_gp *) malloc(sizeof(struct record_gp));

    //Scrittura del Record
    strncpy(out->TesSan, TS, strlen(TS));
    out->durata= durata;
    out->status = status;

    return out;
}

void printgp(struct record_gp * gp){
    if (gp == NULL) {
        fprintf(stderr,"Errore Print Record Green Pass \n");
        return;
    } 
    printf("Green-Pass: TS: %s, Durata %d \n",gp->TesSan, gp->durata);

}