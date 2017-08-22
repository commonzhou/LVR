#include "AVPacketManager.h"
#include "catch.hpp"

/**
 * \brief ����AVPacketManager�ṹ�壬�����ṹ���е�����AVPacketList
 * \param AVPacketMag ����Ŷ�ȡ������Ƶ���ݰ�
 * \param StreamNum ��ǰ�ڵ�Ҫ���������, tile+��Ƶ+ȫͼ
 * \return int -1ʧ�� 0�ɹ�
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
 * \brief ����packet�ڵ㣬������Ϊ�գ��Ա�ÿ���߳��Լ����
 * \param pNode new�����ڵ�ṹ��
 * \return int -1ʧ�� 0�ɹ�
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
 * \brief ��ȡAVPacketList��present��ָ��Ľڵ㡣Ŀǰ��ȡ���������β�ڵ�
 * \param pNode ��õ�AVPacket�ڵ�
 * \param pList Ѱ�ҽڵ������
 * \return int -1ʧ�� 0�ɹ�
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
 * \brief �����ݰ������м������ݰ��ڵ�pNode������pNode�е�used_flagΪ0
 * \param pList ���ӽڵ�ǰ�����ݰ�����
 * \param pNode Ҫ���ӵ����ݰ�ָ��
 * \param mutex ������
 * \return int -1ʧ�� 0�ɹ�
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
 * \brief ɾ���Ѿ���ʹ�õ����ݰ��ڵ�
 * \param pList ɾ���ڵ�ǰ������ָ��
 * \param mutex ������
 * \return int -1ʧ�� 0�ɹ�
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
        // TODO: ��Բ�ͬ��λ���������в���
        pNode = pNode->next;
    }
    ReleaseMutex(mutex);
    return 0;
}


/**
 * \brief ���ٽṹ����
 * \param AVPacketMag �����ٵ�����ָ��
 * \param mutex ������
 * \return int -1ʧ�� 0�ɹ�
 */
int delete_AVPacketManager(AVPacketManager*& AVPacketMag, HANDLE *mutex)
{
    WaitForSingleObject(mutex,INFINITE);
    delete AVPacketMag;
    AVPacketMag = NULL;
    ReleaseMutex(mutex);
    return 0;
}


// ����Ϊ��Ԫ���Դ���
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