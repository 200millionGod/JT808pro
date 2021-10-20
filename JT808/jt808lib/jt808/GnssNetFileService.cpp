
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/sockios.h>
#include <linux/rtc.h>
#include <errno.h>
#include <fcntl.h>
#include <errno.h>

#include "Defines.h"
#include "GnssNetFileService.h"
#include "GnssNetSocket.h"
#include "GnssNetProtocol.h"
#include "debug_log.h"

enum {
	REPLY_STEP_NONE = 0,
	REPLY_STEP_ALARM_UPLOAD = 1,
	REPLY_STEP_FILE_INFO_UPLOAD = 2,
	REPLY_STEP_FILE_UPLOAD = 3,
};

CNetFileService::CNetFileService()
{
	m_stServerList.clear();
	m_bFinished = false;
	m_nSendIndex = 0;
	m_nStep = REPLY_STEP_NONE;
	m_nConnectCount = 0;
	m_stReplyList.clear();
}
  
CNetFileService::~CNetFileService()
{
	m_stReplyList.clear();
	m_stServerList.clear();
	m_stFileDetailList.clear();

	StopService();
	SAFE_DELETE_ARRAY(m_sendMsg.pData);
}

void CNetFileService::InitService(InterFace *pInterFace, Net_Communication_Attr& param)
{		
	//CNetInterFace父类虚函数全部由CDevNetInterface子 类实现
	//即通过CNetInterFace类的m_pInterFace对象调用CDevNetInterface类中的所有实现的函数
	m_bNetAlive 	 = false;
	/*初始化接收，发送类*/
	m_sendRcv.Init(SOCK_STREAM);        //建立网络通信套接字
	m_sendRcv.SetNetParam(param.tcp.timeout,param.tcp.trytimes);
	/* initialize signal parse/package instance*/
	m_SigProc.Init();

	m_SigProc.SetNetParam(param.tcp.timeout,param.tcp.trytimes);
	m_SigPackage.Init(pInterFace);
	m_sigParse.Init(pInterFace, &m_SigPackage);
	m_sendMsg.pData = new char[2800];		//分配发送缓冲区大小
}

bool CNetFileService::StartService()        //由main函数调用
{
	m_sendRcv.StartService();
	//启动接收、发送、数据协议解析线程
	bool bRet = m_NetFileServiceThread.StartThread(NetFileServiceThread, this);     //启动MDVR网络配置设置服务线程
	m_bStart = true;
	return bRet;
}

bool CNetFileService::StopService()
{
	if(m_bStart){
		m_bNetAlive = false;
		m_NetFileServiceThread.StopThread();
		m_sendRcv.StopService();
		m_bStart = false;
	}

	return true;
}

void* CNetFileService::NetFileServiceThread(void *pUsrData)       
{
    CNetFileService *pThis = (CNetFileService*)pUsrData;
    return pThis->NetFileServiceProcess();
}

