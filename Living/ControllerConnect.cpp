#include "ControllerConnect.h"
#include "catch.hpp"
#include <vector>
static struct sockaddr_in server_addr;
#define bufferSize 4096
static HANDLE hMutex;

// TODO: 不太确定互斥锁的使用会不会出问题
// 发送消息的各个线程函数只是读取，因此不需要使用互斥锁，而接收消息的线程函数因为需要将新数据写入
// 链表结构，因此需要用一把锁来保持数据区域，而这把锁应该为所有接收消息的线程所共享
unsigned int _stdcall sendRealMessage(void *params);
unsigned int _stdcall receiveRealMessage(void *params);


/**
 * \brief 创建监听端口，当建立TCP/IP连接，返回端口套接字，供发送线程共享
 * \param IP_SERVER 主控端IP
 * \param portID 主控端监听端口
 * \param privateSpace 本线程私有空间
 * \param socket_port TCP/IP连接套接字
 * \return int -1失败 0成功
 */
int create_socket(char *IP_SERVER, int portID, void *privateSpace, SOCKET *socket_port) {
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

/**
 * \brief 创建接收线程，侦听端口，将接收到的消息放入消息列表
 * \param receiver 线程句柄
 * \param privateSpace 线程私有空间
 * \param socket_port 套接字
 * \param pList 接收消息链表
 * \return int -1失败 0成功
 */
int activate_receive(HANDLE *receiver, void *privateSpace, SOCKET *socket_port, MessageList * pList) {
    InfoNode *iNode = new InfoNode();
    iNode->messageList = pList;
    iNode->socketID = socket_port;

    *receiver = (HANDLE)_beginthreadex(NULL, 0, receiveRealMessage, (void *)iNode, 0, NULL);
    return 0;
}


/**
 * \brief 线程销毁，释放私有空间，关闭端口
 * \param receiver 线程句柄
 * \param privateSpace 线程私有空间
 * \return int -1失败 0成功
 */
int destroy_receive(HANDLE *receiver, void *privateSpace) {
    CloseHandle(*receiver);
    return 0;
}

/**
 * \brief 线程从挂起状态唤醒，检测要发送的数据链表并按照格式发送
 * \param send 线程句柄
 * \param privateSpace 线程私有空间
 * \param socket_port 套接字
 * \param pList 发送信息来源
 * \return int -1失败 0成功
 */
int activate_send(HANDLE *send, void *privateSpace, SOCKET *socket_port, MessageList *pList) {
    InfoNode *iNode = new InfoNode();
    iNode->messageList = pList;
    iNode->socketID = socket_port;

    *send = (HANDLE)_beginthreadex(NULL, 0, sendRealMessage, (void *)iNode, 0, NULL);
    return 0;
}


/**
 * \brief 线程销毁，释放私有空间，关闭套接字
 * \param send 线程句柄
 * \param privateSpace 线程私有空间
 * \param socket_port 套接字
 * \return int -1失败 0成功
 */
int destroy_send(HANDLE *send, void *privateSpace, SOCKET *socket_port) {
    closesocket(*socket_port);
    if (hMutex != INVALID_HANDLE_VALUE) {
        CloseHandle(hMutex);
    }
    CloseHandle(*send);
    WSACleanup();
    return 0;
}



unsigned int _stdcall sendRealMessage(void *params) {
    WaitForSingleObject(hMutex, INFINITE);
    InfoNode *node = (InfoNode *)params;
    subMessageList *subMessageList = node->messageList->pRCL;
    SOCKET *socketID = node->socketID;

    // TODO:此处尚不明确消息节点应该由本模块生成还是由其他模块生成，暂且认为是由其他模块生成，直接使用即可
    MessageNode *messageNode = new MessageNode();
    messageNode->used_flag = 0;
    char *string = "0x01|000000AF|04|0A|01|00000001|02|00000003|05|0000001E|06|0000001E|07|00000001|0c|01210121|0d|01210012|fd|00000002|fe|00013498|ff|0000BB80|0A|01|00000002|02|00000000|04|000F4240|05|0000001E|06|0000001E|07|00000001|08|00000f00|09|00000780|0c|01210121|0d|01210012|0D|01|00000003|02|00000002|03|00000000|04|000186A0|05|0000001E|06|0000001E|07|00000001|08|00000140|09|00000140|0A|00000000|0B|00000000|0c|01210121|0d|01210012|0D|01|00000004|02|00000002|03|00000001|04|000186A0|05|0000001E|06|0000001E|07|00000001|08|00000140|09|00000140|0A|00000500|0B|00000140|0c|01210121|0d|01210012|";
    int size = strlen(string) + 1;
    messageNode->size = size;
    messageNode->CString = (INT8 *)malloc(sizeof(INT8)*size);
    memcpy(messageNode->CString, string, size);
    if (messageNode->used_flag == 0) {
        if (send(*socketID, (char *)messageNode->CString, messageNode->size, 0) == -1) {
            printf("Error sending: %s\n", __FUNCTION__);
            return -1;
        }
        printf("Send successfully: %s\n", (char *)messageNode->CString);
        messageNode->used_flag = 1;
        //update_messageNode(subMessageList, &hMutex);
    }
    ReleaseMutex(hMutex);
    delete node;
    return 0;
}

unsigned int _stdcall receiveRealMessage(void *params) {
    
    InfoNode *node = (InfoNode *)params;
    SOCKET *socketID = node->socketID;
    MessageList *list = node->messageList;
    MessageNode *messageNode = NULL;
    
    char buffer[SOCKETBUFFERSIZE] = { 0 };
    int readSize = 0;
    std::vector<char *> results;

    while ((readSize = recv(*socketID, buffer, SOCKETBUFFERSIZE, 0)) > 0) {
        buffer[readSize] = '\0';

        char *delim = "|";
        char *result = NULL;
        result = strtok(buffer, delim); // type
        results.push_back(result);
        while(result != NULL) {
            result = strtok(NULL, delim);
            results.push_back(result);
        }
        
        char *str = results.at(0);
        UINT8 type = (UINT8)strtol(results.at(0), NULL, 16);
        UINT32 length = (UINT32)strtol(results.at(1), NULL, 16);

        create_messageNode(messageNode);
        messageNode->size = length;
        messageNode->used_flag = 0;
        messageNode->CString = (INT8 *)malloc(sizeof(INT8) * (readSize+1));
        memcpy(messageNode->CString, buffer, readSize + 1);
        messageNode->next = NULL;


        // 根据类型将node加入到对应的消息队列中
        // TODO: 目前对于0x01(编码参数信息)和0x04(终止编码)信息该放入MessageList中的哪个链表没有明确定义
        if (type == 0x01) {
            UINT8 encNum = (UINT8)strtol(results.at(2), NULL, 16);
            printf("encNum is %d\n", encNum);
            int offset = 0;
            int index = 3;
            for (int i = 1; i <= encNum; i++) {
                UINT8 paramNum = (UINT8)strtol(results.at(index), NULL, 16);
                printf("\tparamNum is %d\n", paramNum);
                for (int j = 1; j <= paramNum; j++) {
                    struct Param param;
                    param.paramType = (UINT8)strtol(results.at(index + 2*j-1), NULL, 16);
                    param.value = (UINT32)strtol(results.at(index + j * 2), NULL, 16);
                    printf("\t\tparamType is %x, ", param.paramType);
                    printf("value is %x\n", param.value);
                }
                offset = paramNum;        
                index = index + offset*2 + 1;
            }
        } else if (type == 0x41) {
            
        } else if (type == 0x02) {
            
        } else if (type == 0x42) {
            
        } else if (type == 0x43) {
            
        } else if (type == 0x04) {
            
        }
        ReleaseMutex(hMutex);
    }

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