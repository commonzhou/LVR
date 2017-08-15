# 问题汇集

1. TranscoderManager既要接收Controller的消息，又要给Controller发送消息，那么这个Socket到底该怎么写？

2. 

    init_transcoderManager(pthread_t *transcoderManager, SOCKET *socketID, MessageList *pList ,void *privateSpace)
