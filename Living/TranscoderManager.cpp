#include "TranscoderManager.h"

static void *handleMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;
    
    
    // TODO: ��ʱ�����������ʱ��Ϊ���͵�Ϊ��������Ľṹ��,�������Ӧ���ǿ��Է��ʵ���
    struct TranscoderParam param;
    if(send(*socketID, (char *)&param, sizeof(param),0) < 0) {
        printf("Error while sending: %s\n",__FUNCTION__);
    }
    // TODO: ��ʱ�����������һ���ַ�����ģ�ⷢ�͵���Ϣ
    char message[500] = {'0'};
    int recv_size = 0;
    if ((recv_size = recv(*socketID,message,500,0)) == SOCKET_ERROR) {
        printf("Error while receiving: %s\n",__FUNCTION__);
    }
    message[recv_size] = '\0';
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

