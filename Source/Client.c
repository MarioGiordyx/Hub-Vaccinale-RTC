#include "utility/Wrapper.h"
#include "utility/utils.h"

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

	//Check se l' input della TS è di 8 cifre
	if (strlen(argv[2]) <= 0 || strlen(argv[2]) > 8){
		fprintf(stderr,"Errore Input Tessera Sanitaria, size troppo piccolo \n");
		exit(1);
	}

	BufferCF= argv[2];

	//Check HASH per vedere se sono numeri compresi da 0 a 9
	checkHash(BufferCF,sizeof(BufferCF));

	//Check Connesione
	printf("[+] Connesione Al centro vaccinale IP : %s \n", argv[1]);

	wrapped_connect(CVsk, (struct sockaddr *)&servaddr, sizeof(servaddr));

	printf("Connesione Effetuata al Centro Vaccinale \n");

	wrapped_fullwrite(CVsk,BufferCF,strlen(BufferCF));

	printf("Ultime 8 cifre  mandate al Centro Vaccinale, Arrivederci !\n");
	
	close(CVsk);

    exit(0);

}