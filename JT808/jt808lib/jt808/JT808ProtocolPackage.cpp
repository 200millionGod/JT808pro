#include <netinet/in.h>
#include "JT808ProtocolPackage.h"
#include "JT808ParamPackage.h"
#include "debug_log.h"


static char bcd2ascii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static char ascii2bcd1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
static char ascii2bcd2[6]  = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
static int ASC2BCD(char *bcd, char *asc, uint32 len)
{
    uint8 i = 0;
    char c = 0;
    char index = 0;
	char tmp[12] = {};
	len >>= 1;

	if((len % 2) != 0)
	{
		for(i = 0; i < len; i++)
		{
			//first BCD
	        if(*asc >= 'A' && *asc <= 'F')
			{
	            index = *asc - 'A'; 
	            c = ascii2bcd2[index] << 4;
	        }else if(*asc >= '0' && *asc <= '9')
			{
	            index = *asc - '0';
	            c = ascii2bcd1[index] << 4;
	        }
	        asc++;
			printf("c1|%02x|\n", c);
	        //second BCD
	        if(*asc >= 'A' && *asc <= 'F')
			{
	            index = *asc - 'A'; 
	            c |= ascii2bcd2[index];
	        }else if(*asc >= '0' && *asc <= '9')
			{
	            index = *asc - '0';
	            c |= ascii2bcd1[index];
	        }
	        asc++;
			printf("c2|%02x|\n", c);
	        *bcd++ = c;
		}
	}else
	{
		for(i = 0; i < len; i++)
		{
			//first BCD
			if(*asc >= 'A' && *asc <= 'F')
			{
            	index = *asc - 'A'; 
            	tmp[i] |= ascii2bcd2[index];
        	}else if(*asc >= '0' && *asc <= '9')
			{
            	index = *asc - '0';
            	tmp[i] |= ascii2bcd1[index];
        	}
			asc++;
			//second BCD
	        if(*asc >= 'A' && *asc <= 'F')
			{
	            index = *asc - 'A'; 
	            tmp[i + 1] |= ascii2bcd2[index] << 4;
	        }else if(*asc >= '0' && *asc <= '9')
			{
	            index = *asc - '0';
	            tmp[i + 1] |= ascii2bcd1[index] << 4;
	        }
	        asc++;
		}
		memcpy(bcd, tmp, 10);
	}

    return 0;
}

DWORD CJT808Package::GetADASParas(Adas_Attr* pParam,char* value)
{
	BYTE off = 0;
	memset(value, 0, 128);
	
	off += PackageByte(value+off, pParam->AlarmThres);
	off += PackageByte(value+off, pParam->AlarmVolume);
	off += PackageByte(value+off, pParam->ActiveSnap);
	off += PackageWord(value+off, pParam->TimeThres);
	off += PackageWord(value+off, pParam->MileageThres);
	off += PackageByte(value+off, pParam->SingleSnapNum);
	off += PackageByte(value+off, pParam->SingleSnapInter);
	off += PackageByte(value+off, pParam->PicResolution);
	off += PackageByte(value+off, pParam->VedioResolution);
	off += PackageDword(value+off, pParam->AlarmEnble);
	off += PackageDword(value+off, pParam->EventEnable);
	off += PackageByte(value+off, pParam->Reserve);
	off += PackageByte(value+off, pParam->OWDistanceThres);
	off += PackageByte(value+off, pParam->OWSpeedThres);
	off += PackageByte(value+off, pParam->OWRecordingTime);
	off += PackageByte(value+off, pParam->FDWAlarmTime);
	off += PackageByte(value+off, pParam->FDWTimes);
	off += PackageByte(value+off, pParam->FDWSpeedThres);
	off += PackageByte(value+off, pParam->FDWRecordingTime);
	off += PackageByte(value+off, pParam->FDWSnapNum);
	off += PackageByte(value+off, pParam->FDWSnapInterval);
	off += PackageByte(value+off, pParam->LDWSpeedThres);
	off += PackageByte(value+off, pParam->LDWRecordingTime);
	off += PackageByte(value+off, pParam->LDWSnapNum);
	off += PackageByte(value+off, pParam->LDWSnapInterval);
	off += PackageByte(value+off, pParam->FCWAlarmThres);
	off += PackageByte(value+off, pParam->FCWSpeedThres);
	off += PackageByte(value+off, pParam->FCWRecordingTime);
	off += PackageByte(value+off, pParam->FCWSnapNum);
	off += PackageByte(value+off, pParam->FCWSnapInterval);
	off += PackageByte(value+off, pParam->PCWAlarmThres);
	off += PackageByte(value+off, pParam->PCWSpeedThres);
	off += PackageByte(value+off, pParam->PCWRecordingTime);
	off += PackageByte(value+off, pParam->PCWSnapNum);
	off += PackageByte(value+off, pParam->AlarmThres);
	off += PackageByte(value+off, pParam->PCWSnapInterval);
	off += PackageByte(value+off, pParam->DMWDistanceThres);
	off += PackageByte(value+off, pParam->DMWSpeedThres);
	off += PackageByte(value+off, pParam->DMWRecordingTime);
	off += PackageByte(value+off, pParam->DMWSnapNum);
	off += PackageByte(value+off, pParam->DMWSnapInterval);
	off += PackageByte(value+off, pParam->RSRSnapNum);
	off += PackageByte(value+off, pParam->RSRSnapInterval);
	off += PackageString(value+off, (char *)pParam->inter, 4);

	return off;
}

DWORD CJT808Package::GetDSMParas(Dsm_Attr* pParam,char* value)
{
	BYTE off = 0;
	memset(value, 0, 128);
	
	off += PackageByte(value+off, pParam->AlarmThres);
	off += PackageByte(value+off, pParam->AlarmVolume);
	off += PackageByte(value+off, pParam->ActiveSnap);
	off += PackageWord(value+off, pParam->TimeThres);
	off += PackageWord(value+off, pParam->MileageThres);
	off += PackageByte(value+off, pParam->SingleSnapNum);
	off += PackageByte(value+off, pParam->SingleSnapInter);
	off += PackageByte(value+off, pParam->PicResolution);
	off += PackageByte(value+off, pParam->VedioResolution);
	off += PackageDword(value+off, pParam->AlarmEnble);
	off += PackageDword(value+off, pParam->EventEnable);
	off += PackageWord(value+off, pParam->SmokeInter);
	off += PackageWord(value+off, pParam->PhoneInter);
	off += PackageString(value+off, (char *)pParam->inter, 3);
	off += PackageByte(value+off, pParam->FDWSpeedThres);
	off += PackageByte(value+off, pParam->FDWRecordingTime);
	off += PackageByte(value+off, pParam->FDWSnapNum);
	off += PackageByte(value+off, pParam->FDWSnapInter);
	off += PackageByte(value+off, pParam->PhoneSpeedThres);
	off += PackageByte(value+off, pParam->PhoneRecordingTime);
	off += PackageByte(value+off, pParam->PhoneSnapNum);
	off += PackageByte(value+off, pParam->PhoneSnapInter);
	off += PackageByte(value+off, pParam->SmokeSpeedThres);
	off += PackageByte(value+off, pParam->SmokeRecordingTime);
	off += PackageByte(value+off, pParam->SmokeSnapNum);
	off += PackageByte(value+off, pParam->SmokeSnapInter);
	off += PackageByte(value+off, pParam->DDWSpeedThres);
	off += PackageByte(value+off, pParam->DDWRecordingTime);
	off += PackageByte(value+off, pParam->DDWSnapNum);
	off += PackageByte(value+off, pParam->DDWSnapInter);
	off += PackageByte(value+off, pParam->ADWSpeedThres);
	off += PackageByte(value+off, pParam->ADWRecordingTime);
	off += PackageByte(value+off, pParam->ADWSnapNum);
	off += PackageByte(value+off, pParam->ADWSnapInter);
	off += PackageByte(value+off, pParam->TriggerAuth);
	off += PackageString(value+off, (char *)pParam->inter1, 3);

	return off;
}

/*************************************************************************
*Function:GetAiViAttr
*Description:获取音视频参数
*Params in:
*Params out:
*Result:
*author:huangshimei
*date:20190620
**************************************************************************/

DWORD CJT808Package::GetAiViAttr(JT1078_Audio_video_conf_set* pParam,char* value)
{
	BYTE off = 0;
	memset(value, 0, 128);
	
	off += PackageByte(value+off, pParam->coding_mode);
	off += PackageByte(value+off, pParam->resolution);
	off += PackageWord(value+off, pParam->key_frame_inter);
	off += PackageByte(value+off, pParam->frame_rate);
	off += PackageDword(value+off, pParam->bit_rate);
	off += PackageByte(value+off, pParam->store_coding_mode);
	off += PackageByte(value+off, pParam->store_resolution);
	off += PackageWord(value+off, pParam->store_key_frame_inter);
	off += PackageByte(value+off, pParam->store_frame_rate);
	off += PackageDword(value+off, pParam->store_bit_rate);
	off += PackageWord(value+off, pParam->osd_set);
	off += PackageByte(value+off, pParam->enable_audio);

	return off;
}

