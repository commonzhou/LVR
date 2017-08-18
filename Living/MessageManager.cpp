#include "MessageManager.h"
#include "catch.hpp"


//************************************
// Method:    ����messageManager�ṹ�壬�����ṹ���е�����MessageList���Լ�MessageList�µ�SendRCMessageList, RecRCMessageList, SpeedMessageList
// FullName:  create_messageManager
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: messageManager * messageMag ���������Ϣ��������ָ��
// Parameter: int StreamNum ��ǰ�ڵ�Ҫ���������, tile+��Ƶ+ȫͼ
//************************************
int create_messageManager(MessageManager*& messageMag, int StreamNum)
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
// Method:    ����packet�ڵ㣬������Ϊ�գ��Ա�ÿ���߳��Լ����
// FullName:  create_messageNode
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: MessageNode * pNode
//************************************
int create_messageNode(MessageNode*& pNode)
{
    pNode = new MessageNode();
    if(pNode == NULL) {
        return -1;
    } else {
        return 0;
    }
}

//************************************
// Method:    ��ȡsubMessageList��present��ָ��Ľڵ�
// FullName:  get_messageNode
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: MessageNode * pNode ��õ�Message�ڵ�
// Parameter: subMessageList * pList Ѱ�ҽڵ������
//************************************
int get_messageNode(MessageNode*& pNode, subMessageList *pList)
{
    if(pList == NULL) {
        return -1;
    }
    pNode = pList->pHead;
    if(pNode == NULL) {
        return -1;
    } else {
        return 0;
    }
}

//************************************
// Method:    ����Ϣ����������������Ϣ
// FullName:  add_messageNode
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: subMessageList * pList ��������Ϣ�ڵ������
// Parameter: MessageNode * message ���ӵ���Ϣ�ڵ�
// Parameter: HANDLE * mutex �����������߳�һ��ֻ����һ������
//************************************
int add_messageNode( subMessageList *pList,MessageNode *message,HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
    if(pList->pHead == NULL) {
        pList->pHead = message;
        pList->present = message;
        pList->pTail = message;
        pList->pTail->next = NULL;
    } else {
        pList->pTail->next = message;
        pList->pTail = message;
        pList->pTail->next = NULL;
        pList->present = message;
    }
    ReleaseMutex(mutex);
    return 0;
}

//************************************
// Method:    ��Ϣʹ���������Ϣ����������ɾ����Ϣ
// FullName:  update_messageNode
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: subMessageList * pList ��ɾ����Ϣ�ڵ������
// Parameter: HANDLE * mutex ������
//************************************
int update_messageNode( subMessageList *pList, HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
    // ��presentָ��ɾ�����Ľڵ����һ���ڵ�
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
// Method:    ������Ϣ��������
// FullName:  delete_messageManager
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: messageManager * messageMag �����ٵ�����ָ��
// Parameter: HANDLE * mutex ��������ɾ������ʱ���������������߳�ʹ��
//************************************
int delete_messageManager(MessageManager*& messageMag, HANDLE *mutex)
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

//
//TEST_CASE("MessageManager", "[MessageManager]") {
//    MessageManager *manager = NULL;
//    SECTION("create messageManager") {
//        create_messageManager(manager, 0);
//        REQUIRE(manager != 0);
//        REQUIRE(manager->StreamNum == 0);
//    }
//    SECTION("create messageNode") {
//        MessageNode *node = NULL;
//        create_messageNode(node);
//        REQUIRE(node != NULL);
//    }
//    SECTION("add messageNode") {
//        create_messageManager(manager, 0);
//        MessageNode *node = NULL;
//        create_messageNode(node);
//        add_messageNode(manager->pVHead->pRCL, node, NULL);
//        REQUIRE(manager->pVHead != NULL);
//        REQUIRE(manager->pVHead->pRCL != NULL);
//        REQUIRE(manager->pVHead->pRCL->pTail == node);
//    }
//    SECTION("delete messageManager") {
//        create_messageManager(manager, 0);
//        MessageNode *node = NULL;
//        create_messageNode(node);
//        add_messageNode(manager->pVHead->pRCL, node, NULL);
//        delete_messageManager(manager, NULL);
//        REQUIRE(manager == NULL);
//    }
//}