
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
#include <vector>
#include <dirent.h>

#include "Defines.h"
#include "GnssNetService.h"
#include "GnssNetSocket.h"
#include "GnssNetProtocol.h"
#include "GnssNetFileService.h"

#include "debug_log.h"

static CNetService *s_pobjCNetService;
static pthread_cond_t s_stFileUploadCond    = PTHREAD_COND_INITIALIZER;
static pthread_cond_t s_stFaceCmpUploadCond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t s_stUpAVListCond      = PTHREAD_COND_INITIALIZER;

static bool mdFileSendFinished = true;
static bool fileUpLoadOverTimeFlag = false;
//static CNetFileService m_objFileService;
CNetService::CNetService()
{
	memset(m_szAuthCode,'\0',64);
	memset(m_pFacePicUpload.pic_name,'\0',128);
	
	m_pobjFileService			= NULL;
	m_bAlarmUploadRespone		= false;
	m_pSnapshot					= NULL;
	m_nMediaID					= 0;
	m_pGPSCallback				= NULL;
	m_pFacePic					= NULL;
	m_pFacePicCmp				= NULL;
	m_bShotAlive				= false;
	m_bFaceCmpUpdate			= false;
	m_pFacePicUpload.PicUpdate	= false;
	m_bResendFaceCmpPic			= false;
	m_bRendFaceAuthPic			= false;
	m_nFileUploadSerial			= 0;
	
	pthread_mutex_init(&m_pFacePicUpload.Upload_mutex, NULL);
	pthread_mutex_init(&m_stFileMutex, NULL);
	pthread_mutex_init(&m_stShotMutex, NULL);
	pthread_mutex_init(&m_stFaceCmpMutex, NULL);
	pthread_mutex_init(&m_stUpFacePicMutex, NULL);
	pthread_mutex_init(&m_stFileUploadMutex, NULL);
	
}
  
CNetService::~CNetService()
{
	if(m_pobjFileService){
		delete m_pobjFileService;
		m_pobjFileService = NULL;
	}

	StopService();
	SAFE_DELETE_ARRAY(m_sendMsg.pData);
	pthread_mutex_destroy(&m_stShotMutex);
	pthread_mutex_destroy(&m_stFileMutex);
	pthread_mutex_destroy(&m_stFaceCmpMutex);
	pthread_mutex_destroy(&m_stUpFacePicMutex);
	pthread_mutex_destroy(&m_stFileUploadMutex);
	pthread_mutex_destroy(&m_pFacePicUpload.Upload_mutex);
}


void CNetService::InitService(InterFace *pInterFace, CNetService *pobjCNetService)
{
	//CNetInterFace父类虚函数全部由CDevNetInterface子 类实现
	//即通过CNetInterFace类的m_pInterFace对象调用CDevNetInterface类中的所有实现的函数
	m_fileMapList.clear();
	m_pInterFace = pInterFace;
	g_pMocNetInterface = m_pInterFace;
	s_pobjCNetService = pobjCNetService;

	m_bGpsUpload=false;
	m_bNetAlive = false;
//	m_AuthOk = false;
	Net_Communication_Attr param;
	m_pInterFace->GetNetworkParam(&param);

	m_pInterFace->GetSysCfg(&m_pSnapCtrl);
	/* 获取鉴权码 */
	m_szAuthLen = m_pInterFace->GetAuthorizeCode(m_szAuthCode);
	/* 获取心跳间隔 */

	m_nHeartTimer = param.heartbeat;
	/*初始化接收，发送类*/
	m_sendRcv.Init(SOCK_STREAM);        //建立网络通信套接字
	m_sendRcv.SetNetParam(param.tcp.timeout,param.tcp.trytimes);
	/* initialize signal parse/package instance*/
	m_SigProc.Init();
	m_SigProc.SetNetParam(param.tcp.timeout,param.tcp.trytimes);
	//m_SigProc.SetTransmitToCopyCB(CJT808Package::TransmitToCopy);
	m_SigPackage.Init(m_pInterFace);
	//m_SigPackage.SetAddSendBufCB(CNetService::Send);
	m_sigParse.Init(m_pInterFace,&m_SigPackage);
	m_sendMsg.pData = new char[2800];		//分配发送缓冲区大小
	if(m_sendMsg.pData)
	{
		memset(m_sendMsg.pData, 0, 2800);
	}

	m_pobjFileService = new CNetFileService();
	m_pobjFileService->InitService(pInterFace, param);
	m_pobjFileService->StartService();
}


bool CNetService::StartService()        //由main函数调用
{
#if 0
	JT808_Config_S *sysCfg = new JT808_Config_S;
	if (NULL == sysCfg)
	{
		return false;
	}
	memset(sysCfg, 0, sizeof(JT808_Config_S));

	memcpy(sysCfg->commCfg.phoneNumber, "352538106702783",15);
	printf("sysCfg->commCfg.phoneNumber|%s|\n", sysCfg->commCfg.phoneNumber);
	m_pInterFace->SetSysCfg(sysCfg);

	return true;
#else
	m_sendRcv.StartService();                                          //启动接收、发送、数据协议解析线程
	bool bRet = m_NetServiceThread.StartThread(NetServiceThread, this);     //启动MDVR网络配置设置服务线程
	m_FileManagerThread.StartThread(FileMapThread, this);
	m_ShotThread.StartThread(ShotThread, this);
	m_SnapCtrlThread.StartThread(SnapCtrlThread, this);
	m_FacePicUploadThread.StartThread(FacePicUploadThread, this);
	m_LocTrackingThread.StartThread(LocTrackThread, this);
	m_bStart = true;
	return bRet;
#endif
}

bool CNetService::StopService()
{
	if(m_bStart){
		m_bNetAlive = false;
		m_bGpsUpload = false;
		m_pInterFace->NetSetGnssCmsLinkState(m_bNetAlive);
		m_pobjFileService->StopService();
		m_sendRcv.StopService();
		m_ShotThread.StopThread();
		m_FileManagerThread.StopThread();
		m_NetServiceThread.StopThread();
		m_bStart = false;
	}
	return true;
}

/* 获取Socket连接状态 true 成功 false 失败 */
bool CNetService::GetNetAliveStatus(void)
{
	return	m_bNetAlive;
}

/* 获取保存GPS数据是否上传成功 true 成功 false 失败 */
bool CNetService::GetSaveGpsDataUploadStatus(void)
{
	return m_bGpsUpload;
}

void* CNetService::NetServiceThread(void *pUsrData)       
{
    CNetService *pThis = (CNetService*)pUsrData;
    return pThis->NetServiceProcess();
}

void* CNetService::NetServiceProcess()
{
	NetRcvMsg *pRcvMsg = NULL;
	time_t tCurTime  = 0;
	time_t tLastTime = time(NULL);
	time_t tLastHeartBeart = tLastTime;
	/*added for first connection*/
	bool firstRun = true;
	int heartTime = 0;
	m_userReTryTimes = 0;
	int acc_flag = 1;
	while(!m_NetServiceThread.GetExit(30))     
	{
		tCurTime = time(NULL);
		//从接收消息队列取元素，判断是否有消息，有则进入解析接口
//		DBG_LEVEL_6("before GetMsg.");
		pRcvMsg = m_sendRcv.GetMsg();
		if(pRcvMsg)
		{
			ProcRcvMsg(pRcvMsg);
			tLastTime = tCurTime;
		}else
		{
			//防止因为GPS修改时间而导致的触发
			if( tCurTime < tLastTime || tCurTime < tLastHeartBeart )       
			{
				tLastTime = tCurTime;
				tLastHeartBeart = tCurTime;
			}
			if(!m_sendRcv.ServerIsLinked())	//网络未连接，则与平台进行socket连接
			{
				if((tCurTime - tLastTime) >= 5 || firstRun)	//超时或者第一次
				{
					if(firstRun)
						firstRun = false;
					int nType;
					char szAddr[64];
					int nPort;
					m_pInterFace->GetServerAddr(nType, szAddr, nPort);
					DBG_LEVEL_6("Trying connect with server:%s:%d!", szAddr, nPort);
#if 0
					if(m_sendRcv.Link2Svr(szAddr,nPort))
					{	
						Login();
					}
#else
					if(strlen(szAddr) <= 0)
					{
						DBG_LEVEL_6("Server address invalid!");
					}
					else
					{
						if(m_sendRcv.Link2Svr(szAddr, nPort))
						{
							DBG_LEVEL_6("Server connected Success! Login...");
							Login();	//网络连接成功，进行登录
							if(!m_bNetAlive)
							{
								DBG_LEVEL_6("Server connected failed! m_bNetAlive is false.");
							}
						}
						else
						{
							DBG_LEVEL_6("Server connected failed!");
						}
					}
#endif
					tLastTime = tCurTime;
				}
			}else if(m_sendRcv.ServerIsLinked())	//网络已连接
			{
				if(m_bNetAlive)	//每次循环判断是否登录成功，成功则发送心跳包
				{
					if(acc_flag)	//acc on
					{
						heartTime = (int)m_nHeartTimer;	//60s
					}else		//acc off
					{
						heartTime = 300;			//5 minutes
					}
				
					if((tCurTime - tLastHeartBeart) >= heartTime)
					{
						/* 检测心跳间隔 */
//						printf("NetTest %d SendHeartBeat\n",tCurTime%1000);
						SendHeartBeat();              		
						tLastHeartBeart = tCurTime;	
					} else if((tCurTime - tLastTime) >= (int)(m_nHeartTimer*3)/2) {
						/* 检测心跳超时 */
//						printf("NetTest %d HeartBeat timeout\n",tCurTime%1000);
						m_bNetAlive = false;
						m_sendRcv.CloseConnect();
						m_pInterFace->NetSetGnssCmsLinkState(m_bNetAlive);					
						m_sendRcv.SetAuthorizeStatus(m_bNetAlive);
					}
				}else if(!m_bNetAlive)	//登录失败，则重新登录
				{

					if((tCurTime - tLastTime) >= 5)
					{
//						if(!m_LoginIng)	//登录过程结束(一轮注册鉴权)
						{
							m_userReTryTimes += 1;
							DBG_LEVEL_6("m_userReTryTimes = %d", m_userReTryTimes);
							if(m_userReTryTimes > 3)	//注册、鉴权和注销超过重试次数
							{
								/* 链接鉴权超时，断开后重连 */
	//							DBG_LEVEL_6("Has tried register failed 2 times, close socket link!");
	//							m_sendRcv.CloseConnect();
								Unregister();
								m_userReTryTimes = 0;
							}else
							{
								Login();	//网络连接成功，且重试次数小于3，进行登录
							}
							tLastTime = tCurTime;	//重置时间
						}
					}
				}
				//在网络连接的情况下，不管是否登录成功
				if((tCurTime - tLastTime) >= 5)
				{
					if((tCurTime - tLastTime) >= 90)
					{
						m_sendRcv.CloseConnect();
						m_userReTryTimes = 0;
						tLastTime = tCurTime;	//重置时间
					}
					if(mdFileSendFinished == false)
					{
						fileUpLoadOverTimeFlag = true;
						DBG_LEVEL_6("tCurTime - tLastTime >= 5, set fileUpLoadOverTimeFlag true and unlock s_stFileUploadCond...");
						pthread_mutex_lock(&m_stFileUploadMutex);
						pthread_cond_signal(&s_stFileUploadCond);
						pthread_mutex_unlock(&m_stFileUploadMutex);
					}
				}
			}
		}
//		DBG_LEVEL_6("end of NetServiceProcess...");
	}
	m_bNetAlive = false;
	return NULL;
	
}

void *CNetService::FileMapThread(void *param)
{
	CNetService *pThis = (CNetService*)param;
    return pThis->RemoveFileMap();
}

void *CNetService::RemoveFileMap(void)
{
	time_t tCurTime  = 0;
	while(!m_FileManagerThread.GetExit(2000)){
		tCurTime = time(NULL);
		pthread_mutex_lock(&m_stFileMutex);
		for(vector<Files_Map_S>::iterator iter = m_fileMapList.begin(); iter != m_fileMapList.end();){
			if(tCurTime - iter->time > 120){
				iter = m_fileMapList.erase(iter);
			} else {
				break;
			}
		}
		pthread_mutex_unlock(&m_stFileMutex);
	}

	return NULL;
}


void *CNetService::LocTrackThread(void *param)
{
	CNetService *pThis = (CNetService*)param;
    return pThis->LocationTrack();
}

void *CNetService::LocationTrack(void)
{
	static DWORD last_time		= time(NULL);
	DWORD cur_time 				= 0;
	DWORD total_interval		= 0;
	struGPSData gpsData;
	memset(&gpsData, 0, sizeof(struGPSData));
	
	while(!m_LocTrackingThread.GetExit(1000)){
		if (!m_LocationTrackInter){
			last_time		= time(NULL);
			usleep(60*1000);
			continue;
		}else{
			cur_time = time(NULL);
		
			DBG_LEVEL_6("inter:%d %d total:%d %d", cur_time - last_time, m_LocationTrackInter,
				total_interval, m_LocationTrackPeriod);

			if (cur_time - last_time > m_LocationTrackInter && 
				total_interval < m_LocationTrackPeriod ){
				total_interval += cur_time - last_time;
				last_time = cur_time;
				
				if(m_pGPSCallback){
					m_pGPSCallback(&gpsData);
				}

				ReportPosInfo(&gpsData);				 
			}else if (cur_time - last_time > m_LocationTrackInter && 
				total_interval > m_LocationTrackPeriod ){
				m_LocationTrackInter = 0;
			}
		}
	}
	
	return NULL;
}

void CNetService::SnapCtrlTimer(void* )
{
	BYTE adas_flag	= m_pSnapCtrl.snapCtrl.time.mask & (0x01 << 0);
	BYTE dsm_flag	= m_pSnapCtrl.snapCtrl.time.mask & (0x01 << 1);
	BYTE adas_upload	= m_pSnapCtrl.snapCtrl.time.savemask & (0x01 << 0);
	BYTE dsm_upload	= m_pSnapCtrl.snapCtrl.time.savemask & (0x01 << 1);;	

	static DWORD interval_time = 0, last_time = time(NULL), time_out = 0;
	DWORD cur_time = time(NULL);

	NetMdShotCmd mdShotCmd;
	memset(&mdShotCmd, 0, sizeof(NetMdShotCmd));
	vector<ShotResult> resultList;
	resultList.clear();
	

	if (m_pSnapCtrl.snapCtrl.timeunit){
		time_out = m_pSnapCtrl.snapCtrl.timeInterval * 60;
	}else{
		time_out = m_pSnapCtrl.snapCtrl.timeInterval;
	}

	if (adas_flag)
	{
		interval_time += (cur_time - last_time);
		last_time = cur_time;

		if (interval_time >= time_out){
			interval_time = 0;
			//snap
			ShotResult stShotResult;
			unsigned int pts = 0;
			if(m_pSnapshot(0x64, 1, 0, &pts, &stShotResult.filename[0])){
				stShotResult.pts = pts;
				stShotResult.media_id = m_nMediaID ++;
				resultList.push_back(stShotResult);

				mdShotCmd.chn = 1;

				UpMediaData(mdShotCmd, resultList, UPLOAD_EVENT_TIMER);
			}

		}
	}

	if (dsm_flag)
	{
		interval_time += (cur_time - last_time);
		last_time = cur_time;
		
		if (interval_time >= time_out){
			interval_time = 0;
			//snap
			ShotResult stShotResult;
			unsigned int pts = 0;
			if(m_pSnapshot(0x65, 1, 0, &pts, &stShotResult.filename[0])){
				stShotResult.pts = pts;
				stShotResult.media_id = m_nMediaID ++;
				resultList.push_back(stShotResult);

				mdShotCmd.chn = 2;

				UpMediaData(mdShotCmd, resultList, UPLOAD_EVENT_TIMER);
			}
		}
	}
	
}


