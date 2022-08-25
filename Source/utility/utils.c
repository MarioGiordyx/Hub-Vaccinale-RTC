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

struct record_gp * SearchInto(struct record_gp * gp, FILE * fp){
    ssize_t read;
    struct record_validate * temp = (struct record_validate *) malloc(sizeof(struct record_validate)); //Temp

    while(((read = getline(&temp,sizeof(temp),fp))) != -1){
            // fscanf(fd,"\n\r",temp);
           if (strcmp(gp->TesSan,temp->TesSan)) {
                return temp;
          } 
        }
    return NULL;
}

int SearchModifyRecord (struct record_gp * gp, FILE * fp){
     ssize_t read;
    struct record_validate * temp = (struct record_validate *) malloc(sizeof(struct record_validate)); //Temp

    while(((read = getline(&temp,sizeof(temp),fp))) != -1){
            // fscanf(fd,"\n\r",temp);

           if (strcmp(gp->TesSan,temp->TesSan)) {
                fseek(fp,0,SEEK_CUR); //Pointer sul posto corrente
                temp->status=gp->status; //modifica stato

                //OverWrite con record modificato
                fwrite(temp,sizeof(temp),1,fp);
                fwrite("\r\n",sizeof(char),1,fp);
                return 1;
          } 
        }
    return 0;

}