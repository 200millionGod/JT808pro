#ifndef _GNSS_MOC_PUBLIC_H_
#define _GNSS_MOC_PUBLIC_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

#include "Interface.h"
#include "GnssNetLog.h"


#define MOC_NETWORK_DEBUG
#ifdef  MOC_NETWORK_DEBUG
#include <iostream>
using namespace std;
#endif


const int BUF_LEN_S    =      8;
const int BUF_LEN_M    =      16;
const int BUF_LEN_B    =      32;
const int BUF_LEN_X    =      64;
const int BUF_LEN_64    =      64;


#define SAFE_DELETE_OBJECT(p)  do {if ((p) != NULL) {delete (p); (p) = NULL;}}while(0)
#define SAFE_DELETE_ARRAY(p)   do {if ((p) != NULL) {delete [] (p); (p) = NULL;}} while(0)

#define byte2gb(x) (x / (1024 * 1024 * 1024))
#define kb2gb(x) (x / (1024 * 1024))

#define SETBIT(x,y)			((x) = ((x)|(1<<(y))))
#define CLSBIT(x,y)			((x) = ((x)&(~(1<<(y)))))

extern InterFace * g_pMocNetInterface;


class CNetLock
{
	bool m_bLock;
public:
	CNetLock(){m_bLock = false;}
	void Lock(){while(m_bLock) sleep(1); m_bLock = true;}
	void UnLock(){m_bLock = false;}
};


/********************************************zj  new add*****************/
/*内存分配单位K*/
const int  MEM_SIZE_K   = 1024;

/*消息发送缓冲区,内存分配大小*/
const  unsigned int MSG_BUF_SIZE   =  MEM_SIZE_K*2;     

/*消息接收，发送链表大小*/
const int MSG_RCV_SEND_LIST_SIZE = 120;

/*循环buffer 大小*/
const int FIFO_BUFFER_SIZE   = MEM_SIZE_K * 32;

/*接收消息缓冲区大小*/
const int PACK_BUFFER_SIZE   = MEM_SIZE_K*4;

/*网络通信类型*/
//const int NET_SOCK_TYPE   = SOCK_STREAM;

/*tcp 分包大小*/
const int TCP_PACK_SIZE  = 1024;

/*udp 分包大小*/
const int UDP_PACK_SIZE  = 512;

/*网络超时时间间隔(单位s)*/
const int NETWORK_TIMEOUT =120;  

/* 系统设置的命令id个数*/
const int SYS_CFG_CMD_NUM  =   59;


/*控制info类型的日志是否输出*/
enum  NET_LOG_SWITCH{
	NET_SWITCH_ON,       
	NET_TIPS_OFF,
	NET_INFO_OFF,
};
#define     NET_LOG_SWITCH      NET_SWITCH_ON           


#define 	NET_LOG_INFO       ("消息.")  //  基本信息
#define  	NET_LOG_WARNING    ("警告!")  // 告警信息
#define 	NET_LOG_ERR        ("错误!")  // 错误信息

/*定义日志输出宏*/
#define     PUT2LOG(level,  ...)    PutToLog(__FUNCTION__, level, __VA_ARGS__)

/*输出进入函数体信息到日志*/
#define     PUT2LOG_ENTRY_FUN     PUT2LOG(NET_LOG_INFO, "Entry point.")

/***********************************************************
*函数功能 : 日志输出
*输入参数 :
* @param  leve  		告警类型
* @param  funName	所传入的函数名字
* @param  content 	日志内容
* 输出参数:   
* 返回参数:
* 使用说明:无
* 参考函数:
* 创建作者:
************************************************************/
void  PutToLog(const char* funName, const char* level,const char* content, ...);



/***********************************************************/
/**  
*函数功能:内存查找
*输入参数: 
* @param   null 
* 
* 输出参数: 
* 返回参数: 
* 使用说明:无
* 参考函数:
* 创建作者:
************************************************************/
unsigned char*	mem_search(unsigned char*  src, UINT len, const unsigned char* findMem, UINT findLen);



/***********************************************************/
/**  
*函数功能:内存替换
*输入参数: 
* @param   null 
* 
* 输出参数: 
* 返回参数: 
* 使用说明:无
* 参考函数:
* 创建作者:
************************************************************/
unsigned char*	mem_replace(unsigned char*	src, UINT &len, const unsigned char * oldMem, UINT oldLen, const unsigned char*	newMem, UINT newLen);


#endif

