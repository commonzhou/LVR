#include "ControllerConnect.h"
#include "catch.hpp"
static struct sockaddr_in server_addr;
#define bufferSize 4096
static HANDLE hMutex;

// TODO: 不太确定我这种互斥锁的使用会不会出问题
// 发送消息的各个线程函数只是读取，因此不需要使用互斥锁，而接收消息的线程函数因为需要将新数据写入
// 链表结构，因此需要用一把锁来保持数据区域，而这把锁应该为所有接收消息的线程所共享

unsigned int _stdcall sendFakeMessage(void *params);

unsigned int _stdcall sendRealMessage(void *params);

unsigned int _stdcall receiveFakeMessage(void *params);

unsigned int _stdcall receiveRealMessage(void *params);


//************************************
// Method:    创建监听端口，当建立TCP/IP连接，返回端口套接字，供发送线程共享
// FullName:  create_socket
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: char * IP_SERVER 主控端IP
// Parameter: int portID 主控端监听端口
// Parameter: void * privateSpace 本线程私有空间
// Parameter: SOCKET * socket_port TCP/IP连接套接字
//************************************
int create_socket(char *IP_SERVER, int portID, void *privateSpace, SOCKET *socket_port) {
    //LPCTSTR name = L"controllerConnect";
    //hMutex = CreateMutex(NULL, FALSE, name);
    hMutex = CreateMutex(NULL, FALSE, NULL);
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
// Method:    创建接收线程，侦听端口，将接收到的消息放入消息列表
// FullName:  activate_receive
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: pthread_t * receiver 线程句柄
// Parameter: void * privateSpace 线程私有空间
// Parameter: SOCKET * socket_port 套接字
// Parameter: MessageList * pList 接收消息链表
//************************************
int activate_receive(HANDLE *receiver, void *privateSpace, SOCKET *socket_port, MessageList * pList) {

    printf("socket id:%d, %s\n", *socket_port, __FUNCTION__);
    // 这里的activate_receive应该是每个线程单独一个吧
    InfoNode *iNode = new InfoNode();
    iNode->messageList = pList;
    iNode->socketID = socket_port;

    *receiver = (HANDLE)_beginthreadex(NULL, 0, receiveFakeMessage, (void *)iNode, 0, NULL);
    return 0;
}

//************************************
// Method:    线程销毁，释放私有空间，关闭端口
// FullName:  destroy_receive
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: pthread_t * receiver 线程句柄
// Parameter: void * privateSpace 线程私有空间
//************************************
int destroy_receive(HANDLE *receiver, void *privateSpace) {
    CloseHandle(*receiver);
    return 0;
}

//************************************
// Method:    线程从挂起状态唤醒，检测要发送的数据链表并按照格式发送
// FullName:  activate_send
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: pthread_t * send 线程句柄
// Parameter: void * privateSpace 线程私有空间
// Parameter: SOCKET * socket_port 套接字
// Parameter: MessageList * pList 发送信息来源
//************************************
int activate_send(HANDLE *send, void *privateSpace, SOCKET *socket_port, MessageList *pList) {
    printf("socket id:%d, %s\n", *socket_port, __FUNCTION__);
    InfoNode *iNode = new InfoNode();
    iNode->messageList = pList;
    iNode->socketID = socket_port;
    *send = (HANDLE)_beginthreadex(NULL, 0, sendFakeMessage, (void *)iNode, 0, NULL);
    return 0;
}

//************************************
// Method:    线程销毁，释放私有空间，关闭套接字
// FullName:  destroy_send
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: pthread_t * send 线程句柄
// Parameter: void * privateSpace 线程私有空间
// Parameter: SOCKET * socket_port 套接字
//************************************
int destroy_send(HANDLE *send, void *privateSpace, SOCKET *socket_port) {
    closesocket(*socket_port);
    CloseHandle(hMutex);
    CloseHandle(*send);
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
    delete node;
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
    delete node;
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
    delete node;
    return 0;
}


unsigned int _stdcall receiveRealMessage(void *params) {
    
    InfoNode *node = (InfoNode *)params;
    SOCKET *socketID = node->socketID;
    MessageList *list = node->messageList;
    MessageNode *messageNode = NULL;
    UINT8 *type = (UINT8 *)malloc(sizeof(UINT8));
    UINT32 *length = (UINT32 *)malloc(sizeof(UINT32));

    char buffer[SOCKETBUFFERSIZE] = { 0 };
    int readSize = 0;
    
    while ((readSize = recv(*socketID, buffer, SOCKETBUFFERSIZE, 0)) > 0) {


        buffer[readSize] = '\0';
        puts(buffer);

        // 解析出type信息
        memcpy(type, buffer, sizeof(UINT8));

        // 解析出length信息
        memcpy(length, buffer + sizeof(UINT8), sizeof(UINT32));

        // 根据length为node分配能够容纳下size, used_flag, MessageNode *和字符串的空间
        // 字符串存储的是一个实际的TLV, 包括三方面:type，length和payload, type的长度为sizeof(UINT8), length 长度为sizeof(UINT32), payload长度为length+表示字符串终止符号的1字节
        messageNode = (MessageNode *)malloc(sizeof(int) + sizeof(int) + sizeof(MessageNode *) + (*length+1) + sizeof(UINT8) + sizeof(UINT32));
        memcpy(messageNode->CString, buffer + sizeof(UINT8) + sizeof(UINT32), *length);
        messageNode->CString[*length] = '\0';
        printf("%s Recv %s successfully.\n", __FUNCTION__, (char *)messageNode->CString);
        messageNode->size = *length;
        messageNode->used_flag = 0;
        messageNode->next = NULL;

        // 根据类型将node加入到对应的消息队列中
        // TODO: 目前对于0x01(编码参数信息)和0x04(终止编码)信息该放入MessageList中的哪个链表没有明确定义
        WaitForSingleObject(hMutex, INFINITE);
        switch(*type) {
        case 0x01: // VRLSController发送VRLSTranscoder的初始化编码参数信息
            
            add_messageNode(list->pRCL, messageNode, &hMutex);
            break;
        case 0x02: // VRLSController发送VRLSTranscoder的码控参数
            
            add_messageNode(list->pRRCL, messageNode, &hMutex);
            break;
        case 0x04: // VRLSController发送终止编码         
            add_messageNode(list->pRCL, messageNode, &hMutex);
            break;
        default:
            printf("Wrong Message Type:%s\n", __FUNCTION__);
            break;
        }

        ReleaseMutex(hMutex);
    }
    free(type);
    free(length);
    delete node;
    return 0;
}

//TEST_CASE("ControllerConnect","[ControllerConnect]") {
//    char *IP = "127.0.0.1";
//    int portID = 1453;
//    SOCKET socket;
//    SECTION("create_socket") {
//        create_socket(IP, portID, NULL, &socket);
//        REQUIRE(socket != INVALID_SOCKET);
//    }
//}