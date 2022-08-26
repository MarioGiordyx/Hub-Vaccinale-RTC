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
    char tempo[10];
    char TS[9];
    char TG[9];
    off_t dove = whereisit(fp, gp->TesSan);

    printf("%lld \n",dove);

    if (dove!=0){
        printf("[+] record esistente uscita \n");
        return ;
        }

        lseek(fp,0,SEEK_END);

        printf("[+] record non trovato, inizio scrittura \n");
        //Write singoli elementi
         char gpp[12];
         sprintf(gpp,"%s%d%d\n",gp->TesSan,gp->status,gp->durata);
         write(fp,gpp,sizeof(gpp)-1);
        printf("[+] Scrittura Effetuata \n");
        return;
}

int SeeStatus(char * TS, int fp) {
    char buffT[10];
    char status;
    off_t dove = whereisit(fp, TS);
    if (dove != 0) {//Caso esista
    fseek(fp,dove,SEEK_SET);

    read(fp,buffT,sizeof(buffT));

    return atoi(buffT[8]);
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

    while((row = read(fd,buffT,sizeof(buffT)))> 0){
        temp++;
        strncpy(realTS,buffT,sizeof(realTS));
        realTS[8]='\0';
        printf("%s-%s\n",realTS,TS);

        if (strcmp(realTS,TS)==0){
            return temp;
        } else {
            lseek(fd,12,SEEK_CUR);
            temp += 12;
        }
    }
    printf("non esiste \n");
    temp = 0;
    return temp;
}


int SearchModifyRecord (struct record_gp * gp, FILE * fp, FILE * fg){
    ssize_t read;
    struct record_gp * temp; //Temp
    rewind(fp);

    while(fread(&temp,sizeof(struct record_gp),1, fp)){
           if (strcmp(gp->TesSan,temp->TesSan)) {
                fseek(fp,0,SEEK_CUR); //Pointer sul posto corrente
                temp->status=gp->status; //modifica stato

                //OverWrite con record modificato
                fwrite(&temp,sizeof(temp),1,fp);
                fwrite("\r\n",sizeof(char),1,fp);
                return 1;
          } 
        }
    return 0;

}