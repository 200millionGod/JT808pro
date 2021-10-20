#include <netinet/in.h>
#include "JT808ProtocolParse.h"
#include "JT808ParamPackage.h"
#include "debug_log.h"

static inline BYTE GetByte(char*& pBuffer)
{
	BYTE ret = *((BYTE*)pBuffer);
	pBuffer += 1;
	return ret;
}

static inline WORD GetWord(char*& pBuffer)
{
	WORD ret = *((WORD*)pBuffer);
	pBuffer += 2;
	return ntohs(ret);
}

static inline DWORD GetDWord(char*& pBuffer)
{
	DWORD ret = *((DWORD*)pBuffer);
	pBuffer += 4;

	return ntohl(ret);
}

static inline void GetBCD(BCD* pBCD,char*& pBuffer,int len)
{
	BCD* p = (BCD*)pBuffer;
	memcpy(pBCD,p,len);
	pBuffer += len;
}

static inline void GetString(char* pStr,char*& pBuffer,int len)
{
	if(len > 0) {
		memcpy(pStr,pBuffer,len);
		pStr[len + 1] = '\0';
		pBuffer += len;
	} else {
#if 0		
		pBuffer += sprintf(pStr,pBuffer);
#else
		pBuffer += sprintf(pStr, "%s", pBuffer);
#endif
	}
}

void CJT808Parse::Init(InterFace * pInterFace,CJT808Package* pPackage)
{
	m_pInterFace = pInterFace;
	m_pPackage = pPackage;
}

int CJT808Parse::ReqPacket(NetRcvMsg* pMsg)
{
	return 0;
}

int CJT808Parse::SetSysCfg(NetRcvMsg* pMsg)
{
	DWORD paramID;
	CJT808ParamBase parambase;
	JT808_Config_S sysCfg;		//保存平台参数设置
	m_pInterFace->GetSysCfg(&sysCfg);
	parambase.Init(&sysCfg);
	char* pData = pMsg->pData;
	BYTE num = GetByte(pData);
	
	for(BYTE i = 0; i < num; i++){
		paramID = GetDWord(pData);
		
		parambase.Get(pData,paramID);
	}
	 // 将配置写入文件
	int ret = m_pInterFace->SetSysCfg(&sysCfg);		
	/*设置完成发送通用应答*/
	CommReply( pMsg->wSerialNumber, pMsg->wId, ret);
	return 0;
}

static void SetADASParas(Adas_Attr* pParam,char* value)
{
	pParam->AlarmThres			= GetByte(value);
	pParam->AlarmVolume			= GetByte(value);
	pParam->ActiveSnap			= GetByte(value);
	pParam->TimeThres			= GetWord(value);
	pParam->MileageThres		= GetWord(value);
	pParam->SingleSnapNum		= GetByte(value);
	pParam->SingleSnapInter		= GetByte(value);
	pParam->PicResolution		= GetByte(value);
	pParam->VedioResolution		= GetByte(value);
	pParam->AlarmEnble			= GetDWord(value);
	pParam->EventEnable			= GetDWord(value);
	pParam->Reserve				= GetByte(value);
	pParam->OWDistanceThres		= GetByte(value);
	pParam->OWSpeedThres		= GetByte(value);
	pParam->OWRecordingTime		= GetByte(value);
	pParam->OWSnapNum			= GetByte(value);
	pParam->OWSnapThres			= GetByte(value);
	pParam->FDWAlarmTime		= GetByte(value);
	pParam->FDWTimes			= GetByte(value);
	pParam->FDWSpeedThres		= GetByte(value);
	pParam->FDWRecordingTime	= GetByte(value);
	pParam->FDWSnapNum			= GetByte(value);
	pParam->FDWSnapInterval		= GetByte(value);
	pParam->LDWSpeedThres		= GetByte(value);
	pParam->LDWRecordingTime	= GetByte(value);
	pParam->LDWSnapNum			= GetByte(value);
	pParam->LDWSnapInterval		= GetByte(value);
	pParam->FCWAlarmThres		= GetByte(value);
	pParam->FCWSpeedThres		= GetByte(value);
	pParam->FCWRecordingTime	= GetByte(value);
	pParam->FCWSnapNum			= GetByte(value);
	pParam->FCWSnapInterval		= GetByte(value);
	pParam->PCWAlarmThres		= GetByte(value);
	pParam->PCWSpeedThres		= GetByte(value);
	pParam->PCWRecordingTime	= GetByte(value);
	pParam->PCWSnapNum			= GetByte(value);
	pParam->PCWSnapInterval		= GetByte(value);
	pParam->DMWDistanceThres	= GetByte(value);
	pParam->DMWSpeedThres		= GetByte(value);
	pParam->DMWRecordingTime	= GetByte(value);
	pParam->DMWSnapNum			= GetByte(value);
	pParam->DMWSnapInterval		= GetByte(value);
	pParam->RSRSnapNum			= GetByte(value);
	pParam->RSRSnapInterval		= GetByte(value);
	GetString((char *)pParam->inter,value, 4);
}


static void SetDSMParas(Dsm_Attr* pParam,char* value)
{
	pParam->AlarmThres			= GetByte(value);
	pParam->AlarmVolume 		= GetByte(value);
	pParam->ActiveSnap			= GetByte(value);
	pParam->TimeThres			= GetWord(value);
	pParam->MileageThres		= GetWord(value);
	pParam->SingleSnapNum		= GetByte(value);
	pParam->SingleSnapInter 	= GetByte(value);
	pParam->PicResolution		= GetByte(value);
	pParam->VedioResolution 	= GetByte(value);
	pParam->AlarmEnble			= GetDWord(value);
	pParam->EventEnable 		= GetDWord(value);
	pParam->SmokeInter 			= GetByte(value);
	pParam->PhoneInter 			= GetByte(value);
	GetString((char *)pParam->inter,value, 3);
	pParam->FDWSpeedThres		= GetByte(value);
	pParam->FDWRecordingTime 	= GetByte(value);
	pParam->FDWSnapNum			= GetByte(value);
	pParam->FDWSnapInter		= GetByte(value);
	pParam->PhoneSpeedThres		= GetByte(value);
	pParam->PhoneRecordingTime	= GetByte(value);
	pParam->PhoneSnapNum		= GetByte(value);
	pParam->PhoneSnapInter		= GetByte(value);
	
	pParam->SmokeSpeedThres		= GetByte(value);
	pParam->SmokeRecordingTime 	= GetByte(value);
	pParam->SmokeSnapNum		= GetByte(value);
	pParam->SmokeSnapInter		= GetByte(value);
	pParam->DDWSpeedThres		= GetByte(value);
	pParam->DDWRecordingTime 	= GetByte(value);
	pParam->DDWSnapNum			= GetByte(value);
	pParam->DDWSnapInter		= GetByte(value);
	pParam->ADWSpeedThres		= GetDWord(value);
	pParam->ADWRecordingTime	= GetDWord(value);
	pParam->ADWSnapNum 			= GetByte(value);
	pParam->ADWSnapInter		= GetByte(value);
	pParam->TriggerAuth			= GetByte(value);

	GetString((char *)pParam->inter1,value, 3);
}

/*************************************************************************
*Function:SetAiViAttr
*Description:设置音视频参数
*Params in:
*Params out:
*Result:
*author:huangshimei
*date:20190620
**************************************************************************/

