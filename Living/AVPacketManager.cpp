#include "AVPacketManager.h"
#include "catch.hpp"
//************************************
// Method:    ����AVPacketManager�ṹ�壬�����ṹ���е�����AVPacketList
// FullName:  create_AVPacketManager
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: AVPacketManager * AVPacketMag ��������ռ䣬����Ŷ�ȡ������Ƶ���ݰ�
// Parameter: int StreamNum ��ǰ�ڵ�Ҫ���������, tile+��Ƶ+ȫͼ
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
// Method:    ����packet�ڵ㣬������Ϊ�գ��Ա�ÿ���߳��Լ����
// FullName:  create_packetNode
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: packetNode * pNode new�����ڵ�ṹ��
//************************************
int create_packetNode(PacketNode*& pNode)
{
    // ��������д�������update_AVPacketList()�����ɾ�������Բ���Ҫ��ʾɾ��
    pNode = new PacketNode();
    if(pNode == NULL) {
        return -1;
    } else {
        return 0;
    }
}

//************************************
// Method:    ��ȡAVPacketList��present��ָ��Ľڵ�
// FullName:  get_packetNode
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: packetNode * pNode ��õ�AVPacket�ڵ�
// Parameter: AVPktList * pList Ѱ�ҽڵ������
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
// Method:    �����ݰ������м������ݰ��ڵ�pNode������pNode�е�used_flagΪ0
// FullName:  add_packetNode
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: AVPktList * pList ���ӽڵ�ǰ�����ݰ�����
// Parameter: packetNode * pNode ���ӵ����ݰ�ָ��
// Parameter: HANDLE * mutex �����ݰ�����δ�������߳�ʹ��
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
// Method:    ɾ���Ѿ���ʹ�õ����ݰ��ڵ�
// FullName:  update_AVPacketList
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: AVPktList * pList ɾ���ڵ�ǰ������ָ��
// Parameter: HANDLE * mutex �ڵ�ɾ��ʱ���������������߳�ʹ��
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
        // TODO: ��Բ�ͬ��λ���������в���
        pNode = pNode->next;
    }
    ReleaseMutex(mutex);
    return 0;
}

//************************************
// Method:    ���ٽṹ����
// FullName:  delete_AVPacketManager
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: AVPacketManager * AVPacketMag �����ٵ�����ָ��
// Parameter: HANDLE * mutex ��������ɾ�����ݰ���������ʱ���������������߳�ʹ��
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