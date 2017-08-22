#include "RateControl.h"

/**
 * \brief 获取VRLS_param中的Tile mapping方式等信息，创建rateControl线程，线程suspend
 * \param rateControl 线程NULL指针
 * \param VRLS_param 编码配置信息
 * \param pfvlist 输入帧链表
 * \param pmlist 输出码控信息到链表
 * \param privateSpace 线程私有空间
 * \return int -1失败 0成功
 */
int init_rateControl( HANDLE *rateControl, VRLSParam *VRLS_param,AVFrameList *pfvlist,MessageList *pmlist, void *privateSpace )
{

    return 0;
}


/**
 * \brief 线程从挂起状态唤醒
 * \param rateControl rateControl线程句柄
 * \param privateSpace 线程私有空间
 * \param pfvlist 输入帧链表
 * \param pmlist 输出码控信息到链表
 * \return int -1失败 0成功
 */
int activate_rateControl( HANDLE *rateControl, void *privateSpace, AVFrameList *pfvlist, MessageList *pmlist ) {


    return 0;
}


/**
 * \brief 线程销毁，释放私有空间
 * \param rateControl rateControl线程句柄
 * \param privateSpace 线程私有空间
 * \return int -1失败 0成功
 */
int destroy_rateControl( HANDLE *rateControl, void *privateSpace ) {

    return 0;
}