void CNetService::SnapCtrlDistance(void* )
{

	BYTE adas_flag		= m_pSnapCtrl.snapCtrl.mileage.mask & (0x01 << 0);
	BYTE dsm_flag		= m_pSnapCtrl.snapCtrl.mileage.mask & (0x01 << 1);
	BYTE adas_upload	= m_pSnapCtrl.snapCtrl.mileage.savemask & (0x01 << 0);
	BYTE dsm_upload		= m_pSnapCtrl.snapCtrl.mileage.savemask & (0x01 << 1);;	

	static DWORD last_time = time(NULL), distance = 0;
	static DWORD total_mileage = 0;
	DWORD 		 cur_time = time(NULL);
	struGPSData gpsData;
	memset(&gpsData, 0, sizeof(struGPSData));
	NetMdShotCmd mdShotCmd;
	memset(&mdShotCmd, 0, sizeof(NetMdShotCmd));
	vector<ShotResult> resultList;
	resultList.clear();
	
	if (m_pSnapCtrl.snapCtrl.mileageunit){
		distance = m_pSnapCtrl.snapCtrl.timeInterval * 1000;//
	}else{
		distance = m_pSnapCtrl.snapCtrl.timeInterval;
	}

	
	if(m_pGPSCallback){
		m_pGPSCallback(&gpsData);
	}

	if (adas_flag)
	{
		
		total_mileage += gpsData.usSpeed * (cur_time - last_time);
		last_time = cur_time;
		
		if (total_mileage >= distance){
			total_mileage = 0;

			ShotResult stShotResult;
			unsigned int pts = 0;
			if(m_pSnapshot(0x64, 1, 0, &pts, &stShotResult.filename[0])){
				stShotResult.pts = pts;
				stShotResult.media_id = m_nMediaID ++;
				resultList.push_back(stShotResult);

				mdShotCmd.chn = 1;

				UpMediaData(mdShotCmd, resultList, UPLOAD_EVENT_DISTANCE);
			}
			//snap
		}
	}

	if (dsm_flag)
	{
		total_mileage += gpsData.usSpeed * (cur_time - last_time);
		last_time = cur_time;
		
		if (total_mileage >= distance){
			total_mileage = 0;
			//snap
			ShotResult stShotResult;
			unsigned int pts = 0;
			if(m_pSnapshot(0x64, 1, 0, &pts, &stShotResult.filename[0])){
				stShotResult.pts = pts;
				stShotResult.media_id = m_nMediaID ++;
				resultList.push_back(stShotResult);

				mdShotCmd.chn = 2;

				UpMediaData(mdShotCmd, resultList, UPLOAD_EVENT_DISTANCE);
			}
		}
	}
	
}

void *CNetService::SnapCtrlThread(void *param)
{
	CNetService *pThis = (CNetService*)param;
    return pThis->SnapCtrl();
}

void *CNetService::SnapCtrl(void)
{
	while(!m_SnapCtrlThread.GetExit(SNAP_CTRL_LOOP_INTERAL)){
		SnapCtrlDistance(NULL);
		SnapCtrlTimer(NULL);
	}
	return NULL;
}

void *CNetService::FacePicUploadThread(void *param)
{
	CNetService *pThis = (CNetService*)param;
    return pThis->FacePicUpload();
}

void *CNetService::FacePicUpload(void)
{
	char pic_name[128] = {0};
	char pic_type = 0;
	char update = 0;//1:UpFaceCmpResult  2:UpFacePic
	WORD msg_serial = 0;
	while(!m_FacePicUploadThread.GetExit(FaceCmp_Report_LOOP_INTERAL)){
		FaceCmpResult CmpResult;
		memset(&CmpResult, 0, sizeof(FaceCmpResult));
		VideoUploadResInfo AVInfo;
		memset(&AVInfo, 0, sizeof(VideoUploadResInfo));
		
		pthread_mutex_lock(&m_stFaceCmpMutex);
		if(m_bFaceCmpUpdate){
			memcpy(&CmpResult, &m_stNetFaceCmpReport, sizeof(FaceCmpResult));
			DBG_LEVEL_6("pic_name:%s", m_stNetFaceCmpReport.pic_name);
			m_bFaceCmpUpdate = false;
			update = 1;
		} 
		pthread_mutex_unlock(&m_stFaceCmpMutex);

		if (1 == update){
			UpFaceCmpResult(&CmpResult);
			update = 0;
		}

		pthread_mutex_lock(&m_pFacePicUpload.Upload_mutex);
		if (m_pFacePicUpload.PicUpdate)
		{
			memset(pic_name, 0, 128);
			memcpy(pic_name, m_pFacePicUpload.pic_name, 128);
			pic_type 					= m_pFacePicUpload.pic_type;
			msg_serial 					= m_pFacePicUpload.serial;
			m_pFacePicUpload.PicUpdate 	= false;
			update 						= 2;
			DBG_LEVEL_6("pic_name:%s", pic_name);
		}
		pthread_mutex_unlock(&m_pFacePicUpload.Upload_mutex);

		if (2 == update){
			//upload face auth picture
			UploadFaceAuthPic(pic_name, pic_type, msg_serial);
			update = 0;
		}	
	}

	return NULL;
}

static DWORD getTime(char *timeNow)
{
	time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(timeNow, "%d-%d-%d_%d-%d-%d", \
        timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, \
        timeinfo->tm_mday, timeinfo->tm_hour, \
        timeinfo->tm_min, timeinfo->tm_sec);
    printf("time now is %s.\n", timeNow);

	return mktime(timeinfo);
}

void *CNetService::ShotThread(void *param)
{
	CNetService *pThis = (CNetService*)param;
    return pThis->Shot();
}

void *CNetService::Shot(void)
{
	while(!m_ShotThread.GetExit(100))
	{
		NetMdShotCmd mdShotCmd;
		pthread_mutex_lock(&m_stShotMutex);
		if(m_bShotAlive){
			memcpy(&mdShotCmd, &m_stNetMdShotCmd, sizeof(NetMdShotCmd));
			m_bShotAlive = false;
		} else {
			sleep(1);
			pthread_mutex_unlock(&m_stShotMutex);
			continue;
		}
		pthread_mutex_unlock(&m_stShotMutex);

		vector<ShotResult> resultList;
		resultList.clear();
		ShotResult stShotResult;
		memset(&stShotResult, 0, sizeof(stShotResult));
		if(NULL == m_pSnapshot)
		{	//拍摄回调函数指针为空，直接回复平台
			resultList.push_back(stShotResult);
			CameraSnapShotReply(mdShotCmd.wSerialNumber, resultList, 0);
			continue;
		}
		//立即获取当前时间戳作为id，回复平台
		char *timeNow;
		unsigned int pts = getTime(timeNow);
		m_nMediaID = pts;
		stShotResult.media_id = m_nMediaID;
		stShotResult.pts = m_nMediaID;
		memcpy(stShotResult.filename, timeNow, strlen(timeNow));
		resultList.push_back(stShotResult);
		CameraSnapShotReply(mdShotCmd.wSerialNumber, resultList, 0);
		int ch_id = (int)mdShotCmd.chn;
		//根据命令类型进行拍摄，目前默认拍摄10秒短视频，默认只拍一张图,拍摄并获取文件名
		if(m_pSnapshot(ch_id, mdShotCmd.cmd, mdShotCmd.period, &pts, &stShotResult.filename[0]))
		{
			if(!mdShotCmd.issave)	//上传拍摄图像
			{
				resultList.clear();
				int pts = 0;
				ShotResult stShotResult;
				memset(&stShotResult, 0, sizeof(stShotResult));

				stShotResult.media_id	= m_nMediaID;
				if(mdShotCmd.cmd == 0xFFFF)
				{
					stShotResult.type		= 2;
					stShotResult.enctype	= 5;
				}else
				{
					stShotResult.type		= 0;
					stShotResult.enctype	= 1;
				}
				stShotResult.event		= UPLOAD_EVENT_PLATFORM;	//tmp.event;
				stShotResult.chn		= mdShotCmd.chn;
				resultList.push_back(stShotResult);
				UpMediaData(mdShotCmd, resultList, UPLOAD_EVENT_PLATFORM);
			}
		}
	}
	
	return NULL;
}
#if 0
void *CNetService::Shot(void)
{
	while(!m_ShotThread.GetExit(100)){
		if(NULL == m_pSnapshot){
			return NULL;
		}
		
		NetMdShotCmd mdShotCmd;
		pthread_mutex_lock(&m_stShotMutex);
		if(m_bShotAlive){
			memcpy(&mdShotCmd, &m_stNetMdShotCmd, sizeof(NetMdShotCmd));
			m_bShotAlive = false;
		} else {
			pthread_mutex_unlock(&m_stShotMutex);
			continue;
		}
		pthread_mutex_unlock(&m_stShotMutex);

		vector<ShotResult> resultList;
		resultList.clear();

		
//		int ret = m_pInterFace->MdShotCmd(mdShotCmd, resultList);
//		CameraSnapShotReply(mdShotCmd.wSerialNumber, resultList, ret);
		if(mdShotCmd.cmd == START_RECORD_VIDEO)
		{
			//开始录像
			ShotResult stShotResult;
			unsigned int pts = 0;
			memset(&stShotResult, 0, sizeof(stShotResult));
			if(m_pSnapshot(ch_id, mdShotCmd.cmd, mdShotCmd.period, &pts, &stShotResult.filename[0]))
			CameraSnapShotReply(mdShotCmd.wSerialNumber, resultList, 0);
			printf("gggggggggggggggggggggggggggggggggggggggg1\n");
			//取得开始录像时刻的gps数据，并建立对应文件存储gps数据	
		}else if(mdShotCmd.cmd == STOP_SHOT)
		{
			//停止录像
			if(mdShotCmd.issave)	//只存储不上传视频文件
			{
				CameraSnapShotReply(mdShotCmd.wSerialNumber, resultList, 0);
				printf("gggggggggggggggggggggggggggggggggggggggg2\n");
				
			}else
			{
				CameraSnapShotReply(mdShotCmd.wSerialNumber, resultList, 0);
				printf("gggggggggggggggggggggggggggggggggggggggg3\n");
				//上传视频文件
				usleep(500*1000);
				UpMediaData(mdShotCmd, resultList, UPLOAD_EVENT_PLATFORM);
			}
		}else if(mdShotCmd.cmd == SNAP_FACE_AUTH)
		{
			//人脸识别
			char filename[128] = {0};
			unsigned int pts = 0;
			if (m_pFaceSnap(0x65, &pts, filename) )
			{
				//upload picture
				pthread_mutex_lock(&m_pFacePicUpload.Upload_mutex);
				m_pFacePicUpload.PicUpdate	= true;
				m_pFacePicUpload.ts			= pts;
				m_pFacePicUpload.pic_type	= PICTRUE_TYPE_JPEG;
				m_pFacePicUpload.serial		= mdShotCmd.wSerialNumber;
				memcpy(m_pFacePicUpload.pic_name, filename, 128);
				pthread_mutex_unlock(&m_pFacePicUpload.Upload_mutex);
			}
			CameraSnapShotReply(mdShotCmd.wSerialNumber, resultList, 0);
		}else
		{
			//拍照
			BYTE ch_id = mdShotCmd.chn;
			DBG_LEVEL_6("chn:%02x ", ch_id);
			//根据张数拍照
			for(int i = 1; i <= mdShotCmd.cmd; ++ i){
				ShotResult stShotResult;
				unsigned int pts = 0;
				memset(&stShotResult, 0, sizeof(stShotResult));
				if(m_pSnapshot(ch_id, mdShotCmd.cmd, mdShotCmd.period, &pts, &stShotResult.filename[0])) //拍照并取得媒体id，文件名
				{
					stShotResult.pts = pts;
					stShotResult.media_id = m_nMediaID ++;
					resultList.push_back(stShotResult);
					printf("gggggggggggggggggggggggggggggggggggggggg4\n");
					//取得抓拍照片时刻的gps数据，并建立对应文件存储gps数据
					
				}
				//拍照间隔
				if(i < mdShotCmd.cmd){
					sleep(mdShotCmd.period);
				}
			}

			if(mdShotCmd.issave)	//只存储不上传拍照图片
			{
				printf("gggggggggggggggggggggggggggggggggggggggg5\n");
				CameraSnapShotReply(mdShotCmd.wSerialNumber, resultList, resultList.size() > 0 ? 0 : 1);
			}else
			{
				printf("gggggggggggggggggggggggggggggggggggggggg6\n");
				CameraSnapShotReply(mdShotCmd.wSerialNumber, resultList, resultList.size() > 0 ? 0 : 1);
				usleep(500*1000);
				UpMediaData(mdShotCmd, resultList, UPLOAD_EVENT_PLATFORM);
			}
		}
	}
	
	return NULL;
}
#endif

