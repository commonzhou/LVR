#include "MessageManager.h"
#include "catch.hpp" 

/**
 * \brief 创建messageManager结构体，创建结构体中的所有MessageList，以及MessageList下的SendRCMessageList, RecRCMessageList, SpeedMessageList
 * \param messageMag 创建后的消息管理链表指针
 * \param StreamNum 当前节点要编码的数量, tile+音频+全图
 * \return int -1失败 0成功
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
 * \brief 创建packet节点，各参数为空，以备每个线程自己填充
 * \param pNode 
 * \return int -1失败 0成功
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
 * \brief 获取subMessageList中present所指向的节点，目前就是尾节点
 * \param pNode 获得的节点
 * \param pList 寻找节点的链表
 * \return int -1失败 0成功
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
 * \brief 在消息管理链表中增加信息
 * \param pList 待增加信息节点的链表
 * \param message 增加的信息节点
 * \param mutex 互斥锁，多线程一次只能有一个操作
 * \return int -1失败 0成功
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
 * \brief 消息使用完后，在消息管理链表中删除信息
 * \param pList 待删除信息节点的链表
 * \param mutex 互斥锁
 * \return int -1失败 0成功
 */
int update_messageNode( subMessageList *pList, HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
    // 让present指向被删除掉的节点的下一个节点
    
    MessageNode *presentNode = NULL;
    get_messageNode(presentNode,pList);

    // 删掉从头到present的部分
    // 更新指针之间的指向关系
    // 目前present指向的是pVTail
    if(presentNode != NULL) {
        MessageNode *node = pList->pHead->next;
        MessageNode *afternode = NULL;
        MessageNode *beforenode = pList->pHead;

        // 遍历从pHead到present的节点
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
 * \brief 销毁消息管理链表
 * \param messageMag 待销毁的链表指针
 * \param mutex 互斥锁
 * \return int -1失败 0成功
 */
int delete_messageManager(MessageManager*& messageMag, HANDLE *mutex)
{
    WaitForSingleObject(mutex,INFINITE);
    delete messageMag;
    messageMag = NULL;
    ReleaseMutex(mutex);
    return 0;
}


// 以下为单元测试代码
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