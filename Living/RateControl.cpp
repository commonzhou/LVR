#include "RateControl.h"

/**
 * \brief ��ȡVRLS_param�е�Tile mapping��ʽ����Ϣ������rateControl�̣߳��߳�suspend
 * \param rateControl �߳�NULLָ��
 * \param VRLS_param ����������Ϣ
 * \param pfvlist ����֡����
 * \param pmlist ��������Ϣ������
 * \param privateSpace �߳�˽�пռ�
 * \return int -1ʧ�� 0�ɹ�
 */
int init_rateControl( HANDLE *rateControl, VRLSParam *VRLS_param,AVFrameList *pfvlist,MessageList *pmlist, void *privateSpace )
{

    return 0;
}


/**
 * \brief �̴߳ӹ���״̬����
 * \param rateControl rateControl�߳̾��
 * \param privateSpace �߳�˽�пռ�
 * \param pfvlist ����֡����
 * \param pmlist ��������Ϣ������
 * \return int -1ʧ�� 0�ɹ�
 */
int activate_rateControl( HANDLE *rateControl, void *privateSpace, AVFrameList *pfvlist, MessageList *pmlist ) {


    return 0;
}


/**
 * \brief �߳����٣��ͷ�˽�пռ�
 * \param rateControl rateControl�߳̾��
 * \param privateSpace �߳�˽�пռ�
 * \return int -1ʧ�� 0�ɹ�
 */
int destroy_rateControl( HANDLE *rateControl, void *privateSpace ) {

    return 0;
}
