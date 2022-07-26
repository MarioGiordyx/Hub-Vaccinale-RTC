
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    int list_fd, conn_fd;
    int i;
    struct sockaddr_in serv_add,client;
    socklen_t len;
    char buffer[1024];
    pid_t pid;

    if((list_fd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("socket");
        exit(1);
    }

    serv_add.sin_family = AF_INET;
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_add.sin_port = htons(20);

    if(bind(list_fd,(struct sockaddr *)&serv_add,sizeof(serv_add))<0){
        perror("bind");
        exit(1);
    }
    printf("[+] Bind effettuato \n");

    if(listen(list_fd,1024)<0){
        perror("listen");
        exit(1);
    }
    printf("[+] Listen in corso\n");

    while(1){
        len = sizeof(client);
        if((conn_fd = accept(list_fd, (struct sockaddr *) NULL, NULL)) <0) {
            perror("accept");
            exit(1);
        }
        pid=fork();

        if(pid<0){
            perror ("Fork errato");
            exit(-1);
        }
        if (pid==0){
            if(read(conn_fd,buffer,sizeof(buffer)) <0) {
                fprintf(stderr, "Erorre, impossibile leggere dato dal server");
                 exit(1);
            }
                char *message = "Ricevuto CF, convalido";
                if(write(conn_fd,message,sizeof(message)) <0){
                    fprintf(stderr, "Erorre, non è possibile mandare il CF");
                    exit(1);
                }
            close(conn_fd);
            exit(0);
        } else {
            close(0);
        }
    }
}
