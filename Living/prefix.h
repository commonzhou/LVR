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
#include <string>
#pragma comment(lib,"ws2_32.lib")

#define SOCKETBUFFERSIZE 4096

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
    char resolution[16];
    int fps;
    int bitrate;
    int GOP;
    CodecType acodec;
    int AudioSampleRate;
    int AudioBitrate;
    int AudioChannle;
    EncNodeParam *next;
};


struct TranscoderParam {
    int TransID;
    int EncNum;
    struct EncNodeParam *EncNode;
    struct TranscoderParam *next;
};

// 这是从XML文件中读取出来的配置文件,控制着每个计算节点
struct VRLSParam {
    StreamingType inputFormat;
    StreamingType outputFormat;
    TileMappingType tileMapping;
    
    UINT8 stereoscopic;
    char inputAdd[1024];
    char outputAdd[1024];
    char serverIP[16];
    int serverport;
    int clusterNum;
    struct TranscoderParam *TransParam;
};

struct Param {
    UINT8 paramType;
    UINT32 value;
};

struct EncodingParam {
    UINT8 paramType;
    UINT32 value;
};

struct ParamNode {
    UINT8 paramNum;
    struct EncodingParam *param;
};

struct EncodeSpeed {
    long outputStreamIndex;
    UINT64 pts;
    UINT8 averageFrameRate;
    UINT8 instantFrameRate;
};
#endif