#define  RESNED_MAX_TIMES	(3)
#define  RECONNECT_MAX_TIMES	(2)
void* CNetFileService::NetFileServiceProcess()              
{
	NetRcvMsg *pRcvMsg = NULL;
	bool bConnected = false;
	int nCount = 0;
	int nTimes = 0;
	int nReConnectTimes = 0;
	while(!m_NetFileServiceThread.GetExit(20)) {
		if(bConnected){
			pRcvMsg = m_sendRcv.GetMsg();
			if(pRcvMsg){
				nCount = 0;
				nTimes = 0;
				ProcRcvMsg(pRcvMsg);
			} else {
				if(m_nStep != REPLY_STEP_NONE){
					if(++ nCount > 300){
						if(nTimes >= RESNED_MAX_TIMES){
							DBG_LEVEL_6(" ");
							m_bFinished = true;
							m_bReConnected = true;
							nTimes = 0;
							nCount = 0;
						} else {
							DBG_LEVEL_6(" ");
							reSendfunc();
							++ nTimes;
							nCount = 0;
						}
					}
				}
			}

			if(m_bFinished){
				if(!m_bReConnected){
					m_nStep = REPLY_STEP_NONE;
				}
				
				sleep(1);
				m_sendRcv.CloseConnect();
				m_bFinished = false;
				bConnected = false;
			}
		} else {
			if(m_bReConnected) {
				if((++ nReConnectTimes) > RECONNECT_MAX_TIMES){
					m_bReConnected = false;
					nReConnectTimes = 0;
					m_stServerList.erase(m_stServerList.begin());
				}
			}

			if(m_stServerList.size() > 0){
				DBG_LEVEL_6("ServerIsLinked:%d ", m_sendRcv.ServerIsLinked());
				if(!m_sendRcv.ServerIsLinked()){
					File_Server_Config __server = m_stServerList[0];
					DBG_LEVEL_6("szAddr:%s-nTcpPort:%d", __server.szAddr, __server.nTcpPort);

					if(m_sendRcv.Link2Svr((const char*)&__server.szAddr[0], __server.nTcpPort)){
						m_nConnectCount = 0;
						m_sendRcv.SetAuthorizeStatus(true);
						DBG_LEVEL_6("Server connected Success!");
						
						bConnected = true;
						if(m_bReConnected){
							reSendfunc();
						} else {
							m_nSendIndex = 0;
							if(!UploadAttachmentInfo(__server, m_nSendIndex)){
								m_stServerList.erase(m_stServerList.begin());
								m_bFinished = true;
							}
						}
					} else {
						DBG_LEVEL_6("Server connected failed!");

						if((++ m_nConnectCount) == 10){
							m_nConnectCount = 0;
							m_stServerList.erase(m_stServerList.begin());
						}
						usleep(1000*1000);
					}
				}
			}
		}

		usleep(10*1000);
	}

	m_bNetAlive = false;
	return NULL;
}

int CNetFileService::reSendfunc(void)
{
	m_sendRcv.SendMsg(&m_sendMsg);
	return 0;
}
//处理接收到来自平台的消息体
//根据接收的命令(服务器)做相应的操作
void CNetFileService::ProcRcvMsg(NetRcvMsg* pMsg)               
{
	/*业务处理*/
	DBG_LEVEL_6("CNetFileService-ProcRcvMsg ID :0x%x.", pMsg->wId);
	switch( pMsg->wId )
	{
		case NET_CMD_CMS_REPLY:           //平台通用应答
			ProcCmsReply(pMsg); 
			break;
		case  NET_CMD_FILE_UPLOAD_FINISHED_REPLY:
			FileUpdateFinish(pMsg);
			break;
		default:
			DBG_LEVEL_6("Invalid Message ID");
			CommReply(pMsg->wSerialNumber, pMsg->wId, 3);
			break;
	}
}

//终端通用应答函数
inline void CNetFileService::CommReply( WORD wSerialNumber, WORD wId, BYTE bResult)
{
	char strData[32] = {0};
	DWORD dataLen  = 0;
	DWORD wSerialNum = m_SigPackage.CommReply(strData, dataLen, wSerialNumber, wId, bResult);
	Send(NET_CMD_REPLY, wSerialNum, strData, dataLen, false, 0, 0);
}

