#ifndef _GNSS_MOC_NET_SERVICE_H_
#define _GNSS_MOC_NET_SERVICE_H_

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
#include "GnssNetFileService.h"
//#include "MediaService.h"

#define UPLOAD_EVENT_PLATFORM		0
#define UPLOAD_EVENT_TIMER			1
#define UPLOAD_EVENT_ROBBERY		2
#define UPLOAD_EVENT_COLLISION		3
#define UPLOAD_EVENT_OPEN_DOOR		4
#define UPLOAD_EVENT_CLOSE_DOOR		5
#define UPLOAD_EVENT_OPEN_CLOSE		6
#define UPLOAD_EVENT_DISTANCE		7

#define SNAP_CTRL_LOOP_INTERAL			60*1000 //1 munitues
#define FaceCmp_Report_LOOP_INTERAL		100


#pragma pack(4)

class CNetService
{
private:                          //private表示成员只提供给本类使用，本类以外的函数无法使用
	bool m_bGpsUpload;						   // 保存GPS数据上传标志	new add 20210927
	bool m_bNetAlive;
	bool m_RepResult;
	bool m_LoginIng;
	int            m_nSockType;                // socket  类型
	DWORD          m_dwReplyTimeout;           // 应答超时
	DWORD          m_dwReSendTimes;            // 消息重传的次数
	NetSendMsg 	   m_sendMsg;
	DWORD          m_userReTryTimes;		   // 注册   鉴权 注销重试次数
	/*临时使用变量来储存信息， 最终将会以配置信息写入flash*/
	char m_szAuthCode[64];
	int  m_szAuthLen;
	DWORD m_nHeartTimer;
	bool  m_bAlarmUploadRespone;
private:                 	
	CPthreadFrame m_NetServiceThread;
	CPthreadFrame m_FileManagerThread;
	CPthreadFrame m_ShotThread;
	CPthreadFrame m_SnapCtrlThread;
	CPthreadFrame m_FacePicUploadThread;
	CPthreadFrame m_UpMediaData_pthread;
	CPthreadFrame m_ResendMediaData_pthread;
	
	CNetFileService *m_pobjFileService;
	/*负责和设备进行信息交互*/
	InterFace* m_pInterFace;
	/*接收发送消息*/
	CNetMsgSendRcv  m_sendRcv;
	
	/* 对网络信号处理,解析和封装消息*/
	CNetSigProc   	m_SigProc;
	CJT808Package	m_SigPackage;
	CJT808Parse		m_sigParse;	
	CMutexLock m_mutex;
	vector<Files_Map_S> m_fileMapList;
	vector<ShotResult> fileList;	//多媒体文件上传列表
	vector<SingleFileData> SingleFileList;	//记录多媒体文件分包缓存列表，用于重传
	ResendMediaDataCmd resendMediaData;
	pthread_mutex_t m_stFileMutex;
	pthread_mutex_t m_stShotMutex;
	pthread_mutex_t m_stFaceCmpMutex;
	pthread_mutex_t m_stUpFacePicMutex;//upload face compare picture
	WORD m_nUploadFaceCmpSerial;
	bool m_bStart;
	unsigned int m_nMediaID;
	SetTerminalParam_CallBack  m_pSetTermCfg;
	SnapShot_Callback		   m_pSnapshot;
	FaceSnap_Callback          m_pFaceSnap;
	GetGPS_Callback			   m_pGPSCallback;
	FacePic_Callback		   m_pFacePic;
	FacePicCmpRes_Callback	   m_pFacePicCmp;
	FaceInfo_Callback		   m_pFaceInfo;
	TriggerFaceCmp_Callback    m_pTriggerFaceCmp;
	VideoCfgGet_Callback       m_pVideoCfgGet;
	VideoReqStream_Callback    m_pVideoReqStream;
	VideoTransCtrl_Callback    m_pVideoTransCtrl;
	VideoQueSource_Callback    m_pVideoQueSource;
	VideoReqPlayback_Callback  m_pVideoReqPlayback;
	VideoPlaybackCtrl_Callback m_pVideoPlaybackCtrl;
	VideoUploadFile_Callback   m_pVideoUpFile;
	VideoCtrlUpload_Callback   m_pVideoCtrlUp;
	TextInfo_Callback          m_pTextInfo;
	
