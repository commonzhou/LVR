#include "ControllerConnect.h"

static void *sendMessage(void *params) {
    MessageList *pList = (MessageList *)params;
}

static void *receiveMessage(void *params) {
    MessageList *pList = (MessageList *)params;
}

//************************************
// Method:    create_socket
// FullName:  create_socket
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: char * IP_SERVER
// Parameter: int portID
// Parameter: void * privateSpace
// Parameter: SOCKET * socket_port
//************************************
int create_socket( char *IP_SERVER, int portID, void *privateSpace, SOCKET *socket_port )
{
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Initialised failed : %d\n",WSAGetLastError());
        return -1;
    }

    if ((*socket_port = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d\n",WSAGetLastError());
    }

    server.sin_addr.s_addr = inet_addr(IP_SERVER);
    server.sin_family = AF_INET;
    server.sin_port = htons(portID);


    if(connect(*socket_port, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connect Error\n");
        return -1;
    }
    return 0;
}


//************************************
// Method:    activate_receive
// FullName:  activate_receive
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: pthread_t * receiver
// Parameter: void * privateSpace
// Parameter: SOCKET * socket_port
// Parameter: MessageList * pList
//************************************
int activate_receive(pthread_t *receiver, void *privateSpace, SOCKET *socket_port, MessageList * pList) {
    int error = pthread_create(receiver,NULL,receiveMessage,(void *)pList);
    if(error != 0) {
        return -1;
    }

}

//************************************
// Method:    destroy_receive
// FullName:  destroy_receive
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: pthread_t * receiver
// Parameter: void * privateSpace
//************************************
int destroy_receive(pthread_t *receiver,void *privateSpace) {
    pthread_exit(receiver);
}

//************************************
// Method:    activate_send
// FullName:  activate_send
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: pthread_t * send
// Parameter: void * privateSpace
// Parameter: SOCKET * socket_port
// Parameter: MessageList * pList
//************************************
int activate_send(pthread_t *send, void *privateSpace, SOCKET *socket_port, MessageList *pList) {
    // TODO: 和发送绑定的函数是谁？
    int error = pthread_create(send, NULL, sendMessage, (void *)pList);
    if (error != 0) {
        return -1;
    }
}

//************************************
// Method:    destroy_send
// FullName:  destroy_send
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: pthread_t * send
// Parameter: void * privateSpace
// Parameter: SOCKET * socket_port
//************************************
int destroy_send(pthread_t *send, void *privateSpace, SOCKET *socket_port) {
    pthread_exit(send);
    closesocket(*socket_port);
    WSACleanup();
}