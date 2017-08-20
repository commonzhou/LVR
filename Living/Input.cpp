#include "Input.h"

/**
 * \brief 根据VRLS_param中的编码参数信息、输入源路径，创建input线程和私有空间，线程suspend
 * \param input 线程NULL指针
 * \param VRLS_param 编码配置信息
 * \param VList 解码后的视频帧放入帧链表
 * \param AList 解码后的音频放入帧链表
 * \param privateSpace input线程私有空间
 * \param type 0时解码音视频，1时只解码视频，2时只解码音频
 * \return -1: 失败； 0: 成功
 */
int init_input(HANDLE *input, VRLSParam *VRLS_param, AVFrameList *VList, AVFrameList *AList, void *privateSpace, int type) {
    
    return 0;
}

/**
 * \brief 
 * \param input input线程句柄
 * \param VList 解码后的视频帧放入帧链表
 * \param AList 解码后的音频放入帧链表
 * \param privateSpace input线程私有空间
 * \param type 0时解码音视频，1时只解码视频，2时只解码音频
 * \return -1: 失败； 0: 成功
 */
int activate_input(HANDLE* input, AVFrameList* VList, AVFrameList* AList, void* privateSpace, int type) {
    return  0;
}

/**
 * \brief 线程销毁，释放私有空间
 * \param input input线程句柄
 * \param privateSpace 线程私有空间
 * \return -1: 失败； 0: 成功
 */
int destroy_input(HANDLE* input, void* privateSpace) {


    return 0;
}
