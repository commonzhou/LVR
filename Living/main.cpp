#include "prefix.h"

#include "ControllerConnect.h"
#include "MessageManager.h"
char *IP_SERVER = "192.168.4.97";
int portID = 7747;
#define handleNum 10
int main () {
    HANDLE receiversHandle[handleNum];
    HANDLE sendersHandle[handleNum];
    SOCKET sockets[handleNum] = {0};
    for (int i = 0; i < handleNum;i++) {
        create_socket(IP_SERVER, portID, NULL,&sockets[i]);      
        activate_receive(&receiversHandle[i], NULL, &sockets[i], NULL);
        activate_send(&sendersHandle[i], NULL, &sockets[i], NULL);        
    }
    WaitForMultipleObjects(handleNum, sendersHandle, true, INFINITE);
    WaitForMultipleObjects(handleNum, receiversHandle, true, INFINITE);

    for (int i = 0; i < handleNum;i++) {
        destroy_receive(&receiversHandle[i], NULL);
        destroy_send(&sendersHandle[i], NULL, &sockets[i]);
    }

    return 0;
}