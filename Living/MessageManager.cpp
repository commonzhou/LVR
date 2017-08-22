#include "MessageManager.h"
#include "catch.hpp" 

/**
 * \brief ����messageManager�ṹ�壬�����ṹ���е�����MessageList���Լ�MessageList�µ�SendRCMessageList, RecRCMessageList, SpeedMessageList
 * \param messageMag ���������Ϣ��������ָ��
 * \param StreamNum ��ǰ�ڵ�Ҫ���������, tile+��Ƶ+ȫͼ
 * \return int -1ʧ�� 0�ɹ�
 */
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

/**
 * \brief ����packet�ڵ㣬������Ϊ�գ��Ա�ÿ���߳��Լ����
 * \param pNode 
 * \return int -1ʧ�� 0�ɹ�
 */
int create_messageNode(MessageNode*& pNode)
{
    pNode = new MessageNode();
    if(pNode == NULL) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * \brief ��ȡsubMessageList��present��ָ��Ľڵ㣬Ŀǰ����β�ڵ�
 * \param pNode ��õĽڵ�
 * \param pList Ѱ�ҽڵ������
 * \return int -1ʧ�� 0�ɹ�
 */
int get_messageNode(MessageNode*& pNode, subMessageList *pList)
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


/**
 * \brief ����Ϣ����������������Ϣ
 * \param pList ��������Ϣ�ڵ������
 * \param message ���ӵ���Ϣ�ڵ�
 * \param mutex �����������߳�һ��ֻ����һ������
 * \return int -1ʧ�� 0�ɹ�
 */
int add_messageNode( subMessageList *pList,MessageNode *message,HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
    
    if(pList->pHead == pList->pTail) {
        pList->pHead->next = message;
        pList->pTail = message;
        pList->pTail->next = NULL;
        pList->present = message;
    } else {
        pList->pTail->next = message;
        pList->pTail = message;
        pList->pTail->next = NULL;
        pList->present = message;
    }
    message->used_flag = 0;
    ReleaseMutex(mutex);
    return 0;
}


/**
 * \brief ��Ϣʹ���������Ϣ����������ɾ����Ϣ
 * \param pList ��ɾ����Ϣ�ڵ������
 * \param mutex ������
 * \return int -1ʧ�� 0�ɹ�
 */
int update_messageNode( subMessageList *pList, HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
    // ��presentָ��ɾ�����Ľڵ����һ���ڵ�
    
    MessageNode *presentNode = NULL;
    get_messageNode(presentNode,pList);

    // ɾ����ͷ��present�Ĳ���
    // ����ָ��֮���ָ���ϵ
    // Ŀǰpresentָ�����pVTail
    if(presentNode != NULL) {
        MessageNode *node = pList->pHead->next;
        MessageNode *afternode = NULL;
        MessageNode *beforenode = pList->pHead;

        // ������pHead��present�Ľڵ�
        while (node != NULL && node != presentNode) {
            afternode = node->next;
            if(node->used_flag) {
                delete (node);
                node = afternode;
                beforenode->next = afternode;
            } else {
                beforenode = node;
                node = afternode;
            }
        }
    } else {
        printf("messageNode is NULL, %s\n", __FUNCTION__);
    }
    presentNode = NULL;
    ReleaseMutex(mutex);
    return 0;
}


/**
 * \brief ������Ϣ��������
 * \param messageMag �����ٵ�����ָ��
 * \param mutex ������
 * \return int -1ʧ�� 0�ɹ�
 */
int delete_messageManager(MessageManager*& messageMag, HANDLE *mutex)
{
    WaitForSingleObject(mutex,INFINITE);
    delete messageMag;
    messageMag = NULL;
    ReleaseMutex(mutex);
    return 0;
}


// ����Ϊ��Ԫ���Դ���
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