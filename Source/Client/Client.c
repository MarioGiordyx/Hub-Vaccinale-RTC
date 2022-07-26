#include "wraped.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>


int main(int argc, char **argv) {
	int ASLsk,n;
	char* sendB;
	char recB[256];
	struct sockaddr_in servaddr;

	//Controllo argomenti
	if (argc != 3) {
		fprintf(stderr,"Usage: %s <IP-Address> %s <CF>",argv[1],argv[2]);
		exit(1);
	} else printf("IP %s, CF %s \n",argv[1],argv[2]);

	sendB=argv[2];

    //Setting Socket ASL;
	ASLsk = socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(20);



	//Check Connesione
	if(connect(ASLsk, (struct sockaddr *)&servaddr, sizeof(servaddr))<0) {
		perror("Connection Lost \n");
		exit(1);
	}

	//Write CF
	if(write(ASLsk,sendB,sizeof(sendB)) <0){
		fprintf(stderr, "Erorre, non è possibile mandare il CF");
		exit(1);
	}

	if(read(ASLsk,recB,sizeof(recB)) <0) {
		fprintf(stderr, "Erorre, impossibile leggere dato dal server");
		exit(1);
	}
	printf("<Mess Ricevuto da ASL> : %s \n",recB);

    exit(0);

	return 0;

}