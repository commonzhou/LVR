#include "AVPacketManager.h"
#include "catch.hpp"
//************************************
// Method:    创建AVPacketManager结构体，创建结构体中的所有AVPacketList
// FullName:  create_AVPacketManager
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: AVPacketManager * AVPacketMag 创建链表空间，待存放读取的音视频数据包
// Parameter: int StreamNum 当前节点要编码的数量, tile+音频+全图
//************************************
int create_AVPacketManager(AVPacketManager*& AVPacketMag, int StreamNum)
{
    AVPacketMag = new AVPacketManager();
    if (AVPacketMag == NULL) {
        return -1;
    } 
     AVPacketMag->StreamNum = StreamNum;
     return 0;
}

//************************************
// Method:    创建packet节点，各参数为空，以备每个线程自己填充
// FullName:  create_packetNode
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: packetNode * pNode new创建节点结构体
//************************************
int create_packetNode(PacketNode*& pNode)
{
    // 在这里进行创建，在update_AVPacketList()里进行删除，所以不需要显示删除
    pNode = new PacketNode();
    if(pNode == NULL) {
        return -1;
    } else {
        return 0;
    }
}

//************************************
// Method:    获取AVPacketList中present所指向的节点
// FullName:  get_packetNode
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: packetNode * pNode 获得的AVPacket节点
// Parameter: AVPktList * pList 寻找节点的链表
//************************************
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

//************************************
// Method:    在数据包链表中加入数据包节点pNode，设置pNode中的used_flag为0
// FullName:  add_packetNode
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: AVPktList * pList 增加节点前的数据包链表
// Parameter: packetNode * pNode 增加的数据包指针
// Parameter: HANDLE * mutex 该数据包链表未被其它线程使用
//************************************
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

//************************************
// Method:    删除已经被使用的数据包节点
// FullName:  update_AVPacketList
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: AVPktList * pList 删除节点前的链表指针
// Parameter: HANDLE * mutex 节点删除时，该链表不被其它线程使用
//************************************
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

//************************************
// Method:    销毁结构链表
// FullName:  delete_AVPacketManager
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: AVPacketManager * AVPacketMag 待销毁的链表指针
// Parameter: HANDLE * mutex 互斥锁，删除数据包管理链表时，该链表不被其它线程使用
//************************************
int delete_AVPacketManager(AVPacketManager*& AVPacketMag, HANDLE *mutex)
{
    WaitForSingleObject(mutex,INFINITE);
    delete AVPacketMag;
    AVPacketMag = NULL;
    ReleaseMutex(mutex);
    return 0;
}


TEST_CASE("AVPacketManager","[AVPacketManager]") {
    AVPacketManager *manager = NULL;
    PacketNode *node = NULL;
    SECTION("create AVPacketManager") {
        create_AVPacketManager(manager, 0);
        REQUIRE(manager != NULL);
        REQUIRE(manager->StreamNum == 0);
    }
    SECTION("create packetNode") {
        create_packetNode(node);
        REQUIRE(node != NULL);
    }
    SECTION("add_packetNode") {
        create_AVPacketManager(manager, 0);
        create_packetNode(node);
        add_packetNode(manager->pVHead, node, NULL);
        REQUIRE(manager->pVHead != NULL);
    }
    SECTION("delete packetManager") {
        create_AVPacketManager(manager, 0);
        delete_AVPacketManager(manager, NULL);
        REQUIRE(manager == NULL);
    }
}