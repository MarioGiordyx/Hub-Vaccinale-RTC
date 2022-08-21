#include "Wrapper.h"

int wrapped_socket(int family, int type, int protocol){
    int SockDescriptor;
    if (SockDescriptor = socket(family,type,protocol) < 0) {
        perror("socket");
        exit(1);
    }
    return SockDescriptor;
}

void wrapped_connect(int SockDescriptor, const struct sockaddr * addr, socklen_t addrlen){
    if (connect(SockDescriptor,addr,addrlen) < 0) {
        perror("connect");
        exit(1);
    }
}

void wrapped_bind(int SockDescriptor, const struct sockaddr * addr, socklen_t addrlen){
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

int wrapped_accept(int SockDescriptor, const struct sockaddr * addr, socklen_t addrlen){
    int CommmSockDescriptor;
    if ((CommmSockDescriptor = accept(SockDescriptor,addr,addrlen)) <0) {
        perror("accept");
        exit(1);
    }
    return CommmSockDescriptor;
}