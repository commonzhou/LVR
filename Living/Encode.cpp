#include "prefix.h"
#include "Encode.h"

/**
 * \brief ���ձ������ã�����encode�̣߳����� 
 * \param encode �߳�NULLָ��
 * \param VRLS_param �������ò���
 * \param pAVList ָ������Դ
 * \param pPlist ָ���������
 * \param privateSpace �߳�˽�пռ�
 * \return -1: ʧ�ܣ� 0: �ɹ�
 */
int init_encode(HANDLE* encode, VRLSParam* VRLS_param, AVFrameList* pAVList, AVPktList* pPlist, void* privateSpace) {
    return 0;
}


/**
 * \brief ���������߳�
 * \param encode �߳�
 * \param pAVList ָ������Դ
 * \param pPlist ָ���������
 * \param privateSpace �߳�˽�пռ�
 * \return -1: ʧ�ܣ� 0: �ɹ�
 */
int activate_encode(HANDLE* encode, AVFrameList* pAVList, AVPktList* pPlist, void *privateSpace) {
    return 0;
}

/**
 * \brief �߳����٣��ͷ�˽�пռ�
 * \param encode �߳�
 * \param privateSpace �߳�˽�пռ�
 * \return -1: ʧ�ܣ� 0: �ɹ�
 */
int destroy_encode(HANDLE* encode, void* privateSpace) {
    return 0;
}
