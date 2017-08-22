#include "AVPacketManager.h"
#include "catch.hpp"

/**
 * \brief 创建AVPacketManager结构体，创建结构体中的所有AVPacketList
 * \param AVPacketMag 待存放读取的音视频数据包
 * \param StreamNum 当前节点要编码的数量, tile+音频+全图
 * \return int -1失败 0成功
 */
int create_AVPacketManager(AVPacketManager*& AVPacketMag, int StreamNum)
{
    AVPacketMag = new AVPacketManager();
    if (AVPacketMag == NULL) {
        return -1;
    } 
     AVPacketMag->StreamNum = StreamNum;
     return 0;
}

/**
 * \brief 创建packet节点，各参数为空，以备每个线程自己填充
 * \param pNode new创建节点结构体
 * \return int -1失败 0成功
 */
int create_packetNode(PacketNode*& pNode)
{
  
    pNode = new PacketNode();
    if(pNode == NULL) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * \brief 获取AVPacketList中present所指向的节点。目前获取的是链表的尾节点
 * \param pNode 获得的AVPacket节点
 * \param pList 寻找节点的链表
 * \return int -1失败 0成功
 */
int get_packetNode(PacketNode*& pNode, AVPktList *pList)
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
 * \brief 在数据包链表中加入数据包节点pNode，设置pNode中的used_flag为0
 * \param pList 增加节点前的数据包链表
 * \param pNode 要增加的数据包指针
 * \param mutex 互斥锁
 * \return int -1失败 0成功
 */
int add_packetNode( AVPktList *pList,PacketNode *pNode,HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
   
    if (pList->pVHead == pList->pVTail) {
        pList->pVHead = pNode;
        pList->pVTail = pNode;
        pList->present = pNode;
        pList->pVTail->next = NULL;
    } else {
        pList->pVTail->next = pNode;
        pList->pVTail = pNode;
        pList->present = pNode;
        pList->pVTail->next = NULL;
    }
    pNode->used_flag = 0;
    ReleaseMutex(mutex);
    return 0;
}


/**
 * \brief 删除已经被使用的数据包节点
 * \param pList 删除节点前的链表指针
 * \param mutex 互斥锁
 * \return int -1失败 0成功
 */
int update_AVPacketList( AVPktList *pList,HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
    PacketNode *pNode = pList->pVHead;
    while (pNode != NULL)
    {
        // TODO: 针对不同的位运算结果进行操作
        pNode = pNode->next;
    }
    ReleaseMutex(mutex);
    return 0;
}


/**
 * \brief 销毁结构链表
 * \param AVPacketMag 待销毁的链表指针
 * \param mutex 互斥锁
 * \return int -1失败 0成功
 */
int delete_AVPacketManager(AVPacketManager*& AVPacketMag, HANDLE *mutex)
{
    WaitForSingleObject(mutex,INFINITE);
    delete AVPacketMag;
    AVPacketMag = NULL;
    ReleaseMutex(mutex);
    return 0;
}


// 以下为单元测试代码
//TEST_CASE("AVPacketManager","[AVPacketManager]") {
//    AVPacketManager *manager = NULL;
//    PacketNode *node = NULL;
//    SECTION("create AVPacketManager") {
//        create_AVPacketManager(manager, 0);
//        REQUIRE(manager != NULL);
//        REQUIRE(manager->StreamNum == 0);
//    }
//    SECTION("create packetNode") {
//        create_packetNode(node);
//        REQUIRE(node != NULL);
//    }
//    SECTION("add_packetNode") {
//        create_AVPacketManager(manager, 0);
//        create_packetNode(node);
//        add_packetNode(manager->pVHead, node, NULL);
//        REQUIRE(manager->pVHead != NULL);
//    }
//    SECTION("delete packetManager") {
//        create_AVPacketManager(manager, 0);
//        delete_AVPacketManager(manager, NULL);
//        REQUIRE(manager == NULL);
//    }
//}