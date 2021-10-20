#include <string.h>
#include <stdio.h>
#include <time.h>
#include <arpa/inet.h>

#include "GnssNetPublic.h"
#include "GnssNetSigParsePack.h"
#include "debug_log.h"

BYTE CNetSigParsePack::m_FromMem1[] = {0x7d, 0x02};
BYTE CNetSigParsePack::m_ToMem1[]   = { 0x7e };
BYTE CNetSigParsePack::m_FromMem2[] = {0x7d, 0x01};
BYTE CNetSigParsePack::m_ToMem2[] = { 0x7d };

int CNetSigParsePack::Parse(unsigned char* pBuf, uint16 nMsgLen, NetRcvMsg* pMsg)
{
	int nOffset = 0;

	/*get msg head info*/
	NetMsgHead msgHead;	
	nOffset += 1;							
	printf("pbuf in Parse------------:%d\n", nMsgLen);
	for(int i = 0; i < nMsgLen; ++ i){
		printf("%02X ", pBuf[i]);
	}
	printf("\n");
	memcpy(&msgHead, pBuf + nOffset, sizeof(NetMsgHead));   
	pMsg->wId = ntohs(msgHead.wId);                                                    //消息id
	pMsg->wVer = ntohs(msgHead.wVer); 
	pMsg->wSerialNumber = ntohs(msgHead.wSerialNumber);       //消息流水号
	/*get msg body property*/
	NET_MSG_ATTR_S     msgAttr;					/*实例化消息体属性DWORD*/
	WORD  wProperty = ntohs(msgHead.wProperty);         //转换为主机字节序
	memcpy(&msgAttr.MsgAttr, &wProperty, sizeof(WORD));         

	pMsg->wEncryptType = msgAttr.m_MsgAttr.wEncryptType;         //获取加密类型
	pMsg->wMsgLen      = msgAttr.m_MsgAttr.wMsgLen;
	if (pMsg->wMsgLen > nMsgLen)                                    //消息体内容长度>本帧数据长度
	{
		DBG_LEVEL_6("msg length has error.");
		return -1;
	}

	nOffset += sizeof(NetMsgHead);
	if (msgAttr.m_MsgAttr.wPack == 1)         //如果数据帧有分包
	{
		/*get packet info*/
		NetMsgHeadPack msgPack = {0};
		memcpy(&msgPack, pBuf + nOffset, sizeof(NetMsgHeadPack));
		pMsg->wPacketTotal = ntohs(msgPack.wPacketTotal);
		pMsg->wPacketSn    = ntohs(msgPack.wSerialNuber);
		nOffset += sizeof(NetMsgHeadPack);
	}
	else
	{
		pMsg->wPacketTotal = 1;                 //标记包的总数为1
	}

	/*  msg content pointer assign*/
	memcpy(pMsg->pData, (char*)pBuf + nOffset, pMsg->wMsgLen);      //拷贝消息体所有数据
	/*get check code*/
	nOffset += pMsg->wMsgLen;
	memcpy(&pMsg->nCheckCode, pBuf + nOffset, 1);                        //拷贝校验和CheckSum
	
	/*set receive msg time*/
	pMsg->tLastVisit = time(NULL);                       //标记接收数据时间

	return 0;
}
	
void CNetSigParsePack::RevertMsg(unsigned char* pBuf, uint32& nLen)
{
	mem_replace(pBuf, nLen,  m_FromMem1, 2, m_ToMem1, 1);
	mem_replace(pBuf, nLen,  m_FromMem2, 2, m_ToMem2, 1);
}

