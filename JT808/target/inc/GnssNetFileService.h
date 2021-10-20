
#ifndef _GNSS_NET_FILE_SERVICE_H_
#define _GNSS_NET_FILE_SERVICE_H_

#include <list>
#include <arpa/inet.h>

#include "ThreadFrame.h"
#include "GnssNetPublic.h"
#include "MutexLock.h"
#include "GnssNetProtocol.h"
#include "GnssNetMsgSendRcv.h"
#include "Interface.h"
#include "GnssNetSigProc.h"
#include "JT808ProtocolPackage.h"
#include "JT808ProtocolParse.h"
#include <vector>
//#include "MediaService.h"

using namespace std;

#pragma pack(4)

class CNetFileService
{
private:                          //private表示成员只提供给本类使用，本类以外的函数无法使用
	bool m_bNetAlive;
	bool m_bFinished;
	int     m_nSockType;                // socket  类型
	vector<File_Server_Config> m_stServerList;
	vector<FileInfoS> m_stFileDetailList;
	int m_nSendIndex;
	vector<FileUploadReply_S> m_stReplyList;
	vector<ComReply_S> m_stComReplyList;
private:                 	
	CPthreadFrame m_NetFileServiceThread;

	NetSendMsg 	   m_sendMsg;
	/*接收发送消息*/
	CNetMsgSendRcv  m_sendRcv;

	/* 对网络信号处理,解析和封装消息*/
	CNetSigProc   	m_SigProc;
	CJT808Package	m_SigPackage;
	CJT808Parse		m_sigParse;	
	CMutexLock m_mutex;
	int   		m_nStep;
	bool		m_bStart;
	bool 		m_bReConnected;
	int 		m_nConnectCount;
private:	

	
	static void* NetFileServiceThread(void *pUsrData);
	void* NetFileServiceProcess();

	void ProcRcvMsg(NetRcvMsg* pMsg);

	
	/***********************************************************
	* 函数功能: 终端通用应答
	* 输入参数: 
	* @param  wCmdId   消息类型
	* @param  wSerialNumber   序列号
	* @param  wId 
	* @param  bResult
	* 输出参数: 
	* 返回参数:  通用应答长度
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void CommReply( WORD wSerialNumber, WORD wId, BYTE bResult);
	
	/***********************************************************
	* 函数功能: 处理中心回复
	* 输入参数: 
	* @param msgInfo 消息信息
	* 输出参数: 
	* 返回参数:
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void ProcCmsReply(NetRcvMsg* msgInfo);
	void FileUpdateFinish(NetRcvMsg* pMsg);
	bool UploadAttachmentInfo(File_Server_Config& stFileServerConfig, int nBeginIndex);
	bool UploadFile(int index);
	void UploadFileInfo(int id, int index);
	int UploadFileData(int index);
	int islocked(const char* filename);
	off_t get_file_size(const char* filename);
	void create_file_list(const File_Server_Config& stFileServerConfig, vector<FileInfoS>& stFileDetailList, int nBeginIndex);
	int reSendfunc(void);
	bool reConnect(void);

public:                                                                  //public表示以下提供给外部的调用

	CNetFileService();
	virtual ~CNetFileService();
	bool StartService();
	bool StopService();
	void InitService(InterFace *pInterFace, Net_Communication_Attr& param);
	/***********************************************************
	* 函数功能: 向中心发送消息
	* 输入参数: 
	* @param dwCmdType  信令类型
	* @param pMsgBody     信息体内容
	* @param nLen             消息体长度
	   @param nPackTotal   一帧消息总共分几个包
	   @param nPackSn       现在发送第几个包
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	DWORD SendSig(WORD wCmdType, WORD wSerialNum,void* pMsgBody = NULL, WORD nLen = 0, bool bReSend=true,WORD nPackTotal = 0, WORD nPackSn = 0);
	
	DWORD Send(WORD wCmdType,WORD wSerialNum,void* pMsgBody, WORD nLen, bool bReSend, WORD nPackTotal, WORD nPackSn);

	void SetServerConfig(File_Server_Config& stFileServerConfig);
};

#endif //_GNSS_NET_FILE_SERVICE_H_
 
