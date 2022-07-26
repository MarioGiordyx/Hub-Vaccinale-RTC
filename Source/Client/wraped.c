//
// Created by Simone Cioffi on 20/10/21.
//

#include "wraped.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>


int Socket(int family, int type, int protocol){
    int n;

    if(n== socket(family,type,protocol)<0){
        perror("socket");
        exit(1);
    }
return(n);
}

int Connect(int ciao, const struct sockaddr *protocol, socklen_t socket){
    int n;

    if(n== connect(ciao,protocol,socket)<0){
        perror("connect");
        exit(1);
    }
    return(n);
}

int Bind(int file, const struct sockaddr *address,socklen_t socket){
    int n;

    if(n==bind(file,address,socket)<0){
        perror("Bind");
        exit(1);
    }
    return(n);
}

int Listen(int sockid, int lung){
    int n;

    if(n== listen(sockid,lung)<0){
        perror("listen");
        exit(1);
    }
    return(n);
}

int Accept(int sockfd, struct sockaddr *clientaddr, socklen_t *addr_dim){
    int n;
    if(n==accept(sockfd,clientaddr,addr_dim)<0){
        perror("Accept");
        exit(1);
    }
    return(n);
}