	JT808_Config_S m_pSnapCtrl;
	bool	m_bShotAlive;
	NetMdShotCmd m_stNetMdShotCmd;
	bool m_bFaceCmpUpdate;
	UploadFacePic m_pFacePicUpload;
	bool m_bResendFaceCmpPic;
	RetransmitPacket m_pReFaceCmpPic;
	bool m_bRendFaceAuthPic;
	RetransmitPacket m_pReFaceAuthPic;
	FaceCmpResult m_stNetFaceCmpReport;
	CPthreadFrame m_LocTrackingThread;//position tracking control
	WORD m_LocationTrackInter;        //position tracking interval.0:no tracking
	DWORD m_LocationTrackPeriod;      //Send location reports within the validity period
	WORD m_nFileUploadSerial;
	pthread_mutex_t m_stFileUploadMutex;

private:	

	static void* NetServiceThread(void *pUsrData);
	void* NetServiceProcess();

	static void *FileMapThread(void *param);
	void *RemoveFileMap(void);

	static void *LocTrackThread(void *param);
	void *LocationTrack(void);

	static void *ShotThread(void *param);
	void *Shot(void);

	static void *SnapCtrlThread(void *param);
	void *SnapCtrl(void);
	
	static void *FacePicUploadThread(void *param);
	void *FacePicUpload(void);

	static void *UpMediaData_pthread(void *param);
	void *UpMediaData(void);

	static void *ResendMediaData_pthread(void *param);
	void *ResendMediaData(void);
	
	void SnapCtrlDistance(void* );
	void SnapCtrlTimer(void* );

	void ProcRcvMsg(NetRcvMsg* pMsg);

	void SetSysCfg(NetRcvMsg* pMsg);
	void GetParam(NetRcvMsg* pMsg);
	void GetSelectParam(NetRcvMsg* pMsg);
	void TerminalCtrl(NetRcvMsg* pMsg);
	void GetDevAttr(NetRcvMsg* pMsg);
	void Upgrade(NetRcvMsg* pMsg);
	void ReplyPosition(NetRcvMsg* pMsg);
	void LocationTracking(NetRcvMsg* pMsg);
	void ConfirmAlarm(NetRcvMsg* pMsg);
	void SMSProc(NetRcvMsg* pMsg);
	void EventSet(NetRcvMsg* pMsg);
	void QuestionDown(NetRcvMsg* pMsg);
	void InfoDmdMenuSet(NetRcvMsg* pMsg);
	void InfoService(NetRcvMsg* pMsg);
	void TelCallback(NetRcvMsg* pMsg);
	void TelBookSet(NetRcvMsg* pMsg);
	void VehicleCtrl(NetRcvMsg* pMsg);
	void SetRingRgn(NetRcvMsg* pMsg);
	void SetRectRgn(NetRcvMsg* pMsg);
	void SetPolyRgn(NetRcvMsg* pMsg);
	void DelRgn(NetRcvMsg* pMsg, int rgnType);
	void DelRingRgn(NetRcvMsg* pMsg);
	void DelRectRgn(NetRcvMsg* pMsg);
	void DelPolyRgn(NetRcvMsg* pMsg);
	void DelRoute(NetRcvMsg* pMsg);
	void SetRoute(NetRcvMsg* pMsg);
	void DownLinkTransparent(NetRcvMsg* pMsg);
	void ReqDriverInfo(NetRcvMsg* pMsg);

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

	void CameraSnapShotReply(WORD wSerialNumber,BYTE bResult,WORD num,DWORD* pDataIDs);
	void CameraSnapShotReply(WORD wSerialNumber,vector<ShotResult>& stResultList, BYTE nResult);

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
	void ProcCmsReply(NetRcvMsg* msgInfo, WORD& nSerailNum);


