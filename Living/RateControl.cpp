#include "RateControl.h"


//************************************
// Method:    ��ȡVRLS_param�е�Tile mapping��ʽ����Ϣ������rateControl�̣߳��߳�suspend
// FullName:  init_rateControl
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread * rateControl �߳�NULLָ��
// Parameter: VRLSParam * VRLS_param ����������Ϣ
// Parameter: AVFrameList * pfvlist ����֡����
// Parameter: MessageList * pmlist ��������Ϣ������
// Parameter: void * private �߳�˽�пռ�
//************************************
int init_rateControl( HANDLE *rateControl, VRLSParam *VRLS_param,AVFrameList *pfvlist,MessageList *pmlist, void *privateSpace )
{

    return 0;
}

//************************************
// Method:    �̴߳ӹ���״̬����
// FullName:  activate_rateControl 
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread * rateControl rateControl�߳̾��
// Parameter: void * private �߳�˽�пռ�
// Parameter: AVFrameList * pfvlist ����֡����
// Parameter: MessageList * pmlist ��������Ϣ������
//************************************
int activate_rateControl( HANDLE *rateControl, void *privateSpace, AVFrameList *pfvlist, MessageList *pmlist ) {


    return 0;
}

//************************************
// Method:    �߳����٣��ͷ�˽�пռ�
// FullName:  destroy_rateControl
// Access:    public 
// Returns:   int -1ʧ�� 0�ɹ�
// Qualifier:
// Parameter: pthread * rateControl rateControl�߳̾��
// Parameter: void * private �߳�˽�пռ�
//************************************
int destroy_rateControl( HANDLE *rateControl, void *privateSpace ) {

    return 0;
}