//处理接收到来自平台的消息体
//根据接收的命令(服务器)做相应的操作
void CNetService::ProcRcvMsg(NetRcvMsg* pMsg)               
{
	static WORD nSerialNum = 0;
	/*业务处理*/
	DBG_LEVEL_6("ProcRcvMsg ID :0x%x. SN::%d", pMsg->wId, pMsg->wSerialNumber);
	switch( pMsg->wId )
	{
		case NET_CMD_CMS_REPLY:           //平台通用应答
			ProcCmsReply(pMsg, pMsg->wSerialNumber);
			break;
		case NET_CMD_REGISTER_REPLY://终端注册和应答
			ProcRegisterReply(pMsg);//保存注册码，发送注册码(鉴权消息)==登录到平台
			break;
		case NET_CMD_CONFIG_SET:	 //设置终端参数	
			SetSysCfg(pMsg);
			break;
		case NET_CMD_CONFIG_GET:     //查询终端所有参数
			GetParam(pMsg);
			break;
		case NET_CMD_ENQUIRY_DEVICE_PARAM:     //查询平台指定的终端参数
			GetSelectParam(pMsg);
			break;
		case NET_CMD_CTRL:
			TerminalCtrl(pMsg);       		//终端控制
			break;
		case NET_CMD_DEV_PROPERTY:
			GetDevAttr(pMsg);
			break;
		case NET_CMD_SOFTWARE_UPGRADE://upgrade
			Upgrade(pMsg);
			break;
		case NET_CMD_POS_INFO_GET:   		//位置信息查询
			ReplyPosition(pMsg);
			break;
		case NET_CMD_LOCATION_TRACKING:
			LocationTracking(pMsg);    //协议要求实现控制周期性发送临时位置跟踪消息的启动与停止
			break;
		
		case NET_CMD_ALARM_CONFIRM: 		//人工确认报警	
			ConfirmAlarm(pMsg);
			break;
		
		case NET_CMD_TEXT_INFO_DOWN:        //文本信息下发，按指定的方式TTS朗读、LCD显示等通知驾驶员
			TextInformation(pMsg);
			break;

		case NET_CMD_EVENT_SET:
			EventSet( pMsg);  	//事件设置
			break;

		case NET_CMD_ASK_DOWN:              //提问下发
			QuestionDown(pMsg);
			break;

		case NET_CMD_INFO_OD_MENU_SET:
			InfoDmdMenuSet(pMsg);          //信息点播菜单设置
			break;
 
		case NET_CMD_INFO_SERVICE:          //信息服务
			InfoService(pMsg);
			break;
			
		case NET_CMD_TEL_CALLBACK:          //电话回拨
			TelCallback(pMsg);
			break;

		case NET_CMD_TELBOOK_SET:           //设置电话本
			TelBookSet(pMsg);
			break;

		case NET_CMD_VEHICLE_CTRL:          //车辆控制
			VehicleCtrl(pMsg);
			break;

		case NET_CMD_CIRCLE_AREA_SET:
			SetRingRgn(pMsg);							//设置圆形区域
			break;

		case NET_CMD_RECT_AREA_SET:
			SetRectRgn(pMsg);							//设置矩形区域
			break;

		case NET_CMD_POLYGON_AREA_SET:   	//设置多边形区域
			SetPolyRgn(pMsg);
			break;
			
		case NET_CMD_CIRCLE_AREA_DEL:       // 删除区域
			DelRingRgn(pMsg);
			break;
		
		case NET_CMD_RECT_AREA_DEL:
			DelRectRgn(pMsg);
			break;	
		
		case NET_CMD_POLYGON_AREA_DEL:
			DelPolyRgn(pMsg);
			break;

		case NET_CMD_PATH_SET:              //设置路线
			SetRoute(pMsg);
			break;

		case NET_CMD_PATH_DEL:              //删除路线
			DelRoute(pMsg);
			break;
	
		case NET_CMD_DRIVING_DATA_PICK:     //行驶记录仪数据采集命令
			PickDrivingRecord(pMsg);
			break;

		case NET_CMD_DRIVING_PARAM_DOWN:    //行驶记录仪参数下传命令
 			SetDrivingRecord( pMsg);
			break;

		case NET_CMD_DATA_DOWN_TAANSPARENT_TRANS: //数据下行透传
			DownLinkTransparent(pMsg);
			break;

		case NET_CMD_DRIVER_INFO_REQUEST:		//驾驶员身份信息上报
			ReqDriverInfo(pMsg);
			break;

			//终端上传到平台的多媒体数据丢包时，平台通过
			//此消息ID告诉终端丢失了哪些包,终端给予补传丢失的数据包。
		case NET_CMD_MUTIMEDIA_DATA_UP_REPLY:   //多媒体数据上传应答
			ProcMultimediaUpReply( pMsg);	
			break;

		case NET_CMD_CAMERA_SHOT:                 	   //摄像头通道1立即抓拍命令
			CameraShot( pMsg);
			break;

		//检索图片列表&& 检索音频列表&&  检索视频列表
		case NET_CMD_STORE_MUTLIMEDIA_SEARCH:            //储存多媒体文件检索
			MultimediaSearch( pMsg);
			break;
		case NET_CMD_SINGLE_MUTLIMEDIA_UPLOAD:			//单条储存多媒体文件检索
			SingleMultimediaSearch( pMsg);
			break;
		case NET_CMD_STORE_MUTLIMEDIA_UP:                //储存多媒体数据上传
			MultimediaUp( pMsg);
			break;
		case NET_CMD_RECORD_SOUND_START:                 //录音开始命令
			StartRecord( pMsg);
			break;
		case NET_CMD_FILE_SERVER_INFO:
			AlarmAttachmentUpload(pMsg, nSerialNum);
			m_bAlarmUploadRespone = true;
			break;
		case NET_CMD_FACE_CMP_RESULT_UP_REPLY:		//人脸比对结果上传应答
			FaceCompareResultReply(pMsg);
			break;
		case NET_CMD_FACE_PIC_PUBLISH:				//人脸图片下发
			FacePicturePublish(pMsg);
			break;

		case NET_CMD_FACE_INFO_QUERY:				//人脸信息查询
			FaceInfoQuery(pMsg);
			break;
		
		case NET_CMD_FACE_CMP_TRIGGER:          //face compare active triggering
			TriggerFaceCompare(pMsg);
			break;
		case NET_CMD_FACE_CMP_RESULT_PUB_REPLY:		//人脸比对结果下发应答
			FaceCompareResultPublicReply(pMsg);
			break;
		case NET_CMD_FACE_AUTH_ACTIVE_SNAP:			//人脸认证照片主动抓拍
			FaceAuthActiveSnap(pMsg);
			break;
		case NET_CMD_FACE_AUTH_UPLOAD_REPLY:
			FaceCompareResultReply(pMsg);		//Upload Face Authentication Picture Response
			break;
		/*Audio-Video Communication.*/
		case NET_CMD_AV_CONF_GET:             //查询终端音视频属性
			VideoConfGet(pMsg);
			break;

		case NET_CMD_AV_REQ_STREAM:           //实时音视频传输请求
			VideoReqRealTimeStream(pMsg);
			break;

		case NET_CMD_AV_CTRL_TRANS:           //音视频实时传输控制
			VideoTransControl(pMsg);
			break;

		case NET_CMD_AV_TRANS_STATUS:         //实时音视频传输状态通知
			VideoTransStatusNotice(pMsg);
			break;

		case NET_CMD_AV_QUE_RESOURCE_LIST:    //获取音视频资源文件
			VideoQueResourceList(pMsg);
			break;

		case NET_CMD_AV_REQ_PLAYBACK:         //录像回放请求
			VideoReqPlayback(pMsg);
			break;

		case NET_CMD_AV_PLAYBACK_CTRL:       //录像回放控制
			VideoPlaybackCtrl(pMsg);
			break;

		case NET_CMD_AV_UPLOAD_FILE:       //上传音视频文件
			VideoReqUploadFile(pMsg);
			break;

		case NET_CMD_AV_UPLOAD_FILE_CTRL:  //上传文件控制
			VideoCtrlUploadFile(pMsg);
			break;
		/*END.Audio-Video Communication.*/

		default:
			DBG_LEVEL_6("Invalid Message ID");
			//m_SigPackage.CommReply(pMsg->wSerialNumber, pMsg->wId, 3);
			char szData[32] = {0};
			DWORD dataLen = 0;
			WORD wSerialNum = m_SigPackage.CommReply(szData, dataLen, pMsg->wSerialNumber, pMsg->wId, 3);
			Send(NET_CMD_REPLY, wSerialNum, szData, dataLen, false, 0, 0);
			break;
	}
}

void CNetService::SetSysCfg(NetRcvMsg* pMsg)
{
	JT808_Config_S *sysCfg = new JT808_Config_S;		//保存平台参数设置
	if (NULL == sysCfg)
	{
		return ;
	}
	memset(sysCfg, 0, sizeof(JT808_Config_S));
	m_pInterFace->GetSysCfg(sysCfg);
	m_sigParse.SetSysCfg(pMsg, sysCfg);

	memcpy(&m_pSnapCtrl, sysCfg, sizeof(JT808_Config_S));

	// 将配置写入文件
	int nRet = m_pInterFace->SetSysCfg(sysCfg);
//	if (m_pSetTermCfg){
//		m_pSetTermCfg(sysCfg);
//	}

	CommReply(pMsg->wSerialNumber, pMsg->wId, nRet);

	if (sysCfg){
		delete sysCfg;
	}
}

void CNetService::GetParam(NetRcvMsg* pMsg)
{
	vector<PackageSendMsg> vecPackageMsg;
	vecPackageMsg.clear();
	m_SigPackage.ReplyParam(vecPackageMsg, 0, NULL, pMsg->wSerialNumber);
	
	WORD nSize = vecPackageMsg.size();
	for (WORD i = 0; i < nSize; i++) {
		Send(NET_CMD_CONFIG_GET_REPLY, vecPackageMsg[i].nSerialNum, vecPackageMsg[i].strEvPackage, vecPackageMsg[i].nPackageSize, false, 0, 0);
	}
}

inline void CNetService::GetSelectParam(NetRcvMsg* pMsg)
{
	DWORD dParamTotal = 0;
	DWORD paramID[256] = {0};
	vector<PackageSendMsg> vecPackageMsg;
	vecPackageMsg.clear();

	m_sigParse.GetSelectParam(pMsg, &paramID[0], dParamTotal);
	m_SigPackage.ReplyParam(vecPackageMsg, dParamTotal, &paramID[0], pMsg->wSerialNumber);

	WORD nSize = vecPackageMsg.size();
	for (WORD i = 0; i < nSize; i++) {
		Send(NET_CMD_CONFIG_GET_REPLY, vecPackageMsg[i].nSerialNum, vecPackageMsg[i].strEvPackage, vecPackageMsg[i].nPackageSize, false, 0, 0);
	}
}

inline void CNetService::TerminalCtrl(NetRcvMsg* pMsg)
{
	CommReply(pMsg->wSerialNumber, pMsg->wId, 0);
}

inline void CNetService::GetDevAttr(NetRcvMsg* pMsg)
{
	char szData[128] = {0};
	DWORD dataLen = 0;
	WORD wSerialNum = m_SigPackage.ReplyPoperty(szData, dataLen);
	Send(NET_CMD_DEV_PROPERTY_REPLY, wSerialNum, szData, dataLen, false, 0, 0);
}

inline void CNetService::Upgrade(NetRcvMsg* pMsg)
{
	char szVerNum[32] = {0};
	DWORD nLen = 0;
	m_sigParse.Upgrade(pMsg, szVerNum, nLen);
	CommReply(pMsg->wSerialNumber, pMsg->wId, 0);
	m_pInterFace->Upgrade(szVerNum, pMsg->pData, nLen);
}

inline void CNetService::ReplyPosition(NetRcvMsg* pMsg)
{
	char szData[512] = {0};
	DWORD dataLen = 0;
	struGPSData gpsData;
	memset(&gpsData, 0, sizeof(struGPSData));
	if (m_pGPSCallback) {
		m_pGPSCallback(&gpsData);
		WORD wSerialNum = m_SigPackage.ReplyPosition(szData, dataLen, pMsg->wSerialNumber, gpsData);
		Send(NET_CMD_POS_INFO_GET_REPLY, wSerialNum, szData, dataLen, false, 0, 0);
	}
}

inline void CNetService::LocationTracking(NetRcvMsg* pMsg)
{
	WORD interval = 0;
	DWORD time = 0;
	m_sigParse.LocationTracking(pMsg, interval, time);
	m_LocationTrackInter = interval;
	m_LocationTrackPeriod = time; 
	int ret = 0;

	CommReply(pMsg->wSerialNumber,pMsg->wId,ret);
}

inline void CNetService::ConfirmAlarm(NetRcvMsg* pMsg)
{
	DWORD alarmMask = 0;
	m_sigParse.ConfirmAlarm(pMsg, alarmMask);

	int ret = m_pInterFace->ConfirmAlarm(alarmMask);
	CommReply(pMsg->wSerialNumber,pMsg->wId,ret);
}

inline void CNetService::SMSProc(NetRcvMsg* pMsg)
{
	char strSms[1024] = {0};
	SMS_MASK_E flag;
	m_sigParse.SMSProc(pMsg, strSms, flag);
	int nRet = m_pInterFace->SMSProc(strSms, flag);
	/*文本信息下发回复*/
	CommReply(pMsg->wSerialNumber, pMsg->wId, nRet);
}

inline void CNetService::EventSet(NetRcvMsg* pMsg)
{
	InfoOp opr;
	BYTE num = {0};
	vector<Event_Menu_Item> vecMenuItem;
	vecMenuItem.clear();
	m_sigParse.EventSet(pMsg, opr, vecMenuItem, num);
	size_t nSize = vecMenuItem.size();
	Event_Menu_Item* pMenuItem = new Event_Menu_Item[nSize];

	for (BYTE i = 0; i < nSize; i++) {
		pMenuItem[i].id = vecMenuItem[i].id;
		memcpy(pMenuItem[i].event, vecMenuItem[i].event, sizeof(vecMenuItem[i].event));
	}
	int ret = m_pInterFace->SetEventMenu(opr, pMenuItem, num);
	CommReply( pMsg->wSerialNumber, pMsg->wId,ret);
	if (NULL != pMenuItem) {
		delete []pMenuItem;
	}
}

inline void CNetService::QuestionDown(NetRcvMsg* pMsg)
{
	char szQustion[1024] = {0};
	int num = 0;
	SMS_MASK_E flag;
	vector<Event_Menu_Item> vecMenuItem;
	vecMenuItem.clear();
	m_sigParse.QuestionDown(pMsg, szQustion, flag, vecMenuItem, num);

	int nSize = vecMenuItem.size();
	Event_Menu_Item* pMenuItem = new Event_Menu_Item[nSize];
	for (BYTE i = 0; i < nSize; i++) {
		pMenuItem[i].id = vecMenuItem[i].id;
		memcpy(pMenuItem[i].event, vecMenuItem[i].event, sizeof(vecMenuItem[i].event));
	}

	int ret = m_pInterFace->SetAskAnswer(szQustion, flag, pMenuItem, num);
	CommReply(pMsg->wSerialNumber, pMsg->wId, ret);
	if (NULL != pMenuItem) {
		delete []pMenuItem;
	}
}

inline void CNetService::InfoDmdMenuSet(NetRcvMsg* pMsg)
{
	InfoOp opr;
	BYTE num = {0};
	vector<Msg_Menu_Item> vecMenuItem;
	vecMenuItem.clear();
	m_sigParse.InfoDmdMenuSet(pMsg, opr, vecMenuItem, num);

	int nSize = vecMenuItem.size();
	Msg_Menu_Item* pMenuItem = new Msg_Menu_Item[nSize];
	for (BYTE i = 0; i < nSize; i++) {
		pMenuItem[i].type = vecMenuItem[i].type;
		memcpy(pMenuItem[i].msgname, vecMenuItem[i].msgname, sizeof(vecMenuItem[i].msgname));
	}

	int ret = m_pInterFace->SetMsgMenu(opr, pMenuItem, num);
	CommReply( pMsg->wSerialNumber, pMsg->wId, ret);
	if (NULL != pMenuItem) {
		delete []pMenuItem;
	}
}

inline void CNetService::InfoService(NetRcvMsg* pMsg)
{
	BYTE msgType = {0};
	char *stMessage = (char *)malloc(sizeof(char) * 1024);
	m_sigParse.InfoService(pMsg, msgType, stMessage);
	int ret = m_pInterFace->MessageService(msgType, stMessage);
	CommReply( pMsg->wSerialNumber, pMsg->wId, ret);
	free(stMessage);
	stMessage = NULL;
}

inline void CNetService::TelCallback(NetRcvMsg* pMsg)
{
	char szPhoneNo[24] = {0};
	BYTE bMark = {0};

	m_sigParse.TelCallback(pMsg, bMark, szPhoneNo);
	int ret = m_pInterFace->TelCallback(bMark, szPhoneNo);
	CommReply(pMsg->wSerialNumber,pMsg->wId,ret);
}

inline void CNetService::TelBookSet(NetRcvMsg* pMsg)
{
	InfoOp opr;
	BYTE num = {0};
	vector<Address_Book_Item> vecAddrBook;
	vecAddrBook.clear();

	m_sigParse.TelBookSet(pMsg, opr, vecAddrBook, num);
	int nSize = vecAddrBook.size();
	Address_Book_Item* pBookItem = new Address_Book_Item[nSize];
	for (int i = 0; i < nSize; ++i) {
		pBookItem[i].flag = vecAddrBook[i].flag;
		memcpy(pBookItem[i].name, vecAddrBook[i].name, sizeof(vecAddrBook[i].name));
		memcpy(pBookItem[i].phone, vecAddrBook[i].phone, sizeof(vecAddrBook[i].phone));
	}
	int ret = m_pInterFace->TelBookSet((InfoOp)opr, pBookItem, num);
	CommReply(pMsg->wSerialNumber, pMsg->wId, ret);
	if (NULL != pBookItem) {
		delete []pBookItem;
	}
}

