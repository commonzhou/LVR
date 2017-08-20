#include "ControllerConnect.h"
#include "catch.hpp"
static struct sockaddr_in server_addr;
#define bufferSize 4096
static HANDLE hMutex;

unsigned int _stdcall sendFakeMessage(void *params);

unsigned int _stdcall sendRealMessage(void *params);

unsigned int _stdcall receiveFakeMessage(void *params);

unsigned int _stdcall receiveRealMessage(void *params);


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
    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        printf("CreateMutex error in %s\n", __FUNCTION__);
    }
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
        closesocket(*socket_port);
        return -1;
    }
    return 0;
}

//************************************
// Method:    ���������̣߳������˿ڣ������յ�����Ϣ������Ϣ�б�
// FullName:  activate_receive
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread_t * receiver �߳̾��
// Parameter: void * privateSpace �߳�˽�пռ�
// Parameter: SOCKET * socket_port �׽���
// Parameter: MessageList * pList ������Ϣ����
//************************************
int activate_receive(HANDLE *receiver, void *privateSpace, SOCKET *socket_port, MessageList * pList) {
    printf("socket id:%d, %s\n", *socket_port, __FUNCTION__);
    // �����activate_receiveӦ����ÿ���̵߳���һ����
    InfoNode inode;
    inode.messageList = pList;
    inode.socketID = socket_port;

    *receiver = (HANDLE)_beginthreadex(NULL, 0, receiveRealMessage, (void *)&inode, 0, NULL);
    return 0;
}

//************************************
// Method:    �߳����٣��ͷ�˽�пռ䣬�رն˿�
// FullName:  destroy_receive
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread_t * receiver �߳̾��
// Parameter: void * privateSpace �߳�˽�пռ�
//************************************
int destroy_receive(HANDLE *receiver, void *privateSpace) {
    CloseHandle(*receiver);
    return 0;
}

//************************************
// Method:    �̴߳ӹ���״̬���ѣ����Ҫ���͵������������ո�ʽ����
// FullName:  activate_send
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread_t * send �߳̾��
// Parameter: void * privateSpace �߳�˽�пռ�
// Parameter: SOCKET * socket_port �׽���
// Parameter: MessageList * pList ������Ϣ��Դ
//************************************
int activate_send(HANDLE *send, void *privateSpace, SOCKET *socket_port, MessageList *pList) {
    printf("socket id:%d, %s\n", *socket_port, __FUNCTION__);
    InfoNode inode;
    inode.messageList = pList;
    inode.socketID = socket_port;

    *send = (HANDLE)_beginthreadex(NULL, 0, sendRealMessage, (void *)&inode, 0, NULL);

    return 0;
}

//************************************
// Method:    �߳����٣��ͷ�˽�пռ䣬�ر��׽���
// FullName:  destroy_send
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread_t * send �߳̾��
// Parameter: void * privateSpace �߳�˽�пռ�
// Parameter: SOCKET * socket_port �׽���
//************************************
int destroy_send(HANDLE *send, void *privateSpace, SOCKET *socket_port) {
    closesocket(*socket_port);
    CloseHandle(*send);
    CloseHandle(hMutex);
    WSACleanup();
    return 0;
}


unsigned int _stdcall sendFakeMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = NULL;
    SOCKET *socketID = NULL;
    list = node->messageList;
    socketID = node->socketID;

    for (int i = 0; i < 1; i++) {
        char *message = "Tamarous\n";
        if (send(*socketID, message, strlen(message), 0) == -1) {
            printf("Error sending: %s\n", __FUNCTION__);
            return -1;
        }
        printf("socketID:%d\t,index:%d:\tSend successfully.%s\n", *socketID, i, message);
    }
    
    return 0;
}

unsigned int _stdcall sendRealMessage(void *params) {
    WaitForSingleObject(hMutex, INFINITE);
    InfoNode *node = (InfoNode *)params;
    subMessageList *subMessageList = node->messageList->pRCL;
    SOCKET *socketID = node->socketID;
    MessageNode *messageNode = NULL;
    get_messageNode(messageNode, subMessageList);
    if (messageNode->used_flag == 0) {
        if (send(*socketID, (char *)messageNode->CString, messageNode->size, 0) == -1) {
            printf("Error sending: %s\n", __FUNCTION__);
            return -1;
        }
        printf("%s Send %s successfully.\n", __FUNCTION__, (char *)messageNode->CString);
        messageNode->used_flag = 1;
        update_messageNode(subMessageList, &hMutex);
    }
    ReleaseMutex(hMutex);
    return 0;
}


unsigned int _stdcall receiveFakeMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;

    int readSize = 0;
    char buffer[SOCKETBUFFERSIZE] = { 0 };
    while ((readSize = recv(*socketID, buffer, SOCKETBUFFERSIZE, 0)) > 0) {
        buffer[readSize] = '\0';
        puts(buffer);
        memset(buffer, 0, SOCKETBUFFERSIZE);
    }
    
    return 0;
}

unsigned int _stdcall receiveRealMessage(void *params) {
    WaitForSingleObject(hMutex, INFINITE);
    InfoNode *node = (InfoNode *)params;
    SOCKET *socketID = node->socketID;
    MessageNode *messageNode = NULL;
    char buffer[SOCKETBUFFERSIZE] = { 0 };
    int readSize = 0;
    UINT8 *type = (UINT8 *)malloc(sizeof(UINT8));
    UINT32 *length = (UINT32 *)malloc(sizeof(UINT32));
    while ((readSize = recv(*socketID, buffer, SOCKETBUFFERSIZE, 0)) > 0) {

        buffer[readSize] = '\0';
        puts(buffer);

        // ������type��Ϣ
        memcpy(type, buffer, sizeof(UINT8));

        // ������length��Ϣ
        memcpy(length, buffer + sizeof(UINT8), sizeof(UINT32));

        // ����lengthΪnode�����ܹ�������size, used_flag, MessageNode *���ַ����Ŀռ�
        // �ַ����洢����һ��ʵ�ʵ�TLV, ����������:type��length��payload, type�ĳ���Ϊsizeof(UINT8), length ����Ϊsizeof(UINT32), payload����Ϊlength+��ʾ�ַ�����ֹ���ŵ�1�ֽ�
        messageNode = (MessageNode *)malloc(sizeof(int) + sizeof(int) + sizeof(MessageNode *) + (*length+1) + sizeof(UINT8) + sizeof(UINT32));
        memcpy(messageNode->CString, buffer + sizeof(UINT8) + sizeof(UINT32), *length);
        messageNode->CString[*length] = '\0';
        printf("%s Recv %s successfully.\n", __FUNCTION__, (char *)messageNode->CString);
        messageNode->size = *length;
        messageNode->used_flag = 0;
        messageNode->next = NULL;

        // �������ͽ�node���뵽��Ӧ����Ϣ������
        if (*type == 0x01) {
            subMessageList *subMessageList = node->messageList->pRCL;
            add_messageNode(subMessageList, messageNode, &hMutex);
        }
    }
    free(type);
    free(length);
    ReleaseMutex(hMutex);
    delete node;
    return 0;
}

TEST_CASE("ControllerConnect","[ControllerConnect]") {
    char *IP = "127.0.0.1";
    int portID = 1453;
    SOCKET socket;
    SECTION("create_socket") {
        create_socket(IP, portID, NULL, &socket);
        REQUIRE(socket != INVALID_SOCKET);
    }
}