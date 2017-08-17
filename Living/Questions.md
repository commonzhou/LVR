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



    经过调研，通过TCP/IP的Socket最好不用传递Struct类型的变量，由于不同机器之间的架构不同或者是编译器不同，因此解析数据可能会有差错。
    推荐的方法是将要传送的数据序列化

    使用#pragma pack将struct进行字节对齐

    为了简单起见,暂时先不考虑这些因素,而是认为所有机器均采用相同的编译器与架构.

