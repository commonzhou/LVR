#ifndef INPUT_H
#define INPUT_H
#include "prefix.h"
#include "AVFrameManager.h"

int init_input(HANDLE *input, VRLSParam *VRLS_param, AVFrameList *VList, AVFrameList *AList, void *privateSpace, int type);

int activate_input(HANDLE *input, AVFrameList *VList, AVFrameList *AList, void *privateSpace, int type);

int destroy_input(HANDLE *input, void *privateSpace);
#endif

