#include "MessageManager.h"
//************************************
// Method:    创建messageManager结构体，创建结构体中的所有MessageList，以及MessageList下的SendRCMessageList, RecRCMessageList, SpeedMessageList
// FullName:  create_messageManager
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: messageManager * messageMag 创建后的消息管理链表指针
// Parameter: int StreamNum 当前节点要编码的数量, tile+音频+全图
//************************************
int create_messageManager( MessageManager *messageMag,int StreamNum )
{
    messageMag = new MessageManager();
    if(messageMag == NULL) {
        return -1;
    } else {
        messageMag->StreamNum = StreamNum;
        return 0;
    }
}

//************************************
// Method:    创建packet节点，各参数为空，以备每个线程自己填充
// FullName:  create_messageNode
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: MessageNode * pNode
//************************************
int create_messageNode( MessageNode *pNode )
{
    pNode = new MessageNode();
    if(pNode == NULL) {
        return -1;
    } else {
        return 0;
    }
}

//************************************
// Method:    获取subMessageList中present所指向的节点
// FullName:  get_messageNode
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: MessageNode * pNode 获得的Message节点
// Parameter: subMessageList * pList 寻找节点的链表
//************************************
int get_messageNode( MessageNode *pNode,subMessageList *pList )
{
    if(pList == NULL) {
        return -1;
    }
    pNode = pList->present;
    if(pNode == NULL) {
        return -1;
    } else {
        return 0;
    }
}

//************************************
// Method:    在消息管理链表中增加信息
// FullName:  add_messageNode
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: subMessageList * pList 待增加信息节点的链表
// Parameter: MessageNode * message 增加的信息节点
// Parameter: HANDLE * mutex 互斥锁，多线程一次只能有一个操作
//************************************
int add_messageNode( subMessageList *pList,MessageNode *message,HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
    pList->pTail = message;
    message->next = NULL;
    ReleaseMutex(mutex);
    return 0;
}

//************************************
// Method:    消息使用完后，在消息管理链表中删除信息
// FullName:  update_messageNode
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: subMessageList * pList 待删除信息节点的链表
// Parameter: HANDLE * mutex 互斥锁
//************************************
int update_messageNode( subMessageList *pList, HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
    // 让present指向被删除掉的节点的下一个节点
    MessageNode *presentNode = NULL;
    get_messageNode(presentNode,pList);
    if(presentNode->used_flag) {
        MessageNode *node = pList->pHead;
        while (node->next != presentNode) {
            node = node->next;
        }
        node->next = presentNode->next;
        pList->present = presentNode->next;
    }
    delete presentNode;
    presentNode = NULL;
    ReleaseMutex(mutex);
    return 0;
}

//************************************
// Method:    销毁消息管理链表
// FullName:  delete_messageManager
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: messageManager * messageMag 待销毁的链表指针
// Parameter: HANDLE * mutex 互斥锁，删除链表时，该链表不被其它线程使用
//************************************
int delete_messageManager( MessageManager *messageMag, HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(messageMag == NULL) {
        return -1;
    }
    delete messageMag;
    messageMag = NULL;
    ReleaseMutex(mutex);
    return 0;
}
