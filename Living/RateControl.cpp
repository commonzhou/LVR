#include "RateControl.h"
pthread_key_t key;

//************************************
// Method:    获取VRLS_param中的Tile mapping方式等信息，创建rateControl线程，线程suspend
// FullName:  init_rateControl
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: pthread * rateControl 线程NULL指针
// Parameter: VRLSParam * VRLS_param 编码配置信息
// Parameter: AVFrameList * pfvlist 输入帧链表
// Parameter: MessageList * pmlist 输出码控信息到链表
// Parameter: void * private 线程私有空间
//************************************
int init_rateControl( pthread *rateControl, VRLSParam *VRLS_param,AVFrameList *pfvlist,MessageList *pmlist, void *private )
{
    int err = pthread_create(
        rateControl,NULL,NULL,NULL);
    if (err != 0) {
        return -1;
    }

}

//************************************
// Method:    线程从挂起状态唤醒
// FullName:  activate_rateControl 
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: pthread * rateControl rateControl线程句柄
// Parameter: void * private 线程私有空间
// Parameter: AVFrameList * pfvlist 输入帧链表
// Parameter: MessageList * pmlist 输出码控信息到链表
//************************************
int activate_rateControl( pthread *rateControl, void *private, AVFrameList *pfvlist, MessageList *pmlist )
{
    

}

//************************************
// Method:    线程销毁，释放私有空间
// FullName:  destroy_rateControl
// Access:    public 
// Returns:   int -1失败 0成功
// Qualifier:
// Parameter: pthread * rateControl rateControl线程句柄
// Parameter: void * private 线程私有空间
//************************************
int destroy_rateControl( pthread *rateControl, void *private )
{

}
