#include "Wrapper.h"

int wrapped_socket(int family, int type, int protocol){
    int SockDescriptor;
    if ((SockDescriptor = socket(family,type,protocol)) < 0) {
        perror("socket");
        exit(1);
    }
    return SockDescriptor;
}

void wrapped_connect(int SockDescriptor,struct sockaddr * addr, socklen_t addrlen){
    if (connect(SockDescriptor,addr,addrlen) < 0) {
        perror("connect");
        exit(1);
    }
}

void wrapped_bind(int SockDescriptor, struct sockaddr * addr, socklen_t addrlen){
    if (bind(SockDescriptor,addr,addrlen) < 0){
        perror("bind");
        exit(1);
    }
}

void wrapped_listen(int SockDescriptor, int connNum){
    if (listen(SockDescriptor,connNum) < 0){
        perror("listen");
        exit(1);
    }
}

int wrapped_accept(int SockDescriptor, struct sockaddr * addr, socklen_t addrlen){
    int CommmSockDescriptor;
    if ((CommmSockDescriptor = accept(SockDescriptor,addr,&addrlen)) <0) {
        perror("accept");
        exit(1);
    }
    return CommmSockDescriptor;
}

ssize_t wrapped_fullwrite(int fd, const void * buf, size_t count){
    size_t nleft;
    ssize_t nwritten;

    nleft = count;

    while (nleft > 0) {
        if ((nwritten = write(fd, buf, nleft)) < 0) {
            if (errno == EINTR) continue;
            else exit(nwritten);
        }

        nleft -= nwritten;
        buf += nwritten;
    }

    return nleft;
}

ssize_t wrapped_fullread(int fd,void * buf, size_t count){
    size_t nleft = count;
    ssize_t nread;
    char * next = buf; 

    while (nleft > 0) {
        nread = read(fd, next, nleft);
        if (nread < 0)  {
            //In Caso di Errore
            if (errno == EINTR) {
                continue;
            } else exit(nread);
        } else {
            if (nread == 0) {
            break;
            }
        nleft -= nread;
        next += nread;
        
        }
    }
    return nleft;
}