// 平台通用应答
void CNetFileService::ProcCmsReply(NetRcvMsg* pMsg)
{
	NET_MSG_CMS_REPLY_S msgCmsReply;
	memset(&msgCmsReply, 0, sizeof(msgCmsReply));
	memcpy(&msgCmsReply.wSerialNumber, pMsg->pData, 2);
	memcpy(&msgCmsReply.wId, pMsg->pData+2, 2);
	memcpy(&msgCmsReply.bResult, pMsg->pData+4, 1);      //0：成功/确认；1：失败；2：消息有误；3：不支持             
	msgCmsReply.wSerialNumber = ntohs(msgCmsReply.wSerialNumber);
	msgCmsReply.wId           = ntohs(msgCmsReply.wId);

	DBG_LEVEL_6("CNetFileService--ProcCmsReply ID:0x%.4x,SN:%d,result:%d", 
					msgCmsReply.wId, msgCmsReply.wSerialNumber,msgCmsReply.bResult);
	for(int i = m_stComReplyList.size() - 1; i >= 0; -- i){
		if(m_stComReplyList[i].nSerailNum == msgCmsReply.wSerialNumber){
			if(m_stComReplyList[i].bReply){
				DBG_LEVEL_6("[has been reply]");
				return;
			} else {
				DBG_LEVEL_6("[first reply]");
				m_stComReplyList[i].bReply = true;
				break;
			}
		}
	}

	switch (msgCmsReply.bResult)   //bResult   0：成功；1：车辆已被注册；2：数据库中无该车辆；3：终端已被注册；4：数据库中无该终端
	{
		case NET_CMD_REPLY_SUCCESS:          //平台应答成功
			break;
		case NET_CMD_REPLY_FAILURE:		//平台应答失败!
			break;
		case NET_CMD_REPLY_MSGERR:
			break;
		case NET_CMD_REPLY_NOTSUPPORT:
			break;
		case NET_CMD_REPLY_ALARM_VERIFY:
			break;
		default:
			break;
	}

	if(m_nStep == REPLY_STEP_ALARM_UPLOAD){
		UploadFileInfo(NET_CMD_FILE_INFO_UPLOAD, m_nSendIndex);
	} else if(m_nStep == REPLY_STEP_FILE_INFO_UPLOAD){
		if(!UploadFile(m_nSendIndex)){
			m_nSendIndex += 1;
			if(m_nSendIndex < m_stFileDetailList.size()){
				UploadFileInfo(NET_CMD_FILE_INFO_UPLOAD, m_nSendIndex);
			} else {
				m_stServerList.erase(m_stServerList.begin());
				m_bFinished = true;
			}
		}
	}

	// 删除收到回复的消息
	m_sendRcv.DelMsgBySN(msgCmsReply.wSerialNumber);	
}

int CNetFileService::islocked(const char* filename)
{
	struct flock _lock;
	int ret = 0;
    _lock.l_type =  F_WRLCK;
    _lock.l_whence = SEEK_SET;
    _lock.l_start = 0;
    _lock.l_len = 0;

	int fd = open(filename, O_RDWR);
	if(fd < 0){
		return -1;
	}

	fcntl(fd, F_GETLK, &_lock);
	close(fd);

	return _lock.l_type;
}

off_t CNetFileService::get_file_size(const char* filename)
{
	struct stat statbuf;
	statbuf.st_size = 0;
	int nCount = 0;
	char cmd[256] = {0};
	DBG_LEVEL_6("[filename:%s]", filename);
	
	while(1){
		if(access(filename, R_OK) == 0){
			while(F_UNLCK != islocked(filename)){
				usleep(200*1000);
			}
			usleep(50*1000);
			sprintf(cmd, "/bin/sync %s", filename);
			system(cmd);
			stat(filename, &statbuf);
			break;
		} else {
			if((++ nCount) > 10)
			{
 				system("ls -l /tmp/capture");
				break;
			}
		}
		usleep(100*1000);
	}
	
	DBG_LEVEL_6("[Size:%ld]", (unsigned long int)statbuf.st_size);

	return statbuf.st_size;  
}

