#ifndef AVPACKETMANAGER_H
#define AVPACKETMANAGER_H
#include "prefix.h"


struct PacketNode {
    UINT8 *bitstream;
    int size;
    UINT64 PTS;
    UINT8 used_flag;
    struct PacketNode *next;
    PacketNode():bitstream(NULL),size(0),PTS(0),used_flag(0),next(NULL) {
       
    }
    ~PacketNode() {
        free(bitstream);
        bitstream = NULL;
    }
};

struct AVPktList {
    MEDIAType type;
    int streamID;
    int tileID;
    struct PacketNode *pVHead;
    struct PacketNode *pVTail;
    struct PacketNode *present;
    struct AVPktList *next;
    AVPktList():streamID(0),tileID(0) {
        type = MEDIA_NONE;
        pVHead = new PacketNode();
        pVTail = pVHead;
        present = NULL;
        next = NULL;
    }
    ~AVPktList() {
        PacketNode *node = pVHead;
        PacketNode *ptr = node;
        while(node != NULL) {
            ptr = node->next;
            delete(node);
            node = ptr;
        }
        pVHead = NULL;
        pVTail = NULL;
        present = NULL;
        next = NULL;
    }
};

struct AVPacketManager {
    int StreamNum;
    struct AVPktList *pVHead;
    struct AVPktList *pVTail;
    AVPacketManager() :StreamNum(0) {
        pVHead = new AVPktList();
        pVTail = pVHead;
    }
    ~AVPacketManager() {
        delete pVHead;
        pVHead = NULL;
        pVTail = NULL;
    }
};

int create_AVPacketManager(AVPacketManager*& AVPacketMag, int StreamNum);

int create_packetNode(PacketNode*& pNode);

int get_packetNode(PacketNode*& pNode, AVPktList *pList);

int add_packetNode(AVPktList *pList,PacketNode *pNode,HANDLE *mutex);

int update_AVPacketList(AVPktList *pList,HANDLE *mutex);

int delete_AVPacketManager(AVPacketManager*& AVPacketMag, HANDLE *mutex);

#endif