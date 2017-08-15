#include "ControllerConnect.h"

static void *sendMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;

    // TODO: Some fake messages;
    char *message = "Hello, world\n";
    if (send(*socketID,message,strlen(message),0) == -1) {
         printf("Error sending: %s\n",__FUNCTION__);
         pthread_exit(NULL);
    }
}

static void *receiveMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;

    int recvSize;
    char server_reply[200];
    if ((recvSize = recv(*socketID,server_reply,200,0)) == SOCKET_ERROR) {
        printf("Error receiving: %s\n",__FUNCTION__);
        pthread_exit(NULL);
    }
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
    socket_port = (SOCKET *)malloc(sizeof(SOCKET)); 
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
    pthread_exit(privateSpace);
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
    
    InfoNode *node = new InfoNode();
    node->messageList = pList;
    node->socketID = socket_port;
    
    int error = pthread_create(send, NULL, sendMessage, (void *)node);
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