#include "TranscoderManager.h"



DWORD WINAPI handleMessage(LPVOID params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;
    TLV receivedTLV;
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
    }

    int read_size;
    char *buffer = new char[sizeof (TLV)];
    while ((read_size = recv(*socketID, buffer, sizeof(TLV), 0)) > 0) {
        memcpy(&receivedTLV, buffer, sizeof(TLV));

    }
  
  //************************************
 //Socket will be destroyed here
 //************************************
    closesocket(*socketID);
    WSACleanup();
    return 0;
}

//************************************
// Method:    ���ö˿���Ϣ����ʼ��transcoderManager�̣߳��߳�suspend
// FullName:  init_transcoderManager
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: pthread_t * transcoderManager
// Parameter: SOCKET * socketID ����PC��ת��ڵ��TCP/IP������·
// Parameter: MessageList * pList ��Ҫ�������е���ط�����Ϣ�ͱ�����Ϣ�����б�
// Parameter: void * private �߳�˽�пռ�
//************************************

int init_transcoderManager( HANDLE transcoderManager, SOCKET *socketID,MessageList *pList,void *privateSpace )
{ 
  
 //************************************
 //Private space will be handled here
 //************************************
    pthread_key_t * privateArea= (pthread_key_t *)privateSpace;
    int err= pthread_key_create(privateArea, NULL);    
    if(err != 0) {
        return -1;
    }

    InfoNode *info = new InfoNode();
    info->messageList = pList;
    info->socketID = socketID;
    HANDLE transcoderManager = CreateThread(
        NULL,0,handleMessage,(void *)info,CREATE_SUSPENDED,NULL);
    if(transcoderManager== NULL) {
        printf("CreateThread failed (%d)\n", GetLastError());
        return -1;
    }
    

    //WaitForSingleObject(transcoderManager,INFINITE);
    return 0; 
}
 


//************************************
// Method:    activate_transcoderManager
// FullName:  activate_transcoderManager
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: pthread_t * transcoderManager
// Parameter: SOCKET * socketID
// Parameter: MessageList * pList
// Parameter: void * private
//************************************
int activate_transcoderManager(HANDLE transcoderManager, SOCKET *socketID, MessageList *pList, void *privateSpace) {
    if(ResumeThread(transcoderManager)==-1){
        return -1;
    };

    
    return 0;
}

//************************************
// Method:    destroy_transcoderManager
// FullName:  destroy_transcoderManager
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: pthread_t * transcoderManager
// Parameter: void * private
//************************************
int destroy_transcoderManager( HANDLE transcoderManager,void *privateSpace ) {

    if(CloseHandle(transcoderManager)==0){
        return -1;
    };
    return 0;
}