static void SetAiViAttr(JT1078_Audio_video_conf_set* pParam,char* value)
{
	pParam->coding_mode             = GetByte(value);
	pParam->resolution              = GetByte(value);
	pParam->key_frame_inter         = GetWord(value);
	pParam->frame_rate              = GetByte(value);
	pParam->bit_rate                = GetDWord(value);
	pParam->store_coding_mode       = GetByte(value);
	pParam->store_resolution        = GetByte(value);
	pParam->store_key_frame_inter   = GetWord(value);
	pParam->store_frame_rate        = GetByte(value);
	pParam->store_bit_rate          = GetDWord(value);
	pParam->osd_set                 = GetWord(value);
	pParam->enable_audio            = GetByte(value);
	
}

/*************************************************************************
*Function:SetAiViChn
*Description:设置音视频通道列表
*Params in:
*Params out:
*Result:
*author:huangshimei
*date:20190620
**************************************************************************/
static void SetAiViChn(JT1078_Audio_video_chn_set* pParam,char* value)
{
	int i, j;
	
	pParam->chn_total_num           = GetByte(value);
	pParam->audio_num               = GetByte(value);
	pParam->video_num               = GetByte(value);

	j = pParam->chn_total_num + pParam->audio_num + pParam->video_num;
	for (i = 0; i < j ; i++)
	{
		pParam->chn_table[i].physical_chn    = GetByte(value);
		pParam->chn_table[i].logical_chn     = GetByte(value);
		pParam->chn_table[i].chn_type        = GetByte(value);
		pParam->chn_table[i].connect_ptz     = GetByte(value);
	}
}

/*************************************************************************
*Function:SetSingleChn
*Description:单独通道视频参数设置
*Params in:
*Params out:
*Result:
*author:huangshimei
*date:20190620
**************************************************************************/
static void SetSingleChn(JT1078_single_chn_control* pParam,char* value)
{
	pParam->chn_num                        = GetByte(value);

	for (int i = 0; i < pParam->chn_num; i++)
	{
		pParam->chn_video_param[i].logical_chn           = GetByte(value);
		pParam->chn_video_param[i].coding_mod            = GetByte(value);
		pParam->chn_video_param[i].resolution            = GetByte(value);
		pParam->chn_video_param[i].key_frame_inter       = GetWord(value);
		pParam->chn_video_param[i].frame_rate            = GetByte(value);
		pParam->chn_video_param[i].bit_rate              = GetDWord(value);
		pParam->chn_video_param[i].store_coding_mode     = GetByte(value);
		pParam->chn_video_param[i].store_resolution      = GetByte(value);
		pParam->chn_video_param[i].logical_chn           = GetByte(value);
		pParam->chn_video_param[i].store_key_frame_inter = GetWord(value);
		pParam->chn_video_param[i].store_frame_rate      = GetByte(value);
		pParam->chn_video_param[i].store_bit_rate        = GetDWord(value);
		pParam->chn_video_param[i].osd_set               = GetWord(value);	
	}
}

/*************************************************************************
*Function:SetSpecialChn
*Description:特殊报警录像参数
*Params in:
*Params out:
*Result:
*author:huangshimei
*date:20190620
**************************************************************************/
static void SetSpecialChn(JT1078_special_alarm_param_set* pParam,char* value)
{
	pParam->video_storage_thres          = GetByte(value);
	pParam->video_duration               = GetByte(value);
	pParam->start_time                   = GetByte(value);
	
}

/*************************************************************************
*Function:SetVideoAlarmShield
*Description:视频相关报警屏蔽字
*Params in:
*Params out:
*Result:
*author:huangshimei
*date:20190620
**************************************************************************/
static void SetVideoAlarmShield(JT1078_alarm_shield* pParam,char* value)
{
	pParam->shield                       = GetDWord(value);
}

/*************************************************************************
*Function:SetVideoAlarmAttr
*Description:视频分析报警参数
*Params in:
*Params out:
*Result:
*author:huangshimei
*date:20190620
**************************************************************************/
static void SetVideoAlarmAttr(JT1078_alarm_video_param* pParam,char* value)
{
	pParam->num_of_carries              = GetByte(value);
	pParam->fatigue_thres               = GetByte(value);
}

/*************************************************************************
*Function:SetAwakeAttr
*Description:终端休眠唤醒模式设置数据格式
*Params in:
*Params out:
*Result:
*author:huangshimei
*date:20190620
**************************************************************************/
static void SetAwakeAttr(JT1078_awake_param* pParam,char* value)
{
	pParam->awake_mode                       = GetByte(value);
	pParam->awake_type                       = GetByte(value);
	pParam->set_awake_time                   = GetByte(value);
	pParam->awake_params.flag                = GetByte(value);
	GetBCD(pParam->awake_params.period1_start_time, value, 2);
	GetBCD(pParam->awake_params.period1_close_time, value, 2);
	GetBCD(pParam->awake_params.period2_start_time, value, 2);
	GetBCD(pParam->awake_params.period2_close_time, value, 2);
	GetBCD(pParam->awake_params.period3_start_time, value, 2);
	GetBCD(pParam->awake_params.period3_close_time, value, 2);
	GetBCD(pParam->awake_params.period4_start_time, value, 2);
	GetBCD(pParam->awake_params.period4_close_time, value, 2);
}

int CJT808Parse::SetSysCfg(NetRcvMsg* pMsg, JT808_Config_S*& sysCfg)
{
	DWORD paramID;
	CJT808ParamBase parambase;

	parambase.Init(sysCfg);
	char* pData = pMsg->pData;
	BYTE num = GetByte(pData);	//获取参数总数num

	for (BYTE i = 0; i < num; i++) {
		paramID = GetDWord(pData);
		parambase.Get(pData,paramID);

		switch(paramID)
		{
			case 0xF364:
				SetADASParas(&sysCfg->AdasCfg, (char *)sysCfg->cfg_store);
				break;
			case 0xF365:
				SetDSMParas(&sysCfg->DsmCfg, (char *)sysCfg->cfg_store);
				break;
			case 0x0075:
				SetAiViAttr(&sysCfg->VideoCfg.ai_vi_attr, (char *)sysCfg->cfg_store);
				break;
			case 0x0076:
				SetAiViChn(&sysCfg->VideoCfg.ai_vi_chn, (char *)sysCfg->cfg_store);
				break;
			case 0x0077:
				SetSingleChn(&sysCfg->VideoCfg.single_chn_ctl, (char *)sysCfg->cfg_store);
				break;
			case 0x0079:
				SetSpecialChn(&sysCfg->VideoCfg.special_alarm, (char *)sysCfg->cfg_store);
				break;
			case 0x007A:
				SetVideoAlarmShield(&sysCfg->VideoCfg.alarm_shield, (char *)sysCfg->cfg_store);
				break;
			case 0x007B:
				SetVideoAlarmAttr(&sysCfg->VideoCfg.video_alarm, (char *)sysCfg->cfg_store);
				break;
			case 0x007C:
				SetAwakeAttr(&sysCfg->VideoCfg.awake_attr, (char *)sysCfg->cfg_store);
				break;

			default:
				break;
		}
	}

	return 0;
}

/*int CJT808Parse::GetSelectParam(NetRcvMsg* pMsg)
{
	DWORD paramID[256];
	char* pData = pMsg->pData;
	DWORD num = GetByte(pData);
	for(DWORD i=0;i<num;i++)
	{
		paramID[i] = GetDWord(pData);
	}
	m_pPackage->ReplyParam(num, paramID, pMsg->wSerialNumber);
	return 0;
}*/

WORD CJT808Parse::GetSelectParam(NetRcvMsg* pMsg, DWORD* paramID, DWORD& num)
{
	char* pData = pMsg->pData;
	num = GetByte(pData);
	for(DWORD i = 0; i < num; i++)
	{
		paramID[i] = GetDWord(pData);
	}

	return 0;
}


