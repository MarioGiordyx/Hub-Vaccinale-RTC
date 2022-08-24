#include "utils.h"

struct record_gp * create_record(char * TS, int durata, int status, int where){
    //Allocazione dinamica dell' output 
    struct record_gp * out = (struct record_gp *) malloc(sizeof(struct record_gp));

    //Scrittura del Record
    strncpy(out->TesSan, TS, strlen(TS));
    out->durata= durata;
    out->status = status;
    out->From = where;
    return out;
}

void printgp(struct record_gp * gp){
    if (gp == NULL) {
        fprintf(stderr,"Errore Print Record Green Pass \n");
        return;
    } 
    printf("Green-Pass: TS: %s, Durata %d, From: %d \n",gp->TesSan, gp->durata, gp->From);

}

void checkHash(char * Buffer, int Bsize){
    for (int i = 0; i < Bsize; i++){
        if (Buffer[i] > 57 || Buffer[i] < 48) {//Vede se non è compreson del range dei codici HASH dei numeri
            fprintf(stderr,"Errore Input, non è composta da numeri \n");
            exit(1);
        }  
    }
}

struct record_validate * create_Vrecord(char * TS, int From, int status){
    //Allocazione dinamica dell' output 
    struct record_validate * out = (struct record_validate *) malloc(sizeof(struct record_validate));

    //Scrittura del Record
    strncpy(out->TesSan, TS, strlen(TS));
    out->status = status;
    return out;
}