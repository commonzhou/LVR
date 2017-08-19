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
    }
};

struct subMessageList {
    struct MessageNode *pHead;
    struct MessageNode *pTail;
    struct MessageNode *present;
    struct subMessageList *next;
    subMessageList():pHead(NULL),pTail(NULL),present(NULL),next(NULL) {
        /*pHead = new MessageNode();
        pTail = new MessageNode();
        present = new MessageNode();
        next = NULL;*/
    }
    ~subMessageList() {
        MessageNode *node = pHead;
        MessageNode *ptr = pHead;
        while(node != NULL) {
            ptr = node->next;
            delete(node);
            node = node->next;
        }
        pHead = NULL;
        pTail = NULL;
        present = NULL;
    }
};


struct MessageList {
    int streamID;
    struct subMessageList *pSRCL; // Âë¿Ø·´À¡ÐÅÏ¢Á´±í
    struct subMessageList *pRRCL; // Âë¿Ø½ÓÊÕÐÅÏ¢Á´±í
    struct subMessageList *pRCL; // ±àÂëÐÅÏ¢½ø¶ÈÁÐ±í
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


struct MessageManager {
    int StreamNum; // µ±Ç°½ÚµãÒª±àÂëµÄÊýÁ¿, tile+ÒôÆµ+È«Í¼
    struct MessageList *pVHead; // Ã¿¸öencoder¶ÔÓ¦Ò»¸ölist
    struct MessageList *pVTail;

    MessageManager():StreamNum(0) {
        pVHead = new MessageList();
        pVTail = new MessageList();
    }

    ~MessageManager() {
        delete pVHead;
        delete pVTail;
        pVHead = NULL;
        pVTail = NULL;
    }
};


struct InfoNode {
    SOCKET * socketID;
    MessageList *messageList;
    InfoNode():socketID(NULL),messageList(NULL){}
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

int delete_messageManager(MessageManager *messageMag, HANDLE *mutex);


#endif