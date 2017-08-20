#include "prefix.h"
#include "Encode.h"

/**
 * \brief 按照编码配置，创建encode线程，挂起 
 * \param encode 线程NULL指针
 * \param VRLS_param 编码配置参数
 * \param pAVList 指定编码源
 * \param pPlist 指定输出链表
 * \param privateSpace 线程私有空间
 * \return -1: 失败； 0: 成功
 */
int init_encode(HANDLE* encode, VRLSParam* VRLS_param, AVFrameList* pAVList, AVPktList* pPlist, void* privateSpace) {
    return 0;
}


/**
 * \brief 启动编码线程
 * \param encode 线程
 * \param pAVList 指定编码源
 * \param pPlist 指定输出链表
 * \param privateSpace 线程私有空间
 * \return -1: 失败； 0: 成功
 */
int activate_encode(HANDLE* encode, AVFrameList* pAVList, AVPktList* pPlist, void *privateSpace) {
    return 0;
}

/**
 * \brief 线程销毁，释放私有空间
 * \param encode 线程
 * \param privateSpace 线程私有空间
 * \return -1: 失败； 0: 成功
 */
int destroy_encode(HANDLE* encode, void* privateSpace) {
    return 0;
}