int CJT808Parse::TerminalCtrl(NetRcvMsg* pMsg)
{
	CommReply(pMsg->wSerialNumber,pMsg->wId,0);
	return 0;
}

int CJT808Parse::Upgrade(NetRcvMsg* pMsg)
{
	char szVer[32];
	char* pData = pMsg->pData + 6;
	DWORD len = GetByte(pData);
	if(len>32)
	{
		len = 32;
	}
	GetString(szVer,pData,len);
	len = GetDWord(pData);
	CommReply(pMsg->wSerialNumber,pMsg->wId,0);
	return m_pInterFace->Upgrade(szVer,pData,len);
}

int CJT808Parse::Upgrade(NetRcvMsg* pMsg, char *szVerNum, DWORD& nLen)
{
	char* pData = pMsg->pData + 6;
	nLen = GetByte(pData);
	if (nLen > 32) {
		nLen = 32;
	}
	GetString(szVerNum, pData, nLen);
	nLen = GetDWord(pData);

	return 0;
}


int CJT808Parse::GetLocation(NetRcvMsg* pMsg)
{
	//m_pPackage->ReplyPosition(pMsg->wSerialNumber);
	return 0;
}

int CJT808Parse::LocationTracking(NetRcvMsg* pMsg)
{
	WORD interval = GetWord(pMsg->pData);
	DWORD time = GetDWord(pMsg->pData);
	int ret = m_pInterFace->LocationTracking(interval,time);
	CommReply(pMsg->wSerialNumber,pMsg->wId,ret);
	return ret;
}

int CJT808Parse::LocationTracking(NetRcvMsg* pMsg, WORD& interval, DWORD& time)
{
	interval = GetWord(pMsg->pData);
	time = GetDWord(pMsg->pData);
	
	return 0;
}

int CJT808Parse::ConfirmAlarm(NetRcvMsg* pMsg)
{
	//WORD alarmSN = GetWord(pMsg->pData);
	GetWord(pMsg->pData);
	DWORD alarmMask = GetDWord(pMsg->pData);
	int ret = m_pInterFace->ConfirmAlarm(alarmMask);
	CommReply(pMsg->wSerialNumber,pMsg->wId,ret);
	return ret;
}

int CJT808Parse::ConfirmAlarm(NetRcvMsg* pMsg, DWORD& alarmMask)
{
	//WORD alarmSN = GetWord(pMsg->pData);
	GetWord(pMsg->pData);
	alarmMask = GetDWord(pMsg->pData);
	
	return 0;
}

int CJT808Parse::SMSProc(NetRcvMsg* pMsg)
{
	char sms[1024];
	char* pData = pMsg->pData;
	//短消息标志
	SMS_MASK_E flag = (SMS_MASK_E)GetByte(pData);
#if 0
	//短消息长度
	int len = GetByte(pData);
	if(len>1024)
	{
		len = 1024;
	}
#else
	int len = pMsg->wMsgLen;
#endif	
	GetString(sms,pData,len);
	int ret = m_pInterFace->SMSProc(sms,flag);
	/*文本信息下发回复*/
	CommReply(pMsg->wSerialNumber, pMsg->wId, ret);
	return 0;
}

int CJT808Parse::SMSProc(NetRcvMsg* pMsg, char *strSms, SMS_MASK_E& flag)
{
	char* pData = pMsg->pData;
	//短消息标志
	flag = (SMS_MASK_E)GetByte(pData);
	int nLen = pMsg->wMsgLen;
	GetString(strSms, pData, nLen);
	
	return 0;
}

int CJT808Parse::EventSet(NetRcvMsg* pMsg)
{
	int len;
	char* pData = pMsg->pData;
	// 设置属性
	BYTE opr = GetByte(pData);
	// 信息项总数
	BYTE num = GetByte(pData);
	Event_Menu_Item* pMenuItem = new Event_Menu_Item[num];
	for (int i = 0; i < num; i++)
	{	
		/*类型*/
		pMenuItem[i].id = GetByte(pData);
		len = GetWord(pData);
		if(len>32)
		{
			len = 32;
		}
		GetString(pMenuItem[i].event,pData,len);
	}
	int ret = m_pInterFace->SetEventMenu((InfoOp)opr,pMenuItem,num);
	CommReply( pMsg->wSerialNumber, pMsg->wId,ret);
	return ret;
}

int CJT808Parse::EventSet(NetRcvMsg* pMsg, InfoOp& opr, vector<Event_Menu_Item>& vecMenuItem, BYTE& num)
{
	int len = 0;
	char* pData = pMsg->pData;
	// 设置属性
	opr = (InfoOp)GetByte(pData);
	// 信息项总数
	num = GetByte(pData);
	//Event_Menu_Item* pMenuItem = new Event_Menu_Item[num];
	vecMenuItem.clear();
	for (int i = 0; i < num; i++) {
		Event_Menu_Item stEvent_Menu_Item;
		/*类型*/
		stEvent_Menu_Item.id = GetByte(pData);
		len = GetWord(pData);
		if (len > 32) {
			len = 32;
		}
		GetString(stEvent_Menu_Item.event, pData, len);
		vecMenuItem.push_back(stEvent_Menu_Item);
	}
	
	return 0;
}

int CJT808Parse::QuestionDown(NetRcvMsg* pMsg)
{
	char szQustion[1024];
	Event_Menu_Item answer[4];
	int len;
	char* pData = pMsg->pData;
	// 标志
	SMS_MASK_E flag = (SMS_MASK_E)GetByte(pData);
	//问题内容长度
	len = GetByte(pData);
	if(len>1024)
	{
		len = 1024;
	}
	GetString(szQustion,pData,len);
	int i;
	for (i = 0; i < 4; i++)
	{
		/*类型*/
		answer[i].id = GetByte(pData);
		len = GetWord(pData);
		if(len>32)
		{
			len = 32;
		}
		GetString(answer[i].event,pData,len);
		if(pData-pMsg->pData>pMsg->wMsgLen)
		{
			break;
		}
	}
	int ret = m_pInterFace->SetAskAnswer(szQustion,flag,answer,i);
	CommReply( pMsg->wSerialNumber, pMsg->wId,ret);
	return ret;
}

int CJT808Parse::QuestionDown(NetRcvMsg* pMsg, char *szQustion, SMS_MASK_E& flag, vector<Event_Menu_Item>& vecAnswerItem, int& i)
{
	int len;
	char* pData = pMsg->pData;
	// 标志
	flag = (SMS_MASK_E)GetByte(pData);
	//问题内容长度
	len = GetByte(pData);
	if (len > 1024) {
		len = 1024;
	}
	GetString(szQustion,pData,len);
	for (i = 0; i < 4; i++) {
		Event_Menu_Item stAnswerItem;
		/*类型*/
		stAnswerItem.id = GetByte(pData);
		len = GetWord(pData);
		if (len > 32) {
			len = 32;
		}
		GetString(stAnswerItem.event, pData, len);
		vecAnswerItem.push_back(stAnswerItem);
		if (pData - pMsg->pData > pMsg->wMsgLen) {
			break;
		}
	}
	
	return 0;
}

