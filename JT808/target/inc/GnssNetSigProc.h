/******************************************************************************
  Copyright (C), 2010-2020, Shenzhen Wekomp Technology Co., Ltd.
******************************************************************************
File Name     :NetSigPack.h
Version       : 
Author        : ouyangming
Created       : 2011/12/08
Last Modified :
Description   :实现功能封装成网络包
Function List :
History       :
******************************************************************************/
#ifndef _GNSS_MOC_NET_SIG_PROC_H_
#define _GNSS_MOC_NET_SIG_PROC_H_

#include "GnssNetProtocol.h"
#include "GnssNetPublic.h"


typedef WORD (*TransmitToCopyCallBack)(void* src,void* dst,WORD len);

class CNetSigProc
{

public:
	/***********************************************************
	* 函数功能: 构造函数，析构函数
	* 输入参数: 
	* 输出参数: 
	* 返回参数:
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	CNetSigProc();
	virtual ~CNetSigProc();
	
	
	/***********************************************************
	* 函数功能:  传入外部设备信息获取接口
	* 输入参数: 
	* 输出参数: 
	* 返回参数: true:成功，false:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void Init();
	
	//配置消息转义拷贝回调
	void SetTransmitToCopyCB(TransmitToCopyCallBack pfTransmitToCopy){m_pfTransmitToCopy=pfTransmitToCopy;}

	/***********************************************************
	*函数功能 :  终端鉴权
	*输入参数 :
	* @param  dwCmdType  命令类型
	* @param  pSig              鉴权消息
	* @param  nPackTotal    消息包的总数
	* @param  pMsgBody     消息体信息
	* 输出参数:   
	* 返回参数:
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void PackMsg(WORD wCmdType,WORD wSerialNum,NetSendMsg* pSig, void* pData, WORD nLen, bool bReSend,WORD nPackTotal, WORD nPackSn);
	
	/***********************************************************
	*函数功能 : 设置网络参数
	*输入参数 : 
	* 输出参数:   
	* 返回参数:
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void SetNetParam(DWORD timeout,DWORD sendTimes);

private:
	DWORD          	m_dwReplyTimeout;           // 应答超时
	DWORD          	m_dwReSendTimes;            // 消息重传的次数
	TransmitToCopyCallBack m_pfTransmitToCopy;
};

#endif

