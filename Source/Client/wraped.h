//
// Created by Simone Cioffi on 20/10/21.
//

#ifndef DAY_WRAPED_H
#define DAY_WRAPED_H

#include <sys/socket.h>

int Socket(int family, int type, int protocol);
int Connect(int ciao, const struct sockaddr *protocol, socklen_t socket);
int Bind(int file, const struct sockaddr *address, socklen_t socket);
int Listen(int sockid, int lung);
int Accept(int sockfd, struct sockaddr *clientaddr, socklen_t *addr_dim);

#endif //DAY_WRAPED_H
