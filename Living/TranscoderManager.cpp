#include "TranscoderManager.h"
#define LENGTH_OF_LISTEN_QUEUE     20


unsigned int _stdcall handleMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;
    //char *message = thisline->contents;
    int readSize;
    char buffer[2000];
    //    send(*socketID , message , strlen(message) , 0);
    while ((readSize = recv(*socketID, buffer, 2000, 0)) > 0) {
        buffer[readSize] = '\0';
        puts(buffer);
        memset(buffer,0,2000);
    }
    return 0;
}

//************************************
// Method:    ���ö˿���Ϣ����ʼ��transcoderManager�̣߳��߳�suspend
// FullName:  init_transcoderManager
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread_t * transcoderManager
// Parameter: SOCKET * socketID ����PC��ת��ڵ��TCP/IP������·
// Parameter: MessageList * pList ��Ҫ�������е���ط�����Ϣ�ͱ�����Ϣ�����б�
// Parameter: void * private �߳�˽�пռ�
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
        info->socketID =&new_socket;
        puts("Connection accepted");

        printf("accept is:%d\n",new_socket);
        transcoderManager = (HANDLE *)malloc(sizeof(HANDLE));
        *transcoderManager = (HANDLE)_beginthreadex(NULL, 0, handleMessage, (void *)info,CREATE_SUSPENDED, NULL);
        if(*transcoderManager==0){
            printf("CreateThread failed (%d)\n", GetLastError());
            return -1;
        }
        ResumeThread(*transcoderManager);
        CloseHandle(*transcoderManager);
    }
    //************************************
    //Socket will be destroyed here
    //************************************
    closesocket(*socketID);
    WSACleanup();
    return 0;
}


//************************************
// Method:    �̴߳ӹ���״̬����
// FullName:  activate_transcoderManager
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread_t * transcoderManager  �߳�NULLָ��
// Parameter: SOCKET * socketID ����PC��ת��ڵ��TCP/IP������·
// Parameter: MessageList * pList ��Ҫ�������е���ط�����Ϣ�ͱ�����Ϣ�����б�
// Parameter: void * private �߳�˽�пռ�
//************************************

int activate_transcoderManager(HANDLE *transcoderManager, SOCKET *socketID, MessageList *pList, void *privateSpace) {
    if(ResumeThread(*transcoderManager)==-1){
        return -1;
    };

    return 0;
}


//************************************
// Method:    �߳�����
// FullName:  destroy_transcoderManager
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread_t * transcoderManager �߳̾��
// Parameter: void * private 
//************************************

int destroy_transcoderManager( HANDLE *transcoderManager,void *privateSpace ) {
    if(CloseHandle(*transcoderManager)==0){
        return -1;
    };
    return 0;
}