int  CJT808Parse::InfoDmdMenuSet(NetRcvMsg* pMsg)
{

	int len;
	char* pData = pMsg->pData;
	// 设置属性
	BYTE opr = GetByte(pData);
	// 信息项总数
	BYTE num = GetByte(pData);
	Msg_Menu_Item* pMenuItem = new Msg_Menu_Item[num];
	for (int i = 0; i < num; i++)
	{	
		/*类型*/
		pMenuItem[i].type = GetByte(pData);
		len = GetWord(pData);
		if(len>32)
		{
			len = 32;
		}
		GetString(pMenuItem[i].msgname,pData,len);
	}
	int ret = m_pInterFace->SetMsgMenu((InfoOp)opr,pMenuItem,num);
	CommReply( pMsg->wSerialNumber, pMsg->wId,ret);
	return ret;
}

int CJT808Parse::InfoDmdMenuSet(NetRcvMsg* pMsg, InfoOp& opr, vector<Msg_Menu_Item>& vecMenuItem, BYTE& num)
{
	int len;
	char* pData = pMsg->pData;
	// 设置属性
	opr = (InfoOp)GetByte(pData);
	// 信息项总数
	num = GetByte(pData);
	//Msg_Menu_Item* pMenuItem = new Msg_Menu_Item[num];
	for (int i = 0; i < num; i++)
	{
		Msg_Menu_Item stMenuItem;
		/*类型*/
		stMenuItem.type = GetByte(pData);
		len = GetWord(pData);
		if(len>32)
		{
			len = 32;
		}
		GetString(stMenuItem.msgname, pData, len);
		vecMenuItem.push_back(stMenuItem);
	}
	
	return 0;
}

int CJT808Parse::InfoService(NetRcvMsg* pMsg)
{
	BYTE msgType = GetByte(pMsg->pData);
	WORD msgLen = GetWord(pMsg->pData);
	char* pMessage = new char[msgLen];
	GetString(pMessage,pMsg->pData,msgLen);
	int ret = m_pInterFace->MessageService(msgType,pMessage);
	CommReply( pMsg->wSerialNumber, pMsg->wId, ret);
	delete [] pMessage;
	return 0;
}

int CJT808Parse::InfoService(NetRcvMsg* pMsg, BYTE& msgType, char* pMessage)
{
	msgType = GetByte(pMsg->pData);
	WORD msgLen = GetWord(pMsg->pData);
	pMessage = (char *)realloc(pMessage, msgLen * sizeof(char));
	GetString(pMessage, pMsg->pData, msgLen);
	
	return 0;
}

int CJT808Parse::TelCallback(NetRcvMsg* pMsg)
{
	char szPhoneNo[24];
	// 标志0，普通通话 1， 监听
	BYTE bMark = GetByte(pMsg->pData);
	GetString(szPhoneNo,pMsg->pData,pMsg->wMsgLen-1);
	szPhoneNo[pMsg->wMsgLen] = '\0';
	int ret = m_pInterFace->TelCallback(bMark,szPhoneNo);
	CommReply(pMsg->wSerialNumber,pMsg->wId,ret);
	return 0;
}

int CJT808Parse::TelCallback(NetRcvMsg* pMsg, BYTE& bMark, char *szPhoneNo)
{
	// 标志0，普通通话 1， 监听
	bMark = GetByte(pMsg->pData);
	GetString(szPhoneNo, pMsg->pData, pMsg->wMsgLen-1);
	szPhoneNo[pMsg->wMsgLen] = '\0';
	
	return 0;
}

int CJT808Parse::TelBookSet(NetRcvMsg* pMsg)
{
	Address_Book_Item* pAddrBook = NULL;
	char* pData = pMsg->pData;
	// 设置属性
	BYTE opr = GetByte(pData);
	// 区域总数
	BYTE num = GetByte(pData);
	int len;
	if(opr>0&&num>0)
	{
		pAddrBook = new Address_Book_Item[num];
		for(int i=0;i<num;i++)
		{
			pAddrBook->flag = (DWORD)GetByte(pData);
			len = GetByte(pData);
			GetString(pAddrBook->phone,pData,len);
			len = GetByte(pData);
			GetString(pAddrBook->name,pData,len);
		}
	}
	int ret = m_pInterFace->TelBookSet((InfoOp)opr,pAddrBook,num);
	CommReply(pMsg->wSerialNumber,pMsg->wId,ret);
	if(pAddrBook)
	{
		delete [] pAddrBook;
	}
	return ret;
}

int CJT808Parse::TelBookSet(NetRcvMsg* pMsg, InfoOp& opr, vector<Address_Book_Item>& vecAddrBook, BYTE& num)
{
	Address_Book_Item stAddrBook;
	char* pData = pMsg->pData;
	// 设置属性
	opr = (InfoOp)GetByte(pData);
	// 区域总数
	num = GetByte(pData);
	int len;
	if(opr > 0 && num > 0) {
		for (int i = 0; i < num; i++) {
			memset(&stAddrBook, 0, sizeof(Address_Book_Item));
			stAddrBook.flag = (DWORD)GetByte(pData);
			len = GetByte(pData);
			GetString(stAddrBook.phone, pData, len);
			len = GetByte(pData);
			GetString(stAddrBook.name, pData, len);
			vecAddrBook.push_back(stAddrBook);
		}
	}
	
	return 0;
}

/*int CJT808Parse::VehicleCtrl(NetRcvMsg* pMsg)
{
	m_pInterFace->VehiDoorCtrlSet(GetByte(pMsg->pData));
	m_pPackage->ReplyControl(pMsg->wSerialNumber);
	return 0;
}*/

int CJT808Parse::VehicleCtrl(NetRcvMsg* pMsg, BYTE& bDoorLock)
{
	bDoorLock = GetByte(pMsg->pData);
	
	return 0;
}

int CJT808Parse::SetRingRgn(NetRcvMsg* pMsg)
{
	char* pData = pMsg->pData;
	// 设置属性
	BYTE opr = GetByte(pData);
	// 区域总数
	BYTE regNum = GetByte(pData);
	Ring_Region_Attr* pRegion = new Ring_Region_Attr[regNum];
	
	for (int i = 0; i < regNum; i++)
	{
		//id
		pRegion[i].attr.id = GetDWord(pData);
		//属性
		pRegion[i].attr.attr = GetWord(pData);
		//纬度
		pRegion[i].point.lattitude = GetWord(pData);
		//经度
		pRegion[i].point.longtitude = GetDWord(pData);
		//半径
		pRegion[i].rd = GetDWord(pData);
		/* 区域属性判断 */
		if (pRegion[i].attr.attr & 0x0001)
		{
			// 起始时间
			GetBCD(pRegion[i].attr.stime,pData,6);
			// 结束时间
			GetBCD(pRegion[i].attr.etime,pData,6);
		}
		if (pRegion[i].attr.attr & 0x0002)
		{
			// 最高速度
			pRegion[i].attr.speedmax = GetWord(pData);
			// 超速持续时间
			pRegion[i].attr.overspeedtime = GetByte(pData);
		}
	}	
	
	int result = m_pInterFace->SetRingRgn(opr,pRegion,regNum);
	CommReply(pMsg->wSerialNumber,pMsg->wId,result);
	delete [] pRegion;
	
	return result;
}

