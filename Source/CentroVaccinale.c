
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Wrapper.h"
#include "util.h"

int main(int argc, char *argv[]){
    int list_fd, conn_fd;
    struct sockaddr_in server_cv , server_v;
    socklen_t len;
    char buffer[8];
    pid_t pid;
    
    list_fd=wrapped_socket(AF_INET,SOCK_STREAM,0);

    //SockAddr del CentroVacicnale
    server_cv.sin_family = AF_INET;
    server_cv.sin_addr.s_addr = htonl(INADDR_ANY);
    server_cv.sin_port = htons(28);

    //SockAddr del ServerV
    server_v.sin_family = AF_INET;
    server_v.sin_port= htons(31);

    wrapped_bind(list_fd,(struct sockaddr *)&server_cv,sizeof(server_cv));

    printf("[+] Bind effettuato \n");
    wrapped_listen(list_fd,1024);

    printf("[+] Listen in corso\n");

    while(1){
        conn_fd = wrapped_accept(list_fd, (struct sockaddr *) NULL,NULL);
        pid=fork();

        if(pid<0){
            perror ("Fork errato");
            close(conn_fd);
            exit(-1);
        }

        // Child
        if (pid==0){
            close(conn_fd);
            

            //Lettura 8 cifre Tessesa Sanitaria
            printf("[+] Lettura in corso delle 8 cifre mandate da un client \n");
            
            wrapped_fullread(conn_fd,buffer,sizeof(buffer));
            
            printf("[+] Lettura Effetuata, ");

            //Creazione Socket & connesione con ServerV

            int ServerV_sock = wrapped_socket(AF_INET,SOCK_STREAM,0);

            wrapped_connect(ServerV_sock,(struct sockaddr_in *) &server_v, sizeof(server_v));
            printf("[+] Connessione Effettuata al ServerV, creazione Package \n");

            //Creazione & invio package
            struct record_gp * record;
            record = create_record(buffer,6,0);

            printf("[+] Record Creato, invio in corso al ServerV");

            wrapped_fullwrite(ServerV_sock,record,sizeof(record));

            printf("[+] Record inviato, Chiusura Fork");

            close(ServerV_sock);
            
            exit(0);
        } else {
            close(0);
        }
    }
}
