/******************************************************************************
  Copyright (C), 2010-2020, Shenzhen Wekomp Technology Co., Ltd.
******************************************************************************
File Name     :NetSigParsePack.h
Version       : 
Author        : ouyangming
Created       : 2011/12/08
Last Modified :
Description   :实现消息封装解压基类
Function List :
History       :

******************************************************************************/
#ifndef _GNSS_MOC_NET_SIG_PARSE_PACK_H_
#define _GNSS_MOC_NET_SIG_PARSE_PACK_H_


#include "Defines.h"
#include "GnssNetProtocol.h"


class CNetSigParsePack
{
public:
	int Parse(unsigned char* pBuf, uint16 nMsgLen, NetRcvMsg* pMsg);
	uint32 Package(uint32 dwCmdType,void* pBuffer,void* pData,uint32 nLen,uint16 nSerialNumber,uint16 nPackTotal, uint16 nPackSn);
	static void RevertMsg(unsigned char* pBuf, uint32& nLen);
private:
	static BYTE m_FromMem1[4];
	static BYTE m_ToMem1[4];
	static BYTE m_FromMem2[4];
	static BYTE m_ToMem2[4];
};

#endif