int CJT808Parse::SetRingRgn(NetRcvMsg* pMsg, BYTE& opr, vector<Ring_Region_Attr>& vecRegionAttr, BYTE& regNum)
{
	char* pData = pMsg->pData;
	// 设置属性
	opr = GetByte(pData);
	// 区域总数
	regNum = GetByte(pData);
	//Ring_Region_Attr* pRegion = new Ring_Region_Attr[regNum];
	Ring_Region_Attr stRegionAttr;
	for (int i = 0; i < regNum; i++)
	{
		memset(&stRegionAttr, 0, sizeof(stRegionAttr));
		//id
		stRegionAttr.attr.id = GetDWord(pData);
		//属性
		stRegionAttr.attr.attr = GetWord(pData);
		//纬度
		stRegionAttr.point.lattitude = GetWord(pData);
		//经度
		stRegionAttr.point.longtitude = GetDWord(pData);
		//半径
		stRegionAttr.rd = GetDWord(pData);
		/* 区域属性判断 */
		if (stRegionAttr.attr.attr & 0x0001)
		{
			// 起始时间
			GetBCD(stRegionAttr.attr.stime,pData,6);
			// 结束时间
			GetBCD(stRegionAttr.attr.etime,pData,6);
		}
		if (stRegionAttr.attr.attr & 0x0002)
		{
			// 最高速度
			stRegionAttr.attr.speedmax = GetWord(pData);
			// 超速持续时间
			stRegionAttr.attr.overspeedtime = GetByte(pData);
		}
		vecRegionAttr.push_back(stRegionAttr);
	}
	
	return 0;
}


int CJT808Parse::SetRectRgn(NetRcvMsg* pMsg)
{
	char* pData = pMsg->pData;
	// 设置属性
	BYTE opr = GetByte(pData);
	// 区域总数
	BYTE regNum = GetByte(pData);
	Rectangle_Region_Attr* pRegion = new Rectangle_Region_Attr[regNum];
	
	for (int i = 0; i < regNum; i++)
	{
		//id
		pRegion[i].attr.id = GetDWord(pData);
		//属性
		pRegion[i].attr.attr = GetWord(pData);
		//纬度
		pRegion[i].leftup.lattitude = GetWord(pData);
		//经度
		pRegion[i].leftup.longtitude = GetDWord(pData);
		//纬度
		pRegion[i].rightdown.lattitude = GetWord(pData);
		//经度
		pRegion[i].rightdown.longtitude = GetDWord(pData);
		/* 区域属性判断 */
		if (pRegion[i].attr.attr & 0x0001)
		{
			// 起始时间
			GetBCD(pRegion[i].attr.stime,pData,6);
			// 结束时间
			GetBCD(pRegion[i].attr.etime,pData,6);
		}
		if (pRegion[i].attr.attr & 0x0002)
		{
			// 最高速度
			pRegion[i].attr.speedmax = GetWord(pData);
			// 超速持续时间
			pRegion[i].attr.overspeedtime = GetByte(pData);
		}
	}	
	
	int result = m_pInterFace->SetRectRgn(opr,pRegion,regNum);
	CommReply(pMsg->wSerialNumber,pMsg->wId, result);
	delete [] pRegion;
	
	return result;
}

int CJT808Parse::SetRectRgn(NetRcvMsg* pMsg, BYTE& opr, vector<Rectangle_Region_Attr>& vecRegionAttr, BYTE& regNum)
{
	char* pData = pMsg->pData;
	// 设置属性
	opr = GetByte(pData);
	// 区域总数
	regNum = GetByte(pData);
	Rectangle_Region_Attr stRectangleAttr;

	for (int i = 0; i < regNum; i++)
	{
		memset(&stRectangleAttr, 0, sizeof(stRectangleAttr));
		//id
		stRectangleAttr.attr.id = GetDWord(pData);
		//属性
		stRectangleAttr.attr.attr = GetWord(pData);
		//纬度
		stRectangleAttr.leftup.lattitude = GetWord(pData);
		//经度
		stRectangleAttr.leftup.longtitude = GetDWord(pData);
		//纬度
		stRectangleAttr.rightdown.lattitude = GetWord(pData);
		//经度
		stRectangleAttr.rightdown.longtitude = GetDWord(pData);
		/* 区域属性判断 */
		if (stRectangleAttr.attr.attr & 0x0001)
		{
			// 起始时间
			GetBCD(stRectangleAttr.attr.stime,pData,6);
			// 结束时间
			GetBCD(stRectangleAttr.attr.etime,pData,6);
		}
		if (stRectangleAttr.attr.attr & 0x0002)
		{
			// 最高速度
			stRectangleAttr.attr.speedmax = GetWord(pData);
			// 超速持续时间
			stRectangleAttr.attr.overspeedtime = GetByte(pData);
		}
		vecRegionAttr.push_back(stRectangleAttr);
	}
	
	return 0;
}


int CJT808Parse::SetPolyRgn(NetRcvMsg* pMsg)
{	
	char* pData = pMsg->pData;
	Region_Base_Attr attr;
	
	//id
	attr.id = GetDWord(pData);
	//属性
	attr.attr = GetWord(pData);
	/* 区域属性判断 */
	if (attr.attr & 0x0001)
	{
		// 起始时间
		GetBCD(attr.stime,pData,6);
		// 结束时间
		GetBCD(attr.etime,pData,6);
	}
	if (attr.attr & 0x0002)
	{
		// 最高速度
		attr.speedmax = GetWord(pData);
		// 超速持续时间
		attr.overspeedtime = GetByte(pData);
	}
	/*顶点数*/
	WORD num = GetWord(pData);
	
	Poly_Region_Attr* pRegion = (Poly_Region_Attr*)malloc(sizeof(Poly_Region_Attr)+sizeof(Gnss_Point)*num);
	pRegion->attr = attr;
	pRegion->num = num;
	
	for(int i=0;i<num;i++)
	{
		//纬度
		pRegion->node[i].lattitude = GetWord(pData);
		//经度
		pRegion->node[i].longtitude = GetDWord(pData);
	}
	
	int result = m_pInterFace->SetPolyRgn(pRegion);
	CommReply(pMsg->wSerialNumber,pMsg->wId,result);
	free(pRegion);
	pRegion = NULL;
	
	return result;
}

int CJT808Parse::SetPolyRgn(NetRcvMsg* pMsg, Poly_Region_Attr* pRegion)
{	
	char* pData = pMsg->pData;
	Region_Base_Attr attr;
	
	//id
	attr.id = GetDWord(pData);
	//属性
	attr.attr = GetWord(pData);
	/* 区域属性判断 */
	if (attr.attr & 0x0001)
	{
		// 起始时间
		GetBCD(attr.stime,pData,6);
		// 结束时间
		GetBCD(attr.etime,pData,6);
	}
	if (attr.attr & 0x0002)
	{
		// 最高速度
		attr.speedmax = GetWord(pData);
		// 超速持续时间
		attr.overspeedtime = GetByte(pData);
	}
	/*顶点数*/
	WORD num = GetWord(pData);
	
	pRegion = (Poly_Region_Attr*)realloc(pRegion, sizeof(Poly_Region_Attr) + sizeof(Gnss_Point) * num);
	pRegion->attr = attr;
	pRegion->num = num;
	
	for(int i=0;i<num;i++)
	{
		//纬度
		pRegion->node[i].lattitude = GetWord(pData);
		//经度
		pRegion->node[i].longtitude = GetDWord(pData);
	}
	
	return 0;
}


