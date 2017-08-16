#include "TranscoderManager.h"

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
    }
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

int init_transcoderManager( pthread_t *transcoderManager, SOCKET *socketID,MessageList *pList,void *privateSpace ) {
    InfoNode *info = new InfoNode();
    info->messageList = pList;
    info->socketID = socketID;
    int err = pthread_create(
        transcoderManager,NULL,handleMessage,(void *)info);
    if(err != 0) {
        return -1;
    }
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
int activate_transcoderManager(pthread_t *transcoderManager, SOCKET *socketID, MessageList *pList, void *privateSpace) {
    pthread_join(*transcoderManager, &privateSpace);
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
}