inline void CNetService::VehicleCtrl(NetRcvMsg* pMsg)
{
	char szData[512] = {0};
	DWORD dataLen = 0;
	BYTE bDoorLock = {0};

	m_sigParse.VehicleCtrl(pMsg, bDoorLock);
	m_pInterFace->VehiDoorCtrlSet(bDoorLock);

	WORD wSerialNum = m_SigPackage.ReplyControl(szData, dataLen, pMsg->wSerialNumber);
	Send(NET_CMD_VEHICLE_CTRL_REPLY, wSerialNum, szData, dataLen, true, 0, 0);
}

inline void CNetService::SetRingRgn(NetRcvMsg* pMsg)
{
	BYTE opr = {0};
	BYTE regNum = {0};
	vector<Ring_Region_Attr> vecRegionAttr;
	vecRegionAttr.clear();

	m_sigParse.SetRingRgn(pMsg, opr, vecRegionAttr, regNum);
	int nSize = vecRegionAttr.size();
	Ring_Region_Attr* pRegionAttr = new Ring_Region_Attr[nSize];
	for (int i = 0; i < nSize; ++i) {
		pRegionAttr[i].attr.id = vecRegionAttr[i].attr.id;
		pRegionAttr[i].attr.attr = vecRegionAttr[i].attr.attr;
		memcpy(pRegionAttr[i].attr.stime, vecRegionAttr[i].attr.stime, sizeof(vecRegionAttr[i].attr.stime));
		memcpy(pRegionAttr[i].attr.etime, vecRegionAttr[i].attr.etime, sizeof(vecRegionAttr[i].attr.etime));
		pRegionAttr[i].attr.overspeedtime = vecRegionAttr[i].attr.overspeedtime;
		pRegionAttr[i].attr.speedmax = vecRegionAttr[i].attr.speedmax;
		pRegionAttr[i].point.lattitude = vecRegionAttr[i].point.lattitude;
		pRegionAttr[i].point.longtitude = vecRegionAttr[i].point.longtitude;
		pRegionAttr[i].rd = vecRegionAttr[i].rd;
	}
	int result = m_pInterFace->SetRingRgn(opr, pRegionAttr, regNum);
	CommReply(pMsg->wSerialNumber, pMsg->wId, result);
	if (NULL != pRegionAttr) {
		delete []pRegionAttr;
	}
}

inline void CNetService::SetRectRgn(NetRcvMsg* pMsg)
{
	BYTE opr = {0};
	BYTE regNum = {0};
	vector<Rectangle_Region_Attr> vecRectangleAttr;
	vecRectangleAttr.clear();

	m_sigParse.SetRectRgn(pMsg, opr, vecRectangleAttr, regNum);
	int nSize = vecRectangleAttr.size();
	Rectangle_Region_Attr* pRectangleAttr = new Rectangle_Region_Attr[nSize];
	for (int i = 0; i < nSize; ++i) {
		pRectangleAttr[i].attr.attr = vecRectangleAttr[i].attr.attr;
		pRectangleAttr[i].attr.id = vecRectangleAttr[i].attr.id;
		memcpy(pRectangleAttr[i].attr.stime, vecRectangleAttr[i].attr.stime, sizeof(vecRectangleAttr[i].attr.stime));
		memcpy(pRectangleAttr[i].attr.etime, vecRectangleAttr[i].attr.etime, sizeof(vecRectangleAttr[i].attr.etime));
		pRectangleAttr[i].attr.overspeedtime = vecRectangleAttr[i].attr.overspeedtime;
		pRectangleAttr[i].attr.speedmax = vecRectangleAttr[i].attr.speedmax;
		pRectangleAttr[i].leftup.lattitude = vecRectangleAttr[i].leftup.lattitude;
		pRectangleAttr[i].leftup.longtitude = vecRectangleAttr[i].leftup.longtitude;
		pRectangleAttr[i].rightdown.lattitude = vecRectangleAttr[i].rightdown.lattitude;
		pRectangleAttr[i].rightdown.longtitude = vecRectangleAttr[i].rightdown.longtitude;
	}
	int result = m_pInterFace->SetRectRgn(opr, pRectangleAttr, regNum);
	CommReply(pMsg->wSerialNumber, pMsg->wId, result);
	if (NULL != pRectangleAttr) {
		delete []pRectangleAttr;
	}
}

inline void CNetService::SetPolyRgn(NetRcvMsg* pMsg)
{
	Poly_Region_Attr* pRegion = (Poly_Region_Attr *)malloc(sizeof(Poly_Region_Attr));
	m_sigParse.SetPolyRgn(pMsg, pRegion);

	int result = m_pInterFace->SetPolyRgn(pRegion);
	CommReply(pMsg->wSerialNumber,pMsg->wId,result);
	delete []pRegion;
}

inline void CNetService::SetRoute(NetRcvMsg* pMsg)
{
	Route_Region_Attr* pRegion = (Route_Region_Attr *)malloc(sizeof(Route_Region_Attr));
	m_sigParse.SetRoute(pMsg, pRegion);

	int result = m_pInterFace->SetRouteRgn(pRegion);
	CommReply(pMsg->wSerialNumber, pMsg->wId, result);
	delete []pRegion;
}

inline void CNetService::DownLinkTransparent(NetRcvMsg* pMsg)
{
	char strBuffer[MEM_SIZE_K * 4] = {0};
	TRANSPARENT_TYPE_E type;
	int nSize = 0;

	m_sigParse.DownLinkTrans(pMsg, strBuffer, type, nSize);
	int result = m_pInterFace->DownlinkTrans(type, strBuffer, nSize);
	CommReply(pMsg->wSerialNumber,pMsg->wId,result);
}

inline void CNetService::ReqDriverInfo(NetRcvMsg* pMsg)
{
	char szData[1024] = {0};
	DWORD dataLen = 0;

	WORD wSerialNum = m_SigPackage.ReportDriverInfo(szData, dataLen);
	Send(NET_CMD_DRIVER_IDINFO_UP, wSerialNum, szData, dataLen, true, 0, 0);
}


inline void CNetService::DelRgn(NetRcvMsg* pMsg, int rgnType)
{
	DWORD rgnID[128] = {0};
	DWORD rgnNum = 0;
	//m_ParseMsg.DelRgn(pMsg, &rgnID[0], rgnNum);
	m_sigParse.DelRgn(pMsg, &rgnID[0], rgnNum);
	int result = m_pInterFace->DelRgn(rgnType, rgnID, rgnNum);
	CommReply(pMsg->wSerialNumber, pMsg->wId, result);
}

inline void CNetService::DelRingRgn(NetRcvMsg* pMsg)
{
	DelRgn(pMsg, 0);
}

inline void CNetService::DelRectRgn(NetRcvMsg* pMsg)
{
	DelRgn(pMsg, 1);
}

inline void CNetService::DelPolyRgn(NetRcvMsg* pMsg)
{
	DelRgn(pMsg, 2);
}

inline void CNetService::DelRoute(NetRcvMsg* pMsg)
{
	DelRgn(pMsg, 3);
}

//终端通用应答函数
void CNetService::CommReply( WORD wSerialNumber, WORD wId, BYTE bResult)
{
	char szData[64] = {0};
	DWORD dataLen = 0;
	//WORD wSerialNum = m_PackageMsg.CommReply(szData, dataLen, wSerialNumber, wId, bResult);
	WORD wSerialNum = m_SigPackage.CommReply(szData, dataLen, wSerialNumber, wId, bResult);
	Send(NET_CMD_REPLY, wSerialNum, szData, dataLen, false, 0, 0);
}

void CNetService::CameraSnapShotReply(WORD wSerialNumber,BYTE bResult, WORD num, DWORD* pDataIDs)
{
	char szData[128] = {0};
	memset(&szData, 0, sizeof(szData));
	DWORD dataLen = 0;
	WORD wSerialNum = m_SigPackage.ReplySnapShot(szData, dataLen, wSerialNumber, bResult, num, pDataIDs);
	Send(NET_CMD_CAMERA_SHOT_REPLY, wSerialNum, szData, dataLen, false, 0, 0);
}

void CNetService::CameraSnapShotReply(WORD wSerialNumber, vector<ShotResult>& stResultList, BYTE nResult)
{
	char szData[2048] = {0};
	memset(&szData, 0, sizeof(szData));
	DWORD dataLen = 0;
	WORD wSerialNum = m_SigPackage.ReplySnapShot(szData, dataLen, wSerialNumber, stResultList, nResult);
	Send(NET_CMD_CAMERA_SHOT_REPLY, wSerialNum, szData, dataLen, false, 0, 0);
}

// 平台通用应答
void CNetService::ProcCmsReply(NetRcvMsg* pMsg, WORD& nSerialNum)
{
	NET_MSG_CMS_REPLY_S msgCmsReply;

	memset(&msgCmsReply, 0, sizeof(msgCmsReply));
	memcpy(&msgCmsReply.wSerialNumber, pMsg->pData, 2);
	memcpy(&msgCmsReply.wId, pMsg->pData+2, 2);
	memcpy(&msgCmsReply.bResult, pMsg->pData+4, 1);      //0：成功/确认；1：失败；2：消息有误；3：不支持             

	msgCmsReply.wSerialNumber = ntohs(msgCmsReply.wSerialNumber);
	msgCmsReply.wId           = ntohs(msgCmsReply.wId);
	nSerialNum 				  = msgCmsReply.wSerialNumber;

//	DBG_LEVEL_6("ProcCmsReply ID:0x%.4x,SN:%d,result:%d", msgCmsReply.wId, msgCmsReply.wSerialNumber,msgCmsReply.bResult);
//	DBG_LEVEL_6("msgCmsReply.wSerialNumber =%d, m_nFileUploadSerial =%d && m_nFileUploadSerial =%d\n", msgCmsReply.wSerialNumber, m_nFileUploadSerial, m_nFileUploadSerial);
	if(msgCmsReply.wSerialNumber == m_nFileUploadSerial && m_nFileUploadSerial != 0){
		pthread_mutex_lock(&m_stFileUploadMutex);
		pthread_cond_signal(&s_stFileUploadCond);
		pthread_mutex_unlock(&m_stFileUploadMutex);
	}

	if (msgCmsReply.wSerialNumber == m_nUploadFaceCmpSerial && m_nUploadFaceCmpSerial != 0){
		pthread_mutex_lock(&m_stUpFacePicMutex);
		pthread_cond_signal(&s_stFaceCmpUploadCond);
		pthread_mutex_unlock(&m_stUpFacePicMutex);
	}

	switch (msgCmsReply.bResult)   //bResult   0：成功；1：车辆已被注册；2：数据库中无该车辆；3：终端已被注册；4：数据库中无该终端
	{
	   case NET_CMD_REPLY_SUCCESS:          //平台应答成功
	   {
			switch (msgCmsReply.wId)
			{
				case NET_CMD_AUTHORIZE:		//鉴权消息应答
					m_bNetAlive = true;
					m_pInterFace->NetSetGnssCmsLinkState(m_bNetAlive);   
					m_sendRcv.SetAuthorizeStatus(m_bNetAlive);     	
					DBG_LEVEL_6("terminal login success!Start net sending and receving...");
				break;
				
				case NET_CMD_UNREGISTER:	//注销设备成功!
					DBG_LEVEL_6("terminal unregister success!");
					m_sendRcv.SetAuthorizeStatus(false); 
					break;
				case NET_CMD_SINGLE_MUTLIMEDIA_UPLOAD:
					break;
				case NET_LOCATION_DATA_BATCH_UPLOAD:	//定位数据批量上传成功
					m_bGpsUpload = true;
					DBG_LEVEL_6("Save Gps Upload success!");

					break;
				default:
				break;
			}
			break;
	   	}

		case NET_CMD_REPLY_FAILURE:		//平台应答失败!
		{
			switch(msgCmsReply.wId)		
			{
				case NET_CMD_AUTHORIZE:	//鉴权失败!
//					if(m_userReTryTimes == 3)
					{
						Unregister();
//						m_userReTryTimes = 0;
						printf("m_userReTryTimes after Unregister = %d\n", m_userReTryTimes);
					}
//					Register();
//					m_userReTryTimes += 1;
					break;
				case NET_CMD_UNREGISTER://注销失败!
					Unregister();
					m_userReTryTimes += 1;
					break;
				default:
					break;
			}
			break;
	   	}
		
		case NET_CMD_REPLY_MSGERR:
			break;

		case NET_CMD_REPLY_NOTSUPPORT:
			break;	

		case NET_CMD_REPLY_ALARM_VERIFY:
			break;	

		default:
			break;
	}
	// 删除收到回复的消息
	m_sendRcv.DelMsgBySN(msgCmsReply.wSerialNumber);	
}


//终端注册，平台应答；
void CNetService::ProcRegisterReply(NetRcvMsg* pMsg)     //保存平台下发的注册码，并回传该注册码到平台
{
	BYTE bResult;             		// 平台应答结果
	WORD wSerialNumber;             // 消息流水号

	memcpy(&wSerialNumber, pMsg->pData, sizeof(WORD));
	memcpy(&bResult, pMsg->pData + 2, sizeof(BYTE));
	wSerialNumber = ntohs(wSerialNumber);
//	static int count = 0;
//	if(count == 0)
//	{
//		bResult = SERVER_REPLY_TERMINAL_REGISTERED;
//		count += 1;
//	}
	DBG_LEVEL_6("ProcRegisterReply ID:%x,SN:%d,Ret:%d", pMsg->wId, pMsg->wSerialNumber,bResult);
	switch(bResult)   //bResult   0：成功；1：车辆已被注册；2：数据库中无该车辆；3：终端已被注册；4：数据库中无该终端
	{
		case SERVER_REPLY_REGISTER_SUCCESS:          //平台应答成功
			/*协议: 终端在未注册状态下，应首先进行注册，注册成功后终端将获得鉴权码并进行保存，鉴
			权码在终端登录时使用。*/
			memset(m_szAuthCode, 0, sizeof(m_szAuthCode));
			memcpy(m_szAuthCode, pMsg->pData + 3, pMsg->wMsgLen - 3);   	 
			m_szAuthLen = pMsg->wMsgLen - 3;
			printf("AuthCode:");
			for(int i = 0; i < m_szAuthLen; i ++)
			{
				printf("%02x ", m_szAuthCode[i]);
			}printf("\n");
			DBG_LEVEL_6("AuthCode:%s,len:%d", m_szAuthCode,m_szAuthLen);
			m_pInterFace->SaveAuthCode(m_szAuthCode, m_szAuthLen);        		
			Login();  	//登录到平台,鉴权!
			break;
		case SERVER_REPLY_VEHICLE_REGISTERED:
			Login();  //发送鉴权消息,登录到平台
			break;
		case SERVER_REPLY_NO_VEHICLE:
			break;
		case SERVER_REPLY_TERMINAL_REGISTERED:
			Unregister();	//设备注销重新注册
			break;	
		case SERVER_REPLY_NO_TERMINAL:
			break;	
		default:
			break;
	}
	m_sendRcv.DelMsgByCmdId(NET_CMD_REGISTER); //从链表中删除接收到的注册包消息体
}