void CNetFileService::create_file_list(const File_Server_Config& stFileServerConfig, vector<FileInfoS>& stFileDetailList, int nBeginIndex)
{
	stFileDetailList.clear();
	for(int i = nBeginIndex; i < stFileServerConfig.fileList.size(); ++ i){
		FileInfoS fileDetail;
		memset(&fileDetail, 0, sizeof(FileInfoS));
		fileDetail.name_len = FILE_NAME_LEN;
		char filename[FILE_NAME_LEN + 2] = {0};
		if(stFileServerConfig.fileList[i].nType == FILE_TYPE_IMAGE){
			sprintf(filename, "%02X_%02X_%02X%02X_%02d_%s.jpg", stFileServerConfig.fileList[i].nType, stFileServerConfig.nDevType, stFileServerConfig.nDevType, stFileServerConfig.nAlarmType, stFileServerConfig.fileList[i].nIndex, stFileServerConfig.alarmCode);
		} else if (stFileServerConfig.fileList[i].nType == FILE_TYPE_VIDEO){
			sprintf(filename, "%02X_%02X_%02X%02X_%02d_%s.mp4", stFileServerConfig.fileList[i].nType, stFileServerConfig.nDevType, stFileServerConfig.nDevType, stFileServerConfig.nAlarmType, stFileServerConfig.fileList[i].nIndex, stFileServerConfig.alarmCode);
		} else if (stFileServerConfig.fileList[i].nType == FILE_TYPE_TEXT){
			sprintf(filename, "%02X_%02X_%02X%02X_%02d_%s.bin", stFileServerConfig.fileList[i].nType, stFileServerConfig.nDevType, stFileServerConfig.nDevType, stFileServerConfig.nAlarmType, stFileServerConfig.fileList[i].nIndex, stFileServerConfig.alarmCode);
		} else if (stFileServerConfig.fileList[i].nType == FILE_TYPE_AUDIO){
			sprintf(filename, "%02X_%02X_%02X%02X_%02d_%s.wav", stFileServerConfig.fileList[i].nType, stFileServerConfig.nDevType, stFileServerConfig.nDevType, stFileServerConfig.nAlarmType, stFileServerConfig.fileList[i].nIndex, stFileServerConfig.alarmCode);
		} else if (stFileServerConfig.fileList[i].nType == FILE_TYPE_OTHER){
			sprintf(filename, "%02X_%02X_%02X%02X_%02d_%s.oth", stFileServerConfig.fileList[i].nType, stFileServerConfig.nDevType, stFileServerConfig.nDevType, stFileServerConfig.nAlarmType, stFileServerConfig.fileList[i].nIndex, stFileServerConfig.alarmCode);
		}
		
		memcpy(&fileDetail.name[0], &filename[0], FILE_NAME_LEN);
		fileDetail.size = get_file_size(stFileServerConfig.fileList[i].szFileName);
		if(fileDetail.size > 0){
			fileDetail.type = stFileServerConfig.fileList[i].nType;
			memcpy(&fileDetail.src_file[0], &stFileServerConfig.fileList[i].szFileName[0], strlen(stFileServerConfig.fileList[i].szFileName));
			stFileDetailList.push_back(fileDetail);
		}
	}
}

bool CNetFileService::reConnect(void)
{
	m_sendRcv.CloseConnect();
	usleep(200*1000);
	vector<File_Server_Config>::iterator iter = m_stServerList.begin();
	File_Server_Config __server = m_stServerList[0];

	if(m_sendRcv.Link2Svr((const char*)&__server.szAddr[0], __server.nTcpPort)){
		usleep(300*1000);
		if(!__server.bReSend){
			iter->bReSend = true;
			iter->nResetIndex = m_nSendIndex;
		
			DBG_LEVEL_6("m_nSendIndex:%d ", m_nSendIndex);
		}

		__server.bReSend = true;

		DBG_LEVEL_6(" iter->nResetIndex :%d __server.bReSend:%d", iter->nResetIndex, __server.bReSend);
		return UploadAttachmentInfo(__server, iter->nResetIndex);
		
	}

	return false;
}

bool CNetFileService::UploadFile(int index)
{
	int nRet = UploadFileData(index);
	static int count = 0;
	while(nRet){
		if((++ count) >= 3 || nRet == -2){
			count = 0;
			//m_stServerList.erase(m_stServerList.begin());
			return false;
		}

		usleep(500*1000);
		if(nRet == -1){
			DBG_LEVEL_6(" ");
			return reConnect();
		}
	}

	usleep(500*1000);

	UploadFileInfo(NET_CMD_FILE_UPLOAD_FINISHED, index);
	count = 0;
	return true;
}

