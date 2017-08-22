#include "AVFrameManager.h"
#include "catch.hpp"


/**
 * \brief 创建AVFrameManager结构体，创建结构体中的所有AVFrameList，
 *             以及subPicManager和subPicManager下的AVFrameList
 * \param AVFrameMag 存放解码后待编码前的音视频链表地址
 * \param subpicNum 当前节点编码的Tile数量
 * \return int -1失败 0成功
 */
int create_AVFrameManager(AVFrameManager*& AVFrameMag, int subpicNum) {
    AVFrameMag = new struct AVFrameManager();
    if(! AVFrameMag) {
        return -1;
    }

    AVFrameMag->pSPM->subpicNum = subpicNum;
    return 0;
}


/**
 * \brief 创建AVFrameNode节点，各参数为空，以备每个线程自己填充, 使用该函数创建的node不需要delete
 * \param pNode 创建的节点结构体
 * \return int -1失败 0成功
 */
int create_AVFrameNode(AVFrameNode*& pNode) {
    pNode = new AVFrameNode();
    if(pNode) {
        return 0;
    } else {
        return -1;
    }
}


/**
 * \brief 获取AVFrameList中present所指向的节点
 * \param pNode 获得的AVFrame节点
 * \param pList 寻找节点的链表
 * \return int -1失败 0成功
 */
int get_AVFrameNode(AVFrameNode*& pNode, AVFrameList *pList) {
    if(pList == NULL) {
        return -1;
    } 
    pNode = pList->present;
    if(pNode == NULL) {
        return -1;
    }
    return 0;
}

/**
 * \brief 在指定的pList帧链表中添加一个AVframeNode节点，节点中used_flag设为0，标记尚未被编码使用
 * \param pList 指定的需要调节帧节点的帧链表
 * \param pNode 新增节点指针
 * \param mutex 互斥锁
 * \return int -1失败 0成功
 */
int add_AVFrameNode(AVFrameList *pList,AVFrameNode *pNode,HANDLE *mutex) {
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
    if (pList->pVhead == pList->pVTail) {
        pList->pVhead->next = pNode;
        pList->pVTail = pNode;
        pList->present = pNode;
        pNode->next = NULL;
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
 * \brief 通过位与操作判断帧链表中帧节点是否可以删除，若used_mask对应位的flag为1，则可删除
 * \param pList 删除节点前的链表指针
 * \param used_mask 位掩码，选定需要判断AVFrame节点中used_mask成员相应位是否满足删除节点要求
 * \param mutex 删除该节点前确保该节点未被其它线程使用
 * \return int -1失败 0成功
 */
int update_AVFrameList( AVFrameList *pList,UINT8 used_mask,HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
    
    AVFrameNode *pNode = pList->pVhead->next;
    while(pNode != NULL) {
        // UNDONE: 针对不同的位运算结果进行操作

        pNode = pNode->next;
    }
    ReleaseMutex(mutex);
    return 0;
}


/**
 * \brief 释放AVFrameManager中的所有内存空间
 * \param AVFrameMag 待销毁的链表指针
 * \param mutex 确保该帧管理链表未被其它使用
 * \return int -1失败 0成功
 */
int delete_AVFrameManager(AVFrameManager*& AVFrameMag, HANDLE *mutex)
{
    WaitForSingleObject(mutex,INFINITE);
    delete AVFrameMag;
    AVFrameMag = NULL;
    ReleaseMutex(mutex);
    return 0;
}

// 以下为单元测试代码
//TEST_CASE("AVFrameManager.", "[AVFrameManager]") {
//    AVFrameManager *manager = NULL;
//    SECTION("create_AVFrameManager") {
//        create_AVFrameManager(manager, 0);
//        REQUIRE(manager != NULL);
//        REQUIRE(manager->pProjectList != NULL);
//        REQUIRE(manager->pSPM != NULL);
//        REQUIRE(manager->psourceAList != NULL);
//    }
//    SECTION("add_AVFrameNode") {
//        create_AVFrameManager(manager, 0);
//        AVFrameNode *node = NULL;
//        create_AVFrameNode(node);
//        add_AVFrameNode(manager->pProjectList, node, NULL);
//        REQUIRE(node != NULL);
//    }
//    SECTION("get_AVFrameNode") {
//
//        create_AVFrameManager(manager, 0);
//        AVFrameNode *node = NULL;
//        create_AVFrameNode(node);
//        add_AVFrameNode(manager->pProjectList, node, NULL);
//        AVFrameNode *node2 = NULL;
//        get_AVFrameNode(node2, manager->pProjectList);
//        REQUIRE(node2 != NULL);
//    }
//    SECTION("delete_AVFrameManager") {
//        create_AVFrameManager(manager, 0);
//        REQUIRE(manager != NULL);
//        delete_AVFrameManager(manager, NULL);
//        REQUIRE(manager == NULL);
//    }
//}