//采集行驶记录仪数据上传到服务器平台
/*采集行驶记录数据
平台通过发送行驶记录数据采集命令消息，要求终端上传指定的数据，该消息需要终端
回复行驶记录数据上传消息。*/
int CNetService::PickDrivingRecord(NetRcvMsg* pCmdType)    
{	
	return 0;
}

/*下传行驶记录参数
平台通过发送行驶记录参数下传命令消息，要求终端上传指定的数据，该消息需要终端
回复终端通用应答消息。*/
//调用  m_pInterFace->SetDrivingRecord()设置行车记录仪参数
int CNetService::SetDrivingRecord(NetRcvMsg* pMsg)
{
	BYTE cmd = 0;
    char set[64] = {0};

	// 命令字
	cmd = *pMsg->pData;

	// 数据块
	memcpy(set, pMsg->pData + 1, 64);         //将平台数据复制到set  缓冲区
	
	// 行驶记录参数下传命令终端通用回复
	CommReply(pMsg->wSerialNumber, pMsg->wId, m_pInterFace->SetDrivingRecord(cmd, set));
	return 0;
}


int CNetService::DownLinkTrans(NetRcvMsg* pMsg)
{
	BYTE  type;
	char buf[MEM_SIZE_K * 4] = {0};
	int offset = 0;      // 协议包偏移
	
	// 透传消息类型
	type = *pMsg->pData;
	offset += 1;
	// 拷贝包透传消息内容
	memcpy(buf , pMsg->pData + offset, pMsg->wMsgLen-offset);
	
	CommReply(pMsg->wSerialNumber, pMsg->wId, m_pInterFace->DownlinkTrans((TRANSPARENT_TYPE_E)type, buf,pMsg->wMsgLen-1));
	return 0;
}

void CNetService::getFileList(File_Server_Config& config)
{
	pthread_mutex_lock(&m_stFileMutex);
	config.fileList.clear();
	for(vector<Files_Map_S>::iterator iter = m_fileMapList.begin(); iter != m_fileMapList.end();  ++ iter){
#if 0
		printf("%02X-%02X-%02X %02X:%02X:%02X \n", iter->bcdTime[0],iter->bcdTime[1],iter->bcdTime[2],iter->bcdTime[3],iter->bcdTime[4],iter->bcdTime[5]);
		printf("%02X-%02X-%02X %02X:%02X:%02X \n", config.alarm_flag.datetime[0],config.alarm_flag.datetime[1],config.alarm_flag.datetime[2],config.alarm_flag.datetime[3],config.alarm_flag.datetime[4],config.alarm_flag.datetime[5]);
		printf("%d----%d\n", iter->nSeq, config.alarm_flag.sequence);
#endif
		if(!strncmp((const char*)&iter->bcdTime[0], (const char*)&config.alarm_flag.datetime[0], 6) && iter->nSeq == config.alarm_flag.sequence){
			config.nDevType = iter->nDevType;
			config.nAlarmType = iter->nAlarmType;
			config.fileList.assign(iter->fileList.begin(),iter->fileList.end());
			m_fileMapList.erase(iter);
			break;
		}
	}
	pthread_mutex_unlock(&m_stFileMutex);
}


int CNetService::AlarmAttachmentUpload(NetRcvMsg* pMsg, WORD nSerailNum)
{
	//parse File ServerParam, start upload files
	File_Server_Config config;
	memset(&config, 0, sizeof(File_Server_Config));
	m_sigParse.FileServerInfo(pMsg, config);
	CommReply(pMsg->wSerialNumber, pMsg->wId, 0);

	getFileList(config);
	config.bReSend = false;
	config.nResetIndex = 0;
	m_pobjFileService->SetServerConfig(config);
	#if 0
	printf("%s----------------:%s:%d:%d\n", __func__,config.szAddr, config.nTcpPort,config.nTcpPort);
	printf("%s----------------:%d\n", __func__, config.alarm_flag.id);
	printf("%s----------------:%02x-%02x-%02x %02x:%02x:%02x\n", __func__, config.alarm_flag.datetime[0],config.alarm_flag.datetime[1],config.alarm_flag.datetime[2],config.alarm_flag.datetime[3],config.alarm_flag.datetime[4],config.alarm_flag.datetime[5],config.alarm_flag.datetime[5]);
	printf("%s----------------:%d:%d\n", __func__, config.alarm_flag.sequence, config.alarm_flag.attachment_num);
	printf("%s----------------:%s\n", __func__, config.alarmCode);
	#endif

	return 0;
}


int CNetService::FacePicturePublish(NetRcvMsg* pMsg)
{
	FacePicPublish *FacePicInfo = new FacePicPublish;
	m_sigParse.FacePictureInfo(pMsg, FacePicInfo);
	
	if (0x1 == FacePicInfo->cmd_type || 0x2 == FacePicInfo->cmd_type)
	{
		//delete
		if (m_pFacePic){
			m_pFacePic(FacePicInfo);
		}
		//reply
		CommReply(pMsg->wSerialNumber, pMsg->wId, 0);
	}else{
		CommReply(pMsg->wSerialNumber, pMsg->wId, 0);
		if (m_pFacePic){
			m_pFacePic(FacePicInfo);
		}
	}

	if (FacePicInfo){
		delete FacePicInfo;
	}

	return 0;
}

int CNetService::FaceInfoQuery(NetRcvMsg* pMsg)
{
	WORD wSerial = pMsg->wSerialNumber;

	FaceIDInfo *id_info = new FaceIDInfo;
	if (NULL == id_info){
		return -1;
	}
	memset(id_info, 0, sizeof(FaceIDInfo));
	if (m_pFaceInfo){
		m_pFaceInfo(id_info);

		ReportQueryFaceInfoResult(id_info, wSerial);
	}	

	if (id_info){
		delete id_info;
	}

	return 0;
}

int CNetService::TriggerFaceCompare(NetRcvMsg* pMsg)
{
	//reply
    CommReply(pMsg->wSerialNumber, pMsg->wId, 0);

	if (m_pTriggerFaceCmp){
		m_pTriggerFaceCmp(NULL);
	}	

	return 0;
}

int CNetService::FaceCompareResultReply(NetRcvMsg* pMsg)
{
	//parse File ServerParam, start upload files
	int id  = 0;
	WORD  retryPackCnt = 0;
	short retryIdList[32] = {0};
	int   offset = 0;

	WORD serial = *(pMsg->pData + offset);
	serial = ntohs(serial);
	
	offset += 2;
	// 重传包数
	retryPackCnt = *(pMsg->pData + offset);
	m_pReFaceCmpPic.RetransmitTotal = ntohs(retryPackCnt);
	offset += 2;

	// 重传id列表
	for (int idx = 0; idx < retryPackCnt; ++idx)
	{
		memcpy(&retryIdList[idx],pMsg->pData + offset, sizeof(WORD));
		//retryIdList[idx] = ntohs(retryIdList[idx]);
		m_pReFaceCmpPic.RetransmitId[idx] = ntohs(retryIdList[idx]);
		offset += 2;   
	}

	m_bResendFaceCmpPic = true;
	
	pthread_mutex_lock(&m_stUpFacePicMutex);
	pthread_cond_signal(&s_stFaceCmpUploadCond);
	pthread_mutex_unlock(&m_stUpFacePicMutex);
	
	//todo judge to resend
	

	return 0;
}

int CNetService::FaceCompareResultPublicReply(NetRcvMsg* pMsg)
{
    //Deliver result to jt808 interface
    FaceCmpPubResult *pFaceCmpRes = new FaceCmpPubResult;
	if (NULL == pFaceCmpRes)
	{
		return -1;
	}
	memset(pFaceCmpRes, 0, sizeof(FaceCmpPubResult));
	m_sigParse.FaceCmpResInfo(pMsg, pFaceCmpRes);

    //reply
    CommReply(pMsg->wSerialNumber, pMsg->wId, 0);

    m_pFacePicCmp(pFaceCmpRes);

	if (pFaceCmpRes){
		delete pFaceCmpRes;
	}
    return 0;
}

int CNetService::FaceAuthActiveSnap(NetRcvMsg* pMsg)
{
    //reply
    CommReply(pMsg->wSerialNumber, pMsg->wId, 0);

	char filename[128] = {0};
	unsigned int pts = 0;
	if (m_pFaceSnap(0x65, &pts, filename))
	{
		UploadFaceAuthPic(filename, PICTRUE_TYPE_JPEG, pMsg->wSerialNumber);
	}

/*    //snap
    NetMdShotCmd mdShotCmd;
	memset(&mdShotCmd, 0, sizeof(NetMdShotCmd));
	mdShotCmd.chn 			= 1;
	mdShotCmd.cmd 			= 0x11; //face auth 
	mdShotCmd.issave 		= 0;//upload
	mdShotCmd.wSerialNumber = pMsg->wSerialNumber;

	pthread_mutex_lock(&m_stShotMutex);
	m_bShotUpdate = true;
	memcpy(&m_stNetMdShotCmd, &mdShotCmd, sizeof(NetMdShotCmd));
	pthread_mutex_unlock(&m_stShotMutex);
*/	
    return 0;
}

int CNetService::VideoConfGet(NetRcvMsg* pMsg)
{
	WORD wSerial = pMsg->wSerialNumber;

	VideoGetParam *av_info = new VideoGetParam;
	if (NULL == av_info){
		return -1;
	}
	memset(av_info, 0, sizeof(VideoGetParam));
	if (m_pVideoCfgGet){
		m_pVideoCfgGet(av_info);

		UploadVideoCfg(av_info);
	}	

	if (av_info){
		delete av_info;
	}

	return 0;

}

int CNetService::VideoReqRealTimeStream(NetRcvMsg* pMsg)
{
	WORD wSerial = pMsg->wSerialNumber;

	VideoServerParamSet *server_info = new VideoServerParamSet;
	if (NULL == server_info){
		return -1;
	}
	memset(server_info, 0, sizeof(VideoServerParamSet));
	CommReply(pMsg->wSerialNumber, pMsg->wId, 0);
	
	m_sigParse.VideoServerInfo(pMsg, server_info);
	if (m_pVideoReqStream){
		m_pVideoReqStream(server_info);
	}

	if (server_info){
		delete server_info;
	}

	return 0;
}

int CNetService::VideoTransControl(NetRcvMsg* pMsg)
{
	WORD wSerial = pMsg->wSerialNumber;

	VideoTransCtrl *ctrl = new VideoTransCtrl;
	if (NULL == ctrl){
		return -1;
	}
	memset(ctrl, 0, sizeof(VideoTransCtrl));
	CommReply(pMsg->wSerialNumber, pMsg->wId, 0);
	
	m_sigParse.VideoTransControl(pMsg, ctrl);
	if (m_pVideoTransCtrl){
		m_pVideoTransCtrl(ctrl);
	}	

	if (ctrl){
		delete ctrl;
	}

	return 0;
}

int CNetService::VideoTransStatusNotice(NetRcvMsg* pMsg)
{
	WORD wSerial = pMsg->wSerialNumber;

	VideoTransSta *status = new VideoTransSta;
	if (NULL == status){
		return -1;
	}
	memset(status, 0, sizeof(VideoTransSta));
	CommReply(pMsg->wSerialNumber, pMsg->wId, 0);
	
	m_sigParse.VideoTransStatus(pMsg, status);

	if (status){
		delete status;
	}

	return 0;
}

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
int CNetService::VideoQueResourceList(NetRcvMsg* pMsg)
{
	WORD wSerial = pMsg->wSerialNumber;
	
	VideoResource *info = new VideoResource;
	if (NULL == info){
		return -1;
	}
	memset(info, 0, sizeof(VideoResource));
	VideoUploadResInfo *upload_info = new VideoUploadResInfo;
	if (NULL == upload_info){
		return -1;
	}
	memset(upload_info, 0, sizeof(VideoUploadResInfo));

	info->serial        = wSerial;
	upload_info->serial = wSerial;
	m_sigParse.VideoQueResource(pMsg, info);
	if (m_pVideoQueSource){
		m_pVideoQueSource(info, upload_info);
	}

	UploadVideoResource(upload_info);
	
	if (info){
		delete info;
	}

	if (upload_info){
		if (upload_info->souce_info){
			delete upload_info->souce_info;
		}
		delete upload_info;
	}

	return 0;
}

int CNetService::VideoReqPlayback(NetRcvMsg* pMsg)
{
	WORD wSerial = pMsg->wSerialNumber;

	VideoPlayback *play_back = new VideoPlayback;
	if (NULL == play_back){
		return -1;
	}
	memset(play_back, 0, sizeof(VideoPlayback));
	VideoUploadResInfo *upload_info = new VideoUploadResInfo;
	if (NULL == upload_info){
		return -1;
	}
	memset(upload_info, 0, sizeof(VideoUploadResInfo));
	play_back->serial = pMsg->wSerialNumber;
	m_sigParse.VideoQuePlayback(pMsg, play_back);
	if (m_pVideoReqPlayback){
		m_pVideoReqPlayback(play_back, upload_info);
	}

	UploadVideoResource(upload_info);
	
	if (play_back){
		delete play_back;
	}

	if (upload_info){
			if (upload_info->souce_info){
			delete upload_info->souce_info;
		}
		delete upload_info;
	}

	return 0;
}

int CNetService::VideoPlaybackCtrl(NetRcvMsg* pMsg)
{
	WORD wSerial = pMsg->wSerialNumber;

	VideoPlaybackControl *pb_ctrl = new VideoPlaybackControl;
	if (NULL == pb_ctrl){
		return -1;
	}
	memset(pb_ctrl, 0, sizeof(VideoPlaybackControl));
	m_sigParse.VideoPlaybackCtrl(pMsg, pb_ctrl);
	if (m_pVideoPlaybackCtrl){
		m_pVideoPlaybackCtrl(pb_ctrl);
	}

	if (pb_ctrl){
		delete pb_ctrl;
	}

	return 0;
}

int CNetService::VideoReqUploadFile(NetRcvMsg* pMsg)
{
	WORD wSerial = pMsg->wSerialNumber;

	VideoUpFile *info = new VideoUpFile;
	if (NULL == info){
		return -1;
	}
	memset(info, 0, sizeof(VideoUpFile));
	info->serial = pMsg->wSerialNumber;
	CommReply(pMsg->wSerialNumber, pMsg->wId, 0);

	m_sigParse.VideoUploadFile(pMsg, info);
	if (m_pVideoUpFile){
		m_pVideoUpFile(info);
	}

	if (info){
		delete info;
	}

	return 0;
}

int CNetService::VideoCtrlUploadFile(NetRcvMsg* pMsg)
{
	WORD wSerial = pMsg->wSerialNumber;

	VideoUpFileCtrl *ctrl = new VideoUpFileCtrl;
	if (NULL == ctrl){
		return -1;
	}
	memset(ctrl, 0, sizeof(VideoUpFileCtrl));
	ctrl->serial = pMsg->wSerialNumber;

	m_sigParse.VideoCtrlUpload(pMsg, ctrl);
	if (m_pVideoCtrlUp){
		m_pVideoCtrlUp(ctrl);
	}

	if (ctrl){
		delete ctrl;
	}

	return 0;
}

int CNetService::TextInformation(NetRcvMsg* pMsg)
{
	WORD wSerial = pMsg->wSerialNumber;

	TextInfo info;
	m_sigParse.TextInfoTrans(pMsg, &info);
	if (m_pTextInfo){
		m_pTextInfo(&info);
	}

	/*文本信息下发回复*/
	CommReply(pMsg->wSerialNumber, pMsg->wId, 0);
	
	return 0;
}

