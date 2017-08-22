#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H
#include "prefix.h"

struct MessageNode {
    INT8 *CString;
    int size;
    int used_flag;
    struct MessageNode *next;
    MessageNode(): CString(NULL),size(0),used_flag(0),next(NULL){
        
    }
    ~MessageNode() {
        free(CString);        
        CString = NULL;
        next = NULL;
    }
};

struct subMessageList {
    struct MessageNode *pHead;
    struct MessageNode *pTail;
    struct MessageNode *present;
    struct subMessageList *next;
    subMessageList() {
        pHead = new MessageNode();
        pTail = pHead;
        present = NULL;
        next = NULL;
    }
    ~subMessageList() {
        MessageNode *node = pHead;
        MessageNode *ptr = node;
        while(node != NULL) {
            ptr = node->next;
            delete(node);
            node = ptr;
        }
        pHead = NULL;
        pTail = NULL;
        present = NULL;
    }
};


struct MessageList {
    int streamID;
    struct subMessageList *pSRCL; // ��ط�����Ϣ����
    struct subMessageList *pRRCL; // ��ؽ�����Ϣ����
    struct subMessageList *pRCL; // ������Ϣ�����б�
    struct MessageList *next;

    MessageList():streamID(0) {
        pSRCL = new subMessageList();
        pRRCL = new subMessageList();
        pRCL = new subMessageList();
        next = NULL;
    }
    ~MessageList() {
        delete pSRCL;
        delete pRRCL;
        delete pRCL;
        pSRCL = NULL;
        pRRCL = NULL;
        pRCL = NULL;
    }
};

// TODO: ÿ��һ��Transcoder�ڵ㣬MessageManager�е�����Ӧ�þͶ�һ��Ԫ��
struct MessageManager {
    int StreamNum; // ��ǰ�ڵ�Ҫ���������, tile+��Ƶ+ȫͼ
    struct MessageList *pVHead; // ÿ��encoder��Ӧһ��list
    struct MessageList *pVTail;

    MessageManager():StreamNum(0) {
        pVHead = new MessageList();
        pVTail = pVHead;       
    }

    ~MessageManager() {
        MessageList *node = pVHead;
        MessageList *ptr = node;
        while(node != NULL) {
            ptr = node->next;
            delete(node);
            node = ptr;
        }
        pVHead = NULL;
        pVTail = NULL;
        
    }
};


struct InfoNode {
    SOCKET * socketID;
    MessageList *messageList;
    InfoNode():socketID(NULL),messageList(NULL) {
        
    }
    ~InfoNode() {
        if (!socketID) {
            delete socketID;
            socketID = NULL;
        }
        if(! messageList) {
            delete messageList;
            messageList = NULL;
        }
    }
};



int create_messageManager(MessageManager*& messageMag, int StreamNum);

int create_messageNode(MessageNode*& pNode);

int get_messageNode(MessageNode*& pNode, subMessageList *pList);

int add_messageNode(subMessageList *pList,MessageNode *message,HANDLE *mutex);

int update_messageNode(subMessageList *pList, HANDLE *mutex);

int delete_messageManager(MessageManager*& messageMag, HANDLE *mutex);


#endif