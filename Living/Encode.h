#ifndef ENCODE_H
#define ENCODE_H
#include "prefix.h"
#include "AVFrameManager.h"
#include "AVPacketManager.h"

int init_encode(HANDLE *encode, VRLSParam *VRLS_param, AVFrameList *pAVList, AVPktList *pPlist, void *privateSpace);

int activate_encode(HANDLE *encode, AVFrameList *pAVList, AVPktList *pPlist, void *privateSpace);

int destroy_encode(HANDLE *encode, void *privateSpace);

#endif

