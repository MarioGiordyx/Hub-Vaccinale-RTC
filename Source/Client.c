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
	char* BufferCF;
	struct sockaddr_in servaddr;

	//Controllo argomenti
	if (argc != 3) {
		fprintf(stderr,"Attenzione, il formato di input deve essere : %s <IP-Address>,: %s <Ultime 8 cifre Tessera Sanitaria> \n",argv[1],argv[2]);
		exit(1);
	}

    //Setting Socket Centro Vacinnale;
	CVsk = wrapped_socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(28);

	if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<= 0) {
		fprintf(stderr,"inet_pton error \n");
		exit(1);
	}

	//Check Connesione
	printf("[+] Connesione Al centro vaccinale IP : %s \n", argv[1]);

	wrapped_connect(CVsk, (struct sockaddr *)&servaddr, sizeof(servaddr));

	printf("Connesione Effetuata al Centro Vaccinale \n");
	BufferCF= argv[2];
	
	wrapped_fullwrite(CVsk,BufferCF,strlen(BufferCF));

	printf("Ultime 8 cifre  mandate al Centro Vaccinale, Arrivederci !\n");
	
	close(CVsk);

    exit(0);

}