#include "Muxer.h"

/**
 * \brief ����muxer�̣߳�������ʼ���ֶ� initialization segment��������init segment����Ϣ
 * \param muxer �߳�NULLָ��
 * \param pPackageMag ָ������Դ
 * \param VRLS_param ָ�������ʽ�͵�ַ
 * \param pSubList ָ���������
 * \param privateSpace ���߳�˽�пռ�
 * \return -1: ʧ�ܣ� 0: �ɹ�
 */
int init_muxer(HANDLE* muxer, AVPacketManager* pPackageMag, VRLSParam* VRLS_param, subMessageList* pSubList, void* privateSpace) {

    return 0;
}

/**
 * \brief �����߳�
 * \param muxer �߳�
 * \param pPackageMag ָ������Դ
 * \param VRLS_param ָ�������ʽ�͵�ַ
 * \param pSubList �������
 * \param privateSpace ���߳�˽�пռ�
 * \return -1: ʧ�ܣ� 0: �ɹ�
 */
int activate_muxer(HANDLE* muxer, AVPacketManager* pPackageMag, VRLSParam* VRLS_param, subMessageList* pSubList, void* privateSpace) {

    return 0;
}

/**
 * \brief �߳����٣��ͷ�˽�пռ�
 * \param muxer �߳�
 * \param privateSpace ���߳�˽�пռ�
 * \return -1: ʧ�ܣ� 0: �ɹ�
 */
int destroy_muxer(HANDLE* muxer, void* privateSpace) {

    return 0;
}
