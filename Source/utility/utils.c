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
    out->From=From;
    out->status = status;
    return out;
}

void SearchInto(struct record_gp * gp, int fp){
    char dur[2], stat[2];

    if (whereisit(fp, gp->TesSan)!=-1){//Caso esista
        printf("[+] record esistente uscita \n");
        return ;
        } 

        printf("[+] record non trovato, inizio scrittura \n");
        //Write singoli elementi
        write(fp,gp->TesSan,sizeof(gp->TesSan));

        sprintf(dur,"%d",gp->durata);
        sprintf(stat,"%d",gp->status);
        write(fp,stat,1);
        write(fp,dur,1);
        write(fp, "\n",1);
        
        printf("[+] Scrittura Effetuata \n");
        return;
}

int SeeStatus(char * TS, int fp) {
    char buffT[10];
    char status;
    off_t dove = whereisit(fp, TS);
    if (dove != -1) {//Caso esista
    lseek(fp,dove,SEEK_SET);

    read(fp,buffT,sizeof(buffT));

    return atoi(&buffT[9]);
    } else {
        return  3;
    }
}

off_t whereisit(int fd, char * TS){
    off_t temp = 0;
    char buffT[10];
    char realTS[9];
    int row;

    lseek(fd,0,SEEK_SET); //Set file inizio;

    while((row = read(fd,buffT,10))> 0){
        temp+= row;
        strncpy(realTS,buffT,sizeof(realTS));
        realTS[8]='\0';
        printf("%s-%s\n",realTS,TS);

        if (strncmp(realTS,TS,8)==0){
            return temp;
        } else {
            lseek(fd,12,SEEK_CUR);
            temp += 12;
        }
    }
    return -1;
}


int SearchModifyRecord (struct record_gp * gp, int fp){
    off_t dove = whereisit(fp, gp->TesSan);
    char dur[2], stat[2];
    printf("%lld \n",dove);

    if (dove >-1) {//Caso esista

    //Creazione rewrite
    lseek(fp,(dove-10),SEEK_SET);
    write(fp,gp->TesSan,sizeof(gp->TesSan));

    sprintf(dur,"%d",gp->durata);
    sprintf(stat,"%d",gp->status);
    write(fp,stat,1);
    write(fp,dur,1);
    write(fp, "\n",1);
    
    return 1;
    } else return 0;

}