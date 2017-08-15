#ifndef PREFIX_H
#define PREFIX_H
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <pthread.h>
#include <WinSock2.h>
#pragma comment(lib,"pthreadVC2.lib")
#pragma comment(lib,"ws2_32.lib")
enum MEDIAType {
    MEDIA_AUDIO = 1,
    MEDIA_VIDEO
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

struct TranscoderParam {
    char IP[16];
    int EncNum;
    struct EncNodeParam *EncNode;
    struct TranscoderParam *next;
};

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



#endif

