#ifndef Wrapper_h
#define Wrapper_h

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int wrapped_socket(int family, int type, int protocol);
void wrapped_connect(int SockDescriptor, const struct sockaddr * addr, socklen_t addrlen);
void wrapped_bind(int SockDescriptor, const struct sockaddr * addr, socklen_t addrlen);
void wrapped_listen(int SockDescriptor, int connNum);
int wrapped_accept(int SockDescriptor, const struct sockaddr * addr, socklen_t addrlen);


#endif 