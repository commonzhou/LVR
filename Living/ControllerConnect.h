#ifndef CONTROLLERCONNECT_H
#define CONTROLLERCONNECT_H
#include "prefix.h"
#include "MessageManager.h"


int create_socket(char *IP_SERVER, int portID, void *privateSpace, SOCKET *socket_port);


int activate_receive(HANDLE *receiver, void *privateSpace, SOCKET *socket_port, MessageList * pList);

int destroy_receive( HANDLE *receiver,void *privateSpace);

int activate_send( HANDLE *send, void *privateSpace, SOCKET *socket_port, MessageList *pList);

int destroy_send( HANDLE *send, void *privateSpace, SOCKET *socket_port);

#endif