#include "TranscoderManager.h"

unsigned int _stdcall handleMessage(void *params) {
    InfoNode *node = (InfoNode *)params;
    MessageList *list = node->messageList;
    SOCKET *socketID = node->socketID;
    TLV receivedTLV;


    // 这里拿到的socket应该是绑定好的，并且已经处于listen状态下
    int readSize;
    char *buffer = new char[sizeof (TLV)];
    while ((readSize = recv(*socketID, buffer, sizeof(TLV), 0)) > 0) {
        memcpy(&receivedTLV, buffer, sizeof(TLV));
        // TODO: 对收到的信息进行解析
    }
    return 0;
}

//************************************
// Method:    利用端口信息，初始化transcoderManager线程，线程suspend
// FullName:  init_transcoderManager
// Access:    public 
// Returns:   int -1失败 0成功
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

    transcoderManager = (HANDLE *)malloc(sizeof(HANDLE));
    *transcoderManager = (HANDLE)_beginthreadex(NULL, 0, handleMessage, (void *)info, 0, NULL);

    return 0;
}

//************************************
// Method:    线程从挂起状态唤醒
// FullName:  activate_transcoderManager
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: pthread_t * transcoderManager  线程NULL指针
// Parameter: SOCKET * socketID 主控PC和转码节点间TCP/IP连接链路
// Parameter: MessageList * pList 需要更新其中的码控反馈信息和编码信息进度列表
// Parameter: void * private 线程私有空间
//************************************
int activate_transcoderManager( HANDLE *transcoderManager, SOCKET *socketID, MessageList *pList, void *privateSpace) {
    
    return 0;
}

//************************************
// Method:    线程销毁
// FullName:  destroy_transcoderManager
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: pthread_t * transcoderManager 线程句柄
// Parameter: void * private 
//************************************
int destroy_transcoderManager( HANDLE *transcoderManager,void *privateSpace ) {
    CloseHandle(*transcoderManager);
    return 0;
}

