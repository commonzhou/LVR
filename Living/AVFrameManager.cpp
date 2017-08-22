#include "AVFrameManager.h"
#include "catch.hpp"


/**
 * \brief ����AVFrameManager�ṹ�壬�����ṹ���е�����AVFrameList��
 *             �Լ�subPicManager��subPicManager�µ�AVFrameList
 * \param AVFrameMag ��Ž���������ǰ������Ƶ�����ַ
 * \param subpicNum ��ǰ�ڵ�����Tile����
 * \return int -1ʧ�� 0�ɹ�
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
 * \brief ����AVFrameNode�ڵ㣬������Ϊ�գ��Ա�ÿ���߳��Լ����, ʹ�øú���������node����Ҫdelete
 * \param pNode �����Ľڵ�ṹ��
 * \return int -1ʧ�� 0�ɹ�
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
 * \brief ��ȡAVFrameList��present��ָ��Ľڵ�
 * \param pNode ��õ�AVFrame�ڵ�
 * \param pList Ѱ�ҽڵ������
 * \return int -1ʧ�� 0�ɹ�
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
 * \brief ��ָ����pList֡���������һ��AVframeNode�ڵ㣬�ڵ���used_flag��Ϊ0�������δ������ʹ��
 * \param pList ָ������Ҫ����֡�ڵ��֡����
 * \param pNode �����ڵ�ָ��
 * \param mutex ������
 * \return int -1ʧ�� 0�ɹ�
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
 * \brief ͨ��λ������ж�֡������֡�ڵ��Ƿ����ɾ������used_mask��Ӧλ��flagΪ1�����ɾ��
 * \param pList ɾ���ڵ�ǰ������ָ��
 * \param used_mask λ���룬ѡ����Ҫ�ж�AVFrame�ڵ���used_mask��Ա��Ӧλ�Ƿ�����ɾ���ڵ�Ҫ��
 * \param mutex ɾ���ýڵ�ǰȷ���ýڵ�δ�������߳�ʹ��
 * \return int -1ʧ�� 0�ɹ�
 */
int update_AVFrameList( AVFrameList *pList,UINT8 used_mask,HANDLE *mutex )
{
    WaitForSingleObject(mutex,INFINITE);
    if(pList == NULL) {
        return -1;
    }
    
    AVFrameNode *pNode = pList->pVhead->next;
    while(pNode != NULL) {
        // UNDONE: ��Բ�ͬ��λ���������в���

        pNode = pNode->next;
    }
    ReleaseMutex(mutex);
    return 0;
}


/**
 * \brief �ͷ�AVFrameManager�е������ڴ�ռ�
 * \param AVFrameMag �����ٵ�����ָ��
 * \param mutex ȷ����֡��������δ������ʹ��
 * \return int -1ʧ�� 0�ɹ�
 */
int delete_AVFrameManager(AVFrameManager*& AVFrameMag, HANDLE *mutex)
{
    WaitForSingleObject(mutex,INFINITE);
    delete AVFrameMag;
    AVFrameMag = NULL;
    ReleaseMutex(mutex);
    return 0;
}

// ����Ϊ��Ԫ���Դ���
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