static int CheckKeyWordNum(char *pdata, int length)
{
	return 0;
	if (length < 0)
		return 0;

	int i = 0, keyWordNum = 0;
	for (i = 0; i < length; i++)
	{
		if (pdata[i] == 0x7e || pdata[i] == 0x7d)
			keyWordNum++;
	}

	return keyWordNum;
}

int CNetService::islocked(const char* filename)
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

off_t CNetService::get_file_size(const char* filename)
{
	struct stat statbuf;
	statbuf.st_size = 0;
	int nCount = 0;
	while(1){
		if(access(filename, R_OK) == 0){
			while(F_UNLCK != islocked(filename)){
				usleep(200*1000);
			}
			usleep(50*1000);
			stat(filename, &statbuf);
			break;
		} else {
			if((++ nCount) > 100){
				system("ls -l /tmp/capture");
				break;
			}
		}
		usleep(100*1000);
	}
	return statbuf.st_size;  
}

void *CNetService::UpMediaData(void)
{
	size_t fileListSize = fileList.size();
	if(!fileListSize)
		return NULL;

	while(!mdFileSendFinished && !fileUpLoadOverTimeFlag)
	{
		if(!fileListSize)
		{
			DBG_LEVEL_6("In UpMediaData_pthread fileList.size = 0");
			break;
		}
		for(size_t index = 0; index < fileListSize; ++ index)
		{
			UpMultiMediaEvent(fileList[index].media_id, 0, 0, 0, fileList[index].chn, m_nFileUploadSerial);
			
			pthread_mutex_lock(&m_stFileUploadMutex);
			pthread_cond_wait(&s_stFileUploadCond, &m_stFileUploadMutex);
			pthread_mutex_unlock(&m_stFileUploadMutex);
			DBG_LEVEL_6("In UpMediaData stResultList[%d].filename=|%s| ", index, fileList[index].filename);
			DBG_LEVEL_6("In UpMediaData stResultList[%d].media_id=|%d| ", index, fileList[index].media_id);
			usleep(300*1000);

			off_t file_size = get_file_size(&fileList[index].filename[0]);
			if(file_size > 0)
			{
				FILE * fp = fopen(&fileList[index].filename[0], "r");
				if (fp)
				{
					int pack_num = file_size / 960;
					pack_num += ((file_size % 960) > 0 ? 1 : 0);
					char data[1800] = {0};
					int nRet = 0, keyWordNum = 0;
					int offset = 16;
					NetMdUploadCmd cmd;
					for (int i = 1; i <= pack_num; ++ i)
					{
						/*just for test*/
						if (i == 1){
							offset = 16 + 36;
						} else {
							offset = 16;
						}

						memset(data, 0, sizeof(data));
						nRet = fread(data + offset, 1, 960, fp);
						if (nRet <= 0){
							break;
						}

						keyWordNum = CheckKeyWordNum(data + offset, nRet);
						if (keyWordNum > 0){
							keyWordNum = 0 - keyWordNum;
						}
						cmd.mId = fileList[index].media_id;
						cmd.type = fileList[index].type;
						cmd.enctype = fileList[index].enctype;
						cmd.event = fileList[index].event;
						//缓存多媒体文件数据，用于重传
						SingleFileData tmp;
						memset(&tmp, 0, sizeof(tmp));
						tmp.media_id = fileList[index].media_id;
						tmp.packetid = i;	//文件内的分包id
						memcpy(tmp.data, data, nRet);
						SingleFileList.push_back(tmp);
						
						struGPSData gpsData;
						memset(&gpsData, 0, sizeof(gpsData));
//						getGpsInfo(gpsData);
						char szData[1024] = {0};
						DWORD dataLen = 0;
						WORD wSerialNum = m_SigPackage.UploadMutiMediaData(szData, dataLen, data, nRet - keyWordNum, pack_num, i, &cmd, gpsData, fileList[index].pts);
						Send(NET_CMD_MUTIMEDIA_DATA_UP, wSerialNum, szData, dataLen, false, pack_num, i);
						m_nFileUploadSerial = wSerialNum;						
						//等待每个分包通用应答和单个文件8800回复
						pthread_mutex_lock(&m_stFileUploadMutex);
						pthread_cond_wait(&s_stFileUploadCond, &m_stFileUploadMutex);
						pthread_mutex_unlock(&m_stFileUploadMutex);
						if(fileUpLoadOverTimeFlag)	//传输过程中平台超时无回复
						{
							fclose(fp);
							mdFileSendFinished = true;
							fileUpLoadOverTimeFlag = false;
							fileList.clear();
							DBG_LEVEL_6("In UpMediaData platform reply over time, stop UpMediaData_pthread...");
							return NULL;
						}
					}
					fclose(fp);
				}
			}
		}
		mdFileSendFinished = true;
		fileList.clear();
	}
	return NULL;
}

void *CNetService::UpMediaData_pthread(void *param)
{
	CNetService *pThis = (CNetService*)param;
    return pThis->UpMediaData();
}

void CNetService::UpMediaData(NetMdShotCmd mdShotCmd, vector<ShotResult>& stResultList, BYTE event)
{
	size_t size = stResultList.size();
	printf("In UpMediaData, stResultList.size()=%d\n", stResultList.size());
	if(size > 0)
	{
		fileList.clear();
		std::copy(stResultList.begin(), stResultList.end(), std::back_inserter(fileList));
		//进入线程对文件列表进行传输
		m_UpMediaData_pthread.StartThread(UpMediaData_pthread, this);
	}
}
/*7.10 通用数据传输类
协议中未定义但实际使用中需传递的消息可使用数据上行透传消息和数据下行透传消
息进行上下行数据交换。终端可采用 GZIP 压缩算法压缩较长消息，用数据压缩上报消息上传。*/
//函数待实现




/*7.11 加密类协议
平台与终端之间若需加密通信，可采用RSA 公钥密码系统。平台通过发送平台RSA 公
钥消息向终端告知自己的RSA 公钥，终端回复终端RSA 公钥消息，反之亦然。*/
//函数待实现




//多媒体数据分包重传
void *CNetService::ResendMediaData(void)
{
	if(!SingleFileList.size())
		return NULL;

	int nRet = 0, keyWordNum = 0;
	int offset = 16;

	for(int index = 0; index < SingleFileList.size(); index ++)
	{
		if(resendMediaData.media_id != SingleFileList[index].media_id)
			continue;
		for(int count = 0; count < resendMediaData.retryPackCnt; count ++)
		{
			if(resendMediaData.retryIdList[count] == SingleFileList[index].packetid)
			{
				NetMdUploadCmd cmd;
				if (SingleFileList[index].packetid == 1){
					offset = 16 + 36;
				} else {
					offset = 16;
				}
				nRet = strlen(SingleFileList[index].data);
				keyWordNum = CheckKeyWordNum(SingleFileList[index].data + offset, nRet);
				if (keyWordNum > 0){
					keyWordNum = 0 - keyWordNum;
				}
				cmd.mId = fileList[index].media_id;
				cmd.type = fileList[index].type;
				cmd.enctype = fileList[index].enctype;
				cmd.event = fileList[index].event;

				struGPSData gpsData;
				char szData[1024] = {0};
				DWORD dataLen = 0;
				WORD wSerialNum = m_SigPackage.UploadMutiMediaData(szData, dataLen, SingleFileList[index].data, nRet - keyWordNum, resendMediaData.retryPackCnt, SingleFileList[index].packetid, &cmd, gpsData, fileList[index].pts);
				Send(NET_CMD_MUTIMEDIA_DATA_UP, wSerialNum, szData, dataLen, false, resendMediaData.retryPackCnt, SingleFileList[index].packetid);
				m_nFileUploadSerial = wSerialNum;						
				//等待每个分包通用应答和单个文件8800回复
				pthread_mutex_lock(&m_stFileUploadMutex);
				pthread_cond_wait(&s_stFileUploadCond, &m_stFileUploadMutex);
				pthread_mutex_unlock(&m_stFileUploadMutex);
				if(fileUpLoadOverTimeFlag)	//传输过程中平台超时无回复
				{
					mdFileSendFinished = true;
					fileUpLoadOverTimeFlag = false;
					SingleFileList.clear();
					DBG_LEVEL_6("In ResendMediaData platform reply over time, stop ResendMediaData_pthread...");
					return NULL;
				}
			}
		}
	}

	return NULL;
}


void *CNetService::ResendMediaData_pthread(void *param)
{
	CNetService *pThis = (CNetService*)param;
    return pThis->ResendMediaData();
}

/*终端发送多媒体数据上传消息，上传多媒体数据。每个完整的多媒体数据前需附加摄录
时的位置信息汇报消息体，称为位置多媒体数据。平台根据总包数确定接收超时时间，在收
到全部数据包或达到超时时间后，平台向终端发送多媒体数据上传应答消息，该消息确认收
到全部数据包或要求终端重传指定的数据包*/
int CNetService::ProcMultimediaUpReply(NetRcvMsg* pMsg)
{
	int len = 0; 
	int offset = 0;

	len = strlen(pMsg->pData);
	// 多媒体id
	memcpy(&resendMediaData.media_id, pMsg->pData, 4); 
	resendMediaData.media_id = ntohl(resendMediaData.media_id);
	printf("In ProcMultimediaUpReply, meida_id=%d\n", resendMediaData.media_id);
	offset += 4;
	// 重传包数
	resendMediaData.retryPackCnt = *(pMsg->pData + offset);
	printf("In ProcMultimediaUpReply, retryPackCnt=%u\n", resendMediaData.retryPackCnt);
	offset += 1;

	// 重传id列表
	for (int idx = 0; idx < resendMediaData.retryPackCnt; ++idx)
	{
		memcpy(&resendMediaData.retryIdList[idx],pMsg->pData + offset, sizeof(WORD));
		resendMediaData.retryIdList[idx] = ntohs(resendMediaData.retryIdList[idx]);
		offset += 2;   
	}

	//todo judge to resend
	if(resendMediaData.retryPackCnt)
	{
		//进入线程对文件列表进行重新传输
		m_ResendMediaData_pthread.StartThread(ResendMediaData_pthread, this);
	}else
	{
		pthread_mutex_lock(&m_stFileUploadMutex);
		pthread_cond_signal(&s_stFileUploadCond);
		pthread_mutex_unlock(&m_stFileUploadMutex);
		memset(&m_ResendMediaData_pthread, 0, sizeof(m_ResendMediaData_pthread));
		SingleFileList.clear();
	}
	
//	m_pInterFace->MultimediaDataReply(id, retryPackCnt, retryIdList);
	return 0;
}

void CNetService::MultimediaSearchReply(WORD wSerialNumber, vector<IdxCmdRes>& stResultList)
{
	char szData[2048] = {0};
	memset(&szData, 0, sizeof(szData));
	DWORD dataLen = 0;
	WORD wSerialNum = m_SigPackage.ReplyMutiMediaIndex(szData, dataLen, wSerialNumber, stResultList);
	Send(NET_CMD_STORE_MUTLIMEDIA_SEARCH_REPLY, wSerialNum, szData, dataLen, false, 0, 0);
}

//检索符合条件的多媒体文件列表
int CNetService::MultimediaSearch(NetRcvMsg* pMsg)
{
	NetMdIdxCmd mdIdxCmd;
	memset(&mdIdxCmd, 0, sizeof(NetMdIdxCmd));

	int offset = 0;
	// 多媒体类型
	mdIdxCmd.type = *(pMsg->pData + offset);
	offset += 1;
	// 通道id
	mdIdxCmd.chn  = *(pMsg->pData + offset);
	offset += 1;
	// 事件编码
	mdIdxCmd.event = *(pMsg->pData + offset);
	offset += 1;
	// 起始时间
	memcpy(mdIdxCmd.btime, pMsg->pData + offset, 6);
	offset += 6;
	// 结束时间
	memcpy(mdIdxCmd.etime, pMsg->pData + offset, 6);
	offset += 6;
//	CommReply( pMsg->wSerialNumber, pMsg->wId, 0);

	vector <IdxCmdRes> resultList;
	resultList.clear();
	// 向设备传递检索多媒体数据和提取
	// 检索数据库记录存储的多媒体文件名
    m_pInterFace->MdSearchCmd(mdIdxCmd, resultList);

	MultimediaSearchReply(pMsg->wSerialNumber, resultList);
	
	return 0;
}

//
int CNetService::SingleMultimediaSearch(NetRcvMsg* pMsg)
{
	if(mdFileSendFinished)	//判断是否正在上传多媒体文件，传输中不回复任何消息
	{
		mdFileSendFinished = false;	//标志正在传多媒体文件
		SingleNetMdIdxCmd SingleMdIdxCmd;
		memset(&SingleMdIdxCmd, 0, sizeof(SingleMdIdxCmd));

		int offset = 0;
		// 多媒体ID
		memcpy(&SingleMdIdxCmd.mediaId, pMsg->pData + offset, 4);
		SingleMdIdxCmd.mediaId = ntohl(SingleMdIdxCmd.mediaId);
		printf("In SingleMultimediaSearch, Index mediaId: %d\n", SingleMdIdxCmd.mediaId);
		offset += 4;
		//删除标志
		SingleMdIdxCmd.deleteFlag = *(pMsg->pData + offset);

		//寻找符合条件的单个文件
		NetMdUploadCmd tmp;
		memset(&tmp, 0, sizeof(tmp));
		int nRet = m_pInterFace->SingleMdSearchCmd(SingleMdIdxCmd, tmp);
		printf("In SingleMultimediaSearch:nRet = %d\n", nRet);
		if(!nRet)	//找到符合条件的文件，上传
		{
			vector <ShotResult> resultList;
			resultList.clear();
			int pts = 0;
			ShotResult stShotResult;
			memset(&stShotResult, 0, sizeof(stShotResult));
			stShotResult.media_id	= SingleMdIdxCmd.mediaId;
			stShotResult.type		= tmp.type;
			stShotResult.enctype	= tmp.enctype;
			stShotResult.event		= UPLOAD_EVENT_PLATFORM;	//tmp.event;
			stShotResult.chn		= tmp.chn;
			stShotResult.pts		= pts;
			tmp.mId					= SingleMdIdxCmd.mediaId;

			memcpy(stShotResult.btime, tmp.btime, strlen(tmp.btime));
			memcpy(stShotResult.etime, tmp.etime, strlen(tmp.etime));
			memcpy(stShotResult.filename, tmp.filename, strlen(tmp.filename));
			resultList.push_back(stShotResult);

			printf("In SingleMultimediaSearch, resultList[0].filename |%s|\n", resultList[0].filename);
			NetMdShotCmd mdShotCmd;
			memset(&mdShotCmd, 0, sizeof(mdShotCmd));
			mdShotCmd.chn			= tmp.chn;
			UpMediaData(mdShotCmd, resultList, UPLOAD_EVENT_PLATFORM);
			if(SingleMdIdxCmd.deleteFlag)
			{
				//删除检索文件操作，这里根据具体名字要改
				char RMcmd[100] = {0};
				memset(RMcmd, 0, sizeof(RMcmd));
				sprintf(RMcmd, "rm ./test/%d\n", tmp.filename);
				printf("In SingleMdSearchCmd, RMcmd|%s|\n", RMcmd);
				system(RMcmd);
			}
		}else			//找不到文件，回复通用应答
		{
			CommReply(pMsg->wSerialNumber, pMsg->wId, nRet);
			mdFileSendFinished = true;	//标志已完成多媒体文件传输
		}
	}
	
	return 0;
}

