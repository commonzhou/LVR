#ifndef RATECONTROL_H
#define RATECONTROL_H
#include "prefix.h"
#include "AVFrameManager.h"
#include "MessageManager.h"

int init_rateControl(HANDLE *rateControl, VRLSParam *VRLS_param,AVFrameList *pfvlist,MessageList *pmlist, void *privateSpace);

int activate_rateControl(HANDLE *rateControl, void *privateSpace, AVFrameList *pfvlist, MessageList *pmlist);

int destroy_rateControl(HANDLE *rateControl, void *privateSpace );
#endif