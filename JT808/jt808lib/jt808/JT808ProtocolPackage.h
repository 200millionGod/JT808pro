#ifndef _JT808_PROTOCOL_PACKAGE_H_
#define _JT808_PROTOCOL_PACKAGE_H_

#include "Interface.h"
#include "GnssNetProtocol.h"
#include "GnssNetSigParsePack.h"
#include "ThreadFrame.h"
#include "MutexLock.h"

typedef DWORD (*AddSendBufCallBack)(WORD wCmdType,WORD wSerialNum,void* pMsgBody,WORD nLen,bool bReSend,WORD nPackTotal,WORD nPackSn);

class CJT808Package
{
public:
	void Init(InterFace * pInterFace);
	//配置添加消息到发送缓冲回调
	void SetAddSendBufCB(AddSendBufCallBack pfAddSendBuf){m_pfAddSendBuf=pfAddSendBuf;}
	//设备通用应答
	void CommReply(WORD wSerialNumber, WORD wId, BYTE bResult);
	DWORD CommReply(char *szData, DWORD &dataLen, WORD wSerialNumber, WORD wId, BYTE bResult);
	//设备心跳
	void HeartBeat(void);
	DWORD HeartBeat(char *szData, DWORD &dataLen);
	//注册
	void Register(void);
	DWORD Register(char *szData, DWORD &dataLen);
	//注销
	void UnRegister(void);
	WORD UnRegister(char *szData, DWORD &dataLen);
	//登陆
	void Login(char* pAuthCode);
	WORD Login(char *szData, DWORD &dataLen, char* pAuthCode);
	
	//查询设备参数应答
	void ReplyParam(DWORD count,DWORD* pId,WORD wSerialNumber);
	WORD ReplyParam(vector<PackageSendMsg>& vecPackageMsg, DWORD count,DWORD* pId,WORD wSerialNumber);
	//查询设备属性应答
	void ReplyPoperty(WORD wSerialNumber);
	WORD ReplyPoperty(char *szData, DWORD &dataLen);
	//升级结果通知
	void ReportUpgradeResult(BYTE bType,BYTE bResult);
	WORD ReportUpgradeResult(char *szData, DWORD &dataLen, BYTE bType, BYTE bResult);
	//汇报位置信息
	void ReportPosition(void* pData);
	WORD ReportPosition(char *szData, DWORD &dataLen, void* pData);

	//汇报保存GPS位置信息
	WORD ReportSaveGpsPosition(char *szData, DWORD &dataLen, void* pData,int bodyLen,int packNum);
	
	//汇报报警信息
	void ReportAlarm(void* pData, WORD& wSerialNum);
	WORD ReportAlarm(char *szData, DWORD &dataLen, void* pData);
	//汇报设备状态
	void ReportDevStatus(DWORD value);
	WORD ReportDevStatus(char *szData, DWORD &dataLen, DWORD value);
	//汇报设备事件
	void ReportEvent(BYTE eid);
	WORD ReportEvent(char *szData, DWORD &dataLen, BYTE eid);
	//驾驶员身份信息上报
	void ReportDriverInfo(void);
	WORD ReportDriverInfo(char *szData, DWORD &dataLen);
	//查询位置信息应答
	void ReplyPosition(WORD wSerialNumber, struGPSData& gnssData);
	WORD ReplyPosition(char *szData, DWORD &dataLen, WORD wSerialNumber, struGPSData& gnssData);
	//提问应答
	void ReplyAsk(WORD wSerialNumber,BYTE bResult);
	WORD ReplyAsk(char *szData, DWORD &dataLen, WORD wSerialNumber, BYTE bResult);
	//信息点播/取消
	void OrderMessage(BYTE bType,BYTE bFlag);
	WORD OrderMessage(char *szData, DWORD &dataLen, BYTE bType, BYTE bFlag);
	//车辆控制应答
	void ReplyControl(WORD wSerialNumber);
	WORD ReplyControl(char *szData, DWORD &dataLen, WORD wSerialNumber);
	//行驶记录数据上传
	int UploadDrivingData(void* pBuffer,WORD wSerialNumber);
	//电子运单上报
	void UploadElectBill(void* pData,DWORD len);
	void UploadElectBill(vector<PackageSendMsg>& vecPackageMsg, void* pData,DWORD len);
	//定位数据批量上传
	int UploadBatchLocation(void* pBuffer);
	//CAN总线数据上传
	int UploadCANData(void* pBuffer);
	//多媒体事件信息上传
	void UploadMutiMediaEvent(DWORD id,BYTE type,BYTE encode,BYTE event, BYTE chn, WORD &nSerialNum);
	WORD UploadMutiMediaEvent(char *szData, DWORD &dataLen, DWORD id, BYTE type, BYTE encode, BYTE event, BYTE chn);
	//多媒体数据上传
	//int UploadMutiMediaData(void* pBuffer, int nLen, short PackNum, short PackId, NetMdUploadCmd *pCmd);
	//int UploadMutiMediaData(void* pBuffer, int nLen, short PackNum, short PackId, NetMdUploadCmd *pCmd, struGPSData& gpsData, time_t& pts, WORD& nSerailNum);
	WORD UploadMutiMediaData(char *szData, DWORD &dataLen, void* pBuffer, int nLen, short PackNum, short PackId, NetMdUploadCmd *pCmd);
	WORD UploadMutiMediaData(char *szData, DWORD &dataLen, void* pBuffer, int nLen, short PackNum, short PackId, NetMdUploadCmd *pCmd, struGPSData& gpsData, time_t &pts);
	//WORD UploadMutiMediaData(char *szData, DWORD &dataLen, void* pBuffer, int nLen, short PackNum, short PackId, NetMdUploadCmd *pCmd, struGPSData& gpsData, time_t &pts, WORD& nSerailNum);

