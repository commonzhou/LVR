#include "prefix.h"
#include "ControllerConnect.h"
#include "MessageManager.h"
char *IP_SERVER = "192.168.4.97";
//char *IP_SERVER = "127.0.0.1";
int portID = 7747;
#define handleNum 2
#define transcoderNum 2

int main() {
    HANDLE receiversHandle[handleNum];
    HANDLE sendersHandle[handleNum];
    SOCKET sockets[handleNum] = {0};

    MessageManager *messageManager = NULL;
    create_messageManager(messageManager, 0);
    MessageNode *node = NULL;
    create_messageNode(node);
    node->used_flag = 0;
    node->next = NULL;
    node->size = 1;
    node->CString = (INT8 *)malloc(sizeof(INT8)+1);
    char value = 0x41;
    memcpy(node->CString, &value, sizeof(char));
    node->CString[sizeof(char)] = '\0';
    add_messageNode(messageManager->pVHead->pRCL, node, NULL);

    node = NULL;
    create_messageNode(node);
    node->used_flag = 0;
    node->next = NULL;
    node->size = 1;
    node->CString = (INT8 *)malloc(sizeof(INT8)+1);
    value = 0x42;
    memcpy(node->CString, &value, sizeof(char));
    node->CString[sizeof(char)] = '\0';
    add_messageNode(messageManager->pVHead->pRCL, node, NULL);

    for (int i = 0; i < handleNum;i++) {
        create_socket(IP_SERVER, portID, NULL,&sockets[i]);   
        activate_send(&sendersHandle[i], NULL, &sockets[i], messageManager->pVHead);
        activate_receive(&receiversHandle[i], NULL, &sockets[i], messageManager->pVHead);        
    }


    WaitForMultipleObjects(handleNum, sendersHandle, true, INFINITE);
    WaitForMultipleObjects(handleNum, receiversHandle, true, 200);

    for (int i = 0; i < handleNum;i++) {
        destroy_send(&sendersHandle[i], NULL, &sockets[i]);
        destroy_receive(&receiversHandle[i], NULL);

    }

    return 0;
}