#include "utility/Wrapper.h"
#include "utility/utils.h"

int main(int argc, char **argv) {
	int ServerGsk;
	struct record_validate * vd;
	char validation[2];
	struct sockaddr_in servaddr;

	//Controllo argomenti
	if (argc != 3) {
		fprintf(stderr,"Attenzione, il formato di input deve essere : %s <IP-Address>,: %s <Ultime 8 cifre Tessera Sanitaria> \n",argv[1],argv[2]);
		exit(1);
	}

    //Setting Socket Centro Vacinnale;
	ServerGsk = wrapped_socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(29);

	if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<= 0) {
		fprintf(stderr,"inet_pton error \n");
		exit(1);
	}

	//Check se l' input della TS è di 8 cifre
	if (strlen(argv[2]) <= 0 || strlen(argv[2]) > 8){
		fprintf(stderr,"Errore Input Tessera Sanitaria, size troppo piccolo \n");
		exit(1);
	}

	//Check HASH per vedere se sono numeri compresi da 0 a 9
	checkHash(argv[2],sizeof(argv[2]));

	//Check Connesione
	printf("[+] Connesione Al serverG IP : %s \n", argv[1]);

	wrapped_connect(ServerGsk, (struct sockaddr *)&servaddr, sizeof(servaddr));

	printf("Connesione Effetuata al serverG \n");

	vd = create_Vrecord(argv[2],1,2);
	
	wrapped_fullwrite(ServerGsk,vd,sizeof(struct record_validate));

	printf("Package mandato al serverG, in attesa di convalida Green Pass...\n");

	wrapped_fullread(ServerGsk, validation, sizeof(validation));

	printf("Risposta Dal ServerG per TS: %s, : %s \n",argv[2],validation);

	printf("Arrivederci! \n");

    exit(0);

}