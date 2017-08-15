#ifndef TRANSCODERMANAGER_H
#define TRANSCODERMANAGER_H
#include "prefix.h"
int init_transcoderManager(pthread_t *transcoderManager, SOCKET *socketID,MessageList *pList,void *private);

int activate_transcoderManager(pthread_t *transcoderManager, SOCKET *socketID, MessageList *pList, void *private);

int destroy_transcoderManager(pthread_t *transcoderManager,void *private);
#endif