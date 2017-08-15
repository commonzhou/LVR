#ifndef CONTROLLERCONNECT_H
#define CONTROLLERCONNECT_H
#include "prefix.h"
#include "MessageManager.h"

WSAData wsa;
struct sockaddr_in server;


int create_socket(char *IP_SERVER, int portID, void *privateSpace, SOCKET *socket_port);

int activate_receive(pthread_t *receiver, void *privateSpace, SOCKET *socket_port, MessageList * pList);

int destroy_receive(pthread_t *receiver,void *privateSpace);

int activate_send(pthread_t *send, void *privateSpace, SOCKET *socket_port, MessageList *pList);

int destroy_send(pthread_t *send, void *privateSpace, SOCKET *socket_port);

#endif