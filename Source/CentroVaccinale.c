
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Wrapper.h"

int main(int argc, char *argv[]){
    int list_fd, conn_fd;
    struct sockaddr_in serv_add;
    socklen_t len;
    char buffer[21];
    pid_t pid;
    
    list_fd=wrapped_socket(AF_INET,SOCK_STREAM,0);

    serv_add.sin_family = AF_INET;
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_add.sin_port = htons(28);

    wrapped_bind(list_fd,(struct sockaddr *)&serv_add,sizeof(serv_add));

    printf("[+] Bind effettuato \n");
    wrapped_listen(list_fd,1024);

    printf("[+] Listen in corso\n");

    while(1){
        conn_fd = wrapped_accept(list_fd, (struct sockaddr *) NULL,NULL);
        pid=fork();

        if(pid<0){
            perror ("Fork errato");
            exit(-1);
        }
        if (pid==0){
            printf("[+] Connesione effettuata da un Client ! \n");
            
            wrapped_fullread(conn_fd,buffer,sizeof(buffer));
            
            printf("[+] Ricevuto Da Client: %s \n",buffer);

            char *message = "Ricevuto-Conv";
            wrapped_fullwrite(conn_fd,message,strlen(message));
            
            printf("[-] Messaggio Mandato, Terminazione Fork \n");
            close(conn_fd);
            exit(0);
        } else {
            close(0);
        }
    }
}
