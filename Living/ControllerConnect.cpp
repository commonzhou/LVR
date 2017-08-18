#include "ControllerConnect.h"
struct sockaddr_in server_addr;
#define bufferSize 4096
unsigned int _stdcall sendMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = NULL;
    SOCKET *socketID = NULL;
    if(node->messageList != NULL) {
        list = node->messageList;
    } 
    if (node->socketID != NULL) {
        socketID = node->socketID;
    }
    for (int i = 0; i < 5; i++) {           
        char *message = "Tamarous\n";
        if (send(*socketID, message, strlen(message), 0) == -1) {
            printf("Error sending: %s\n", __FUNCTION__);
            return -1;
        }
        printf("socketID:%d\t,index:%d:\tSend successfully.%s\n", *socketID,i,message);
            
    }
    
    return 0;
}

unsigned int _stdcall receiveMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;
    struct FlexibleTLV *flexibleTLV = NULL;

    int readSize = 0;
    char buffer[bufferSize] = {'0'};
    while((readSize = recv(*socketID, buffer, bufferSize, 0)) > 0) {
        buffer[readSize] = '\0';

        // ���Ƚ�����������Ϣ
        UINT8 *type = (UINT8 *)malloc(sizeof(UINT8)); 
        memcpy(type, buffer, sizeof(UINT8));
        if(*type == 0x01) {
            // Ȼ�����TLV�еĳ�����Ϣ
            UINT32 *length = (UINT32 *)malloc(sizeof(UINT32)); 
            memcpy(length, buffer + sizeof(UINT8), sizeof(UINT32));

            // ����InitPayload
            struct InitPayload *initPayload = (struct InitPayload*)malloc(sizeof(struct InitPayload));
            // ����memcpy֮��,initPayload֮��Ӧ�þ;�����Ӧ�е���Ϣ,Ȼ����ҪinitPayload֮�е�ָ���һ������ParamNode����Ϣ.
            memcpy(initPayload, buffer + sizeof(UINT8) + sizeof(UINT32), sizeof(struct InitPayload));

            // ����encNum��paramNum
            
        }
        puts(buffer);
        memset(buffer, 0, bufferSize);
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
    if ((*socket_port = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        return -1;
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = inet_addr(IP_SERVER);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portID);
    if (connect(*socket_port, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
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
    printf("socket id:%d, %s\n", *socket_port, __FUNCTION__);
    // �����activate_receiveӦ����ÿ���̵߳���һ����
    InfoNode *node = new InfoNode();
    
    if(pList != NULL) {
        node->messageList = pList;
    }
    if (socket_port != NULL) {
        node->socketID = socket_port;
    }

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
    CloseHandle(*receiver);
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
    printf("socket id:%d, %s\n", *socket_port, __FUNCTION__);
    InfoNode *node = new InfoNode();
    node->messageList = pList;
    node->socketID = socket_port;

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
    
    closesocket(*socket_port);
    CloseHandle(*send);
    WSACleanup();
    return 0;
}