//设置线路
int CJT808Parse::SetRoute(NetRcvMsg* pMsg)
{
	char* pData = pMsg->pData;
	Route_Region_Attr routeAttr;
	
	//id
	routeAttr.id = GetDWord(pData);
	//属性
	routeAttr.attr = GetWord(pData);
	/* 区域属性判断 */
	if (routeAttr.attr & 0x0001)
	{
		// 起始时间
		GetBCD(routeAttr.stime,pData,6);
		// 结束时间
		GetBCD(routeAttr.etime,pData,6);
	}
	/*顶点数*/
	routeAttr.num = GetWord(pData);
	
	Route_Region_Attr* pRegion = (Route_Region_Attr*)malloc(sizeof(Route_Region_Attr)+sizeof(Line_Node_Attr)*routeAttr.num);
	pRegion->id = routeAttr.id;
	pRegion->attr = routeAttr.attr;
	for(int i=0;i<6;i++)
	{
		pRegion->stime[i] = routeAttr.stime[i];
		pRegion->etime[i] = routeAttr.etime[i];
	}
	pRegion->num = routeAttr.num;
	
	for(int i=0;i<pRegion->num;i++)
	{
		//拐点ID
		pRegion->node[i].nodeid = GetDWord(pData);
		// 路段id
		pRegion->node[i].Sectionid = GetDWord(pData);
		// 拐点纬度
		pRegion->node[i].point.lattitude = GetDWord(pData);
		// 拐点纬度
		pRegion->node[i].point.longtitude = GetDWord(pData);
		// 路段宽度
		pRegion->node[i].width = GetByte(pData);
		// 路段属性
		pRegion->node[i].attr = GetByte(pData);
		if( pRegion->node[i].attr & 0x0001)
		{
			//路段行驶过长阈值
			pRegion->node[i].drivingmax = GetWord(pData);
			// 路段行驶不足阈值
			pRegion->node[i].drivingmin = GetWord(pData);
		}
		if (pRegion->node[i].attr & 0x0002)
		{
			// 路段最高速度
			pRegion->node[i].speedmax = GetWord(pData);
			// 路段超速持续时间
			pRegion->node[i].overspeedtime = GetByte(pData);
		}
	}
	int result = m_pInterFace->SetRouteRgn(pRegion);
	CommReply(pMsg->wSerialNumber,pMsg->wId,result);
	free(pRegion);
	pRegion = NULL;
	
	return result;
}

//设置线路
int CJT808Parse::SetRoute(NetRcvMsg* pMsg, Route_Region_Attr* pRegion)
{
	char* pData = pMsg->pData;
	Route_Region_Attr routeAttr;
	
	//id
	routeAttr.id = GetDWord(pData);
	//属性
	routeAttr.attr = GetWord(pData);
	/* 区域属性判断 */
	if (routeAttr.attr & 0x0001)
	{
		// 起始时间
		GetBCD(routeAttr.stime,pData,6);
		// 结束时间
		GetBCD(routeAttr.etime,pData,6);
	}
	/*顶点数*/
	routeAttr.num = GetWord(pData);
	
	pRegion = (Route_Region_Attr *)realloc(pRegion, sizeof(Route_Region_Attr) + sizeof(Line_Node_Attr) * routeAttr.num);
	pRegion->id = routeAttr.id;
	pRegion->attr = routeAttr.attr;
	for(int i=0;i<6;i++)
	{
		pRegion->stime[i] = routeAttr.stime[i];
		pRegion->etime[i] = routeAttr.etime[i];
	}
	pRegion->num = routeAttr.num;
	
	for(int i=0;i<pRegion->num;i++)
	{
		//拐点ID
		pRegion->node[i].nodeid = GetDWord(pData);
		// 路段id
		pRegion->node[i].Sectionid = GetDWord(pData);
		// 拐点纬度
		pRegion->node[i].point.lattitude = GetDWord(pData);
		// 拐点纬度
		pRegion->node[i].point.longtitude = GetDWord(pData);
		// 路段宽度
		pRegion->node[i].width = GetByte(pData);
		// 路段属性
		pRegion->node[i].attr = GetByte(pData);
		if( pRegion->node[i].attr & 0x0001)
		{
			//路段行驶过长阈值
			pRegion->node[i].drivingmax = GetWord(pData);
			// 路段行驶不足阈值
			pRegion->node[i].drivingmin = GetWord(pData);
		}
		if (pRegion->node[i].attr & 0x0002)
		{
			// 路段最高速度
			pRegion->node[i].speedmax = GetWord(pData);
			// 路段超速持续时间
			pRegion->node[i].overspeedtime = GetByte(pData);
		}
	}
	
	return 0;
}

int CJT808Parse::DelRingRgn(NetRcvMsg* pMsg)
{
	return DelRgn(pMsg,0);
}

int CJT808Parse::DelRectRgn(NetRcvMsg* pMsg)
{
	return DelRgn(pMsg,1);
}

int CJT808Parse::DelPolyRgn(NetRcvMsg* pMsg)
{
	return DelRgn(pMsg,2);
}

int CJT808Parse::DelRoute(NetRcvMsg* pMsg)
{
	return DelRgn(pMsg,3);
}

/*int CJT808Parse::ReqDriverInfo(NetRcvMsg* pMsg)
{
	m_pPackage->ReportDriverInfo();
	return 0;
}*/

int CJT808Parse::DownLinkTrans(NetRcvMsg* pMsg)
{
	char* pData = pMsg->pData;
	TRANSPARENT_TYPE_E type = (TRANSPARENT_TYPE_E)GetByte(pData);
	int result = m_pInterFace->DownlinkTrans(type,pData,pMsg->wMsgLen - 1);
	CommReply(pMsg->wSerialNumber, pMsg->wId, result);
	return 0;
}

int CJT808Parse::DownLinkTrans(NetRcvMsg* pMsg, char* strBuffer, TRANSPARENT_TYPE_E& type, int& nSize)
{
	char* pData = pMsg->pData;
	type = (TRANSPARENT_TYPE_E)GetByte(pData);
	nSize =  pMsg->wMsgLen - 1;
	memcpy(strBuffer, pData, nSize);
	
	return 0;
}

void CJT808Parse::CommReply(WORD wSerialNumber, WORD wId, BYTE bResult)
{	
	//m_pPackage->CommReply(wSerialNumber,wId,bResult);
}

int CJT808Parse::DelRgn(NetRcvMsg* pMsg,int rgnType)
{
	char* pData = pMsg->pData;
	DWORD rgnID[125];
	
	DWORD rgnNum = GetByte(pData);
	
	for(DWORD i=0;i<rgnNum&&i<125;i++)
	{
		rgnID[i] = GetDWord(pData);
	}
	int result = m_pInterFace->DelRgn(rgnType,rgnID,rgnNum);
	CommReply(pMsg->wSerialNumber, pMsg->wId, result);
	
	return result;
}

int CJT808Parse::DelRgn(NetRcvMsg* pMsg, DWORD* rgnID, DWORD& rgnNum)
{
	char* pData = pMsg->pData;
	rgnNum = GetByte(pData);

	for(DWORD i = 0; i < rgnNum && i < 125; i++)
	{
		rgnID[i] = GetDWord(pData);
	}
	
	return 0;
}


int CJT808Parse::FileServerInfo(NetRcvMsg* pMsg, File_Server_Config& _config)
{
	char *pData = pMsg->pData;
	if(pData != NULL){
		memset(&_config, 0, sizeof(File_Server_Config));
		int len = GetByte(pData);
		GetString((char *)&_config.szAddr[0], pData, len);
		_config.nTcpPort = GetWord(pData);
		_config.nUdpPort = GetWord(pData);
		for(int i = 0; i < 7; ++ i){
			_config.alarm_flag.id[i] = GetByte(pData);
		}
		GetBCD(_config.alarm_flag.datetime, pData, 6);
		_config.alarm_flag.sequence = GetByte(pData);
		_config.alarm_flag.attachment_num = GetByte(pData);
		++ pData;
		for(int i = 0; i < 32; ++ i){
			_config.alarmCode[i] = GetByte(pData);
		}
		//reversed
		//CommReply(pMsg->wSerialNumber, pMsg->wId, 1);
	}

	return 0;
}

