#ifndef TRANSCODERMANAGER_H
#define TRANSCODERMANAGER_H
#include "prefix.h"
#include "MessageManager.h"

struct sockaddr_in server,client;

int init_transcoderManager(pthread_t *transcoderManager, SOCKET *socketID,MessageList *pList,void *privateSpace);

int activate_transcoderManager(pthread_t *transcoderManager, SOCKET *socketID, MessageList *pList, void *privateSpace);

int destroy_transcoderManager(pthread_t *transcoderManager,void *privateSpace);
#endif