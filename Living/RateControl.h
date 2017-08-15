#ifndef RATECONTROL_H
#define RATECONTROL_H
#include "prefix.h"
#include "AVFrameManager.h"
#include "MessageManager.h"

int init_rateControl(pthread_t *rateControl, VRLSParam *VRLS_param,AVFrameList *pfvlist,MessageList *pmlist, void *private);

int activate_rateControl(pthread_t *rateControl, void *private, AVFrameList *pfvlist, MessageList *pmlist);

int destroy_rateControl(pthread_t *rateControl, void *private);
#endif