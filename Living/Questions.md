# 问题汇集
1. 

     int init_transcoderManager(pthread_t *transcoderManager, SOCKET *socketID, MessageList *pList ,void *privateSpace)

创建这个线程之后，该给他传递什么参数？每一个`transcoderManager`对应一个Transcoder的话，也就是说这个manager管理着transcoder和controller之间的socket连接，通过这个socket连接在两者之间交换信息，那么这个manager实际调用的函数应该知道每个连接的socket和传进来的MessageList。在实际调用的函数中，一边发送编码参数信息（**这个参数从哪里来**），一边接收信息，然后对信息进行解析，如果接收到的是编码进度信息，生成编码进度信息链表，如果解析到的是码控反馈信息，那么生成码控反馈信息链表。同时需要管理码控反馈信息链表和编码进度信息链表，建立、更新、删除。

2. 上面说的发送与接收的信息的具体的数据结构是什么样子？能否确定