bool CNetFileService::UploadAttachmentInfo(File_Server_Config& stFileServerConfig, int nBeginIndex)
{
	char szData[1024] = {0};
	WORD dataLen = 0;
	create_file_list(stFileServerConfig, m_stFileDetailList, nBeginIndex);
	if(m_stFileDetailList.size() == 0){
		return false;
	}
	stFileServerConfig.alarm_flag.attachment_num = m_stFileDetailList.size();
	DBG_LEVEL_6("bReSend:%d", stFileServerConfig.bReSend);
	WORD wSerialNum = m_SigPackage.UploadAttachmentInfo(szData, dataLen, stFileServerConfig, m_stFileDetailList);

	Send(NET_CMD_ALARM_ATTACHMENT_INFO_UPLOAD, wSerialNum, szData, dataLen, false, 0, 0);
	m_nStep = REPLY_STEP_ALARM_UPLOAD;
	//m_nSendIndex = 0;
	DBG_LEVEL_6("m_nSendIndex:%d", m_nSendIndex);

	return true;
}

void CNetFileService::UploadFileInfo(int id, int index)
{
	FileInfoS stFileInfo;
	memcpy(&stFileInfo, (void *)&m_stFileDetailList[index], sizeof(stFileInfo));
	char szData[128] = {0};
	DWORD dataLen = 0;
	WORD wSerialNum = m_SigPackage.UploadFileInfo(szData, dataLen, stFileInfo, id);
	Send(id, wSerialNum, szData, dataLen, false, 0, 0);

	m_nStep = REPLY_STEP_FILE_INFO_UPLOAD;
	if(id == NET_CMD_FILE_UPLOAD_FINISHED){
		if(m_stReplyList.size() > 20){
			m_stReplyList.erase(m_stReplyList.begin());
		}

		bool bExist = false;
		for(int i = m_stReplyList.size() - 1; i >= 0; -- i){
			if(!strncmp(&m_stReplyList[i].szFileName[0], (const char *)&stFileInfo.name[0], FILE_NAME_LEN_MAX)){
				bExist = true;
				break;
			}
		}

		if(!bExist){
			FileUploadReply_S stReply;
			memset(&stReply, 0, sizeof(FileUploadReply_S));
			stReply.bReply = false;
			memcpy(&stReply.szFileName[0], &stFileInfo.name[0], FILE_NAME_LEN_MAX);
			m_stReplyList.push_back(stReply);
		}
	}
}

void CNetFileService::FileUpdateFinish(NetRcvMsg* pMsg)
{
	if(NULL == pMsg){
		return;
	}

	File_Finish_Respone_s stResponse;
	memset(&stResponse, 0, sizeof(File_Finish_Respone_s));
	m_sigParse.UploadFileFinish(pMsg, stResponse);
	int i = m_stReplyList.size() - 1;
	
	DBG_LEVEL_6("[szFileName:%s]", stResponse.szFileName);

	for(; i >= 0; -- i){
		DBG_LEVEL_6("****[szFileName:%s]", m_stReplyList[i].szFileName);
		if(!strncmp(&m_stReplyList[i].szFileName[0], &stResponse.szFileName[0], FILE_NAME_LEN_MAX)){
			if(m_stReplyList[i].bReply){
				return;
			} else {
				m_stReplyList[i].bReply = true;
			}
			break;
		}
	}

	if(m_nSendIndex + 1 >= m_stFileDetailList.size()){
		m_nStep = REPLY_STEP_NONE;
		m_stFileDetailList.clear();
		m_nSendIndex = 0;
		m_bReConnected = false;
		#if 0 //maybe keep connecting for next upload
		if(m_stServerList.size() > 2){
			m_stServerList.erase(m_stServerList.begin());
			File_Server_Config __server_1 = m_stServerList[0];
			File_Server_Config __server_2 = m_stServerList[1];
			if(memcmp(&__server_1.szAddr[0], &__server_2.szAddr[0], 15) && (__server_1.nTcpPort == __server_2.nTcpPort)){
				m_bFinished = true;
				m_bReConnected = false;
			} else {
				UploadAttachmentInfo(__server_2);
			}
		} else {
			m_bFinished = true;
		}
		#else
		m_stServerList.erase(m_stServerList.begin());
		m_bFinished = true;
		DBG_LEVEL_6("文件上传完成");
		#endif
	} else {
		m_nSendIndex += 1;
		UploadFileInfo(NET_CMD_FILE_INFO_UPLOAD, m_nSendIndex);
	}
	
}

