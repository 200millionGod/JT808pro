#ifndef __VIDEOCLIENT__
#define __VIDEOCLIENT__

#define _UTILS_H
//#ifdef LOG_TAG
//#undef LOG_TAG
//#endif

//#define LOG_TAG 				"VideoClient.h"



typedef struct
{
	unsigned int check;		//0xabcd1234
	unsigned short seq;		//帧序号
	unsigned char nlist;	    //发送缓冲数量
	unsigned char bkey;
	long long tick;		//时间戳
	int len;				// > 0  data长度     0 心跳   -1 连接数满 
	unsigned char data[];
} FramePacket;



typedef void (*result_callback)(const FramePacket* framePacket);
#ifdef __cplusplus
extern "C" {
#endif

/**
*remPort 远程端口
*/
 void init(int remPort);

/**
*摄像头预览
*ip ip地址
*locPort 本地端口
*cameraId 0 前摄，1 后摄
*imainstream 0主码流，1子码流
*callback  数据回调
*/
int videoUploadStartEx(char *ip,int locPort,int cameraId,int imainstream, result_callback callback);
/**
*摄像头预览停止
*cameraId 0 前摄，1 后摄
*/
void videoUploadStopEx(int cameraId);

/**
*前摄预览
*ip ip地址
*locPort 本地端口
*imainstream 0主码流，1子码流
*callback  数据回调
*/
void videoUploadStart(char *ip,int locPort,int imainstream, result_callback callback);
/**
*前摄预览停止
*/
void videoUploadStop();





#ifdef __cplusplus
}
#endif

#endif
