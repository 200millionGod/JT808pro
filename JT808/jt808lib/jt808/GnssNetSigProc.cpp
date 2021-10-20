#include <stdio.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include "GnssNetPublic.h"
#include "GnssNetSigProc.h"


CNetSigProc::CNetSigProc()
{
	m_pfTransmitToCopy = NULL;
	m_dwReSendTimes = 3;
	m_dwReplyTimeout = 10;
}

CNetSigProc::~CNetSigProc()
{
}
	

void CNetSigProc::Init()
{
	
}
//协议打包函数
//char* pMsgBody指向canbus、GPS坐标信息，驾驶员信息或者录像文件信息
/*void CNetSigProc::PackMsg(WORD wCmdType,WORD wSerialNum,NetSendMsg* pSig, void* pData, WORD nLen, bool bReSend,WORD nPackTotal, WORD nPackSn)
{   
	if(bReSend)
	{
		pSig->wReSendTimes			  = m_dwReSendTimes;       //重发次数
		pSig->tTimeout				  = m_dwReplyTimeout;       //超时时间间隔
		pSig->tLastVisit			  = time(NULL);                                //最近一次访问的时间
	}
	else
	{
		pSig->wReSendTimes = 0;
		pSig->tTimeout = 0;
		pSig->tLastVisit = 0;
	}
	pSig->wId					  = wCmdType;						//消息id
	pSig->wPacketTotal			  = nPackTotal; 					// 封包总数
	pSig->wPacketSn 			  = nPackSn;						// 包序列号
	pSig->wSerialNumber 		  = wSerialNum;                     //消息流水号

	if(m_pfTransmitToCopy){
		pSig->wTotalLen = m_pfTransmitToCopy(pSig->pData,pData,nLen);
	} else {
		pSig->pData = (char *)pData;
		pSig->wTotalLen = nLen;
	}	

}*/

void CNetSigProc::PackMsg(WORD wCmdType, WORD wSerialNum, NetSendMsg* pSig, void* szData, WORD nLen, bool bReSend,WORD nPackTotal, WORD nPackSn)
{
    if (bReSend) {
        pSig->wReSendTimes = m_dwReSendTimes;
        pSig->tTimeout = m_dwReplyTimeout;
        pSig->tLastVisit = time(NULL);
    } else {
        pSig->wReSendTimes = 0;
        pSig->tTimeout = 0;
        pSig->tLastVisit = 0;
    }

	pSig->wId					  = wCmdType;						//消息id
	pSig->wPacketTotal			  = nPackTotal; 					// 封包总数
	pSig->wPacketSn 			  = nPackSn;						// 包序列号
	pSig->wSerialNumber 		  = wSerialNum;                     //消息流水号

    pSig->wTotalLen = nLen;
    memcpy(pSig->pData, szData, nLen);
}

void CNetSigProc::SetNetParam(DWORD timeout,DWORD sendTimes)
{
	m_dwReplyTimeout = timeout;
	m_dwReSendTimes  = sendTimes;
}

