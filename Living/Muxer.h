#ifndef MUXER_H
#define MUXER_H
#include "prefix.h"
#include "AVPacketManager.h"
#include "MessageManager.h"
int init_muxer(HANDLE *muxer, AVPacketManager *pPackageMag, VRLSParam *VRLS_param,
    subMessageList *pSubList, void *privateSpace);

int activate_muxer(HANDLE *muxer, AVPacketManager *pPackageMag, VRLSParam *VRLS_param, subMessageList *pSubList, void *privateSpace);

int destroy_muxer(HANDLE *muxer, void *privateSpace);

#endif

