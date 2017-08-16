#ifndef AVPACKETMANAGER_H
#define AVPACKETMANAGER_H
#include "prefix.h"
struct AVPacketManager {
    int StreamNum;
    struct AVPacketList *pVHead;
    struct AVPacketList *pVTail;
    AVPacketManager():StreamNum(0) {
        pVHead = new AVPacketList();
        pVTail = new AVPacketList();
    }
    ~AVPacketManager() {
        delete pVHead;
        delete pVTail;
        pVHead = NULL;
        pVTail = NULL;
    }
};

struct PacketNode {
    UINT8 *bitstream;
    int size;
    UINT64 PTS;
    UINT8 used_flag;
    struct PacketNode *next;
    PacketNode():size(0),PTS(0),used_flag(0) {
        bitstream = NULL;
        next = NULL;
    }
    ~PacketNode() {
        free(bitstream);
        bitstream = NULL;
    }
};

struct AVPacketList {
    MEDIAType type;
    int streamID;
    int tileID;
    struct PacketNode *pVHead;
    struct PacketNode *pVTail;
    struct PacketNode *present;
    struct AVPacketList *next;
    AVPacketList():streamID(0),tileID(0) {
        pVHead = new PacketNode();
        pVTail = new PacketNode();
        present = new PacketNode();
        next = NULL;
    }
    ~AVPacketList() {
        PacketNode *node = pVHead;
        PacketNode *ptr = node;
        while(node != NULL) {
            ptr = node->next;
            delete(node);
            node = node->next;
        }
        pVHead = NULL;
        pVTail = NULL;
        present = NULL;
    }
};


int create_AVPacketManager(AVPacketManager *AVPacketMag,int StreamNum);

int create_packetNode(PacketNode *pNode);

int get_packetNode(PacketNode *pNode,AVPacketList *pList);

int add_packetNode(AVPacketList *pList,PacketNode *pNode,HANDLE *mutex);

int update_AVPacketList(AVPacketList *pList,HANDLE *mutex);

int delete_AVPacketManager(AVPacketManager *AVPacketMag,HANDLE *mutex);

#endif