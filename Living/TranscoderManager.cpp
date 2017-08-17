#include "TranscoderManager.h"

unsigned int _stdcall handleMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;
    TLV receivedTLV;


    // �����õ���socketӦ���ǰ󶨺õģ������Ѿ�����listen״̬��
    int readSize;
    char *buffer = new char[sizeof (TLV)];
    while ((readSize = recv(*socketID, buffer, sizeof(TLV), 0)) > 0) {
        memcpy(&receivedTLV, buffer, sizeof(TLV));
        // TODO: ���յ�����Ϣ���н���
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

    transcoderManager = (HANDLE *)malloc(sizeof(HANDLE));
    *transcoderManager = (HANDLE)_beginthreadex(NULL, 0, handleMessage, (void *)info, 0, NULL);

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
int activate_transcoderManager( HANDLE *transcoderManager, SOCKET *socketID, MessageList *pList, void *privateSpace) {
    
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
    CloseHandle(*transcoderManager);
    return 0;
}

