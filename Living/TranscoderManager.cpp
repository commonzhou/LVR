#include "TranscoderManager.h"



//************************************
// Method:    init_transcoderManager
// FullName:  init_transcoderManager
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: pthread_t * transcoderManager
// Parameter: SOCKET * socketID
// Parameter: MessageList * pList
// Parameter: void * private
//************************************
int init_transcoderManager( pthread_t *transcoderManager, SOCKET *socketID,MessageList *pList,void *private )
{
    int err = pthread_create(
        transcoderManager,NULL,NULL,NULL);
    if(err != 0) {
        return -1;
    }
    
    
    return 0;
}

//************************************
// Method:    activate_transcoderManager
// FullName:  activate_transcoderManager
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: pthread_t * transcoderManager
// Parameter: SOCKET * socketID
// Parameter: MessageList * pList
// Parameter: void * private
//************************************
int activate_transcoderManager( pthread_t *transcoderManager, SOCKET *socketID, MessageList *pList, void *private )
{

}

//************************************
// Method:    destroy_transcoderManager
// FullName:  destroy_transcoderManager
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: pthread_t * transcoderManager
// Parameter: void * private
//************************************
int destroy_transcoderManager( pthread_t *transcoderManager,void *private )
{

}