int CJT808Parse::UploadFileFinish(NetRcvMsg* pMsg, File_Finish_Respone_s& stResponse)
{
	char *pData = pMsg->pData;
	int len = GetByte(pData);
	
	GetString(&stResponse.szFileName[0], pData, len);
	stResponse.nType = GetByte(pData);
	stResponse.nResult = GetByte(pData);
	DBG_LEVEL_6("[%02X]-", stResponse.nResult);
	
	if(stResponse.nResult != 0x00){
		stResponse.nReSendPackageNum = GetByte(pData);
		DBG_LEVEL_6("-[%02X]-", stResponse.nReSendPackageNum);
	
		for(int i = 0; i < stResponse.nReSendPackageNum; ++ i){
			ReSendData_s stData;
			stData.nOffset = GetDWord(pData);
			stData.nLen = GetDWord(pData);
			stResponse.reSendDataList.push_back(stData);
		}
	}
	
	return 0;
}

int CJT808Parse::FaceCmpResInfo(NetRcvMsg* pMsg,  FaceCmpPubResult *pFaceCmpRes)
{
    char *pData = pMsg->pData;
    pFaceCmpRes->result = GetByte(pData);

	DBG_LEVEL_6("result:%d ", pFaceCmpRes->result);
	if (1 == pFaceCmpRes->result ){
		//compare faild
		//return 0;
	}
    pFaceCmpRes->similarity = GetByte(pData);
    pFaceCmpRes->similar_degree = GetWord(pData);
    pFaceCmpRes->ID_len = GetByte(pData);
	GetString(&pFaceCmpRes->ID[0], pData, pFaceCmpRes->ID_len);
	
	DBG_LEVEL_6("len:%02x face id:%s", pFaceCmpRes->ID_len, pFaceCmpRes->ID);
	
	DBG_LEVEL_6("result:%02x simi:%02x degree:%04x len:%02x id:%s", pFaceCmpRes->result,
			pFaceCmpRes->similarity, pFaceCmpRes->similar_degree, pFaceCmpRes->ID_len, pFaceCmpRes->ID);

	return 0;
}

int CJT808Parse::FacePictureInfo(NetRcvMsg* pMsg,        FacePicPublish* picinfo)
{
	int i, j;
	char *pData = pMsg->pData;
	picinfo->wSerialNumber			= pMsg->wSerialNumber;
	picinfo->cmd_type				= GetByte(pData);
	picinfo->list_num				= GetByte(pData);
	DBG_LEVEL_6("cdm:%02x list num:%02x", picinfo->cmd_type, picinfo->list_num);

	for (i = 0; i < picinfo->list_num; i++)
	{
		picinfo->face_info[i].face_id_len	= GetByte(pData);
		GetString(&picinfo->face_info[i].face_id[0], pData, picinfo->face_info[i].face_id_len);
		DBG_LEVEL_6("len:%02x face id:%s", picinfo->face_info[i].face_id_len, picinfo->face_info[i].face_id);
		
		picinfo->face_info[i].name_len		= GetByte(pData);
		GetString(&picinfo->face_info[i].name[0], pData, picinfo->face_info[i].name_len);
		DBG_LEVEL_6("name len:%d name:%s ", picinfo->face_info[i].name_len, picinfo->face_info[i].name);
		picinfo->face_info[i].download_proc	= GetByte(pData);
		picinfo->face_info[i].pic_src		= GetByte(pData);
		picinfo->face_info[i].pic_num		= GetByte(pData);
		DBG_LEVEL_6("src:%02x pic_num:%02x ", picinfo->face_info[i].pic_src, picinfo->face_info[i].pic_num);

		for (j = 0; j < picinfo->face_info[i].pic_num; j++)
		{		
			picinfo->face_info[i].pic_info[j].pic_url_len	= GetByte(pData);
			DBG_LEVEL_6("pic_url_len:%02x ", picinfo->face_info[i].pic_info[j].pic_url_len);

			GetString((char *)&picinfo->face_info[i].pic_info[j].pic_url[0], pData, picinfo->face_info[i].pic_info[j].pic_url_len);
			DBG_LEVEL_6("proc:%02x add:%s  addr len:%02x ", picinfo->face_info[i].download_proc, picinfo->face_info[i].pic_info[j].pic_url, picinfo->face_info[i].pic_info[j].pic_url_len);

			picinfo->face_info[i].pic_info[j].pic_name_len	= GetByte(pData);
			GetString(&picinfo->face_info[i].pic_info[j].pic_name[0], pData, picinfo->face_info[i].pic_info[j].pic_name_len);
			DBG_LEVEL_6("pic name len:%02x pic name:%s", picinfo->face_info[i].pic_info[j].pic_name_len, picinfo->face_info[i].pic_info[j].pic_name);
		
			picinfo->face_info[i].pic_info[j].pic_type	= GetByte(pData);
			picinfo->face_info[i].pic_info[j].pic_size	= GetDWord(pData);
			DBG_LEVEL_6("pic type:%02x pic size:%08x ", picinfo->face_info[i].pic_info[j].pic_type, picinfo->face_info[i].pic_info[j].pic_size);

			picinfo->face_info[i].pic_info[j].up_left_coordi[0]				=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].up_left_coordi[1]				=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].down_right_coordi[0]			=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].down_right_coordi[1]			=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].left_eye_center_coordi[0]		=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].left_eye_center_coordi[1]		=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].right_eye_center_coordi[0]	=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].right_eye_center_coordi[1]	=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].nose_coordi[0]				=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].nose_coordi[1]				=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].left_mouth_coordi[0]			=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].left_mouth_coordi[1]			=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].right_mouth_coordi[0]			=	GetWord(pData);
			picinfo->face_info[i].pic_info[j].right_mouth_coordi[1]			=	GetWord(pData);

			DBG_LEVEL_6("up_left_coordi:%04x %04x ", picinfo->face_info[i].pic_info[j].up_left_coordi[0], picinfo->face_info[i].pic_info[j].up_left_coordi[1]);
			DBG_LEVEL_6("down_right_coordi:%04x %04x ", picinfo->face_info[i].pic_info[j].down_right_coordi[0], picinfo->face_info[i].pic_info[j].down_right_coordi[1]);
			DBG_LEVEL_6("left_eye_center_coordi:%04x %04x ", picinfo->face_info[i].pic_info[j].left_eye_center_coordi[0], picinfo->face_info[i].pic_info[j].left_eye_center_coordi[1]);
			DBG_LEVEL_6("right_eye_center_coordi:%04x %04x ", picinfo->face_info[i].pic_info[j].right_eye_center_coordi[0], picinfo->face_info[i].pic_info[j].right_eye_center_coordi[1]);
			DBG_LEVEL_6("nose_coordi:%04x %04x ", picinfo->face_info[i].pic_info[j].nose_coordi[0], picinfo->face_info[i].pic_info[j].nose_coordi[1]);
			DBG_LEVEL_6("left_mouth_coordi:%04x %04x ", picinfo->face_info[i].pic_info[j].left_mouth_coordi[0], picinfo->face_info[i].pic_info[j].left_mouth_coordi[1]);
			DBG_LEVEL_6("right_mouth_coordi:%04x %04x ", picinfo->face_info[i].pic_info[j].right_mouth_coordi[0], picinfo->face_info[i].pic_info[j].right_mouth_coordi[1]);
		}	
	}

	return 0;
}

