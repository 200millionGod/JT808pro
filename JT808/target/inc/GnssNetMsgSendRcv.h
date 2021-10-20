/******************************************************************************
  Copyright (C), 2010-2020, Shenzhen Wekomp Technology Co., Ltd.
******************************************************************************
File Name      :MocNetMsgSendRcv.h
Version         : 
Author           : ouyangming
Created        : 2011/12/22
Last Modified :
Description    :队列操作和以太网收发处理
Function List  :
History         :  
******************************************************************************/
#ifndef __GNSS_MOC_NET_MSG_SEND_RCV_H__
#define __GNSS_MOC_NET_MSG_SEND_RCV_H__

#include <pthread.h>
#include "GnssNetSocketOp.h"
#include "GnssNetFifo.h"
#include "GnssNetProtocol.h"
#include "GnssNetMsgList.h"
#include "GnssNetSigParsePack.h"
#include "ThreadFrame.h"


class CNetMsgSendRcv
{

public:
	/***********************************************************
	* 函数功能:  构造，析构函数
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者:
	************************************************************/
	CNetMsgSendRcv();
	~CNetMsgSendRcv();

	/***********************************************************
	* 函数功能:  初始化函数
	* 输入参数: 
	* param  nSockType    sock类型
	* param  pSrvIp          服务器ip
	* param  nSrvPort       服务器端口
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool Init(int socket_type);

	/***********************************************************
	* 函数功能:  启动线程服务
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool StartService();
	

	/***********************************************************
	* 函数功能:  停止线程服务
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool StopService();


	/***********************************************************
	* 函数功能:  从接收队列中获取一个消息
	* 输入参数: 
	* 输出参数: 
	* 返回参数:  
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	NetRcvMsg* GetMsg();	
	
	/***********************************************************
	* 函数功能:  添加一个消息到发送队列
	* 输入参数: 
	* param  pMsg    待发送的消息
	* 输出参数: 
	* 返回参数:  0:成功，1:消息队列以满
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	int SendMsg(NetSendMsg* pMsg);

	/***********************************************************
	* 函数功能:  根据序列号删除发送队列消息
	* 输入参数: 
	* param      nSerailNum 序列号
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool DelMsgBySN(int nSerailNum);


	/***********************************************************
	* 函数功能:  根据命令id删除发送队列消息
	* 输入参数: 
	* param      nCmdId  命令id
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool DelMsgByCmdId(int nCmdId);

	/***********************************************************
	* 函数功能:  设置鉴权状态
	* 输入参数: 
	* param      bMark
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	void SetAuthorizeStatus(bool bMark);

	/***********************************************************
	* 函数功能:  更新网络参数
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	void SetNetParam(DWORD timeout,DWORD sendTimes);

	/***********************************************************
	* 函数功能:  检查终端与服务器的连接是否正常
	* 输入参数: 
	* 输出参数: 
	* 返回参数:  0 未连接或者已断开，1 已经连接
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool ServerIsLinked(void);
	void CloseConnect(void);

	/***********************************************************
	* 函数功能:  链接服务器
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool Link2Svr(const char* pAddr,int port,int nTimeOut=10000);
	

	int SendData(const char *data, int size);
private:

	/***********************************************************
	* 函数功能:  发送消息线程
	* 输入参数: 
	* pData  调用对象
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	static void* SendThread(void* pData);

	
	/***********************************************************
	* 函数功能:  发送处理
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	void*  SendProc();


	/***********************************************************
	* 函数功能:  接收消息线程
	* 输入参数: 
	* pData  调用对象
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者:
	************************************************************/
	static void* RcvThread(void* pData);

	
	/***********************************************************
	* 函数功能:  发送处理
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	void*  RcvProc();

	/***********************************************************
	* 函数功能:  接收消息线程
	* 输入参数: 
	* pData  调用对象
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/

	
	static void* TransThread(void* pData);
	
	/***********************************************************
	* 函数功能:  发送处理
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	void*  TransProc();

private:
	//本类中须要用到的其他的类，在此声明
	NetRcvMsg*              m_pRcvMsg;         //接收消息
	CNetMsgList<NetSendMsg> m_lstSend;         //将消息放入发送队列，由其他线程发送该消息
	CNetMsgList<NetRcvMsg>  m_lstRcv;          //保存接收消息队列，由其他线程取出该消息，根据消息命令读写设备配置
	CFifo     				m_RcvBuf;          //消息接收缓冲池
	CSocketOp  			    m_Socket;          //建立socket通信API接口
	int                     m_dwReplyTimeout;  // 应答超时
	int                     m_dwReSendTimes;   // 消息重传的次数
	
	CPthreadFrame           m_sendThread;      //消息发送线程 创建和终止控制相关
	CPthreadFrame           m_rcvThread;       //消息接收线程创建和终止控制相关
	CPthreadFrame           m_transThread;     //消息转换处理线程创建和终止控制相关
	CNetSigParsePack        m_parse;           //消息解析类

	bool                    m_bAuthorized;     //鉴权成功/失败标志
	char 					m_szServerAddr[16];
	int 					m_nPort;
	int 					m_nConnectTimeOut;
	pthread_mutex_t			m_stBufResetMutex;
};

#endif

