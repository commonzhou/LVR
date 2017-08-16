#include "ControllerConnect.h"
struct sockaddr_in server;


unsigned int _stdcall sendMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;

    // UNDONE: ��ʱ�õ��Ǽ�����
    char *message = "Hello, world\n";
    if (send(*socketID, message, strlen(message), 0) == -1) {
        printf("Error sending: %s\n", __FUNCTION__);
        pthread_exit(NULL);
    }

    // UNDONE: ��δ�MessageList���ɱ��������Ϣ, ��ز�����
    return;
}

unsigned int _stdcall receiveMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;

    int readSize;
    char *buffer = new char[sizeof(TLV)];
    struct TLV * receivedTLV = new TLV();
    while ((readSize = recv(*socketID, buffer, sizeof(TLV), 0)) > 0) {
        memcpy(&receivedTLV, buffer, sizeof(TLV));
        // TODO: ���յ�����Ϣ���н���
    }

    return 0;
}

//************************************
// Method:    ���������˿ڣ�������TCP/IP���ӣ����ض˿��׽��֣��������̹߳���
// FullName:  create_socket
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: char * IP_SERVER ���ض�IP
// Parameter: int portID ���ض˼����˿�
// Parameter: void * privateSpace ���߳�˽�пռ�
// Parameter: SOCKET * socket_port TCP/IP�����׽���
//************************************
int create_socket(char *IP_SERVER, int portID, void *privateSpace, SOCKET *socket_port) {
    WSAData wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Initialised failed : %d\n", WSAGetLastError());
        return -1;
    }
    socket_port = (SOCKET *)malloc(sizeof(SOCKET));
    if ((*socket_port = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
    }

    server.sin_addr.s_addr = inet_addr(IP_SERVER);
    server.sin_family = AF_INET;
    server.sin_port = htons(portID);
    if (connect(*socket_port, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connect Error\n");
        return -1;
    }

    return 0;
}


//************************************
// Method:    activate_receive
// FullName:  activate_receive
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread_t * receiver
// Parameter: void * privateSpace
// Parameter: SOCKET * socket_port
// Parameter: MessageList * pList
//************************************
int activate_receive(HANDLE *receiver, void *privateSpace, SOCKET *socket_port, MessageList * pList) {

    InfoNode *node = new InfoNode();
    node->messageList = pList;
    node->socketID = socket_port;

    receiver = (HANDLE *)malloc(sizeof(HANDLE));
    *receiver = (HANDLE)_beginthreadex(NULL, 0, receiveMessage, (void *)node, 0, NULL);

    return 0;
}

//************************************
// Method:    destroy_receive
// FullName:  destroy_receive
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread_t * receiver
// Parameter: void * privateSpace
//************************************
int destroy_receive(HANDLE *receiver, void *privateSpace) {
    pthread_exit(privateSpace);
    return 0;
}

//************************************
// Method:    activate_send
// FullName:  activate_send
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread_t * send
// Parameter: void * privateSpace
// Parameter: SOCKET * socket_port
// Parameter: MessageList * pList
//************************************
int activate_send(HANDLE *send, void *privateSpace, SOCKET *socket_port, MessageList *pList) {

    InfoNode *node = new InfoNode();
    node->messageList = pList;
    node->socketID = socket_port;


    send = (HANDLE *)malloc(sizeof(HANDLE));
    *send = (HANDLE)_beginthreadex(NULL, 0, sendMessage, (void *)node, 0, NULL);

    // TODO: �������˽������
    return 0;
}

//************************************
// Method:    destroy_send
// FullName:  destroy_send
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread_t * send
// Parameter: void * privateSpace
// Parameter: SOCKET * socket_port
//************************************
int destroy_send(HANDLE *send, void *privateSpace, SOCKET *socket_port) {
    pthread_exit(send);
    closesocket(*socket_port);
    WSACleanup();
    return 0;
}