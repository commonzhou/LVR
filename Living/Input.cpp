#include "Input.h"

/**
 * \brief ����VRLS_param�еı��������Ϣ������Դ·��������input�̺߳�˽�пռ䣬�߳�suspend
 * \param input �߳�NULLָ��
 * \param VRLS_param ����������Ϣ
 * \param VList ��������Ƶ֡����֡����
 * \param AList ��������Ƶ����֡����
 * \param privateSpace input�߳�˽�пռ�
 * \param type 0ʱ��������Ƶ��1ʱֻ������Ƶ��2ʱֻ������Ƶ
 * \return -1: ʧ�ܣ� 0: �ɹ�
 */
int init_input(HANDLE *input, VRLSParam *VRLS_param, AVFrameList *VList, AVFrameList *AList, void *privateSpace, int type) {
    
    return 0;
}

/**
 * \brief 
 * \param input input�߳̾��
 * \param VList ��������Ƶ֡����֡����
 * \param AList ��������Ƶ����֡����
 * \param privateSpace input�߳�˽�пռ�
 * \param type 0ʱ��������Ƶ��1ʱֻ������Ƶ��2ʱֻ������Ƶ
 * \return -1: ʧ�ܣ� 0: �ɹ�
 */
int activate_input(HANDLE* input, AVFrameList* VList, AVFrameList* AList, void* privateSpace, int type) {
    return  0;
}

/**
 * \brief �߳����٣��ͷ�˽�пռ�
 * \param input input�߳̾��
 * \param privateSpace �߳�˽�пռ�
 * \return -1: ʧ�ܣ� 0: �ɹ�
 */
int destroy_input(HANDLE* input, void* privateSpace) {


    return 0;
}