int CJT808Parse::VideoServerInfo(NetRcvMsg* pMsg,  VideoServerParamSet *pServerInfo)
{
    char *pData = pMsg->pData;
	pServerInfo->server_addr_len     = GetByte(pData);
	GetString(pServerInfo->server_addr, pData, pServerInfo->server_addr_len);
	pServerInfo->tcp_port            = GetWord(pData);
	pServerInfo->udp_port            = GetWord(pData);
	pServerInfo->logical_chn         = GetByte(pData);
	pServerInfo->data_type           = GetByte(pData);
	pServerInfo->stream_type         = GetByte(pData);

	DBG_LEVEL_6("server_addr_len:%02x server_addr:%s tcp_port:%04x udp_port:%04x logical_chn:%02x stream_type:%02x stream_type:%02x", 
			pServerInfo->server_addr_len,pServerInfo->server_addr,
			pServerInfo->tcp_port, pServerInfo->udp_port, pServerInfo->logical_chn, 
			pServerInfo->data_type, pServerInfo->stream_type);

	return 0;
}
int CJT808Parse::VideoTransControl(NetRcvMsg* pMsg,	VideoTransCtrl *pTransCtrl)
{
	char *pData = pMsg->pData;
	pTransCtrl->logic_chn	     = GetByte(pData);
	pTransCtrl->control_instrut	 = GetByte(pData);
	pTransCtrl->close_av	     = GetByte(pData);
	pTransCtrl->switch_stream	 = GetByte(pData);

	DBG_LEVEL_6("logic_chn:%02x control_instrut:%02x close_av:%02x switch_stream:%02x ", 
			pTransCtrl->logic_chn,pTransCtrl->control_instrut, pTransCtrl->close_av,pTransCtrl->switch_stream);

	return 0;
}

int CJT808Parse::VideoTransStatus(NetRcvMsg* pMsg,  VideoTransSta *pTransSta)
{
    char *pData = pMsg->pData;
	pTransSta->logic_chn     = GetByte(pData);
	pTransSta->packet_loss   = GetByte(pData);
	
	DBG_LEVEL_6("logic_chn:%02x packet_loss:%02x ", 
			pTransSta->logic_chn,pTransSta->packet_loss);
	
	return 0;
}

int CJT808Parse::VideoQueResource(NetRcvMsg* pMsg,	VideoResource *pResourceInfo)
{
    char *pData = pMsg->pData;
	pResourceInfo->logic_chn     = GetByte(pData);
	GetBCD(pResourceInfo->start_time, pData, 6);
	GetBCD(pResourceInfo->end_time, pData, 6);
	pResourceInfo->alarm_flag1   = GetDWord(pData);
	pResourceInfo->alarm_flag2   = GetDWord(pData);
	pResourceInfo->type          = GetByte(pData);
	pResourceInfo->stream_type   = GetByte(pData);
	pResourceInfo->store_type    = GetByte(pData);

	DBG_LEVEL_6("logic_chn:%02x alarm_flag1:%d alarm_flag2:%d type:%02x stream_type:%02x store_type:%02x ", 
			pResourceInfo->logic_chn, pResourceInfo->alarm_flag1, pResourceInfo->alarm_flag2, 
			pResourceInfo->type, pResourceInfo->stream_type,pResourceInfo->store_type);

	return 0;
}

int CJT808Parse::VideoQuePlayback(NetRcvMsg* pMsg,	VideoPlayback *pPlayback)
{
    char *pData = pMsg->pData;
	pPlayback->addr_len     = GetByte(pData);
	GetString(pPlayback->addr, pData, pPlayback->addr_len);
	pPlayback->tcp_port      = GetWord(pData);
	pPlayback->udp_port      = GetWord(pData);
	pPlayback->logic_chn     = GetByte(pData);
	pPlayback->type          = GetByte(pData);
	pPlayback->stream_type   = GetByte(pData);
	pPlayback->store_type    = GetByte(pData);
	pPlayback->pb_method     = GetByte(pData);
	pPlayback->multiple      = GetByte(pData);
	
	GetBCD(pPlayback->start_time, pData, 6);
	GetBCD(pPlayback->end_time, pData, 6);

	DBG_LEVEL_6("len:%02x addr:%s port:%02x port:%02x chn:%02x type:%02x stream:%02x store:%02x method:%02x multiple:%02x ", 
			pPlayback->addr_len, pPlayback->addr, pPlayback->tcp_port, 
			pPlayback->udp_port, pPlayback->logic_chn, pPlayback->type, 
			pPlayback->stream_type, pPlayback->store_type, pPlayback->pb_method, pPlayback->multiple);
	
	return 0;
}

int CJT808Parse::VideoPlaybackCtrl(NetRcvMsg* pMsg,	VideoPlaybackControl *pPb_ctrl)
{
    char *pData = pMsg->pData;
	pPb_ctrl->logic_chn     = GetByte(pData);
	pPb_ctrl->control       = GetByte(pData);
	pPb_ctrl->multiple      = GetByte(pData);
	
	GetBCD(pPb_ctrl->pb_position, pData, 6);

	DBG_LEVEL_6("logic_chn:%02x control:%02x multiple:%02x", 
			pPb_ctrl->logic_chn,pPb_ctrl->control, pPb_ctrl->multiple);

	return 0;
}

int CJT808Parse::VideoUploadFile(NetRcvMsg* pMsg,	VideoUpFile *pInfo)
{
    char *pData = pMsg->pData;
	pInfo->addr_len         = GetByte(pData);
	GetString(pInfo->addr, pData, pInfo->addr_len);
	pInfo->ftp_port         = GetWord(pData);
	pInfo->name_len         = GetByte(pData);
	GetString(pInfo->user_name, pData, pInfo->name_len);
	pInfo->pwd_len          = GetByte(pData);
	GetString(pInfo->user_pwd, pData, pInfo->pwd_len);
	pInfo->path_len         = GetByte(pData);
	GetString(pInfo->path, pData, pInfo->path_len);
	pInfo->logic_chn        = GetByte(pData);
	GetBCD(pInfo->start_time, pData, 6);
	GetBCD(pInfo->end_time, pData, 6);
	pInfo->alarm_flag1      = GetDWord(pData);
	pInfo->alarm_flag2      = GetDWord(pData);
	pInfo->type             = GetByte(pData);
	pInfo->stream_type      = GetByte(pData);
	pInfo->store_type       = GetByte(pData);
	pInfo->task             = GetByte(pData);

	DBG_LEVEL_6("len:%02x addr:%s port:%02x len:%02x name:%s pwd_len:%02x pwd:%s len:%02x path:%s chn:%02x type:%02x stream:%02x store:%02x task:%02x", 
			pInfo->addr_len, pInfo->addr, pInfo->ftp_port,
			pInfo->name_len, pInfo->user_name, pInfo->pwd_len,
			pInfo->user_pwd, pInfo->path_len,
			pInfo->path, pInfo->logic_chn, pInfo->type,
			pInfo->stream_type, pInfo->store_type ,pInfo->task);

	return 0;
}

int CJT808Parse::VideoCtrlUpload(NetRcvMsg* pMsg,	VideoUpFileCtrl *pInfo)
{
	char *pData = pMsg->pData;
	pInfo->serial 		= GetByte(pData);
	pInfo->ctrl 		= GetByte(pData);
	
	DBG_LEVEL_6("serial:%02x ctrl:%02x ", pInfo->serial,pInfo->ctrl);

	return 0;
}

int CJT808Parse::TextInfoTrans(NetRcvMsg* pMsg,	TextInfo *pInfo)
{
	char *pData = pMsg->pData;
	pInfo->flag 		= GetByte(pData);
	GetString(pInfo->text, pData, pMsg->wMsgLen - 1);

	DBG_LEVEL_6("flag:%02x text:%s ", pInfo->flag, pInfo->text);

	return 0;
}


