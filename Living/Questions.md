# 消息传递机制
消息在Controller与Transcoder之间进行传递.

## Controller/transcoderManager

### 发送

    // 初始化编码参数信息, type=0x01, payload = InitPayload
    struct TLV {
        UINT8 type;
        UINT32 length;
        void * payload;
    };


    // 编码参数
    struct TranscoderParam {
        char IP[16];
        int EncNum;
        struct EncNodeParam *EncNode;
        struct TranscoderParam *next;
    };

    // 终止编码,type=0x04,payload=NULL,lenght=0
    struct TLV {
        UINT8 type;
        UINT32 length;
        void * payload;
    };

### 接收
    // 初始化结果参数, type = 0x41, payload = 0或1
    struct TLV {
        UINT8 type;
        UINT32 length;
        void * payload;
    };



##　controllerConnect/Transcoder

### 发送
    // 初始化结果参数, type = 0x41, payload = 0或1
    struct TLV {
        UINT8 type;
        UINT32 length;
        void * payload;
    };

### 接收
    // 初始化编码参数信息, type=0x01, payload = InitPayload
    struct TLV {
        UINT8 type;
        UINT32 length;
        void * payload;
    };


    // 编码参数
    struct TranscoderParam {
        char IP[16];
        int EncNum;
        struct EncNodeParam *EncNode;
        struct TranscoderParam *next;
    };


    // 终止编码,type=0x04,payload=NULL,lenght=0
    struct TLV {
        UINT8 type;
        UINT32 length;
        void * payload;
    };