/*多媒体数据上传
终端发送多媒体数据上传消息，上传多媒体数据。每个完整的多媒体数据前需附加摄录
时的位置信息汇报消息体，称为位置多媒体数据。平台根据总包数确定接收超时时间，在收
到全部数据包或达到超时时间后，平台向终端发送多媒体数据上传应答消息，该消息确认收
到全部数据包或要求终端重传指定的数据包。*/
int CNetService::MultimediaUp(NetRcvMsg* pMsg)
{
	NetMdUploadCmd mdUploadCmd;
	memset(&mdUploadCmd, 0, sizeof(NetMdUploadCmd));
	int  offset = 0;

	// 多媒体类型
	mdUploadCmd.type = *(pMsg->pData + offset);
	offset += 1;

	// 通道id
	mdUploadCmd.chn = *(pMsg->pData + offset);
	offset += 1;

	// 事件项编码
	mdUploadCmd.event = *(pMsg->pData + offset);
	offset += 1;

	// 起始时间
	memcpy(mdUploadCmd.btime, pMsg->pData + offset,  6);
	offset += 6;

	//结束时间
	memcpy(mdUploadCmd.etime, pMsg->pData + offset, 6);
	offset += 6;

	// 删除标志
	mdUploadCmd.mark = *(pMsg->pData + offset);
	offset += 1;

	NetMdIdxCmd mdIdxCmd;
	memset(&mdIdxCmd, 0, sizeof(mdIdxCmd));
	mdIdxCmd.type	= mdUploadCmd.type;
	mdIdxCmd.chn	= mdUploadCmd.chn;
	mdIdxCmd.event	= mdUploadCmd.event;
	memcpy(mdIdxCmd.btime, mdUploadCmd.btime, 6);
	memcpy(mdIdxCmd.etime, mdUploadCmd.etime, 6);
	vector <IdxCmdRes> stResultList;
	stResultList.clear();
#if 1
	m_pInterFace->MdSearchCmd(mdIdxCmd, stResultList);
	if(stResultList.size())
	{
		vector <ShotResult> resultList;
		resultList.clear();
		for(int index = 0; index < stResultList.size(); index ++)
		{
			int pts = 0;
			ShotResult stShotResult;
			memset(&stShotResult, 0, sizeof(stShotResult));
			stShotResult.media_id	= stResultList[index].mediaId;
			stShotResult.type		= stResultList[index].type;
//			stShotResult.enctype	= stResultList[index].enctype;
			stShotResult.event		= UPLOAD_EVENT_PLATFORM;	//tmp.event;
			stShotResult.chn		= stResultList[index].chn;
			stShotResult.pts		= pts;
			printf("In MultimediaUp, stShotResult.chn[%u], stResultList.chn[%u]\n", stShotResult.chn, stResultList[index].chn);
			memcpy(&stShotResult.btime, mdUploadCmd.btime, strlen(mdUploadCmd.btime));
			memcpy(&stShotResult.etime, mdUploadCmd.etime, strlen(mdUploadCmd.etime));
			memcpy(&stShotResult.filename, stResultList[index].fileName, strlen(stResultList[index].fileName));
			resultList.push_back(stShotResult);
		}
		for(int i = 0; i < resultList.size(); i ++)
			printf("In MultimediaUp, resultList[%d].filename|%s|\n", i, resultList[i].filename);
		NetMdShotCmd mdShotCmd;
		memset(&mdShotCmd, 0, sizeof(mdShotCmd));
		mdFileSendFinished = false;
		UpMediaData(mdShotCmd, resultList, UPLOAD_EVENT_PLATFORM);
	}else
	{
		CommReply(pMsg->wSerialNumber, pMsg->wId, 1);
		mdFileSendFinished = true;	//标志已完成多媒体文件传输
	}
#endif
	return 0;
}



/*摄像头立即拍摄
平台通过发送摄像头立即拍摄命令消息，对终端下发拍摄命令，该消息需要终端回复终
端通用应答消息。若指定实时上传，则终端拍摄后上传摄像头图像/视频，否则对图像/视频
进行存储。*/
void CNetService::CameraShot (NetRcvMsg* pMsg)
{
	NetMdShotCmd mdShotCmd;
	memset(&mdShotCmd, 0, sizeof(NetMdShotCmd));
	int  offset  = 0;
	
	// 通道id
	mdShotCmd.chn = *(pMsg->pData + offset);
	offset += 1;

	//拍摄命令
	memcpy(&mdShotCmd.cmd ,pMsg->pData + offset, 2);
	mdShotCmd.cmd = ntohs(mdShotCmd.cmd);
	offset += 2;

	//拍摄间隔
	memcpy(&mdShotCmd.period, pMsg->pData + offset, 2);
	int interval = ntohs(mdShotCmd.period);
	mdShotCmd.period = (interval == 0 ? 1 : interval);
	offset += 2;

	//保存标志
	memcpy(&mdShotCmd.issave, pMsg->pData + offset, 1);
	offset += 1;

	//分辨率
	memcpy(&mdShotCmd.res, pMsg->pData + offset, 1);
	offset += 1;

	//图像视频质量
	memcpy(&mdShotCmd.quality, pMsg->pData + offset, 1);
	offset += 1;

	//亮度
	memcpy(&mdShotCmd.brightness, pMsg->pData + offset, 1);
	offset += 1;

	//对比度
	memcpy(&mdShotCmd.contrast, pMsg->pData + offset, 1);
	offset += 1;

	//饱和度
	memcpy(&mdShotCmd.saturation, pMsg->pData + offset, 1);
	offset += 1;

	//色度
	memcpy(&mdShotCmd.chroma, pMsg->pData + offset, 1);
	offset += 1;
#if 0
	m_pInterFace->MdShotCmd(mdShotCmd);
	CameraSnapShotReply(pMsg->wSerialNumber,0,0,NULL);
#else
	mdShotCmd.wSerialNumber = pMsg->wSerialNumber;
	pthread_mutex_lock(&m_stShotMutex);

	//设置拍摄线程启动标志，拷贝数据，拍摄线程进行拍摄并回复
	m_bShotAlive = true;
	memcpy(&m_stNetMdShotCmd, &mdShotCmd, sizeof(NetMdShotCmd));
//	if(((!mdShotCmd.issave) && (0x64 == mdShotCmd.chn)) 
//		|| ((!mdShotCmd.issave) && (0x65 == mdShotCmd.chn)) )
//	{
//		m_bShotUpdate = true;
//	}	
//	memcpy(&m_stNetMdShotCmd, &mdShotCmd, sizeof(NetMdShotCmd));
//	}// else {
//		vector<ShotResult> resultList;
//		resultList.clear();
//		//若拍照，则拍照并生成图片图片，将成功拍照的结果往resultList队列添加元素
//		//若录像，则录像并生成视频文件，将成功录像的结果往resultList队列添加元素
//		CameraSnapShotReply(pMsg->wSerialNumber, resultList, 2);
//	}
	pthread_mutex_unlock(&m_stShotMutex);
#endif
}



/*平台通过发送录音开始命令消息，对终端下发录音命令，该消息需要终端回复终端通用
应答消息。若指定实时上传，则终端录制后上传音频数据，否则对音频数据进行存储。*/
int CNetService::StartRecord(NetRcvMsg* pMsg)
{
	NetRecordCmd recordCmd;
	memset(&recordCmd, 0, sizeof(NetRecordCmd));
	int  offset = 0;

	// 录音命令
	recordCmd.onoff = *(pMsg->pData + offset);
	offset += 1;

	// 录音时间
	memcpy(&recordCmd.lasttime, pMsg->pData + offset, 2); 
	recordCmd.lasttime = ntohs(recordCmd.lasttime);
	offset += 2;

	// 保存标志
	recordCmd.issave = *(pMsg->pData + offset);
	offset += 1;

	// 音频采样率
	recordCmd.voicefreq = *(pMsg->pData + offset);
	offset += 1;

	// 录音开始命令应答
	CommReply( pMsg->wSerialNumber, pMsg->wId, 0);

	//像设备传递 录音开始命令
	m_pInterFace->MdRecordVoiceCmd(recordCmd);
	return 0;
}


void CNetService::SendHeartBeat()
{
	//m_SigPackage.HeartBeat();
	char szData[32] = {0};
	DWORD dataLen = 0;
	//WORD wSerialNum = m_PackageMsg.HeartBeat(szData, dataLen);
	WORD wSerialNum = m_SigPackage.HeartBeat(szData, dataLen);
	Send(NET_CMD_HEARTBEAT, wSerialNum, szData, dataLen, false, 0, 0);
}

//#include <stdio.h>
//DWORD CNetService::SendSig(WORD wCmdType,WORD wSerialNum,void* pMsgBody, WORD nLen, bool bReSend, WORD nPackTotal, WORD nPackSn)
DWORD CNetService::Send(WORD wCmdType, WORD wSerialNum, void* pMsgBody, WORD nLen, bool bReSend, WORD nPackTotal, WORD nPackSn)
{
	CAutoLock lock(&m_mutex);
	m_SigProc.PackMsg(wCmdType, wSerialNum, &m_sendMsg, pMsgBody, nLen, bReSend, nPackTotal, nPackSn);
	return m_sendRcv.SendMsg(&m_sendMsg);
}

/*DWORD CNetService::Send(WORD wCmdType,WORD wSerialNum,void* pMsgBody, WORD nLen, bool bReSend, WORD nPackTotal, WORD nPackSn)
{
	if(NULL != s_pobjCNetService){
		return s_pobjCNetService->SendSig(wCmdType,wSerialNum,pMsgBody,nLen,bReSend,nPackTotal,nPackSn);
	}
	return 0;
}*/

//向平台发送设备信息注册设备，以获得鉴权码
void  CNetService::Register()		
{
	char szData[200] = {0};
	DWORD dataLen = 0;
	WORD wSerialNum = m_SigPackage.Register(szData, dataLen);
	Send(NET_CMD_REGISTER, wSerialNum, szData, dataLen, false, 0, 0);
}

void  CNetService::Unregister()
{
	PUT2LOG_ENTRY_FUN;
	char szData[32] = {0};
	DWORD dataLen = 0;
	//WORD wSerialNum = m_PackageMsg.UnRegister(szData, dataLen);
	WORD wSerialNum = m_SigPackage.UnRegister(szData, dataLen);
	Send(NET_CMD_UNREGISTER, wSerialNum, szData, dataLen, false, 0, 0);
	//清空缓存和本地保存的鉴权码
	remove(".config");
	printf("Unregister, remove .config!!!!!!!!!!!!!!!!!!!!!!\n");
	m_szAuthLen = 0;
	memset(m_szAuthCode, 0, sizeof(m_szAuthCode));
}

void CNetService::Login()
{
#if 1
	int acc_flag = 1;
	if(m_szAuthLen > 0)
	{
		DBG_LEVEL_6("Login Auth Code:%s", m_szAuthCode);
		//m_SigPackage.Login(m_szAuthCode);
		char szData[200] = {0};
		DWORD dataLen = 0;
		WORD wSerialNum = m_SigPackage.Login(szData, dataLen, m_szAuthCode);
		Send(NET_CMD_AUTHORIZE, wSerialNum, szData, dataLen, false, 0, 0);
	}
	else
	{
		/* 没有鉴权码时，ACC        ON时注册 */
		if(acc_flag)	//ACC on
		{
			DBG_LEVEL_6("Register to server!");
			Register();
		}else
		{
			DBG_LEVEL_6("acc is off, remain silence!");
		}
	}
#endif
}

void CNetService::ReportUpgrade(BYTE type,BYTE result)
{
	
}

//位置信息上报
void CNetService::ReportPosInfo(struGPSData* pData)
{
	//m_SigPackage.ReportPosition(pData);
	char szData[256] = {0};
	DWORD dataLen = 0;
	WORD wSerialNum = m_SigPackage.ReportPosition(szData, dataLen, pData);
	Send(NET_CMD_POS_INFO_UP, wSerialNum, szData, dataLen, true, 0, 0);
}

#if 1
//终端无网络 GPS存储信息上报 
void CNetService::ReportSaveGpsInfo(BYTE* pData,int len,int packNum)	//pData 传入数据体 和长度 packNum：数据体一共有多少包
{
	#if 1
	char szData[1024] = {0};
	DWORD dataLen = 0;
	WORD wSerialNum = m_SigPackage.ReportSaveGpsPosition(szData, dataLen, pData,len,packNum);
	Send(NET_CMD_POS_INFO_UP, wSerialNum, szData, dataLen, true, 0, 0);
	#endif
}
#endif

//报警信息上报
void CNetService::ReportAlarm(AlarmData_s* pData, vector<File_Info_s>& fileList)
{
	WORD wSerialNum = 0;
	DWORD dataLen = 0;
	char szData[1024] = {0};
	memset(&szData, 0, sizeof(szData));

	wSerialNum = m_SigPackage.ReportAlarm(szData, dataLen, pData);
	Send(NET_CMD_POS_INFO_UP, wSerialNum, szData, dataLen, true, 0, 0);
	if(fileList.size() > 0){
		Files_Map_S fileMap;
		fileMap.nDevType = pData->extend_type;

		if(fileMap.nDevType == DEV_DSM ){
			fileMap.nAlarmType = pData->dsm_alarm.type;
			fileMap.nSeq = pData->dsm_alarm.alarm_flag.sequence;
			memcpy(&fileMap.bcdTime[0], &pData->dsm_alarm.alarm_flag.datetime[0], 6);
		}else if (fileMap.nDevType == DEV_ADAS){
			fileMap.nAlarmType = pData->adas_alarm.type;
			fileMap.nSeq = pData->adas_alarm.alarm_flag.sequence;
			memcpy(&fileMap.bcdTime[0], &pData->adas_alarm.alarm_flag.datetime[0], 6);
		}

		fileMap.time = time(NULL);
		fileMap.fileList.assign(fileList.begin(), fileList.end());
		pthread_mutex_lock(&m_stFileMutex);
		m_fileMapList.push_back(fileMap);
		pthread_mutex_unlock(&m_stFileMutex);
	}

	//wait respone timeout:50*100 ms
	static int count = 0;
	while(m_bAlarmUploadRespone){
		usleep(100 * 1000);
		if(++ count == 50){
			break;
		}
	}
	count = 0;

	m_bAlarmUploadRespone = false;
}

//状态信息上报
void CNetService::ReportDevStatus(DWORD status)
{
	DWORD dataLen = 0;
	char szData[1024] = {0};
	memset(&szData, 0, sizeof(szData));

	WORD wSerialNum = m_SigPackage.ReportDevStatus(szData, dataLen, status);
	Send(NET_CMD_POS_INFO_UP, wSerialNum, szData, dataLen, true, 0, 0);
}

//事件报告
void CNetService::ReportEvent(BYTE eventId)
{
	DWORD dataLen = 0;
	char szData[32] = {0};
	memset(&szData, 0, sizeof(szData));
	WORD wSerialNum = m_SigPackage.ReportEvent(szData, dataLen, eventId);
	Send(NET_CMD_EVENT_UP, wSerialNum, szData, dataLen, true, 0, 0);
}


