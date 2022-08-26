#include "utility/Wrapper.h"
#include "utility/utils.h"

int main( int argc, char *argv[]){
     int list_fd, conn_fd;
    struct sockaddr_in server_v , server_g;
    socklen_t len;
    pid_t pid;

    list_fd=wrapped_socket(AF_INET,SOCK_STREAM,0);
    

     //SockAddr del ServerV
    server_g.sin_family = AF_INET;
    server_g.sin_addr.s_addr = htonl(INADDR_ANY);
    server_g.sin_port= htons(29);

    server_v.sin_family = AF_INET;
    server_v.sin_port= htons(31);
    server_v.sin_addr = server_g.sin_addr;

   wrapped_bind(list_fd,(struct sockaddr *)&server_g,sizeof(server_g));

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
           struct record_validate temp_v;
           printf("[+] Lettura package dal socket \n"); 

           wrapped_fullread(conn_fd, &temp_v, sizeof(struct record_validate));

            printf("[+] Lettura package fatto \n");
            int serverVSK = wrapped_socket(AF_INET,SOCK_STREAM,0);

            printf("[+] Creazione Package risposta\n");

            //controllo provenienza package
            if(temp_v.From == 1){//proviene da ClientS
                printf("[+] Richiesta Ricevuta da ClientS \n");
                struct record_gp* check_gp;
                char buffer[8];
                char risposta[8];
                strncpy(buffer,temp_v.TesSan,sizeof(buffer));
                printf("%s \n",buffer);

                check_gp = create_record(buffer, 0, 0, 1);

                printf("[+] record creato, avvio Connesione ServerV \n");
                wrapped_connect(serverVSK,(struct sockaddr_in *) &server_v, sizeof(server_v));
                printf("[+] Connesione Stabilita con ServerV, Send Package \n");

                wrapped_fullwrite(serverVSK, check_gp, sizeof(struct record_gp));

                printf("[+] Package inviato, in attesa di risposta \n");
                
                wrapped_fullread(serverVSK, risposta, sizeof(risposta));

                printf("[+] Risposta ottenuta, comunicazione al ClientS della validazione \n");

                wrapped_fullwrite(conn_fd, risposta, sizeof(risposta));

                close(conn_fd);
                close(serverVSK);

                exit(0);
            } else if (temp_v.From == 2) {//proviene da ClientT
                int duration;
                if (temp_v.status == 0){
                    duration = 6;
                } else duration = 0;

                struct record_gp * validate_gp = create_record(temp_v.TesSan, duration, temp_v.status,2);

                wrapped_connect(serverVSK,(struct sockaddr_in *) &server_v, sizeof(server_v));

                wrapped_fullwrite(serverVSK, validate_gp, sizeof(validate_gp));

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