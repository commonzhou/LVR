#include "TranscoderManager.h"

static void *handleMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;
    
    
    // TODO: 临时解决方案，暂时认为发送的为编码参数的结构体,编码参数应该是可以访问到的
    struct TranscoderParam param;
    if(send(*socketID, (char *)&param, sizeof(param),0) < 0) {
        printf("Error while sending: %s\n",__FUNCTION__);
    }
    // TODO: 临时解决方案，用一个字符串来模拟发送的消息
    char message[500] = {'0'};
    int recv_size = 0;
    if ((recv_size = recv(*socketID,message,500,0)) == SOCKET_ERROR) {
        printf("Error while receiving: %s\n",__FUNCTION__);
    }
    message[recv_size] = '\0';
}

//************************************
// Method:    利用端口信息，初始化transcoderManager线程，线程suspend
// FullName:  init_transcoderManager
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: pthread_t * transcoderManager
// Parameter: SOCKET * socketID 主控PC和转码节点间TCP/IP连接链路
// Parameter: MessageList * pList 需要更新其中的码控反馈信息和编码信息进度列表
// Parameter: void * private 线程私有空间
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

