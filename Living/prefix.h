#ifndef PREFIX_H
#define PREFIX_H
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <pthread.h>
#pragma comment(lib,"pthreadVC2.lib")
#pragma comment(lib,"ws2_32.lib")

#define debug_print(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
    __LINE__, __func__, __VA_ARGS__); } while (0)



enum MEDIAType {
    MEDIA_AUDIO = 1,
    MEDIA_VIDEO,
    MEDIA_NONE
};

enum StreamingType {
    ST_RTMP = 1,
    ST_HLS = 2,
    ST_DASH
};

enum TileMappingType {
    Tile42 = 1
};

enum Type {
    Audio = 0,
    SmallCube,
    Tile
};

enum CodecType {
    CT_H264 = 1,
    CUDA_H264,
    CT_H265,
    CT_AAC
};

enum ProjectionType {
    ERP = 0,
    CUBE
};

enum EncodePreset {
    faster = 1,
    veryfast,
    ultrafast
};

struct EncNodeParam {
    Type encodeType;
    ProjectionType projection;
    int Tile_ID;
    CodecType vcodec;
    EncodePreset preset;
    int width;
    int height;
    int fps;
    int bitrate;
    int GOP;
    CodecType acodec;
    int AudioSampleRate;
    int AudioBitrate;
    int AudioChannle;
};

// 
struct TranscoderParam {
    char IP[16];
    int EncNum;
    struct EncNodeParam *EncNode;
    struct TranscoderParam *next;
};

// 这是从XML文件中读取出来的配置文件,控制着每个计算节点
struct VRLSParam {
    StreamingType inputFormat;
    StreamingType outputFormat;
    TileMappingType tileMapping;
    UINT32 StreamID;
    UINT8 stereoscopic;
    char inputAdd[1024];
    char outputAdd[1024];
    char mpdName[1024];
    int transcoderNodeNum;
    int serverport;
    struct TranscoderParam *TransParam;
};



struct EncodingParam {
    UINT8 paramType;
    UINT32 value;
};


struct InitialPayload {
    UINT8 encNum;
    UINT8 paramNum;
    struct EncodingParam *params; 
};

struct TLV {
    UINT8 type;
    UINT32 length;
    void * payload;
};


struct EncodeSpeed {
    long outputStreamIndex;
    UINT64 pts;
    UINT8 averageFrameRate;
    UINT8 instantFrameRate;
};
#endif