/*************************************************************************
*Function:GetAiViChn
*Description:获取音视频通道列表
*Params in:
*Params out:
*Result:
*author:huangshimei
*date:20190620
**************************************************************************/
DWORD CJT808Package::GetAiViChn(JT1078_Audio_video_chn_set* pParam,char* value)
{
	BYTE i, j, off = 0;
	memset(value, 0, 128);
	
	off += PackageByte(value+off, pParam->chn_total_num);
	off += PackageByte(value+off, pParam->audio_num);
	off += PackageByte(value+off, pParam->video_num);
	

	j = pParam->chn_total_num + pParam->audio_num + pParam->video_num;
	for (i = 0; i < j ; i++)
	{
		off += PackageByte(value+off, pParam->chn_table[i].physical_chn);
		off += PackageByte(value+off, pParam->chn_table[i].logical_chn);
		off += PackageByte(value+off, pParam->chn_table[i].chn_type);
		off += PackageByte(value+off, pParam->chn_table[i].connect_ptz);

	}

	return off;
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
DWORD CJT808Package::GetSingleChn(JT1078_single_chn_control* pParam,char* value)
{
	BYTE i, off = 0;
	memset(value, 0, 128);
	
	off += PackageByte(value+off, pParam->chn_num);

	for (int i = 0; i < pParam->chn_num; i++)
	{
		off += PackageByte(value+off, pParam->chn_video_param[i].logical_chn);
		off += PackageByte(value+off, pParam->chn_video_param[i].coding_mod);
		off += PackageByte(value+off, pParam->chn_video_param[i].resolution);
		off += PackageWord(value+off, pParam->chn_video_param[i].key_frame_inter);
		off += PackageByte(value+off, pParam->chn_video_param[i].frame_rate);
		off += PackageDword(value+off, pParam->chn_video_param[i].bit_rate);
		off += PackageByte(value+off, pParam->chn_video_param[i].store_coding_mode);
		off += PackageByte(value+off, pParam->chn_video_param[i].store_resolution);
		off += PackageWord(value+off, pParam->chn_video_param[i].store_key_frame_inter);
		off += PackageByte(value+off, pParam->chn_video_param[i].store_frame_rate);
		off += PackageDword(value+off, pParam->chn_video_param[i].store_bit_rate);
		off += PackageByte(value+off, pParam->chn_video_param[i].osd_set);
	}

	return off;
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
DWORD CJT808Package::GetSpecialChn(JT1078_special_alarm_param_set* pParam,char* value)
{
	BYTE off = 0;
	memset(value, 0, 128);
	
	off += PackageByte(value+off, pParam->video_storage_thres);
	off += PackageByte(value+off, pParam->video_duration);
	off += PackageByte(value+off, pParam->start_time);

	return off;
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
DWORD CJT808Package::GetVideoAlarmShield(JT1078_alarm_shield* pParam,char* value)
{
	BYTE off = 0;
	memset(value, 0, 128);
	
	off += PackageDword(value+off, pParam->shield);

	return off;
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
DWORD CJT808Package::GetVideoAlarmAttr(JT1078_alarm_video_param* pParam,char* value)
{
	BYTE off = 0;
	memset(value, 0, 128);
	
	off += PackageByte(value+off, pParam->num_of_carries);
	off += PackageByte(value+off, pParam->fatigue_thres);

	return off;	
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
DWORD CJT808Package::GetAwakeAttr(JT1078_awake_param* pParam,char* value)
{
	BYTE off = 0;
	memset(value, 0, 128);
	off += PackageByte(value+off, pParam->awake_mode);
	off += PackageByte(value+off, pParam->awake_type);
	off += PackageByte(value+off, pParam->set_awake_time);
	off += PackageByte(value+off, pParam->awake_params.flag);
	off += PackageBytes(value+off, pParam->awake_params.period1_start_time, 2);
	off += PackageBytes(value+off, pParam->awake_params.period1_close_time, 2);
	off += PackageBytes(value+off, pParam->awake_params.period2_start_time, 2);
	off += PackageBytes(value+off, pParam->awake_params.period2_close_time, 2);
	off += PackageBytes(value+off, pParam->awake_params.period3_start_time, 2);
	off += PackageBytes(value+off, pParam->awake_params.period3_close_time, 2);
	off += PackageBytes(value+off, pParam->awake_params.period4_start_time, 2);
	off += PackageBytes(value+off, pParam->awake_params.period4_close_time, 2);
	
	return off;	
}


void CJT808Package::Init(InterFace * pInterFace)
{
	m_pInterFace = pInterFace;
	memset(&m_alarmData,0,sizeof(AlarmData_s));
	//m_alarmData.alarm |= 0x00010020;
	//m_alarmData.status |= 0x800c00c1;
	//m_alarmData.status |= 0x3;
	
	if(m_pInterFace != NULL){
		unsigned char szPhone[20] = {0};
		m_pInterFace->GetPhoneNumber(szPhone);
//		printf("szPhone:%s\n", szPhone);
		ASC2BCD((char*)m_bcdPhoneNo, (char *)&szPhone[0], 16);
//		printf("mmm_bcdPhoneNo:");
//		for(int i = 0; i < 10; i++)
//		{
//			printf("%02x ", m_bcdPhoneNo[i]);
//		}
//		printf("\n");
 	}

	m_nSerialNumber = 0;
}

//设备通用应答
DWORD CJT808Package::CommReply(char *szData, DWORD &dataLen, WORD wSerialNumber, WORD wId, BYTE bResult)
{
	char strData[24] = {0};
	char* pData = strData + 17;
	/*fill serialnumber*/
//	PackageWord(strData + 12, wSerialNumber);
	printf("In CommReply,wSerialNumber = %d\n", wSerialNumber);
	PackageWord(pData, wSerialNumber);
	/*fill command type*/
//	PackageWord(strData + 14, wId);
	printf("In CommReply,wId = %d\n", wId);
	PackageWord(pData + 2, wId);
	/*fill anser result*/
//	PackageByte(strData + 16, bResult);
	printf("In CommReply,bResult = %d\n", bResult);
	PackageByte(pData + 4, bResult);
	/* 消息封装 */
	WORD wSerialNum = Package(NET_CMD_REPLY, strData, 5);
	dataLen = TransmitToCopy(szData, strData, 5 + 18);
	return wSerialNum;
}

//设备心跳
DWORD CJT808Package::HeartBeat(char *szData, DWORD &dataLen)
{
	char strData[24] = {0};

	/* 消息封装 */
	WORD wSerialNum = Package(NET_CMD_HEARTBEAT, strData, 0);
	dataLen = TransmitToCopy(szData, strData, 18);
	return wSerialNum;
}

//注册

//注册
#if 0
DWORD CJT808Package::Register(char *szData, DWORD &dataLen)
{
	WORD off;
	char strData[64];
	char* pData = strData + 12;
	Vehicle_Info vehicleInfo;
	Terminal_Attr termAttr;
	m_pInterFace->GetVehicleInfo(&vehicleInfo);
	m_pInterFace->GetTerminalAttr(&termAttr);
	/* 更新手机号码 */
	//m_pInterFace->GetPhoneNumber(m_bcdPhoneNo);
	// 省域id
	PackageWord(pData, vehicleInfo.provinceid);
	// 市县域id
	PackageWord(pData + 2, vehicleInfo.cityid);
	// 制造商id
	PackageBytes(pData + 4, termAttr.manufacturerID, 5);
	// 终端类型
	PackageBytes(pData + 9, termAttr.devType, 20);
	//终端ID
	PackageBytes(pData + 29, termAttr.devID, 7);
	//车牌颜色
	PackageByte(pData + 36, vehicleInfo.platecolor);
	//车牌
	off = PackageString(pData + 37, vehicleInfo.license, 16);

	/* 消息封装 */
	WORD wSerialNum = Package(NET_CMD_REGISTER, strData, off + 37);
	dataLen = TransmitToCopy(szData, strData, off + 37 + 13);
	return wSerialNum;
}
#endif

#if 1
DWORD CJT808Package::Register(char *szData, DWORD &dataLen)
{
	WORD off;
	char strData[200];
	char* pData = strData + 17;
	Vehicle_Info vehicleInfo;
	Terminal_Attr termAttr;
	m_pInterFace->GetVehicleInfo(&vehicleInfo);
	m_pInterFace->GetTerminalAttr(&termAttr);
	/* 更新手机号码 */
//	m_pInterFace->GetPhoneNumber(m_bcdPhoneNo);
	// 省域id
	PackageWord(pData, vehicleInfo.provinceid);
	printf("IN Register,vehicleInfo.provinceid = %d\n", vehicleInfo.provinceid);
	// 市县域id
	PackageWord(pData + 2, vehicleInfo.cityid);
	printf("IN Register,vehicleInfo.cityid = %d\n", vehicleInfo.cityid);
	// 制造商id
//	PackageBytes(pData + 4, termAttr.manufacturerID, 11);
	PackageBytes(pData + 5, termAttr.manufacturerID, 10);
	printf("IN Register,termAttr.manufacturerID = %s\n", termAttr.manufacturerID);
	// 终端类型
//	PackageBytes(pData + 15, termAttr.devType, 30);
	PackageBytes(pData + 40, termAttr.devType, 5);
	printf("IN Register,termAttr.devType = %s\n", termAttr.devType);
	//终端ID
	PackageBytes(pData + 45, termAttr.devID, 30);
//	PackageBytes(pData + 59, termAttr.devID, 16);
	printf("IN Register,termAttr.devID = %s\n", termAttr.devID);
	//车牌颜色
	PackageByte(pData + 75, vehicleInfo.platecolor);
	//车牌
	off = PackageString(pData + 76, vehicleInfo.license, 16);
	printf("IN Register,vehicleInfo.license = |%s|\n", vehicleInfo.license);
	printf("IN Register,off = %d\n", off);
	/* 消息封装 */
	WORD wSerialNum = 0;
	wSerialNum = Package(NET_CMD_REGISTER, strData, off + 76);
	dataLen = TransmitToCopy(szData, strData, off + 76 + 18);
	printf("IN Register,dataLen = %d\n", dataLen);
	return wSerialNum;
}
#endif
//注销
WORD CJT808Package::UnRegister(char *szData, DWORD &dataLen)
{
	char strData[32];

	/* 消息封装 */
	WORD wSerialNum = Package(NET_CMD_UNREGISTER, strData, 0);
	dataLen = TransmitToCopy(szData, strData, 18);
	return wSerialNum;
}

//登陆
WORD CJT808Package::Login(char *szData, DWORD &dataLen, char* pAuthCode)
{
	int authCodeLen = strlen(pAuthCode);
	char strData[128];
	char* pData = strData + 17;
	Terminal_Attr termAttr;
	memset(&termAttr, 0, sizeof(termAttr));
	m_pInterFace->GetTerminalAttr(&termAttr);
	printf("In Login,authCodeLen = %d\n", authCodeLen);
	//鉴权码长度
	PackageByte(pData, (BYTE)authCodeLen);
	//鉴权码内容
	PackageString(pData + 1, pAuthCode, authCodeLen);
	//终端IMEI
	printf("In Login,termAttr.devIMEI:");
	for(int index = 0; index < 15; index ++)
	{
		printf("%x ", termAttr.devIMEI[index]);
	}printf(".\n");
	printf("In Login,termAttr.devIMEI:%s.\n", termAttr.devIMEI);
	PackageBytes(pData + 1 + authCodeLen, termAttr.devIMEI, 15);
	//软件版本号
	PackageBytes(pData + 1 + authCodeLen + 15, termAttr.softVer, 20);
#if 0
	/* 更新手机号码 */
	m_pInterFace->GetPhoneNumber(m_bcdPhoneNo);
	off = PackageString(szData+12,pAuthCode,64);
	wSerialNum = Package(NET_CMD_UNREGISTER,szData,0);
	Send(NET_CMD_UNREGISTER,wSerialNum,szData,off+13,false,0,0);
#else
	/* 消息封装 */
	WORD wSerialNum = Package(NET_CMD_AUTHORIZE, strData, 1 + authCodeLen + 15 + 20);
	dataLen = TransmitToCopy(szData, strData, 1 + authCodeLen + 15 + 20 + 18);
	return wSerialNum;
#endif
}

//查询设备参数应答
WORD CJT808Package::ReplyParam(vector<PackageSendMsg>& vecPackageMsg, DWORD count,DWORD* pId,WORD wSerialNumber)
{
	char szData[2048];
	BYTE total = 0, len = 0;
	char* pData = szData;
	CJT808ParamBase parambase;
	JT808_Config_S *sysCfg = new JT808_Config_S;
	m_pInterFace->GetSysCfg(sysCfg);
	parambase.Init(sysCfg);
	pData += 3;

	if (count == 0){
		count = 0x0111;
	}
	
	for(DWORD i=0;i<count;i++)
	{
		switch(pId[i])
		{
			case 0xF364:
				len = GetADASParas(&sysCfg->AdasCfg, (char *)sysCfg->cfg_store);
				pData[4] = len;
				break;
			case 0xF365:
				len = GetDSMParas(&sysCfg->DsmCfg, (char *)sysCfg->cfg_store);
				pData[4] = len;
				break;
			case 0x0075:
				len = GetAiViAttr(&sysCfg->VideoCfg.ai_vi_attr, (char *)sysCfg->cfg_store);
				pData[4] = len;
				break;
			case 0x0076:
				len = GetAiViChn(&sysCfg->VideoCfg.ai_vi_chn, (char *)sysCfg->cfg_store);
				pData[4] = len;
				break;
			case 0x0077:
				len = GetSingleChn(&sysCfg->VideoCfg.single_chn_ctl, (char *)sysCfg->cfg_store);
				pData[4] = len;
				break;
			case 0x0079:
				len = GetSpecialChn(&sysCfg->VideoCfg.special_alarm, (char *)sysCfg->cfg_store);
				pData[4] = len;
				break;
			case 0x007A:
				len = GetVideoAlarmShield(&sysCfg->VideoCfg.alarm_shield, (char *)sysCfg->cfg_store);
				pData[4] = len;
				break;
			case 0x007B:
				len = GetVideoAlarmAttr(&sysCfg->VideoCfg.video_alarm, (char *)sysCfg->cfg_store);
				pData[4] = len;
				break;
			case 0x007C:
				len = GetAwakeAttr(&sysCfg->VideoCfg.awake_attr, (char *)sysCfg->cfg_store);
				pData[4] = len;
				break;

			default:
				break;
		}
		if(parambase.Set(pData,pId[i])==0)
		{
		 	total += 1;
		}
	}
	
	//填充应答流水号
	PackageWord(szData,wSerialNumber);
	//参数总数
	PackageByte(szData+2,total);

	PackageToSend(NET_CMD_CONFIG_GET_REPLY, vecPackageMsg, szData, pData-szData, NULL, 0, false);

	if (sysCfg)
	{
		delete sysCfg;
	}

	return 0;
}

//查询设备属性应答
WORD CJT808Package::ReplyPoperty(char *szData, DWORD &dataLen)
{
	char strData[128] = {0};
	char* pData = strData + GNSS_NET_PROC_MSG_HEAD_LEN;
	Terminal_Attr TermInfo;
	m_pInterFace->GetTerminalAttr(&TermInfo);

	/* 终端类型 */
	PackageWord(pData,TermInfo.supportMask);
	/* 制造商 */
	PackageBytes(pData + 2, TermInfo.manufacturerID, 5);
	/* 终端型号 */
	PackageBytes(pData + 7, TermInfo.devType, 20);
	/* 终端ID */
	PackageBytes(pData + 27, TermInfo.devID, 7);
	/* SIM卡ICCID */
	PackageBytes(pData + 34, TermInfo.simICCID, 10);
	/* 硬件版本号 */
	int n = PackageString(pData + 45, TermInfo.hardVer, 20);
	PackageByte(pData + 44, n);
	/* 终端固件版本号 */
	int m = PackageString(pData + 46 + n, TermInfo.hardVer, 20);
	PackageByte(pData + 45 + n, m);
	/* GNSS模块属性 */
	PackageByte(pData + 46 + n + m, TermInfo.gnss);
	/* 通讯模块属性 */
	PackageByte(pData + 47 + n + m, TermInfo.wireless);
	
	WORD wSerialNum = Package(NET_CMD_DEV_PROPERTY_REPLY, strData, 48 + n + m);
	dataLen = TransmitToCopy(szData, strData, 48 + n + m + 13);
	return wSerialNum;
}

//升级结果通知
WORD CJT808Package::ReportUpgradeResult(char *szData, DWORD &dataLen, BYTE bType, BYTE bResult)
{
	char strData[32] = {0};
	WORD wSerialNum = 0;

	/* 升级类型 */
	PackageByte(strData + 12,bType);
	/* 升级结果 */
	PackageByte(strData + 13,bResult);

	wSerialNum = Package(NET_CMD_SOFTWARE_UPGRADE_REPLY, szData, 2);
	dataLen = TransmitToCopy(szData, strData, 15);
	return wSerialNum;
}

//汇报位置信息
WORD CJT808Package::ReportPosition(char *szData, DWORD &dataLen, void* pData)
{
	char strData[256] = {0};
	WORD off;
	DWORD devStatus;
	devStatus = m_alarmData.status;
	m_alarmData.extend_type = DEV_NONE;
	m_alarmData.alarm = 0;
	off = PosStautsInfo(strData + GNSS_NET_PROC_MSG_HEAD_LEN, pData, &m_alarmData, devStatus);

	WORD wSerialNum = Package(NET_CMD_POS_INFO_UP, strData, off);
	dataLen = TransmitToCopy(szData, strData, off + 18);	/* 消息头长度+1个字节校验值 */
	return wSerialNum;
}
#if 1
//汇报保存的GPS位置信息
WORD CJT808Package::ReportSaveGpsPosition(char *szData, DWORD &dataLen, void* pData,int bodyLen,int packNum)
{
	#if 1
	char strData[1024] = {0};
	WORD off;
	off = (WORD)bodyLen+3;
	
	/* 数据项个数 */
	PackageWord(strData + 17,packNum);
	/* 位置数据类型 0：正常位置批量汇报 1：盲区补报*/
	PackageByte(strData + 19,1);
	
	memcpy(strData+(GNSS_NET_PROC_MSG_HEAD_LEN+3),pData,bodyLen);	/* 3:表67 定位数据批量上传数据格式 前3个字节*/

	WORD wSerialNum = Package(NET_LOCATION_DATA_BATCH_UPLOAD, strData, off);
	dataLen = TransmitToCopy(szData, strData, off + 18);	/* 消息头长度+1个字节校验值 */
	return wSerialNum;
	#endif
}
#endif

//汇报报警信息
WORD CJT808Package::ReportAlarm(char *szData, DWORD &dataLen, void* pData)
{
	char strData[1024] = {0};
	memcpy(&m_alarmData, pData, sizeof(AlarmData_s));
	DBG_LEVEL_6("alarm type:%d dev:%04x type :%02x", m_alarmData.alarm, m_alarmData.extend_type, 
		m_alarmData.dsm_alarm.type);

	struGPSData* pGnssData = &(m_alarmData.gnss);
	DWORD devStatus = m_alarmData.status;
	if (m_alarmData.extend_type == DEV_DSM){
		m_alarmData.alarm = m_alarmData.dsm_alarm.type;
	}else if (m_alarmData.extend_type == DEV_ADAS){
		m_alarmData.alarm = m_alarmData.dsm_alarm.type;
	}else{
	}

	WORD off = PosStautsInfo(strData + 12, pGnssData, pData, devStatus);

	WORD wSerialNum = Package(NET_CMD_POS_INFO_UP, strData, off);
	dataLen = TransmitToCopy(szData, strData, off + 13);
	return wSerialNum;
}

//汇报设备状态
WORD CJT808Package::ReportDevStatus(char *szData, DWORD &dataLen, DWORD value)
{
	char strData[512] = {0};
	DWORD devStatus = value;
	struGPSData gnssData;
	m_alarmData.status = (int)value;
	m_pInterFace->GetGnssData(&gnssData);
	WORD off = PosStautsInfo(strData + 12, &gnssData, &m_alarmData, devStatus);

	WORD wSerialNum = Package(NET_CMD_POS_INFO_UP, strData, off);
	dataLen = TransmitToCopy(szData, strData, off + 13);
	return wSerialNum;
}

//汇报设备事件
WORD CJT808Package::ReportEvent(char *szData, DWORD &dataLen, BYTE eid)
{
	char strData[16] = {0};
	PackageByte(strData + 12,eid);

	WORD wSerialNum = Package(NET_CMD_EVENT_UP, strData, 1);
	dataLen = TransmitToCopy(szData, strData, 14);
	return wSerialNum;
}

//查询位置信息应答
WORD CJT808Package::ReplyPosition(char *szData, DWORD &dataLen, WORD wSerialNumber, struGPSData& gnssData)
{
	char strData[512] = {0};
	DWORD devStatus = m_alarmData.status;
	WORD off = PackageWord(strData + 17, wSerialNumber);
	m_alarmData.alarm = 0;
	off += PosStautsInfo(strData + 19, &gnssData, &m_alarmData, devStatus);

	WORD wSerialNum = Package(NET_CMD_POS_INFO_GET_REPLY, strData, off);
	dataLen = TransmitToCopy(szData, strData, off + 18);
	return wSerialNum;
}

//提问应答
WORD CJT808Package::ReplyAsk(char *szData, DWORD &dataLen, WORD wSerialNumber, BYTE bResult)
{
	char strData[16] = {0};

	/*fill serialnumber*/
	PackageWord(strData + 12, wSerialNumber);
	/*fill anser result*/
	PackageByte(strData + 14, bResult);

	WORD wSerialNum = Package(NET_CMD_ASK_REPLY, szData, 3);
	dataLen = TransmitToCopy(szData, strData, 16);
	return wSerialNum;
}

//信息点播/取消
WORD CJT808Package::OrderMessage(char *szData, DWORD &dataLen, BYTE bType, BYTE bFlag)
{
	char strData[16] = {0};
	
	/* 信息类型 */
	PackageByte(strData,bType);
	/* 点播/取消标志 */
	PackageByte(strData + 1,bFlag);

	WORD wSerialNum = Package(NET_CMD_INFO_OD_CANCEL, strData, 2);
	dataLen = TransmitToCopy(szData, strData, 15);
	return wSerialNum;
}

//车辆控制应答
WORD CJT808Package::ReplyControl(char *szData, DWORD &dataLen, WORD wSerialNumber)
{
	char strData[512] = {0};
	struGPSData gnssData;
	
	m_pInterFace->GetGnssData(&gnssData);
	int devStatus = m_alarmData.status;
	PackageWord(strData + 12, wSerialNumber);
	int off = PosStautsInfo(strData + 14, &gnssData, &m_alarmData, devStatus);

	/* 消息体封装 */
	WORD wSerialNum = Package(NET_CMD_VEHICLE_CTRL_REPLY, strData, off);
	dataLen = TransmitToCopy(szData, strData, off + 13);
	return wSerialNum;
}

//行驶记录数据上传
int CJT808Package::UploadDrivingData(void* pBuffer,WORD wSerialNumber)
{
	return 0;
}

//电子运单上报
void CJT808Package::UploadElectBill(vector<PackageSendMsg>& vecPackageMsg, void* pData,DWORD len)
{
	char szHead[4];
	/* 长度 */
	PackageDword(szHead, len);
	PackageToSend(NET_CMD_ELECT_BILL_UP, vecPackageMsg, pData, len, szHead, 4);
}

//驾驶员身份信息上报
WORD CJT808Package::ReportDriverInfo(char *szData, DWORD &dataLen)
{
	char strData[512] = {0};
	BYTE m,n;
	WORD off = 12;
	IC_Read_Info icInfo;
	Driver_Info info;
	
	m_pInterFace->GetDriverInfo(&icInfo, &info);
	/* 状态 */
	off += PackageByte(strData + off, icInfo.status);
	/* 时间 */
	off += PackageBytes(strData + off, icInfo.time, 6);
	if (icInfo.status == 0x01) {
		/* IC卡读取结果 */
		off += PackageByte(strData + off, icInfo.result);
		if (icInfo.result == 0x00) {
			/* 驾驶员姓名 */
			n = PackageString(strData + off + 1, info.name, 20);
			/* 驾驶员姓名长度 */
			off += PackageByte(strData + off, n);
			off += n;
			/* 从业资格证编码 */
			off += PackageBytes(strData + off, (BYTE*)info.qc, 20);
			/* 发证机构名称 */
			m = PackageString(strData + off + 1, info.issuingAuthority, 40);
			/* 发证机构名称长度 */
			off += PackageByte(strData + off, m);
			off += m;
			/* 证件有效期 */
			off += PackageBytes(strData + off, (BYTE*)info.date, 4);
		}
	}
	
	WORD wSerialNum = Package(NET_CMD_DRIVER_IDINFO_UP, strData, off);
	dataLen = TransmitToCopy(szData, strData, off + 13);
	return wSerialNum;
}

//定位数据批量上传
int CJT808Package::UploadBatchLocation(void* pBuffer)
{
	return 0;
}

//CAN总线数据上传
int CJT808Package::UploadCANData(void* pBuffer)
{
	return 0;
}

//多媒体事件信息上传
WORD CJT808Package::UploadMutiMediaEvent(char *szData, DWORD &dataLen, DWORD id, BYTE type, BYTE encode, BYTE event, BYTE chn)
{
	char strData[32];

	/* 多媒体数据id */
	PackageDword(strData + 17, id);
	/* 多媒体类型 */
	PackageByte(strData + 21, type);
	/*  多媒体格式编码 */
	PackageByte(strData + 22, encode);
	/* 事件项编码 */
	PackageByte(strData + 23, event);
	/* 通道id */
	PackageByte(strData + 24, chn);
	
	WORD wSerialNum = Package(NET_CMD_MULTIMEDIA_EVENT_UP, strData, 8);
	dataLen = TransmitToCopy(szData, strData, 8 + 17 + 1);

	printf("In UploadMutiMediaEvent szData:");
	for(int i = 0; i < 28; i++)
	{
		printf("%02x ", szData[i]);
	}
	printf("\n");
	
	return wSerialNum;
}

//多媒体数据上传
WORD CJT808Package::UploadMutiMediaData(char *szData, DWORD &dataLen, void* pBuffer, int nLen, short PackNum, short PackId, NetMdUploadCmd *pCmd)
{
	char *data = (char *)pBuffer + 16;
	int offset = 0;

	if(PackId == 1)
	{
		offset = 36;
		PackageDword(data, pCmd->mId);
		PackageByte(data + 4, pCmd->type);
		PackageByte(data + 5, pCmd->enctype);
		PackageByte(data + 6, pCmd->event);
		PackageByte(data + 7, pCmd->chn);
	}

	WORD wSerialNum = Package(NET_CMD_MUTIMEDIA_DATA_UP, (char *)pBuffer, nLen + offset, PackNum, PackId);
	dataLen = TransmitToCopy(szData, pBuffer, nLen + offset + 17);
	return wSerialNum;
}

WORD CJT808Package::UploadMutiMediaData(char *szData, DWORD &dataLen, void* pBuffer, int nLen, short PackNum, short PackId, NetMdUploadCmd *pCmd, struGPSData& gpsData, time_t &pts)
{
	char *data = (char *)pBuffer + 17;
	int offset = 0;
	WORD wSerialNum = 0;

	if(PackId == 1)
	{
		offset = 36;
		PackageDword(data, pCmd->mId);
		PackageByte(data + 4, pCmd->type);
		PackageByte(data + 5, pCmd->enctype);
		PackageByte(data + 6, pCmd->event);
		PackageByte(data + 7, pCmd->chn);

		char *pBuffer = data + 8;
		DWORD m,n;
		BYTE strData[8] = {0};
		/* 报警标志 */
		PackageDword(pBuffer, 0x00);
		/* 设备状态 */
		PackageDword(pBuffer + 4, 0x00);
		if(gpsData.cGpsStatus == 'A'){
			m = gpsData.fLatitude;
			n = gpsData.fLongitude;
		} else {
			m = 0;
			n = 0;
		}
		/* 纬度 */
		PackageDword(pBuffer + 8, m);
		/* 经度 */
		PackageDword(pBuffer + 12, n);
		/* 高程 */
		PackageWord(pBuffer + 16, 0);
		/* 速度 */
		PackageWord(pBuffer + 18, gpsData.usSpeed / 10);
		/* 方向 */
		PackageWord(pBuffer + 20, gpsData.usGpsAngle / 100);
		/* 时间 */
		//time_t curtime =  time(NULL);
		tm *ptm = localtime(&pts);
		strData[0] = (((ptm->tm_year-100)/10) << 4)+(ptm->tm_year%10);
		strData[1] = (((ptm->tm_mon+1)/10) << 4)+((ptm->tm_mon+1)%10);
		strData[2] = ((ptm->tm_mday/10) << 4)+(ptm->tm_mday%10);
		strData[3] = ((ptm->tm_hour/10) << 4)+(ptm->tm_hour%10);
		strData[4] = ((ptm->tm_min/10) << 4)+(ptm->tm_min%10);
		strData[5] = ((ptm->tm_sec/10) << 4)+(ptm->tm_sec%10);
		PackageBytes(pBuffer + 22, strData, 6);
	}
	printf("In UploadMutiMediaData, len[%d] offset[%d]\n", nLen, offset);
	wSerialNum = Package(NET_CMD_MUTIMEDIA_DATA_UP, (char *)pBuffer, nLen + offset, PackNum, PackId);
	dataLen = TransmitToCopy(szData, pBuffer, nLen + offset + 21 + 1);
	return wSerialNum;
}

//立即拍摄命令应答
WORD CJT808Package::ReplySnapShot(char *szData, DWORD &dataLen, WORD wSerialNumber, BYTE bResult, WORD num, DWORD* pDataIDs)
{
	char strData[128] = {0};
	char *data = strData + 17;

	/*fill serialnumber*/
	PackageWord(data, wSerialNumber);
	/*fill anser result*/
	PackageByte(data + 2, bResult);
	/*多媒体个数*/
	PackageWord(data + 3, num);
	/* 多媒体列表 */
	for (int i = 0; i < num; i++) {
		PackageDword(data + 5 + 4 * i, pDataIDs[i]);
	}
	
	DBG_LEVEL_6(": [%.2x] [%x] [%.2x] [%.8x] ", *(WORD *)data, *(data+2), *(WORD *)(data + 3), *(DWORD *)(data + 5));

	/* 消息体封装 */
	WORD wSerialNum = Package(NET_CMD_CAMERA_SHOT_REPLY, strData, 5 + 4 * num);
	dataLen = TransmitToCopy(szData, strData, 18 + 4 * num);
	return wSerialNum;
}

WORD CJT808Package::ReplySnapShot(char *szData, DWORD &dataLen, WORD wSerialNumber, vector<ShotResult>& vecResultList, BYTE nResult)
{
	WORD nSize = vecResultList.size();
	DBG_LEVEL_6("wSerialNumber[%.4x] nResult[%d] num[%d]", wSerialNumber, nResult, nSize);

	//char strData[nSize * 4 + 64] = {0};
	char strData[1024] = {0};
	char *data = strData + 17;

	/*fill serialnumber*/
	PackageWord(data, wSerialNumber);
	/*fill anser result*/
	PackageByte(data + 2, nResult);
	/*多媒体个数*/
	PackageWord(data + 3, vecResultList.size());
	/* 多媒体列表 */
	for(int i = 0; i < nSize; i++)
	{
		PackageDword(data + 5 + 4 * i, vecResultList[i].media_id);
	}
	DBG_LEVEL_6("wSerialNumber[%.2x] nResult[%x] vecResultList.size[%.2x] list[%.8x] ", *(WORD *)data, *(data+2), *(WORD *)(data + 3), *(DWORD *)(data + 5));

	/* 消息体封装 */
	WORD wSerialNum = Package(NET_CMD_CAMERA_SHOT_REPLY, strData, 5 + 4 * nSize);
	dataLen = TransmitToCopy(szData, strData, 18 + 4 * nSize + 5);
	return wSerialNum;
}

//存储多媒体数据检索应答
WORD CJT808Package::ReplyMutiMediaIndex(char *szData, DWORD &dataLen, WORD wSerialNumber, vector<IdxCmdRes>& vecResultList)
{
	WORD wSerialNum = 0;
	char strData[1024] = {0};
	char *data = strData + 17;

	WORD nSize = vecResultList.size();
	DBG_LEVEL_6("wSerialNumber[%.4x] num[%d]", wSerialNumber, nSize);

	/*fill serialnumber*/
	PackageWord(data, wSerialNumber);
	/*fill mediaIndxTotalNumber*/
	PackageWord(data + 2, nSize);
	for (int i = 0; i < nSize; i++)	//7为多媒体索引项数据长度，28为gps数据长度
	{
		PackageDword(data + 4 + ((7 + 28) * i), vecResultList[i].mediaId);
		PackageByte(data + 8 + ((7 + 28) * i), vecResultList[i].type);
		PackageByte(data + 9 + ((7 + 28) * i), vecResultList[i].chn);
		PackageByte(data + 10 + ((7 + 28) * i), vecResultList[i].event);
		/* 报警标志 */
		PackageDword(data + 11 + ((7 + 28) * i), 0x00);
		/* 设备状态 */
		PackageDword(data + 15 + ((7 + 28) * i), 0x00);
		DWORD m = 0, n = 0;
		BYTE strData[8] = {0};
		if(m == 0 && n == 0)/*gps数据可用*/
		{	
			m = 1;//gpsData.fLatitude;
			n = 1;//gpsData.fLongitude;
		}else
		{
			m = 0;
			n = 0;
		}
		/* 纬度 */
		PackageDword(data + 19 + ((7 + 28) * i), m);
		/* 经度 */
		PackageDword(data + 23 + ((7 + 28) * i), n);
		/* 高程 */
		PackageWord(data + 27 + ((7 + 28) * i), 0);
		/* 速度 */
		PackageWord(data + 29 + ((7 + 28) * i), /*gpsData.usSpeed*/m / 10);
		/* 方向 */
		PackageWord(data + 31 + ((7 + 28) * i), /*gpsData.usGpsAngle*/n / 100);
		//time_t curtime =  time(NULL);
		time_t pts;//time_t就是long int 类型
		pts = time(NULL);
		tm *ptm = localtime(&pts);
		strData[0] = (((ptm->tm_year-100)/10) << 4)+(ptm->tm_year%10);
		strData[1] = (((ptm->tm_mon+1)/10) << 4)+((ptm->tm_mon+1)%10);
		strData[2] = ((ptm->tm_mday/10) << 4)+(ptm->tm_mday%10);
		strData[3] = ((ptm->tm_hour/10) << 4)+(ptm->tm_hour%10);
		strData[4] = ((ptm->tm_min/10) << 4)+(ptm->tm_min%10);
		strData[5] = ((ptm->tm_sec/10) << 4)+(ptm->tm_sec%10);
		PackageBytes(data + 33 + ((7 + 28) * i), strData, 6);
	}
	wSerialNum = Package(NET_CMD_STORE_MUTLIMEDIA_SEARCH_REPLY, strData, 4 + 35 * nSize);
	printf("In ReplyMutiMediaIndex, szData after packet head:");
	for(int a = 0; a < 21; a ++)
	{
		printf("%02x ", strData[a]);
	}printf("\n");
	dataLen = TransmitToCopy(szData, strData, 18 + 4 + (7 + 28) * nSize);	//17 = 4+12+1 
	printf("In ReplyMutiMediaIndex, szData after Transmit:");
	for(int a = 0; a < 22; a ++)
	{
		printf("%02x ", szData[a]);
	}printf("\n");
	return wSerialNum;
}

//WORD SingleReplyMutiMediaIndex(char *szData, DWORD &dataLen, WORD wSerialNumber)
//{

//}


//数据透传
void CJT808Package::UploadTransParent(vector<PackageSendMsg>& vecPackageMsg, BYTE bType,void* pData,DWORD len)
{
	char szHead[4];
	/* 透传消息类型 */
	PackageByte(szHead,bType);
	PackageToSend(NET_CMD_DATA_UP_TAANSPARENT_TRANS, vecPackageMsg, pData, len, szHead, 1);
}

//数据压缩上报
int CJT808Package::UploadCompressData(void* pBuffer)
{
	return 0;
}

//RAS公钥
int CJT808Package::UploadRASKey(void* pBuffer)
{
	return 0;
}

WORD CJT808Package::TransmitToCopy(void* src,void* dst,WORD len)
{
	char* p = (char*)src;
	char ch;

	/* 标志位 */
	*p++ = 0x7E;
	/* 转义 */
	for(WORD i=0;i<len;i++)
	{
		ch = ((char*)dst)[i];
		if(ch==0x7E)
		{
			*p++ = 0x7D;
			*p++ = 0x02;
		}
		else 
		{
			*p++ = ch;
			if(ch==0x7D)
			{
				*p++ = 0x01;
			}
		}
	}
	/* 标志位 */
	*p++ = 0x7E;
	
	return (WORD)(p-((char*)src));
}

WORD CJT808Package::UploadAttachmentInfo(char *szData, WORD& dataLen, File_Server_Config& stFileServerConfig, vector<FileInfoS>& stFileDetailList)
{
	WORD off;
	char strData[1024] = {0};
	char* pData = strData + 12;

	DBG_LEVEL_6("stFileServerConfig.bReSend:%d", stFileServerConfig.bReSend);

	PackageBytes(pData, &stFileServerConfig.alarm_flag.id[0], 7);
	PackageBytes(pData + 7, &stFileServerConfig.alarm_flag.id[0], 7);
	PackageBytes(pData + 14, &stFileServerConfig.alarm_flag.datetime[0], 6);
	PackageByte(pData + 20, stFileServerConfig.alarm_flag.sequence);
	PackageByte(pData + 21, stFileServerConfig.alarm_flag.attachment_num);
	PackageBytes(pData + 23, &stFileServerConfig.alarmCode[0], 32);
	PackageByte(pData + 55, (stFileServerConfig.bReSend ? 0x01:0x00));
	PackageByte(pData + 56, stFileServerConfig.alarm_flag.attachment_num);
	int offset = 0;
	for(int i = 0; i < stFileDetailList.size(); ++ i){
		offset = 56 + 55 * i;
		PackageByte(pData + offset + 1, stFileDetailList[i].name_len);
		PackageString(pData + offset + 2, (char *)&stFileDetailList[i].name[0], stFileDetailList[i].name_len);
		PackageDword(pData + offset + stFileDetailList[i].name_len + 2, stFileDetailList[i].size);
	}

	WORD wSerialNum = Package(NET_CMD_ALARM_ATTACHMENT_INFO_UPLOAD, strData, 57 + stFileDetailList.size() * 55);
	dataLen = TransmitToCopy(szData, strData, 57 + stFileDetailList.size() * 55 + 13);
	return wSerialNum;
}

WORD CJT808Package::UploadFileInfo(char *szData, DWORD &dataLen, FileInfoS& stFileInfo, int id)
{
	WORD off;
	char strData[128] = {0};
	char* pData = strData + 12;
	PackageByte(pData, stFileInfo.name_len);
	PackageString(pData + 1, (char *)&stFileInfo.name[0], stFileInfo.name_len);
	PackageByte(pData + stFileInfo.name_len + 1, stFileInfo.type);
	PackageDword(pData + stFileInfo.name_len + 2, stFileInfo.size);

	WORD wSerialNum = Package(id, strData, 56);
	dataLen = TransmitToCopy(szData, strData, 69);
	return wSerialNum;
}

void CJT808Package::PackageFileHeadData(File_Data_S stFileData, char *msg_buffer, int file_data_len)
{
	if(NULL != msg_buffer){
		char *pData = msg_buffer;
		pData[0] = stFileData.bFlag[0];
		pData[1] = stFileData.bFlag[1];
		pData[2] = stFileData.bFlag[2];
		pData[3] = stFileData.bFlag[3];
		PackageBytes(pData + 4, stFileData.filename, 50);
		PackageDword(pData + 54, stFileData.offset);
		PackageDword(pData + 58, stFileData.length);
		memcpy(pData + 62, &stFileData.data[0], file_data_len);
	}
}

WORD CJT808Package::FacePicDownload(char *szData, DWORD &dataLen, void* pData)
{
	unsigned int off = 0;
	char strData[1024] = {0};
	char *data = (char *)strData + GNSS_NET_PROC_MSG_HEAD_LEN;
	FacePicDownloadReply *info = (FacePicDownloadReply *)pData;
	
	/*fill serialnumber*/
	PackageWord(data, info->serial);
	/*fill anser result*/
	PackageByte(data + 2, info->result);
	/*fill download num*/
	PackageByte(data + 3, info->download_num);
	/*fill cur download */
	PackageByte(data + 4, info->cur_download);
	/*fill face id len*/
	PackageByte(data + 5, info->face_id_len);
	/*fill face id*/
	PackageString(data + 6, (char *)info->face_id, info->face_id_len);

	off = 6 + info->face_id_len;

	WORD wSerialNum = Package(NET_CMD_FACE_PIC_DOWNLOAD_REPLY, strData, off);
	dataLen = TransmitToCopy(strData, szData, off + GNSS_NET_PROC_MSG_HEAD_LEN + GNSS_NET_PROC_CHECK_SUM_LEN);

	return wSerialNum;
}

	
WORD CJT808Package::UploadFaceInfo(char *szData, DWORD &dataLen, void* pData, WORD wSerial)
{
	unsigned int off = 0, i, len = 0;
	char strData[1024] = {0};
	char *data = (char *)strData + GNSS_NET_PROC_MSG_HEAD_LEN;
	FaceIDInfo *info = (FaceIDInfo *)pData;
	DBG_LEVEL_6("wSerial:%04x", wSerial);

	/*fill serialnumber*/
	off +=PackageWord(data, wSerial);
	DBG_LEVEL_6("%02x %02x", data[0], data[1]);

	/*fill face num*/
	off +=PackageByte(data + off, info->face_num);
	DBG_LEVEL_6("%02x %02x %02x ", data[0], data[1], data[3]);

	for (i = 0; i < info->face_num; i++)
	{
		len = info->face_info[i].Id_len;
		if (len > 32){
			len = 32;
		}
		/*fill face id len*/
		PackageByte(data+off, len);
		
		off+= 1;
		/*fill face id*/
		PackageString(data + off, (char *)info->face_info[i].Id, len);
		off += len;
	}

	WORD wSerialNum = Package(NET_CMD_FACE_INFO_QUERY_REPLY, strData, off);
	dataLen = TransmitToCopy(szData, strData, off + GNSS_NET_PROC_MSG_HEAD_LEN + GNSS_NET_PROC_CHECK_SUM_LEN);

	return wSerialNum;
}

WORD CJT808Package::ReportFaceCmp(char *szData, DWORD &dataLen, 
												char* pPicData, void* pCmpData, int nlen,
												WORD nPackTotal, WORD nPackSn)
{
	unsigned int off = 0, i, len = 0;
	char strData[1300] = {0};
	char *data = NULL;

	if (nPackTotal){
		data = (char *)strData + GNSS_NET_PROC_MSG_SUB_HEAD_LEN;
	}else {
		data = (char *)strData + GNSS_NET_PROC_MSG_HEAD_LEN;
	}
	
	FaceCmpResult *compare = (FaceCmpResult *)pCmpData;

	if (1 == nPackSn){
		struGPSData gnssData;
		
		m_pInterFace->GetGnssData(&gnssData);
		int devStatus = m_alarmData.status;
		
		/*fill face num*/
		off += PackageByte(data, compare->result);
		off += PackageByte(data + off, compare->similarity);
		off += PackageWord(data+ off, compare->similar_degree);
		off += PackageByte(data+ off, compare->cmp_type);
		off += PackageByte(data+ off, compare->ID_len);
		off += PackageString(data+ off, compare->ID, compare->ID_len);

		//postion
		off += PosStautsInfo(data+ off, &gnssData, &m_alarmData, devStatus);
		DBG_LEVEL_6("off:%d", off );

		off += PackageByte(data+ off, compare->pic_type);
	}
	else{
		if (NULL != pPicData){
			memcpy(data + off, &pPicData[0], nlen);
			off += nlen;
		}
	}
	
	WORD wSerialNum = Package(NET_CMD_FACE_CMP_RESULT_UP, strData, off, nPackTotal, nPackSn);

	if (nPackTotal){
		off += 4;//subpackage
	}
	
	dataLen = TransmitToCopy(szData, strData, off + GNSS_NET_PROC_MSG_HEAD_LEN + GNSS_NET_PROC_CHECK_SUM_LEN);

	return wSerialNum;
}


WORD CJT808Package::UploadFacePic(char *szData, DWORD &dataLen, char* pPicData, 
													char pic_type, WORD msg_serial, int nlen, WORD nPackTotal, WORD nPackSn)
{
	unsigned int off = 0, i, len = 0;
	char strData[1300] = {0};
	char *data  = (char *)strData + GNSS_NET_PROC_MSG_SUB_HEAD_LEN;;
	
	if (1 == nPackSn){
		
		/*fill msg serial*/
		off += PackageWord(data, msg_serial);	
		/*fill picture type*/
		off += PackageByte(data, pic_type);	
	}

	
	if (NULL != pPicData){
		memcpy(data + off, &pPicData[0], nlen);
		off += nlen;
	}

	WORD wSerialNum = Package(NET_CMD_FACE_AUTH_UPLOAD, strData, off , nPackTotal, nPackSn);
	if (nPackTotal)off += 4;//subpackage
	
	dataLen = TransmitToCopy(szData, strData, off + GNSS_NET_PROC_MSG_HEAD_LEN + GNSS_NET_PROC_CHECK_SUM_LEN);

	return wSerialNum;
}

WORD CJT808Package::UploadVideoInfo(char *szData, DWORD &dataLen, void* pData)
{
	unsigned int off = 0;
	char strData[1024] = {0};
	char *data = (char *)strData + GNSS_NET_PROC_MSG_HEAD_LEN;
	VideoGetParam *info = (VideoGetParam *)pData;
	
	/*音频编码方式*/
	off += PackageByte(data, info->audio_coding);
	/*音频声道数*/
	off += PackageByte(data + off, info->audio_chn_num);
	/*音频采样率*/
	off += PackageByte(data + off, info->audio_frequency);
	/*音频采样位数*/
	off += PackageByte(data + off, info->audio_sampling_bits);
	/*音频帧长度 */
	off += PackageWord(data + off, info->audio_frame_len);
	/*是否支持音频输出*/
	off += PackageByte(data + off, info->support_audio);
	/*视频编码方式*/
	off += PackageByte(data + off, info->video_coding);
	/*终端支持的最大音频物理通道*/
	off += PackageByte(data + off, info->max_audio_num);
	/*终端支持的最大视频物理通道*/
	off += PackageByte(data + off, info->max_video_num);

	WORD wSerialNum = Package(NET_CMD_AV_CONF_UPLOAD, strData, off);
	dataLen = TransmitToCopy(szData, strData, off + GNSS_NET_PROC_MSG_HEAD_LEN + GNSS_NET_PROC_CHECK_SUM_LEN);

	return wSerialNum;
}


WORD CJT808Package::ReportCarries(char *szData, DWORD &dataLen, void* pData)
{
	unsigned int off = 0;
	char strData[1024] = {0};
	char *data = (char *)strData + GNSS_NET_PROC_MSG_HEAD_LEN;
	VideoUploadCarries *info = (VideoUploadCarries *)pData;
	
	/*起始时间*/
	off += PackageBytes(data + off, info->start_time, 6);
	/*结束时间*/
	off += PackageBytes(data + off, info->end_time, 6);
	/*上车人数*/
	off += PackageWord(data + off, info->boarders_num);
	/*下车人数*/
	off += PackageWord(data + off, info->down_num);
	
	WORD wSerialNum = Package(NET_CMD_AV_UPLOAD_PASSENGER_NUM, strData, off);
	dataLen = TransmitToCopy(szData, strData, off + GNSS_NET_PROC_MSG_HEAD_LEN + GNSS_NET_PROC_CHECK_SUM_LEN);

	return wSerialNum;
}

WORD CJT808Package::ReportResourceList(char *szData, DWORD &dataLen, void* pData, WORD nPackTotal, WORD nPackSn, int num)
{
	unsigned int off = 0, i;
	char strData[1024] = {0};
	char *data;
	if (nPackTotal){
		data = (char *)strData + GNSS_NET_PROC_MSG_SUB_HEAD_LEN ;
	}else{
		data = (char *)strData + GNSS_NET_PROC_MSG_HEAD_LEN;
	}
	VideoUploadResInfo *info = (VideoUploadResInfo *)pData;

	if (nPackSn == 1 || (nPackTotal == 0)){//第一个包
		/*流水号*/
		off += PackageWord(data + off, info->serial);
		/*音视频资源总数*/
		off += PackageDword(data + off, info->list_num);
	}

	for (i = 0; i < num; i++)
	{
		/*逻辑通道号*/
		off += PackageByte(data + off, info->souce_info[i].logic_chn);
		/*开始时间*/
		off += PackageBytes(data + off, info->souce_info[i].start_time, 6);
		/*结束时间*/
		off += PackageBytes(data + off, info->souce_info[i].end_time, 6);
		/*报警标志*/
		off += PackageDword(data + off, info->souce_info[i].alarm_flag1);
		/*报警标志*/
		off += PackageDword(data + off, info->souce_info[i].alarm_flag2);
		/*音视频资源类型*/
		off += PackageByte(data + off, info->souce_info[i].type);
		/*码流类型*/
		off += PackageByte(data + off, info->souce_info[i].stream_type);
		/*存储器类型*/
		off += PackageByte(data + off, info->souce_info[i].store_type);
		/*文件大小*/
		off += PackageDword(data + off, info->souce_info[i].file_size);
	}
	
	WORD wSerialNum = Package(NET_CMD_AV_UPLOAD_RESOURCE_LIST, strData, off, nPackTotal, nPackSn);
	if (nPackTotal){
		off += 4;//subpackage
	}
	dataLen = TransmitToCopy(szData, strData, off + GNSS_NET_PROC_MSG_HEAD_LEN + GNSS_NET_PROC_CHECK_SUM_LEN);

	return wSerialNum;
}

WORD CJT808Package::ReportUpResult(char *szData, DWORD &dataLen, void* pData)
{
	unsigned int off = 0;
	char strData[1024] = {0};
	char *data = (char *)strData + GNSS_NET_PROC_MSG_HEAD_LEN;
	VideoUpFileResult *result = (VideoUpFileResult *)pData;
	
	/*上传指令流水号*/
	off += PackageWord(data + off, result->serial);
	/*文件上传结果*/
	off += PackageByte(data + off, result->result);
	
	WORD wSerialNum = Package(NET_CMD_AV_UPLOAD_FILE_FINISH, strData, off);
	dataLen = TransmitToCopy(szData, strData, off + GNSS_NET_PROC_MSG_HEAD_LEN + GNSS_NET_PROC_CHECK_SUM_LEN);

	return wSerialNum;
}

WORD CJT808Package::PackageToSend(WORD sid, vector<PackageSendMsg>& vecPackageMsg, void* pData, DWORD nLen, void* pDataHead, WORD nDataHeadLen, bool bReSend)
{
	char szData[1460];
	WORD wSerialNum;
	WORD packtotal;
	WORD ret;

	nLen+=nDataHeadLen;
	if(nLen>1446)	//1460-14(封装字节数)
	{
		WORD sendDataLen = 1426;	//1460-14(封装字节数)-4(消息包封装字节数)-大约转义增加的字符
		NET_MSG_ATTR_S msgAttr;
		char* pMsgBody = (char*)pData;
		packtotal = (nLen-1)/sendDataLen + 1;
		int i=1;

		wSerialNum = SerialNumSucc();
		/* 消息ID */
		PackageWord(szData,sid);
		/* 消息体属性 */
		memset(&msgAttr.MsgAttr,0,sizeof(NET_MSG_ATTR_S));
		msgAttr.m_MsgAttr.wPack = 1;
		msgAttr.m_MsgAttr.wEncryptType = 0;
		/* 终端手机号 */
		PackageBytes(szData+4,m_bcdPhoneNo,6);
		/* 消息流水号 */
		
		/* 消息包总数 */
		PackageWord(szData+12,packtotal);
		/* 包序号 */

		while(nLen>0)
		{
			if(nLen<(DWORD)sendDataLen)
			{
				sendDataLen = (WORD)nLen;
			}
			/* 消息体属性 */
			msgAttr.m_MsgAttr.wMsgLen = sendDataLen;
			PackageWord(szData+2,msgAttr.MsgAttr);
			/* 消息包流水号 */
			PackageWord(szData+10,wSerialNum);
			wSerialNum+=1;
			/* 包序号 */
			PackageWord(szData+14,i);
			/* 消息体 */
			if((i==1)&&(nDataHeadLen>0))
			{
				memcpy(szData+16,pDataHead,nDataHeadLen);
				memcpy(szData+nDataHeadLen+16,pMsgBody,sendDataLen-nDataHeadLen);
				pMsgBody += sendDataLen-nDataHeadLen;
			}
			else
			{
				memcpy(szData+16,pMsgBody,sendDataLen);
				pMsgBody += sendDataLen;
			}
			/* 校验码 */
			BYTE check=szData[0];
			WORD len = sendDataLen + 16;
			for (WORD n = 1; n < len ; n++)
			{
				check ^= szData[i];
			}
			PackageByte(szData+len,check);
			nLen -= sendDataLen;

			PackageSendMsg stPackageSendMsg;
			memset(&stPackageSendMsg, 0, sizeof(stPackageSendMsg));
			WORD dataLen = TransmitToCopy(stPackageSendMsg.strEvPackage, szData, sendDataLen + 17);
			stPackageSendMsg.nPackageSize = dataLen;
			stPackageSendMsg.nSerialNum = wSerialNum;

			vecPackageMsg.push_back(stPackageSendMsg);
			i+=1;
		}
	}
	else
	{
#if 0		
		wSerialNum = Package(sid,szData,nLen);
		memcpy(szData+12,pData,nLen);
		ret = Send(sid,wSerialNum,pData,nLen+12,bReSend,0,0);
#else
		memcpy(szData + 12, pData, nLen);
		wSerialNum = Package(sid, szData, nLen);
		


		int send_len = nLen + GNSS_NET_PROC_MSG_HEAD_LEN + GNSS_NET_PROC_CHECK_SUM_LEN;
		PackageSendMsg stPackageSendMsg;
		memset(&stPackageSendMsg, 0, sizeof(stPackageSendMsg));
		WORD dataLen = TransmitToCopy(stPackageSendMsg.strEvPackage, szData, send_len);
		stPackageSendMsg.nPackageSize = dataLen;
		stPackageSendMsg.nSerialNum = wSerialNum;

		vecPackageMsg.push_back(stPackageSendMsg);
#endif
	}
	return ret;
}

WORD CJT808Package::Package(WORD sid, char* pBuffer,WORD dataLen, WORD nPackTotal, WORD nPackSn)
{
	WORD off = dataLen+GNSS_NET_PROC_MSG_HEAD_LEN;
	WORD serialNum = SerialNumSucc();
#if 1
	NET_MSG_ATTR_S msgAttr;
	/* 消息ID */
	PackageWord(pBuffer,sid);
	/* 消息体属性 */
	memset(&msgAttr.MsgAttr,0,sizeof(NET_MSG_ATTR_S));
	/* 协议版本号 */
	PackageByte(pBuffer+4, 0x01);
	/* 终端手机号 */
//	PackageBytes(pBuffer+5, m_bcdPhoneNo, 10);
	PackageBytes(pBuffer+7, m_bcdPhoneNo, 8);
	printf("m_bcdPhoneNo:");
	for(int i = 0; i < 10; i++)
	{
		printf("%02x ", m_bcdPhoneNo[i]);
	}
	printf("\n");
	/* 消息流水号 */
	PackageWord(pBuffer+15,serialNum);
	/* 消息包封装项 */
	if ( nPackTotal != 0 )
	{	
		/* 消息包总数 */
		PackageWord(pBuffer+17,nPackTotal);
		/* 包序号 */
		PackageWord(pBuffer+19,nPackSn);
		msgAttr.m_MsgAttr.wPack = 1;
		off += 4;
	}
	/*填充消息体属性*/
	msgAttr.m_MsgAttr.wEncryptType = 0;
	msgAttr.m_MsgAttr.wMsgLen = dataLen;
	msgAttr.m_MsgAttr.wVer = 1;
	PackageWord(pBuffer+2,msgAttr.MsgAttr);
	/* 校验码 */
	BYTE* p = (BYTE*)pBuffer;
	BYTE check=p[0];
	printf("In Package, off=%d\n", off);
	for (WORD i = 1; i < off; i++)
	{
		check ^= p[i];
	}
	PackageByte(pBuffer+off,check);
	printf("In Package, check[%02x]\n", check);
#endif	
	return serialNum;
}


WORD CJT808Package::SerialNumSucc(WORD add)
{
//	CAutoLock lock(&m_mutex);
	WORD num = m_nSerialNumber;
	m_nSerialNumber += add;

	return num;
}

WORD CJT808Package::PosStautsInfo(char* pBuffer, void* pGnssData,void* pAlarmData, WORD devstatus)
{
	WORD off;
	DWORD m,n;
	BYTE szData[8];
	struGPSData* pGnss = (struGPSData*)pGnssData;
	AlarmData_s* pAlarm = (AlarmData_s*)pAlarmData;

	if (pGnss->GpsLocation){
		devstatus |= (1 << 18);
	}

	if (pGnss->BeiDouPosition){
		devstatus |= (1 << 19);
	}

	devstatus |= pGnss->status;
	
	/* 报警标志 */
	PackageDword(pBuffer, pAlarm->alarm);
	/* 设备状态 */
	PackageDword(pBuffer+4, devstatus);
	if(pGnss->cGpsStatus=='A'){
		m = pGnss->fLatitude;
		n = pGnss->fLongitude;
	} else {
		m = 0;
		n= 0;
	}
	/* 纬度 */
	PackageDword(pBuffer+8,m);
	/* 经度 */
	PackageDword(pBuffer+12,n);
	/* 高程 */
	PackageWord(pBuffer+16,pGnss->Altitude);
	/* 速度 */
	PackageWord(pBuffer+18,pGnss->usSpeed/10);
	/* 方向 */
	PackageWord(pBuffer+20,pGnss->usGpsAngle/100);
	/* 时间 */
	time_t curtime =  time(NULL);
	tm *ptm = localtime(&curtime);
	szData[0] = (((ptm->tm_year-100)/10) << 4)+(ptm->tm_year%10);
	szData[1] = (((ptm->tm_mon+1)/10) << 4)+((ptm->tm_mon+1)%10);
	szData[2] = ((ptm->tm_mday/10) << 4)+(ptm->tm_mday%10);
	szData[3] = ((ptm->tm_hour/10) << 4)+(ptm->tm_hour%10);
	szData[4] = ((ptm->tm_min/10) << 4)+(ptm->tm_min%10);
	szData[5] = ((ptm->tm_sec/10) << 4)+(ptm->tm_sec%10);
	PackageBytes(pBuffer+22,szData,6);

	off = 28;
	#if 0
	off += PackageByte(pBuffer+off,0x03);
	off += PackageByte(pBuffer+off,0x02);
	off += PackageWord(pBuffer+off,0x00);
	#endif
	if(pAlarm->alarm&MSK_OVER_SPEED) 
	{
		off += PackageByte(pBuffer+off,0x11);
		if(pAlarm->speed.position==0)
		{
			off += PackageByte(pBuffer+off,1);
			off += PackageByte(pBuffer+off,pAlarm->speed.position);
		}
		else
		{
			off += PackageByte(pBuffer+off,5);
			off += PackageByte(pBuffer+off,pAlarm->speed.position);
			off += PackageDword(pBuffer+off,pAlarm->speed.id);
		}
	}
	if(pAlarm->alarm&MSK_INOUT_REGION)
	{
		off += PackageByte(pBuffer+off,0x12);
		off += PackageByte(pBuffer+off,6);
		off += PackageByte(pBuffer+off,pAlarm->areaIO.position);
		off += PackageDword(pBuffer+off,pAlarm->areaIO.id);
		off += PackageByte(pBuffer+off,pAlarm->areaIO.inout);
	}
	if(pAlarm->alarm&MSK_INOUT_PATH)
	{
		off += PackageByte(pBuffer+off,0x12);
		off += PackageByte(pBuffer+off,6);
		off += PackageByte(pBuffer+off,pAlarm->routeIO.position);
		off += PackageDword(pBuffer+off,pAlarm->routeIO.id);
		off += PackageByte(pBuffer+off,pAlarm->routeIO.inout);
	
	}

	if(pAlarm->alarm&MSK_SEG_TIMEOUT)
	{
		off += PackageByte(pBuffer+off,0x13);
		off += PackageByte(pBuffer+off,7);
		off += PackageDword(pBuffer+off,pAlarm->rtSecTm.id);
		off += PackageWord(pBuffer+off,pAlarm->rtSecTm.time);
		off += PackageByte(pBuffer+off,pAlarm->rtSecTm.result);
	}

	if(pAlarm->extend_type != DEV_NONE){
		if(DEV_DSM == pAlarm->extend_type){
			Alarm_Dsm_S *dsmAlarm = &pAlarm->dsm_alarm;
			off += PackageByte(pBuffer + off, 0x65);
		
			if (0xE2 == dsmAlarm->type){
				off += PackageByte(pBuffer + off, 52+dsmAlarm->dirver_id_len);//Additional information length
				printf("aaadsmAlarm->type=%d\n", dsmAlarm->type);
			}else{
				off += PackageByte(pBuffer + off, 47);
				printf("bbbdsmAlarm->type=%d\n", dsmAlarm->type);
			}
			
			off += PackageDword(pBuffer + off, dsmAlarm->id);
			off += PackageByte(pBuffer + off, dsmAlarm->flag);
			off += PackageByte(pBuffer + off, dsmAlarm->type);
			off += PackageByte(pBuffer + off, dsmAlarm->level);
			off += PackageByte(pBuffer + off, dsmAlarm->tired_level);
			off += 4;
			off += PackageByte(pBuffer + off, dsmAlarm->speed);
			off += PackageWord(pBuffer + off, dsmAlarm->altitude);
			off += PackageDword(pBuffer + off, dsmAlarm->latitude);
			off += PackageDword(pBuffer + off, dsmAlarm->longitude);
			PackageBytes(pBuffer+off, dsmAlarm->datetime, 6);
			off += 6;
			off += PackageWord(pBuffer + off, dsmAlarm->vechile_status);

			PackageBytes(pBuffer+off, dsmAlarm->alarm_flag.id, 7);
			off += 7;
			PackageBytes(pBuffer+off, dsmAlarm->alarm_flag.datetime, 6);
			off += 6;
			off += PackageByte(pBuffer+off, dsmAlarm->alarm_flag.sequence);
			off += PackageByte(pBuffer+off, dsmAlarm->alarm_flag.attachment_num);
			off += 1;

			if (0xE2 == dsmAlarm->type){
				off	+= PackageByte(pBuffer+off, dsmAlarm->result);
				off	+= PackageByte(pBuffer+off, dsmAlarm->similarity);
				off	+= PackageWord(pBuffer+off, dsmAlarm->similar_degree);
				//off	+= PackageByte(pBuffer+off, dsmAlarm->cmp_type);
				off	+= PackageByte(pBuffer+off, dsmAlarm->dirver_id_len);
				off	+= PackageBytes(pBuffer+off, (BYTE *)dsmAlarm->dirver_id, dsmAlarm->dirver_id_len);
			}
			
		} else if(DEV_ADAS == pAlarm->extend_type){
			Alarm_Adas_S *adasAlarm = &pAlarm->adas_alarm;
			off += PackageByte(pBuffer + off, 0x64);
			off += PackageByte(pBuffer + off, 47);
			off += PackageDword(pBuffer + off, adasAlarm->id);
			off += PackageByte(pBuffer + off, adasAlarm->flag);
			off += PackageByte(pBuffer + off, adasAlarm->type);
			off += PackageByte(pBuffer + off, adasAlarm->level);
			off += PackageByte(pBuffer + off, adasAlarm->speedFront);
			off += PackageByte(pBuffer + off, adasAlarm->distanceFront);
			off += PackageByte(pBuffer + off, adasAlarm->deviationType);
			off += PackageByte(pBuffer + off, adasAlarm->roadFlag);
			off += PackageByte(pBuffer + off, adasAlarm->roadFlagData);
			off += PackageByte(pBuffer + off, adasAlarm->speed);
			off += PackageWord(pBuffer + off, adasAlarm->altitude);
			off += PackageDword(pBuffer + off, adasAlarm->latitude);
			off += PackageDword(pBuffer + off, adasAlarm->longitude);
			PackageBytes(pBuffer+off, adasAlarm->datetime, 6);
			off += 6;
			off += PackageWord(pBuffer+off, adasAlarm->vechile_status);
			PackageBytes(pBuffer+off, adasAlarm->alarm_flag.id, 7);
			off += 7;
			PackageBytes(pBuffer+off, adasAlarm->alarm_flag.datetime, 6);
			off += 6;
			off += PackageByte(pBuffer + off, adasAlarm->alarm_flag.sequence);
			off += PackageByte(pBuffer + off, adasAlarm->alarm_flag.attachment_num);
			off += 1;
		}
		
	}

	if (pGnss->SignalIntensity){
		off += PackageByte(pBuffer+off,0x30);
		off += PackageByte(pBuffer+off,1);
		off += PackageByte(pBuffer+off,pGnss->SignalIntensity);
	}

	if (pGnss->GPSSatelliteNum){
		off += PackageByte(pBuffer+off,0x31);
		off += PackageByte(pBuffer+off,1);
		off += PackageByte(pBuffer+off,pGnss->GPSSatelliteNum);
	}

	return off;
}

inline WORD CJT808Package::PackageByte(void* pBuffer,BYTE data)
{
	((BYTE*)pBuffer)[0] = data;
	return 1;
}
inline WORD CJT808Package::PackageWord(void* pBuffer,WORD data)
{
	WORD m = htons(data);
	memcpy(pBuffer, &m, 2);
	return 2;
}
inline WORD CJT808Package::PackageDword(void* pBuffer,DWORD data)
{
	DWORD n = htonl(data);
	memcpy(pBuffer, &n, 4);
	return 4;
}
inline WORD CJT808Package::PackageBytes(void* pBuffer,BYTE* pData,BYTE len)
{
	memcpy(pBuffer,pData,len);
	return len;
}
inline WORD CJT808Package::PackageString(void* pBuffer,char* pData,BYTE maxLen)
{
	/*modified by jim*/
#if 0
	WORD len = strlen(pData)+1;
#else
	WORD len = strlen(pData);
	printf("licence len = %d\n", len);
#endif
	if(len>=maxLen)
	{
		len = maxLen;
		pData[maxLen] = '\0';
	}
	memcpy(pBuffer,pData,len);
	return len;
}
