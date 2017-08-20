#include "Muxer.h"

/**
 * \brief 创建muxer线程，产生初始化分段 initialization segment，并产生init segment的信息
 * \param muxer 线程NULL指针
 * \param pPackageMag 指定输入源
 * \param VRLS_param 指定输出格式和地址
 * \param pSubList 指定进度输出
 * \param privateSpace 本线程私有空间
 * \return -1: 失败； 0: 成功
 */
int init_muxer(HANDLE* muxer, AVPacketManager* pPackageMag, VRLSParam* VRLS_param, subMessageList* pSubList, void* privateSpace) {

    return 0;
}

/**
 * \brief 启动线程
 * \param muxer 线程
 * \param pPackageMag 指定输入源
 * \param VRLS_param 指定输出格式和地址
 * \param pSubList 进度输出
 * \param privateSpace 本线程私有空间
 * \return -1: 失败； 0: 成功
 */
int activate_muxer(HANDLE* muxer, AVPacketManager* pPackageMag, VRLSParam* VRLS_param, subMessageList* pSubList, void* privateSpace) {

    return 0;
}

/**
 * \brief 线程销毁，释放私有空间
 * \param muxer 线程
 * \param privateSpace 本线程私有空间
 * \return -1: 失败； 0: 成功
 */
int destroy_muxer(HANDLE* muxer, void* privateSpace) {

    return 0;
}