int CNetService::UpFaceCmpResult(FaceCmpResult *faceCmpData)
{
	int pack_num = 0, i;
	
	off_t file_size = get_file_size(faceCmpData->pic_name);
	if (file_size){
		FILE * fp = fopen(faceCmpData->pic_name, "r");
		if (fp){
			pack_num = file_size / 960;
			pack_num += ((file_size % 960) > 0 ? 1 : 0);
			pack_num += 1;//head
			
			char data[960] = {0};
			int nRet = 0;
			for (i = 1; i <= pack_num ; ++ i){
				/*i = 1, just head*/
				if (i != 1) {
					//picture data
					memset(data, 0, sizeof(data));
					nRet = fread(data , 1, 960, fp);
					if (nRet <= 0){
						break;
					}
				}

				char szData[1024] = {0};
				DWORD dataLen = 0;
				WORD wSerialNum = m_SigPackage.ReportFaceCmp(szData, dataLen, data, faceCmpData, nRet, pack_num, i);
				Send(NET_CMD_FACE_CMP_RESULT_UP, wSerialNum, szData, dataLen, true, 0, 0);

				m_nUploadFaceCmpSerial = wSerialNum;	
			}

			fclose(fp);
			
		}

		pthread_mutex_lock(&m_stUpFacePicMutex);		
		pthread_cond_wait(&s_stFaceCmpUploadCond, &m_stUpFacePicMutex);
		pthread_mutex_unlock(&m_stUpFacePicMutex);

		if (m_bResendFaceCmpPic){
			m_bResendFaceCmpPic = false;
			//resend
			FILE * Re_fp = fopen(faceCmpData->pic_name, "r");
			if (Re_fp){
				pack_num = file_size / FILE_DATA_UNIT_SIZE;
				pack_num += ((file_size % FILE_DATA_UNIT_SIZE) > 0 ? 1 : 0);
				pack_num += 1;//head
				
				char data[FILE_DATA_UNIT_SIZE] = {0};
				int nRet = 0;
				for (i = 1; i <= pack_num ; ++ i){
					for (int j = 0; j < m_pReFaceCmpPic.RetransmitTotal; j++){
						if (i == m_pReFaceCmpPic.RetransmitId[j])
						{
							memset(data, 0, sizeof(data));
							nRet = fread(data , 1, FILE_DATA_UNIT_SIZE, Re_fp);
							if (nRet <= 0){
								break;
							}

							char szData[1300] = {0};
							DWORD dataLen = 0;
							WORD wSerialNum = m_SigPackage.ReportFaceCmp(szData, dataLen, data, faceCmpData, nRet, pack_num, i);
							Send(NET_CMD_FACE_CMP_RESULT_UP, wSerialNum, szData, dataLen, true, 0, 0);

							break;
						}

					}
					
				}
				fclose(Re_fp);
			}

		}
	}
	
	return 0;
}


//信息点播/取消
void CNetService::InfoDemand(BYTE type, BYTE flag)
{
	DWORD dataLen = 0;
	char szData[32] = {0};
	memset(&szData, 0, sizeof(szData));
	WORD wSerialNum = m_SigPackage.OrderMessage(szData, dataLen, type, flag);
	Send(NET_CMD_INFO_OD_CANCEL, wSerialNum, szData, dataLen, true, 0, 0);
}

/*采集电子运单数据
终端采集电子运单数据上传平台。*/
void CNetService::ReportElectronBill(DWORD len, char* content)
{
	vector<PackageSendMsg> vecPackageMsg;
	vecPackageMsg.clear();
	m_SigPackage.UploadElectBill(vecPackageMsg, content, len);

	WORD nSize = vecPackageMsg.size();
	for (WORD i = 0; i < nSize; i++) {
		Send(NET_CMD_CONFIG_GET_REPLY, vecPackageMsg[i].nSerialNum, vecPackageMsg[i].strEvPackage, vecPackageMsg[i].nPackageSize, false, 0, 0);
	}
}

void CNetService::UplinkTrans(TRANSPARENT_TYPE_E type, char *buf, int size)
{
	vector<PackageSendMsg> vecPackageMsg;
	vecPackageMsg.clear();
	m_SigPackage.UploadTransParent(vecPackageMsg, type, buf, size);
	WORD nSize = vecPackageMsg.size();
	for (WORD i = 0; i < nSize; i++) {
		Send(NET_CMD_CONFIG_GET_REPLY, vecPackageMsg[i].nSerialNum, vecPackageMsg[i].strEvPackage, vecPackageMsg[i].nPackageSize, false, 0, 0);
	}
}


/*多媒体事件信息上传
终端因特定事件而主动拍摄或录音时，应在事件发生后主动上传多媒体事件消息，该消
息需要平台回复通用应答消息。*/
void CNetService::UpMultiMediaEvent(DWORD dataid, BYTE type, BYTE format, BYTE event, BYTE chn, WORD& nSerailNum)
{
	DWORD dataLen = 0;
	char szData[32] = {0};
	WORD wSerialNum = m_SigPackage.UploadMutiMediaEvent(szData, dataLen, dataid, type, format, event, chn);
	Send(NET_CMD_MULTIMEDIA_EVENT_UP, wSerialNum, szData, dataLen, 0, 0, 0);
	nSerailNum = wSerialNum;
	printf("In UpMultiMediaEvent nSerailNum=%d, wSerialNum=%d\n", nSerailNum, wSerialNum);
}

//发送多媒体音视频文件数据API，由MultiMedia.cpp调用
short CNetService::UpMultiMediaData(char* pBuf, int nLen, short PackNum, short PackId, NetMdUploadCmd *pCmd)
{	
	DWORD dataLen = 0;
	char szData[1024] = {0};
	WORD wSerialNum = m_SigPackage.UploadMutiMediaData(szData, dataLen, pBuf, nLen, PackNum, PackId, pCmd);
	Send(NET_CMD_MUTIMEDIA_DATA_UP, wSerialNum, szData, dataLen, false, PackNum, PackId);

	return 0;
}

int CNetService::GetSerialNumber(short msgid,short packid)
{
	return 0;
}

bool CNetService::UpSendCan(char *data, int len)
{
	return false;
}

int CNetService::ReportFacePicDownload(FacePicDownloadReply *pData)
{
	WORD wSerialNum = 0;
	DWORD dataLen = 0;
	char szData[1024] = {0};
	memset(&szData, 0, sizeof(szData));
	wSerialNum = m_SigPackage.FacePicDownload(szData, dataLen, pData);
	Send(NET_CMD_FACE_PIC_DOWNLOAD_REPLY, wSerialNum, szData, dataLen, false, 0, 0);
	return 0;
}

int CNetService::ReportFaceCmpResult(FaceCmpResult *faceCmpData)
{
	WORD wSerialNum = 0;
	DWORD dataLen = 0;
	char szData[1024] = {0};

	if (0 == faceCmpData->result || 1 == faceCmpData->result)//upload attachments
	{
		pthread_mutex_lock(&m_stFaceCmpMutex);
		DBG_LEVEL_6("faceCmpData:%s", faceCmpData->pic_name);
		memcpy(&m_stNetFaceCmpReport, faceCmpData, sizeof(FaceCmpResult));
		DBG_LEVEL_6("pic_name:%s-", m_stNetFaceCmpReport.pic_name);
		m_bFaceCmpUpdate = true;
			
		pthread_mutex_unlock(&m_stFaceCmpMutex);
	}
	else
	{
		memset(&szData, 0, sizeof(szData));
		wSerialNum = m_SigPackage.ReportFaceCmp(szData, dataLen, NULL, faceCmpData, 0, 0, 1);
		Send(NET_CMD_FACE_CMP_RESULT_UP, wSerialNum, szData, dataLen, true, 0, 0);
	}
	
	return 0;
}

int CNetService::UploadFaceAuthPic(char *filename, char pic_type, WORD msg_serial)
{
	int pack_num = 0, i, j, nRet = 0;
	off_t file_size = get_file_size(filename);
	DBG_LEVEL_6("filename:%s--msg_serial:%04x", filename, msg_serial);

	if (file_size){
		FILE * fp = fopen(filename, "r");
		if (fp){
			pack_num = file_size / 960;
			pack_num += ((file_size % 960) > 0 ? 1 : 0);
			DBG_LEVEL_6("pack_num:%d", pack_num);

			char data[960] = {0};
			
			for (i = 1; i <= pack_num ; ++ i){	
				memset(data, 0, sizeof(data));
				nRet = fread(data , 1, 960, fp);
				if (nRet <= 0){
					break;
				}

				char szData[1024] = {0};
				DWORD dataLen = 0;
				WORD wSerialNum = m_SigPackage.UploadFacePic(szData, dataLen, data, pic_type, msg_serial, nRet, pack_num, i);
				Send(NET_CMD_FACE_AUTH_UPLOAD, wSerialNum, szData, dataLen, true, 0, 0);
				m_nUploadFaceCmpSerial = wSerialNum;
			}

			fclose(fp);
		}

		pthread_mutex_lock(&m_stUpFacePicMutex);				
		pthread_cond_wait(&s_stFaceCmpUploadCond, &m_stUpFacePicMutex);
		pthread_mutex_unlock(&m_stUpFacePicMutex);
		
		if (m_bRendFaceAuthPic){
			m_bRendFaceAuthPic = false;
			//resend
			FILE * Re_fp = fopen(filename, "r");
			if (Re_fp){
				pack_num = file_size / FILE_DATA_UNIT_SIZE;
				pack_num += ((file_size % FILE_DATA_UNIT_SIZE) > 0 ? 1 : 0);
				pack_num += 1;//head
				
				char data[FILE_DATA_UNIT_SIZE] = {0};
				nRet = 0;
				for (i = 1; i <= pack_num ; ++ i){
					for (j = 0; j < m_pReFaceAuthPic.RetransmitTotal; j++){
						if (i == m_pReFaceAuthPic.RetransmitId[j])
						{
							if (i != 1){
								memset(data, 0, sizeof(data));
								nRet = fread(data , 1, FILE_DATA_UNIT_SIZE, Re_fp);
								if (nRet <= 0){
									break;
								}
							}

							char szData[1300] = {0};
							DWORD dataLen = 0;
							WORD wSerialNum = m_SigPackage.UploadFacePic(szData, dataLen, data, pic_type, msg_serial, nRet, pack_num, i);
							Send(NET_CMD_FACE_AUTH_UPLOAD, wSerialNum, szData, dataLen, true, 0, 0);

							break;
						}

					}
					
				}
				fclose(Re_fp);
			}
		}
	}
	
	return 0;
}

int CNetService::ReportQueryFaceInfoResult(FaceIDInfo *faceInfoData, WORD wSerial)
{
	WORD wSerialNum = 0;
	DWORD dataLen = 0;
	char szData[1024] = {0};
	memset(&szData, 0, sizeof(szData));
	wSerialNum = m_SigPackage.UploadFaceInfo(szData, dataLen, faceInfoData, wSerial);
	Send(NET_CMD_FACE_INFO_QUERY_REPLY, wSerialNum, szData, dataLen, false, 0, 0);
	
	return 0;
}


int CNetService::UploadVideoCfg(VideoGetParam *av_info)
{
	WORD wSerialNum = 0;
	DWORD dataLen = 0;
	char szData[1024] = {0};
	memset(&szData, 0, sizeof(szData));
	wSerialNum = m_SigPackage.UploadVideoInfo(szData, dataLen, av_info);
	Send(NET_CMD_AV_CONF_UPLOAD, wSerialNum, szData, dataLen, false, 0, 0);
	
	return 0;
}

//乘客流量上报
int CNetService::ReportVideoCarries(VideoUploadCarries *pCarries)
{
	WORD wSerialNum = 0;
	DWORD dataLen = 0;
	char szData[1024] = {0};
	memset(&szData, 0, sizeof(szData));

	wSerialNum = m_SigPackage.ReportCarries(szData, dataLen, pCarries);
	Send(NET_CMD_AV_UPLOAD_PASSENGER_NUM, wSerialNum, szData, dataLen, false, 0, 0);

	return 0;
}

#define ONE_PACKET_NUM    32

//终端上传音视频资源列表
int CNetService::UploadVideoResource(VideoUploadResInfo *psource)
{
	WORD wSerialNum = 0;
	DWORD dataLen = 0;
	char szData[1024] = {0};
	memset(&szData, 0, sizeof(szData));
	
	if (psource->list_num > ONE_PACKET_NUM){
		UploadAVSubPackageRes(psource);

	} else {
		wSerialNum = m_SigPackage.ReportResourceList(szData, dataLen, psource, 0, 0, psource->list_num);
		Send(NET_CMD_AV_UPLOAD_RESOURCE_LIST, wSerialNum, szData, dataLen, false, 0, 0);
	}

	return 0;
}


//终端上传音视频资源列表,分包
int CNetService::UploadAVSubPackageRes(VideoUploadResInfo *psource)
{
	WORD wSerialNum = 0;
	DWORD dataLen = 0, i, j;
	char szData[1024] = {0};
	memset(&szData, 0, sizeof(szData));
	VideoUploadResInfo up_list;

	int pack_num = psource->list_num / ONE_PACKET_NUM + ((psource->list_num % ONE_PACKET_NUM)?1:0);
	DBG_LEVEL_6("[list_num:%d pack_num:%d]", psource->list_num, pack_num);

	int cur_num = 0;
	for (i = 0; i < pack_num; i++)
	{
		memset(&up_list, 0, sizeof(VideoUploadResInfo));
		up_list.serial = psource->serial;
		up_list.list_num = psource->list_num;
		if (i == pack_num -1){
			cur_num = psource->list_num - (ONE_PACKET_NUM* i);
		}else{
			cur_num = ONE_PACKET_NUM;
		}

		up_list.souce_info = new VideoResource[cur_num];
		for (j = 0; j < cur_num; j++)
		{
			up_list.souce_info[j].logic_chn   = psource->souce_info[i * ONE_PACKET_NUM + j].logic_chn;
			memcpy(up_list.souce_info[j].start_time, psource->souce_info[i * ONE_PACKET_NUM + j].start_time, 6);
			memcpy(up_list.souce_info[j].end_time, psource->souce_info[i * ONE_PACKET_NUM + j].end_time, 6);
			up_list.souce_info[j].alarm_flag1 = psource->souce_info[i * ONE_PACKET_NUM + j].alarm_flag1;
			up_list.souce_info[j].alarm_flag2 = psource->souce_info[i * ONE_PACKET_NUM + j].alarm_flag2;
			up_list.souce_info[j].type        = psource->souce_info[i * ONE_PACKET_NUM + j].type;
			up_list.souce_info[j].stream_type = psource->souce_info[i * ONE_PACKET_NUM + j].stream_type;
			up_list.souce_info[j].store_type  = psource->souce_info[i * ONE_PACKET_NUM + j].store_type;
			up_list.souce_info[j].file_size   = psource->souce_info[i * ONE_PACKET_NUM + j].file_size;
			 
		}

		wSerialNum = m_SigPackage.ReportResourceList(szData, dataLen, &up_list, pack_num, i+1, cur_num);

		if (pack_num - 1 == i){
			Send(NET_CMD_AV_UPLOAD_RESOURCE_LIST, wSerialNum, szData, dataLen, false, pack_num, i+1);
		}
		else{
			Send(NET_CMD_AV_UPLOAD_RESOURCE_LIST, wSerialNum, szData, dataLen, true, pack_num, i+1);
		}

		if (up_list.souce_info){
			delete up_list.souce_info;
			up_list.souce_info = NULL;
		}
	}

	return 0;
}

//文件上传结果
int CNetService::ReportVideoUploadResult(VideoUpFileResult *pResult)
{
	WORD wSerialNum = 0;
	DWORD dataLen = 0;
	char szData[1024] = {0};
	memset(&szData, 0, sizeof(szData));

	wSerialNum = m_SigPackage.ReportUpResult(szData, dataLen, pResult);
	Send(NET_CMD_AV_UPLOAD_FILE_FINISH, wSerialNum, szData, dataLen, false, 0, 0);

	return 0;
}

