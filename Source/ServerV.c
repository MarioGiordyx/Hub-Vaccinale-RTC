#include "utility/Wrapper.h"
#include "utility/utils.h"
#include <string.h>
#include "pthread.h" //Gestione tramite MUTEX file .txt

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //Dichiarazione globale Mutex;

int CheckWhereFrom(struct record_gp * gp, FILE * fd, FILE * fg);

int CheckWhereFrom(struct record_gp * gp, FILE * fd, FILE * fg){
    if (gp==NULL){
        fprintf(stderr,"Package Green Pass Vuoto, ritorno \n");
        return -1;
    }
    if (gp->From == 0) { //Proviene da CentroVaccinale
 
    pthread_mutex_lock(&mutex); //Entra in mutua esclusione
    printf("[+] Entro in Mutua Esclusione \n");
    SearchInto(gp,fd,fg);
    pthread_mutex_unlock(&mutex);
    return 3;
    
    } else if (gp->From == 1) { //Proviene da ServerG, richiesta di ClientS
        pthread_mutex_lock(&mutex); 
        struct record_gp * temp; //= SearchInto(gp,fg);
        pthread_mutex_unlock(&mutex);

        if (temp == NULL) return -1; // Caso sia nullo
        return temp->status;
    } if (gp->From == 2) {// Proviene da ServerG, richeista di ClientT
        printf("[+] Cerco Recrd \n");

        pthread_mutex_lock(&mutex); //Entra in mutua esclusione
        int v = SearchModifyRecord(gp,fd,fg);
        pthread_mutex_unlock(&mutex);  //Esce in mutua esclusione

        if (v == 0) {// non sia presente
            printf("[+] record non presente \n");
            
        } else if (v == 1) {
            printf("[+] Record Modificato");
        }
        return 2;

    }

    return -1;
}

int main(int argc, char *argv[]){
    int list_fd, conn_fd;
    struct sockaddr_in server_v;
    socklen_t len;
    pid_t pid;
    FILE *fgpr;
    FILE *fgpw;
    char respond[2];

    printf("[+] Open Green-Pass File \n");
    fgpr = fopen("gp.txt","r");
    fgpw = fopen("gp.txt","w");

    if (fgpr == NULL || fgpw == NULL){
        fprintf(stderr,"Errore Apertura File .txt \n");
        exit(1);
    }

    list_fd=wrapped_socket(AF_INET,SOCK_STREAM,0);

    //SockAddr del ServerV
    server_v.sin_family = AF_INET;
    server_v.sin_addr.s_addr = htonl(INADDR_ANY);
    server_v.sin_port= htons(31);

    wrapped_bind(list_fd,(struct sockaddr *)&server_v,sizeof(server_v));

    printf("[+] Bind effettuato \n");
    wrapped_listen(list_fd,1024);

    printf("[+] Listen in corso\n");

    while(1){
        conn_fd = wrapped_accept(list_fd, (struct sockaddr *) NULL,NULL);
        pid=fork();

        if(pid<0){
            perror("Fork Errato");
            close(conn_fd);
            exit(-1);
        }

        //Child
        if (pid==0){
            //Lettura in input packeg
            struct record_gp temp_gp;
            
            printf("[+] Lettura Package dal Socket \n");

            wrapped_fullread(conn_fd,&temp_gp,sizeof(struct record_gp));
            
            printf("[+] Lettura package fatto \n");

            close(conn_fd);

            printf(" \n");

            int v = CheckWhereFrom(&temp_gp,fgpr,fgpw); //Verifica se richiesta è avvenuta
            
            //switch per i 3 casi di richiesta (1 non validio, 2 valido, 3 modifica)
            switch(v){
                case 0: // Caso Richiesta da ClientS valido
                printf("[+] Validazione effetuata, invio report \n");
                snprintf(respond,sizeof(respond),"SI");
                wrapped_fullwrite(conn_fd,respond,sizeof(respond));
                break;

                case 1: //Caso Richiesta da ClientS non valido
                printf("[+] Validazione effetuata, invio report \n");
                snprintf(respond,sizeof(respond),"NO");
                wrapped_fullwrite(conn_fd,respond,sizeof(respond));
                break;

                case 2:
                break;

                case 3:
                break;

                default:
                fprintf(stderr,"Errore risposta Richiesta, Richiesta non elaborabile \n");
            }

            close(conn_fd);

            printf("[-] Chiusura Connesione e Fork in corso \n");
            
            exit(0);

        } else {
            close(0);
        }
    }
}