#include "TranscoderManager.h"
#include "MessageManager.h"
#include <iostream>  
#include <string>  
#include <vector>  
#define LENGTH_OF_LISTEN_QUEUE     20
using namespace std;  
int threadNum=0;
unsigned int _stdcall handleMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;
    subMessageList *pRCL=list->pRCL;
    MessageNode *present=pRCL->present;
    int size=present->size;
    INT8 *CString=present->CString;
    printf("enter the son thread\n");
    int readSize;
    char sendBuffer[2000];
    memset(sendBuffer,0,2000);
    memcpy(sendBuffer,CString,size);
    sendBuffer[size]= '\0';                                               //strlen
    char recvBuffer[2000]={0};
    while ((readSize = recv(*socketID, recvBuffer, 2000, 0)) > 0) {
        recvBuffer[readSize] = '\0';
        puts(recvBuffer);
        //////////////////////////////////                     parse the recv
        std::vector<char*> splits;
        char delims[] = "|";
        char *result = NULL;
        result = strtok( recvBuffer, delims );
        while( result != NULL ) {
            printf( "result is %s\n", result );
            splits.push_back(result);
            result = strtok( NULL, delims );     
        }    
        printf("%d\n",splits.at(0));
        UINT8 typeof;UINT32 lengthof;UINT8 encNumof;  
        typeof=(UINT8)strtol (splits.at(0),NULL,16);
        lengthof=(UINT32)strtol(splits.at(1),NULL,16);
        printf("type is %d\n",typeof);
        if (typeof == 0x01) {
            UINT8 encNum = (UINT8)strtol(splits.at(2), NULL, 16);
            printf("encNum is %d\n", encNum);
            int offset = 0;
            int index = 3;
            for (int i = 1; i <= encNum; i++) {
                UINT8 paramNum = (UINT8)strtol(splits.at(index), NULL, 16);
                printf("\tparamNum is %d\n", paramNum);
                for (int j = 1; j <= paramNum; j++) {
                    UINT8 paramType = (UINT8)strtol(splits.at(index + 2*j-1), NULL, 16);
                    UINT32 valueof = (UINT32)strtol(splits.at(index + j * 2), NULL, 16);
                    printf("\t\tparamType is %x, ", paramType);
                    printf("value is %x\n", valueof);
                }
                offset = paramNum;        
                index = index + offset*2 + 1;
            }
        } else if (typeof == 0x41) {

        } else if (typeof == 0x02) {

        } else if (typeof == 0x42) {

        } else if (typeof == 0x43) {

        } else if (typeof == 0x04) {

        }
        else{
            printf("This is not TLV information\n");
        } 
  
        memset(recvBuffer,0,2000);
        if(send(*socketID , sendBuffer , strlen(sendBuffer)+1 , 0)<0) {         //plus 1 makes the last 0 could be sent to client
            printf("Send failed:%d",WSAGetLastError());
            return -1;
        }
        printf("Send message successfully.\n");
    }
    closesocket(*socketID);
    printf("Will return from %s\n",__FUNCTION__);
    return 0;
}

//************************************
// Method: 利用端口信息，初始化transcoderManager线程，线程suspend
// FullName: init_transcoderManager
// Access: public
// Returns: int -1失败 0成功
// Qualifier:
// Parameter: pthread_t * transcoderManager
// Parameter: SOCKET * socketID 主控PC和转码节点间TCP/IP连接链路
// Parameter: MessageList * pList 需要更新其中的码控反馈信息和编码信息进度列表
// Parameter: void * private 线程私有空间
//************************************

int init_transcoderManager( HANDLE *transcoderManager, SOCKET *socketID,MessageList *pList,void *privateSpace ) {
    InfoNode *info = new InfoNode();
    info->messageList = pList;
    info->socketID = socketID;
    SOCKET new_socket;
    int length;

    if((*socketID = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(7747);
    if( bind(*socketID ,(struct sockaddr *)&server , sizeof(server))==SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        return -1;
    }
    if (listen(*socketID, LENGTH_OF_LISTEN_QUEUE))
    {
        printf("Server Listen Failed : %d\n" , WSAGetLastError());
        return -1;
    }
    printf("Welcome ,intial success\n");
    length= sizeof(struct sockaddr_in);
    timeval timeout = {600, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(*socketID, &fds);
    int b=select(*socketID+1, &fds, NULL, NULL, &timeout);
    if(select(*socketID+1, &fds, NULL, NULL, &timeout)==SOCKET_ERROR){
        printf("select failed with error code : %d" , WSAGetLastError());
        return -1;
    };
    printf("You have %d handle\n",b);

    if (FD_ISSET(*socketID, &fds))
    {
        while(1){
        new_socket = accept(*socketID , (struct sockaddr *)&client, &length);

        if (new_socket <0)
        {
            printf("accept failed with error code : %d" , WSAGetLastError());
            return -1;
        }
        threadNum++;
        puts("Connection accepted");
        info->socketID=&new_socket;                            //very important
        printf("accept is:%d\n",new_socket);
        transcoderManager = (HANDLE *)malloc(sizeof(HANDLE));
        *transcoderManager = (HANDLE)_beginthreadex(NULL, 0, handleMessage, (void *)info,0, NULL);
        if(*transcoderManager==0){
            printf("CreateThread failed (%d)\n", GetLastError());
            return -1;
        }
        printf("Create %d Thread Successfully:%s.\n",a,__FUNCTION__);
        }
    }
    closesocket(*socketID);
    delete(info);
    info=NULL;
    printf("Will return from %s\n", __FUNCTION__);
    return 0;
}


//************************************
// Method: 线程从挂起状态唤醒
// FullName: activate_transcoderManager
// Access: public
// Returns: int -1失败 0成功
// Qualifier:
// Parameter: pthread_t * transcoderManager 线程NULL指针
// Parameter: SOCKET * socketID 主控PC和转码节点间TCP/IP连接链路
// Parameter: MessageList * pList 需要更新其中的码控反馈信息和编码信息进度列表
// Parameter: void * private 线程私有空间
//************************************

int activate_transcoderManager(HANDLE *transcoderManager, SOCKET *socketID, MessageList *pList, void *privateSpace) {
    if(ResumeThread(*transcoderManager)==-1){
        return -1;
    };
    return 0;
}


//************************************
// Method: 线程销毁
// FullName: destroy_transcoderManager
// Access: public
// Returns: int -1失败 0成功
// Qualifier:
// Parameter: pthread_t * transcoderManager 线程句柄
// Parameter: void * private
//************************************

int destroy_transcoderManager( HANDLE *transcoderManager,void *privateSpace ) {
    if(CloseHandle(*transcoderManager)==0){
        return -1;
    };
    WSACleanup();
    return 0;
}
