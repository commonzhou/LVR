#include "TranscoderManager.h"
#include "MessageManager.h"
#define LENGTH_OF_LISTEN_QUEUE     20

unsigned int _stdcall handleMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;
    subMessageList *pRCL=list->pRCL;
    MessageNode *present=pRCL->present;
    int size=present->size;
    INT8 *CString=present->CString;
    printf("进入子线程\n");
    int readSize;
    char sendBuffer[2000];
    memset(sendBuffer,0,2000);
    memcpy(sendBuffer,CString,size);
    sendBuffer[size]= '\0';                                               //strlen
  

    char recvBuffer[2000]={0};
    while ((readSize = recv(*socketID, recvBuffer, 2000, 0)) > 0) {
        recvBuffer[readSize] = '\0';
        puts(recvBuffer);
        memset(recvBuffer,0,2000);
        if(send(*socketID , sendBuffer , strlen(sendBuffer) , 0)<0) {
            printf("Send failed:%d",WSAGetLastError());
            return -1;
        }
        printf("Send message successfully.\n");
    }
    closesocket(*socketID);
    CloseHandle(GetCurrentThread());
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
    length= sizeof(struct sockaddr_in);
    while(1){
        new_socket = accept(*socketID , (struct sockaddr *)&client, &length);
    
        if (new_socket <0)
        {
            printf("accept failed with error code : %d" , WSAGetLastError());
            return -1;
        }
        puts("Connection accepted");
        info->socketID=&new_socket;                            //very important
        printf("accept is:%d\n",new_socket);
        transcoderManager = (HANDLE *)malloc(sizeof(HANDLE));
        *transcoderManager = (HANDLE)_beginthreadex(NULL, 0, handleMessage, (void *)info,0, NULL);
        //WaitForSingleObject(*transcoderManager, 5000);
        if(*transcoderManager==0){
            printf("CreateThread failed (%d)\n", GetLastError());
            return -1;
        }
         printf("Create Thread Successfully: %s.\n",__FUNCTION__);
    }
    //************************************
    //Socket will be destroyed here
    //************************************
   closesocket(*socketID);
   WSACleanup();
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
    return 0;
}
int main(int argc , char *argv[]){
    WSADATA wsa;
    HANDLE transcoderManager=NULL;
    SOCKET socketID=NULL;

    MessageManager *messageManager = NULL;
    create_messageManager(messageManager, 0);
    MessageNode *node = NULL;
    create_messageNode(node);
    node->used_flag = 0;
    node->next = NULL;
    char *value = "hello";
    node->size = strlen(value)+1;
    node->CString = (INT8 *)malloc(sizeof(INT8)*(node->size));
    memcpy(node->CString, &value, node->size);
    add_messageNode(messageManager->pVHead->pRCL, node, NULL);

    node = NULL;
    create_messageNode(node);
    node->used_flag = 0;
    node->next = NULL;

    char val[] = "world";
    node->size =  sizeof(val);
    node->CString = (INT8 *)malloc(sizeof(INT8)*node->size);
    memcpy(node->CString, &val, strlen(val)+1);
    add_messageNode(messageManager->pVHead->pRCL, node, NULL);
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
    //for(int i=0;i<size;i++){
    //  printf("%d\n",buffer[i]);
    //}

    init_transcoderManager( &transcoderManager, &socketID,messageManager->pVHead, NULL);
    //WaitForSingleObject(transcoderManager, 1000);
    //
   /* while(flags){
           destroy_transcoderManager( &transcoderManager,NULL );
    }*/
    
    return 0;
}

