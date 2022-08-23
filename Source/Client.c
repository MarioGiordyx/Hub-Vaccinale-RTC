#include "Wrapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>


int main(int argc, char **argv) {
	int CVsk,n;
	char* sendB;
	char recB[21];
	struct sockaddr_in servaddr;

	//Controllo argomenti
	if (argc != 3) {
		fprintf(stderr,"Usage: %s <IP-Address> %s <CF>",argv[1],argv[2]);
		exit(1);
	} else printf("IP %s, CF %s \n",argv[1],argv[2]);

    //Setting Socket ASL;
	CVsk = wrapped_socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(28);

	if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<= 0) {
		fprintf(stderr,"inet_pton error \n");
		exit(1);
	}

	//Check Connesione
	wrapped_connect(CVsk, (struct sockaddr *)&servaddr, sizeof(servaddr));

	printf("Inizio Mandando CFF a ASL \n");

	//Write CF
	sendB=argv[2];
	sendB[21]='\0';

	wrapped_fullwrite(CVsk,sendB,strlen(sendB));

	printf("Mess Mandato, Aspetto Rispsota \n");
	
	//Read Risp
	wrapped_fullread(CVsk,recB,sizeof(recB));
	printf("<Mess Ricevuto da ASL> : %s \n",recB);

    exit(0);

}