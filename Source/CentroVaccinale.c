#include "utility/GP_Utils.h"

int main(int argc, char *argv[]){
    int list_fd, conn_fd; //File Descritpor per i socket di listening e connection
    struct sockaddr_in server_cv , server_v; //Famiglie di sockaddr per il serverV e il Centro Vaccinale
    char buffer[8]; //Buffer per  scrittura e lettura CF
    pid_t pid; //PId
    
    list_fd=wrapped_socket(AF_INET,SOCK_STREAM,0); //Richiamo Funzione Wrapper Socket, passando dati AF_INET (IPV4), SOCK_STREAM(Bidirezionale), 0 per i servizi.

    //SockAddr del CentroVacicnale
    server_cv.sin_family = AF_INET;
    server_cv.sin_addr.s_addr = htonl(INADDR_ANY);
    server_cv.sin_port = htons(28);

    //SockAddr del ServerV
    server_v.sin_family = AF_INET;
    server_v.sin_port= htons(31);
    server_v.sin_addr = server_cv.sin_addr; //Si ipotizza che il server V sia sulla stessa macchina

    wrapped_bind(list_fd,(struct sockaddr *)&server_cv,sizeof(server_cv)); //Wrapper per il Bind , si passa il FD del CV, la sua famiglie e il suo size

    printf("[+] Bind effettuato \n"); 
    wrapped_listen(list_fd,1024); //Wrapper per l' ascolto.

    printf("[+] Listen in corso\n");

    while(1){
        conn_fd = wrapped_accept(list_fd, (struct sockaddr *) NULL,NULL); //Wrapper per accettare la connesisone in entrata
        pid=fork();

        if(pid<0){
            perror ("Fork errato");
            close(conn_fd);
            exit(-1);
        }

        // Child
        if (pid==0){
        
            //Lettura 8 cifre Tessesa Sanitaria
            printf("[+] Lettura in corso delle 8 cifre mandate da un client \n");
            
            wrapped_fullread(conn_fd,buffer,sizeof(buffer)); //Lettura in FullRead passando il FD della connesione in entrta, il buffer di lettura e il suo size
            
            printf("[+] Lettura Effetuata, creazione Package \n");

            close(conn_fd); //Chiusura socket di connesio

            //Creazione Socket & connesione con ServerV

            int ServerV_sock = wrapped_socket(AF_INET,SOCK_STREAM,0); //Creazione Socket tramite funzione Wrapper

            wrapped_connect(ServerV_sock,(struct sockaddr_in *) &server_v, sizeof(server_v)); //Connesione al ServerV
            printf("[+] Connessione Effettuata al ServerV, creazione Package \n");

            //Creazione & invio package
            struct record_gp * new_gp;
            new_gp = create_record(buffer,6,0,0);

            printf("[+] Record Creato, invio in corso al ServerV \n");

            printgp(new_gp); //Print Record

            wrapped_fullwrite(ServerV_sock,new_gp,sizeof(struct record_gp)); //Scrittura del Record a ServerV.

            printf("[+] Record inviato, Chiusura Fork \n");

            close(ServerV_sock); //Chiususra Socket ServerV
            
            exit(0);
        } else {
            close(0);
        }
    }
}