void CNetFileService::SetServerConfig(File_Server_Config& stFileServerConfig)
{
	m_stServerList.push_back(stFileServerConfig);
}

int CNetFileService::UploadFileData(int index)
{
	FileInfoS stFileInfo;
	memset(&stFileInfo, 0, sizeof(FileInfoS));
	memcpy(&stFileInfo, (void *)&m_stFileDetailList[index], sizeof(FileInfoS));

	File_Data_S stFileData;
	memset(&stFileData, 0, sizeof(File_Data_S));
	stFileData.bFlag[0] = 0x30;
	stFileData.bFlag[1] = 0x31;
	stFileData.bFlag[2] = 0x63;
	stFileData.bFlag[3] = 0x64;
	stFileData.offset = 0;
	memcpy(&stFileData.filename[0], stFileInfo.name, FILE_NAME_LEN);
	FILE *fp = NULL;
	bool first_send = true;
	int nRet = 0, pack_num = 0, less_size = 0, unit_size = 0;

	fp = fopen((char *)&stFileInfo.src_file[0], "rb");
	if(fp){
		if(stFileInfo.size > 0){
			int big_pack_num = (stFileInfo.size / (60 * FILE_DATA_UNIT_SIZE)) + ((stFileInfo.size % (60 * FILE_DATA_UNIT_SIZE)) > 0 ? 1 : 0);

			for(int j = 0; j < big_pack_num; ++ j){
				first_send = true;
				int datasize = 60 * FILE_DATA_UNIT_SIZE;
				if(j == big_pack_num - 1){
					datasize = stFileInfo.size % (60 * FILE_DATA_UNIT_SIZE);
				}

				less_size = datasize % FILE_DATA_UNIT_SIZE;
				pack_num = datasize / FILE_DATA_UNIT_SIZE + (less_size > 0?1:0);

				for(int i = 0; i < pack_num; ++ i){
					if (i == pack_num -1 && less_size > 0){
						unit_size = less_size;
					}else{
						unit_size = FILE_DATA_UNIT_SIZE;
					}
					fseek(fp, i * FILE_DATA_UNIT_SIZE + j * (60 * FILE_DATA_UNIT_SIZE), SEEK_SET);
					fread(&stFileData.data[0], unit_size, 1, fp);
					
					if(first_send){
						first_send = false;
						char buffer[2000] = {0};
						stFileData.offset = j * (60 * FILE_DATA_UNIT_SIZE);
						stFileData.length = datasize;

						m_SigPackage.PackageFileHeadData(stFileData, &buffer[0], unit_size);
						nRet = m_sendRcv.SendData(&buffer[0], unit_size + 62);
					} else {
						nRet = m_sendRcv.SendData(&stFileData.data[0], unit_size);
					}

					if(nRet <= 0){
						fclose(fp);
						DBG_LEVEL_6("[send Error:%d]", nRet);

						return -1;
					}
				}

			}
		}

		fclose(fp);
	}else{	
		DBG_LEVEL_6("[open faild]");
	}

	return 0;
}

DWORD CNetFileService::Send(WORD wCmdType,WORD wSerialNum,void* pMsgBody, WORD nLen, bool bReSend, WORD nPackTotal, WORD nPackSn)
{
	if(m_stComReplyList.size() > 20){
		m_stComReplyList.erase(m_stComReplyList.begin());
	}

	bool bExist = false;
	for(int i = m_stComReplyList.size() - 1; i >= 0; -- i){
		if(m_stComReplyList[i].nSerailNum == wSerialNum){
			bExist = true;
			break;
		}
	}

	if(!bExist){
		ComReply_S stComReply;
		stComReply.nSerailNum = wSerialNum;
		stComReply.bReply = false;
		m_stComReplyList.push_back(stComReply);
	}

	CAutoLock lock(&m_mutex);
	m_SigProc.PackMsg(wCmdType,wSerialNum,&m_sendMsg,pMsgBody,nLen,bReSend,nPackTotal,nPackSn);	
	return m_sendRcv.SendMsg(&m_sendMsg);
}