	//立即拍摄命令应答
	void ReplySnapShot(WORD wSerialNumber,BYTE bResult,WORD num,DWORD* pDataIDs);
	void ReplySnapShot(WORD wSerialNumber, vector<ShotResult>& stResultList, BYTE nResult);
	WORD ReplySnapShot(char *szData, DWORD &dataLen, WORD wSerialNumber, BYTE bResult, WORD num, DWORD* pDataIDs);
	WORD ReplySnapShot(char *szData, DWORD &dataLen, WORD wSerialNumber, vector<ShotResult>& vecResultList, BYTE nResult);
	//存储多媒体数据检索应答
	WORD ReplyMutiMediaIndex(char *szData, DWORD &dataLen, WORD wSerialNumber, vector<IdxCmdRes>& vecResultList);
	//单条存储多媒体数据检索应答
//	WORD SingleReplyMutiMediaIndex(char *szData, DWORD &dataLen, WORD wSerialNumber);
	//数据透传
	void UploadTransParent(BYTE bType,void* pData,DWORD len);
	void UploadTransParent(vector<PackageSendMsg>& vecPackageMsg, BYTE bType,void* pData,DWORD len);
	//数据压缩上报
	int UploadCompressData(void* pBuffer);
	//RAS公钥
	int UploadRASKey(void* pBuffer);
	static WORD TransmitToCopy(void* src,void* dst,WORD len);
	/*上报报警附件信息*/
	void UploadAttachmentInfo(File_Server_Config& stFileServerConfig, vector<FileInfoS>& stFileDetailList);
	WORD UploadAttachmentInfo(char *szData, WORD &dataLen, File_Server_Config& stFileServerConfig, vector<FileInfoS>& stFileDetailList);
	/*上报报警文件件信息*/
	void UploadFileInfo(FileInfoS& stFileInfo, int id);
	WORD UploadFileInfo(char *szData, DWORD &dataLen, FileInfoS& stFileInfo, int id);
	void PackageFileHeadData(File_Data_S stFileData, char *msg_buffer, int file_data_len);
	/*人脸图片下载应答*/
	WORD FacePicDownload(char *szData, DWORD &dataLen, void* pData);
	WORD UploadFaceInfo(char *szData, DWORD &dataLen, void* pData, WORD wSerial);
	WORD UploadFacePic(char *szData, DWORD &dataLen, char* pPicData, char pic_type, WORD msg_serial, int nlen, WORD nPackTotal, WORD nPackSn);
	WORD ReportFaceCmp(char *szData, DWORD &dataLen, char* pPicData, void* pFaceCmp, int nlen,  WORD nPackTotal, WORD nPackSn);

	/*音视频控制模块*/
	WORD UploadVideoInfo(char *szData, DWORD &dataLen, void* pData);
	/*上传乘客流量*/
	WORD ReportCarries(char *szData, DWORD &dataLen, void* pData);
	/*上传音视频资源列表*/
	WORD ReportResourceList(char *szData, DWORD &dataLen, void* pData, WORD nPackTotal, WORD nPackSn, int num);
	/*文件上传结果*/
	WORD ReportUpResult(char *szData, DWORD &dataLen, void* pData);
	/*extensions*/
private:
	DWORD Send(WORD sid,WORD wSerialNum,void* pData,WORD nLen,bool bReSend=true, WORD nPackTotal = 0, WORD nPackSn=0);
	WORD PackageToSend(WORD sid,void* pData,DWORD nLen,void* pDataHead=NULL,WORD nDataHeadLen=0,bool bReSend=true);
	WORD PackageToSend(WORD sid, vector<PackageSendMsg>& vecPackageMsg, void* pData, DWORD nLen, void* pDataHead = NULL, WORD nDataHeadLen = 0, bool bReSend = true);
	WORD Package(WORD sid,char* pBuffer,WORD dataLen,WORD nPackTotal=0, WORD nPackSn=0);
	WORD PosStautsInfo(char* pBuffer,void* pGnssData,void* pAlarmData,WORD devstatus);
	WORD SerialNumSucc(WORD add=1);
	WORD PackageByte(void* pBuffer,BYTE data);
	WORD PackageWord(void* pBuffer,WORD data);
	WORD PackageDword(void* pBuffer,DWORD data);
	WORD PackageBytes(void* pBuffer,BYTE* pData,BYTE len);
	WORD PackageString(void* pBuffer,char* pData,BYTE maxLen);
	
	DWORD GetADASParas(Adas_Attr* pParam,char* value);
	DWORD GetDSMParas(Dsm_Attr* pParam,char* value);	
	DWORD GetAiViAttr(JT1078_Audio_video_conf_set* pParam,char* value);
	DWORD GetAiViChn(JT1078_Audio_video_chn_set* pParam,char* value);
	DWORD GetSingleChn(JT1078_single_chn_control* pParam,char* value);
	DWORD GetSpecialChn(JT1078_special_alarm_param_set* pParam,char* value);
	DWORD GetVideoAlarmShield(JT1078_alarm_shield* pParam,char* value);
	DWORD GetVideoAlarmAttr(JT1078_alarm_video_param* pParam,char* value);
	DWORD GetAwakeAttr(JT1078_awake_param* pParam,char* value);
	
	InterFace* m_pInterFace;
	BCD m_bcdPhoneNo[12];
	WORD m_nSerialNumber;
	CMutexLock m_mutex;
	AddSendBufCallBack m_pfAddSendBuf;
	AlarmData_s	m_alarmData;
};

#endif	//_JT808_PROTOCOL_PACKAGE_H_

