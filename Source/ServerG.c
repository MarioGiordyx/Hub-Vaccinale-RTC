#include "utility/GP_Utils.h"

int main( int argc, char *argv[]){
     int list_fd, conn_fd; //File Descriptor per i socket di Listening e Connection
    struct sockaddr_in server_v , server_g; //famiglie per il ServerV e ServerG
    pid_t pid;

    list_fd=wrapped_socket(AF_INET,SOCK_STREAM,0); //Creazione Socket di Listen

     //SockAddr del ServerV
    server_g.sin_family = AF_INET;
    server_g.sin_addr.s_addr = htonl(INADDR_ANY);
    server_g.sin_port= htons(29);

    server_v.sin_family = AF_INET;
    server_v.sin_port= htons(31);
    server_v.sin_addr = server_g.sin_addr;

    wrapped_bind(list_fd,(struct sockaddr *)&server_g,sizeof(server_g)); //Bind della Listen, passando i dati della famiglia di ServerG

    printf("[+] Bind effettuato \n");
    wrapped_listen(list_fd,1024); //Apertura in Listen della socket list_fd di ServerG

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
           //creazione record di lettura temporanea
           struct record_validate temp_v;
           printf("[+] Lettura package dal socket \n"); 

            //lettura record dalla client connesso
           wrapped_fullread(conn_fd, &temp_v, sizeof(struct record_validate));

            printf("[+] Lettura package fatto \n");
            //Creazione Socket di ServerV
            int serverVSK = wrapped_socket(AF_INET,SOCK_STREAM,0);

            printf("[+] Creazione Package risposta\n");

            //controllo provenienza package
            if(temp_v.From == 1){//proviene da ClientS
                printf("[+] Richiesta Ricevuta da ClientS \n");
                //Creazione del Record GP da fare il check
                struct record_gp* check_gp;
                char buffer[8]; //Buffer per le 8 cifre della tessera Sanitaria
                char risposta[4];  //Buffer per la rispoista;

                //Creazione Record GP
                strncpy(buffer,temp_v.TesSan,sizeof(buffer));
                check_gp = create_record(buffer, 0, 0, 1);

                //Connessione con ServerV
                printf("[+] record creato, avvio Connesione ServerV \n");
                wrapped_connect(serverVSK,(struct sockaddr_in *) &server_v, sizeof(server_v));
                printf("[+] Connesione Stabilita con ServerV, Send Package \n");

                //Scrittura nuovo Record a ServerV
                wrapped_fullwrite(serverVSK, check_gp, sizeof(struct record_gp));
                printf("[+] Package inviato, in attesa di risposta \n");
                
                //Lettura rispsota di ServerV
                wrapped_fullread(serverVSK, risposta,4);
                printf("[+] Risposta ottenuta, comunicazione al ClientS della validazione \n");
                close(serverVSK);
                
                //Scrittura Rispsota al Client
                wrapped_fullwrite(conn_fd, risposta, sizeof(risposta));
                close(conn_fd);
                

                exit(0);
            } else if (temp_v.From == 2) {//proviene da ClientT
                int duration; //Durata Temp

                if (temp_v.status == 0){ //Nel caso sia valido
                    duration = 6;
                } else duration = 0; //nel caso non sia valido

                //Connessione al ServerV
                wrapped_connect(serverVSK,(struct sockaddr_in *) &server_v, sizeof(server_v));

                //Creazione record da modificare
                struct record_gp * validate_gp;
                validate_gp = create_record(temp_v.TesSan, duration, temp_v.status,2);
                
                //Scrittura nuovo Record a ServerV
                wrapped_fullwrite(serverVSK, validate_gp, sizeof(struct record_gp));

                printf("[+] Package Mandato, Chiusura Fork \n");

                close(conn_fd);
                close(serverVSK);

                exit(0);
            }
        } else {
            close(0);
        }
    }

}