	/***********************************************************
	* 函数功能: 处理注册回复
	* 输入参数: 
	* @param msgInfo 消息信息
	* 输出参数: 
	* 返回参数:
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void ProcRegisterReply(NetRcvMsg* pMsg);

	
	/***********************************************************
	* 函数功能: 行驶记录采集
	* 输入参数: 
	* @param pCmdType  上载记录类型
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int PickDrivingRecord(NetRcvMsg* pCmdType);


	
	/***********************************************************
	* 函数功能: 行驶记录采集
	* 输入参数: 
	* @param pCmdType  下传命令字
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SetDrivingRecord(NetRcvMsg* pMsg);
	
	
	/***********************************************************
	*函数功能 :  心跳消息
	*输入参数 : 
	* 输出参数:   
	* 返回参数:
	* 使用说明:  无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	void SendHeartBeat();


	/***********************************************************
	* 函数功能: 下行透传
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int DownLinkTrans(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 多媒体数据上传应答
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int ProcMultimediaUpReply(NetRcvMsg* pMsg);
	int ResendMediaData(WORD id);

	/***********************************************************
	* 函数功能: 储存多媒体数据检索
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int MultimediaSearch(NetRcvMsg* pMsg);


	/***********************************************************
	* 函数功能: 单条储存多媒体数据检索
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SingleMultimediaSearch(NetRcvMsg* pMsg);
	
	void MultimediaSearchReply(WORD wSerialNumber, vector<IdxCmdRes>& stResultList);
	/***********************************************************
	* 函数功能: 储存多媒体数据上传
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int MultimediaUp(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 启动录音
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int StartRecord(NetRcvMsg* pMsg);
    void getFileList(File_Server_Config& config);
	int AlarmAttachmentUpload(NetRcvMsg* pMsg, WORD nSerailNum);
	void UpMediaData(NetMdShotCmd mdShotCmd, vector<ShotResult>& stResultList, BYTE event);
	int islocked(const char* filename);
	off_t get_file_size(const char* filename);

	/***********************************************************
	* 函数功能: 人脸图片下发
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-05-05
	************************************************************/
	int FacePicturePublish(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 人脸信息查询
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-05-05
	************************************************************/
	int FaceInfoQuery(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: Triggering face compare
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-05-11
	************************************************************/
	int TriggerFaceCompare(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 人脸比对结果应答
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-05-05
	************************************************************/
	int FaceCompareResultReply(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 人脸比对结果下发应答
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:hongming 2019-05-07
	************************************************************/
	int FaceCompareResultPublicReply(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 人脸认证照片主动抓拍
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-05-10
	************************************************************/
	int FaceAuthActiveSnap(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 查询终端音视频属性
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-06-20
	************************************************************/
	int VideoConfGet(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 实时音视频传输请求
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-06-21
	************************************************************/
	int VideoReqRealTimeStream(NetRcvMsg* pMsg);
	
	/***********************************************************
	* 函数功能: 实时音视频传输控制
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-06-21
	************************************************************/
	int VideoTransControl(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 实时音视频传输状态通知
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-06-21
	************************************************************/
	int VideoTransStatusNotice(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 查询资源列表
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-06-22
	************************************************************/
	int VideoQueResourceList(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 远程录像回放请求
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-06-24
	************************************************************/
	int VideoReqPlayback(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 远程录像回放控制
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-06-24
	************************************************************/
	int VideoPlaybackCtrl(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 录像文件上传
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-06-24
	************************************************************/
	int VideoReqUploadFile(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 录像文件上传控制
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-06-24
	************************************************************/
	int VideoCtrlUploadFile(NetRcvMsg* pMsg);

	/***********************************************************
	* 函数功能: 文本信息下发
	* 输入参数: 
	* @param   pMsg  接收消息
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:huangshimei 2019-07-03
	************************************************************/
	int TextInformation(NetRcvMsg* pMsg);
	
public:                                                                  //public表示以下提供给外部的调用

	CNetService();
	virtual ~CNetService();
	bool StartService();
	bool SendAlarm(int i);
	bool StopService();
	void SetTermCfg_Callback(SetTerminalParam_CallBack pTermCfg){m_pSetTermCfg = pTermCfg;};
	void SetSnapShot_Callback(SnapShot_Callback pSnapshot){m_pSnapshot = pSnapshot;};
	void SetFaceSnap_Callback(FaceSnap_Callback pFaceSnap){m_pFaceSnap = pFaceSnap;};
	void SetGPSData_Callback(GetGPS_Callback pGetGPS){m_pGPSCallback = pGetGPS;};
	void SetFacePic_Callback(FacePic_Callback pFacePic){m_pFacePic = pFacePic;};
	void SetFaceCmpRes_Callback(FacePicCmpRes_Callback pFacePicCmpData){m_pFacePicCmp = pFacePicCmpData;};
	void SetFaceInfo_Callback(FaceInfo_Callback pFaceInfo){m_pFaceInfo = pFaceInfo;};	
	void SetFaceCmp_Callback(TriggerFaceCmp_Callback pFaceCmp){m_pTriggerFaceCmp = pFaceCmp;};	

	bool GetNetAliveStatus(void);
	bool GetSaveGpsDataUploadStatus(void);
	/*************************************************************************
	*Function:GetVideoCfg_Callback
	*Description:获取音视频参数
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190621
	**************************************************************************/
	void GetVideoCfg_Callback(VideoCfgGet_Callback pVideoCfg){m_pVideoCfgGet = pVideoCfg;};	

	/*************************************************************************
	*Function:ReqVideoStream_Callback
	*Description:实时音视频传输请求
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190621
	**************************************************************************/
	void ReqVideoStream_Callback(VideoReqStream_Callback pReqStream){m_pVideoReqStream = pReqStream;};	

	/*************************************************************************
	*Function:CtrlVideoTrans_Callback
	*Description:实时音视频传输控制
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190621
	**************************************************************************/
	void CtrlVideoTrans_Callback(VideoTransCtrl_Callback pTransCtrl){m_pVideoTransCtrl = pTransCtrl;};	

	/*************************************************************************
	*Function:CtrlVideoTrans_Callback
	*Description:实时音视频传输控制
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190621
	**************************************************************************/
	void QueVideoSource_Callback(VideoQueSource_Callback pSource){m_pVideoQueSource = pSource;};

	/*************************************************************************
	*Function:ReqVideoPlayback_Callback
	*Description:请求录像回放
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190624
	**************************************************************************/
	void ReqVideoPlayback_Callback(VideoReqPlayback_Callback pPlayback){m_pVideoReqPlayback = pPlayback;};
	/*************************************************************************
	*Function:CtrlVideoPlayback_Callback
	*Description:录像回放控制
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190624
	**************************************************************************/
	void CtrlVideoPlayback_Callback(VideoPlaybackCtrl_Callback pPBCtrl){m_pVideoPlaybackCtrl = pPBCtrl;};
	/*************************************************************************
	*Function:ReqVideoUpFile_Callback
	*Description:音视频文件上传
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190624
	**************************************************************************/
	void ReqVideoUpFile_Callback(VideoUploadFile_Callback pUpFile){m_pVideoUpFile = pUpFile;};
	
	/*************************************************************************
	*Function:ReqVideoUpFile_Callback
	*Description:音视频文件上传
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190624
	**************************************************************************/
	void CtrlVideoUpFile_Callback(VideoCtrlUpload_Callback pUpCtrl){m_pVideoCtrlUp = pUpCtrl;};
	
	/*************************************************************************
	*Function:TextInformation_Callback
	*Description:文本信息下发处理回调
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190703
	**************************************************************************/
	void TextInformation_Callback(TextInfo_Callback pInfo){m_pTextInfo = pInfo;};

	void InitService(InterFace *pInterFace, CNetService *pobjCNetService = NULL);
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
	//DWORD SendSig(WORD wCmdType, WORD wSerialNum,void* pMsgBody = NULL, WORD nLen = 0, bool bReSend=true,WORD nPackTotal = 0, WORD nPackSn = 0);
	DWORD Send(WORD wCmdType,WORD wSerialNum,void* pMsgBody, WORD nLen, bool bReSend, WORD nPackTotal, WORD nPackSn);
	
	
	/***********************************************************
	* 函数功能:  终端向平台注册 	
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void  Register();

	/***********************************************************
	*函数功能 :  终端向平台注销 
	*输入参数 : 
	* 输出参数:   
	* 返回参数:
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void  Unregister();
	

	/***********************************************************
	*函数功能 :  终端鉴权
	*输入参数 : 
	* 输出参数:   
	* 返回参数:
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void Login();

	void ReportUpgrade(BYTE type,BYTE result);
	
	void ReportPosInfo(struGPSData* pData);

	void ReportSaveGpsInfo(BYTE* pData,int len,int packNum);

	void ReportAlarm(AlarmData_s* pData, vector<File_Info_s>& fileList);

	void ReportDevStatus(DWORD status);
	
	/***********************************************************
	* 函数功能:  事件报告
	* 输入参数:
	* @param  eventId   事件id
	* 输出参数:   
	* 返回参数:
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void ReportEvent(BYTE eventId);

	/***********************************************************
	* 函数功能:  信息点播
	* 输入参数:
	* @param   type          信息类型
	* @param   flag           点播/取消标志
	* 输出参数:   
	* 返回参数:
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void InfoDemand(BYTE type, BYTE flag);

	/***********************************************************
	* 函数功能:  电子运单上报
	* 输入参数:
	* @param   len             长度
	* @param   content       内容
	* 输出参数:   
	* 返回参数:
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void ReportElectronBill(DWORD len, char* content);
	

	/***********************************************************
	* 函数功能:  驾驶员信息采集上报
	* 输入参数:
	* 输出参数:   
	* 返回参数:
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void ReportDriverInfo();
	
	
	/***********************************************************
	* 函数功能: 数据上行透传
	* 输入参数: 
	* 输出参数:
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void UplinkTrans(TRANSPARENT_TYPE_E type, char *buf, int size);
	

	/***********************************************************
	* 函数功能: 多媒体事件信息上传
	* 输入参数: 
	* param   dataid    多媒体数据id
	* param   type      多 媒体类型
	* param   format   多媒体格式编码
	* param   event     多媒体事件编码
	* param   chn        通道id
	* 输出参数:
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void UpMultiMediaEvent(DWORD dataid, BYTE type, BYTE format, BYTE event, BYTE chn, WORD &nSerialNum);
	

	/***********************************************************
	* 函数功能: 多媒体数据上传
	* 输入参数: 
	* param pBuf      多媒体数据
	* param nLen      长度
	* 输出参数:
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	short UpMultiMediaData(char* pBuf, int nLen, short PackNum, short PackId, NetMdUploadCmd *pCmd);


	/***********************************************************
	* 函数功能: 摄像头立即拍摄
	* 输入参数: 
	* param  id	  	通道id
	* param  cmd	  	拍摄命令0:停止拍摄, 0xffff:表示录像,其他拍摄张数
	* param  interval   拍照间隔,录像时间， 0表示按最小间隔拍照或一直录像
	* param  mark  	保存标志1:保存, 0:实时上传
	* param  picprop    图片属性
	* 输出参数:
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void CameraShot( NetRcvMsg* pMsg);
	
	bool UpSendCan(char *data, int len);
	int GetSerialNumber(short msgid,short packid);

	/***********************************************************
	* 函数功能: 多媒体数据上传
	* 输入参数: 
	* param pBuf      多媒体数据
	* param nLen      长度
	* 输出参数:
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int ReportFacePicDownload(FacePicDownloadReply *pData);
	int ReportFaceCmpResult(FaceCmpResult *faceCmpData);	
	int UpFaceCmpResult(FaceCmpResult *faceCmpData);
	int UploadFaceAuthPic(char *filename, char pic_type, WORD msg_serial);
	int ReportQueryFaceInfoResult(FaceIDInfo *faceInfoData, WORD wSerial);
	//DWORD Send(WORD wCmdType,WORD wSerialNum,void* pMsgBody, WORD nLen, bool bReSend, WORD nPackTotal, WORD nPackSn);

	/*************************************************************************
	*Function:UploadVideoCfg
	*Description:上传音视频属性
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190620
	**************************************************************************/
	int UploadVideoCfg(VideoGetParam *av_info);

	/*************************************************************************
	*Function:ReportVideoCarries
	*Description:上传乘客流量
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190621
	**************************************************************************/
	int ReportVideoCarries(VideoUploadCarries *pCarries);

	/*************************************************************************
	*Function:UploadVideoResource
	*Description:上传音视频资源列表
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190624
	**************************************************************************/
	int UploadVideoResource(VideoUploadResInfo *psource);

	/*************************************************************************
	*Function:UploadAVSubPackageRes
	*Description:上传音视频资源列表，分包发送
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190624
	**************************************************************************/
	int UploadAVSubPackageRes(VideoUploadResInfo *psource);

	/*************************************************************************
	*Function:ReportVideoUploadResult
	*Description:文件上传结果
	*Params in:
	*Params out:
	*Result:
	*author:huangshimei
	*date:20190624
	**************************************************************************/
	int ReportVideoUploadResult(VideoUpFileResult *pResult);
};

#endif
 
