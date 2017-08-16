#include "TranscoderManager.h"

#define	RUN 	1
#define STOP 	0

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int status = STOP;

static void *handleMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;
    TLV receivedTLV;


    // �����õ���socketӦ���ǰ󶨺õģ������Ѿ�����listen״̬��
    int read_size;
    char *buffer = new char[sizeof (TLV)];
    while ((read_size = recv(*socketID, buffer, sizeof(TLV), 0)) > 0) {
        memcpy(&receivedTLV, buffer, sizeof(TLV));
        // TODO: ���յ�����Ϣ���н���
    };
    while(1)
    {
        pthread_mutex_lock(&mut);
        while(!status)
        {
            pthread_cond_wait(&cond, &mut);
        }
        pthread_mutex_unlock(&mut);
    }
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

int init_transcoderManager( pthread_t *transcoderManager, SOCKET *socketID,MessageList *pList,void *privateSpace )
{ 
 //************************************
 //SocketID will be handled here
 //************************************
    WSADATA wsa;
    char  server_reply[2000];
    int  recv_size;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return -1;
    }
    if((recv_size = recv(*socketID , server_reply , 2000 , 0)) == SOCKET_ERROR)
    {
        puts("recv failed");
        return -1;
    }
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
    int err = pthread_create(
        transcoderManager,NULL,handleMessage,(void *)info);
    if(err != 0) {
        return -1;
    }
    closesocket(*socketID);
    WSACleanup();
    return 0; 

 //************************************
 //Socket will be destroyed here
 //************************************



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
int activate_transcoderManager(pthread_t *transcoderManager, SOCKET *socketID, MessageList *pList, void *privateSpace) {
    if (status == STOP)
    {
        pthread_mutex_lock(&mut);
        status = RUN;
        pthread_cond_signal(&cond);
        printf("pthread run!\n");
        pthread_mutex_unlock(&mut);
    }
    else
    {
        printf("pthread run already\n");
    }
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
int destroy_transcoderManager( pthread_t *transcoderManager,void *privateSpace ) {

    pthread_cancel(*transcoderManager);
    pthread_exit(NULL);
    return 0;
}
