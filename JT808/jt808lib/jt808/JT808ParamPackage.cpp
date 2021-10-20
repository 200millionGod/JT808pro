#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

#include "JT808ParamPackage.h"
#include "debug_log.h"

#define CASE_DEF_DWORD(x,y)		case x: ExchangeDWord(pData,y);break;
#define CASE_DEF_WORD(x,y)		case x: ExchangeWord(pData,y);break;
#define CASE_DEF_BYTE(x,y)		case x: ExchangeByte(pData,y);break;
#define CASE_DEF_BYTES(x,y,z)	case x: ExchangeBCD(pData,y,z);break;
#define CASE_DEF_STRING(x,y,z)	case x: ExchangeString(pData,y,z);break;

static WORD GetCollisionParam(Collision_Alarm_Attr* pParam)
{
	WORD	param = pParam->acceleration&0x00FF;
	param = (param<<8) + (pParam->time&0x00FF);
	return param;
}

static DWORD GetSnapCtrl(Snap_Ctrl_Attr* pParam)
{
	DWORD param = pParam->timeInterval&0xFF;
	param = (param<<1) + (pParam->timeunit&0x01);
	param = (param<<8) + (pParam->time.savemask&0xFF);
	param = (param<<8) + (pParam->time.mask&0xFF);
	return param;
}

static DWORD GetSnapCtrl2(Snap_Ctrl_Attr* pParam)
{
	DWORD param = pParam->mileageInterval&0xFF;
	param = (param<<1) + (pParam->mileageunit&0x01);
	param = (param<<8) + (pParam->mileage.savemask&0xFF);
	param = (param<<8) + (pParam->mileage.mask&0xFF);
	return param;
}

static void SetCollisionParam(Collision_Alarm_Attr* pParam,WORD value)
{
	pParam->acceleration = (value>>8)&0xFF;
	pParam->time = value&0xFF;
}

static void SetSnapCtrl(Snap_Ctrl_Attr* pParam,DWORD value)
{
	pParam->timeInterval = (value>>17)&0xFF;
	pParam->timeunit = (value>>16)&0x01;
	pParam->time.savemask = (value>>8)&0xFF;
	pParam->time.mask = value&0xFF;
}

static void SetSnapCtrl2(Snap_Ctrl_Attr* pParam,DWORD value)
{
	pParam->mileageInterval = (value>>17)&0xFF;
	pParam->mileageunit = (value>>16)&0x01;
	pParam->mileage.savemask = (value>>8)&0xFF;
	pParam->mileage.mask = value&0xFF;
}

void CJT808ParamBase::Init(JT808_Config_S* pSysCfg)
{
	pConfig = pSysCfg;
}

void CJT808ParamBase::Get(char*& pData,DWORD id)
{
	flag = 0;
	DBG_LEVEL_6("id:%02x ", id);
	
	Exchange(pData,id);
	
	pData += pData[0] + 1;

}

DWORD CJT808ParamBase::Set(char*& pData,DWORD id)
{
	flag = 1;
	DWORD* pID = (DWORD*)pData;
	pData += 4;
	DWORD ret = Exchange(pData,id);
	
	if( ret == 0 )
	{
		pID[0] = htonl(id);
		DBG_LEVEL_6("pID: %08x ", pID[0]);
		
		pData += pData[0] + 1;
	}
	else
	{
		pData -= 4;
	}
	return ret;
}

DWORD CJT808ParamBase::Exchange(char*& pData, DWORD id)
{
	DWORD ret = 0;
	BYTE len = pData[0];
	DBG_LEVEL_6("id:----%04x,len: %d", id, len);
	
	if (!flag){
		memset(pConfig->cfg_store, 0, 128);
	}
	
	switch(id)
	{
		//心跳间隔
		CASE_DEF_DWORD(0x0001,pConfig->commCfg.heartbeat);
		//TCP消息超时应答时间
		CASE_DEF_DWORD(0x0002,pConfig->commCfg.tcp.timeout);
		//TCP消息重传次数
		CASE_DEF_DWORD(0x0003,pConfig->commCfg.tcp.trytimes);
		//UDP消息应答超时时间
		CASE_DEF_DWORD(0x0004,pConfig->commCfg.udp.timeout);
		//UDP消息重传次数
		CASE_DEF_DWORD(0x0005,pConfig->commCfg.udp.trytimes);
		//SMS消息应答超时时间
		CASE_DEF_DWORD(0x0006,pConfig->commCfg.sms.timeout);
		//SMS消息重传次数
		CASE_DEF_DWORD(0x0007,pConfig->commCfg.sms.trytimes);
		//主服务器APN
		CASE_DEF_STRING(0x0010,pConfig->centerSrvCfg.main.apn,len);
		//主服务器无线拨号用户名
		CASE_DEF_STRING(0x0011,pConfig->centerSrvCfg.main.username,len);
		//主服务器无线拨号密码
		CASE_DEF_STRING(0x0012,pConfig->centerSrvCfg.main.psw,len);
		//主服务器地址
		CASE_DEF_STRING(0x0013,pConfig->centerSrvCfg.main.addr,len);
		//备份服务器APN
		CASE_DEF_STRING(0x0014,pConfig->centerSrvCfg.backup.apn,len);
		//备份服务器无线拨号用户名
		CASE_DEF_STRING(0x0015,pConfig->centerSrvCfg.backup.username,len);
		//备份服务器无线拨号密码
		CASE_DEF_STRING(0x0016,pConfig->centerSrvCfg.backup.psw,len);
		//备份服务器地址
		CASE_DEF_STRING(0x0017,pConfig->centerSrvCfg.backup.addr,len);
		//服务器tcp 端口
		CASE_DEF_DWORD(0x0018,pConfig->centerSrvCfg.tcpport);
		//服务器UDP端口
		CASE_DEF_DWORD(0x0019,pConfig->centerSrvCfg.udpport);
		// 道路运输证IC 卡认证主服务器IP地址或域名
		CASE_DEF_STRING(0x001A,pConfig->icSrvCfg.main,len);
		//道路运输证IC 卡认证服务器TCP端口
		CASE_DEF_DWORD(0x001B,pConfig->icSrvCfg.tcpport);
		//道路运输证IC 卡认证服务器UDP端口
		CASE_DEF_DWORD(0x001C,pConfig->icSrvCfg.udpport);
		//道路运输证IC 卡认证备份服务器IP地址或域名，端口同主服务器	
		CASE_DEF_STRING(0x001D,pConfig->icSrvCfg.backup,len);
		//报告策略
		CASE_DEF_DWORD(0x0020,pConfig->reportCfg.tactics);
		//报告方案
		CASE_DEF_DWORD(0x0021,pConfig->reportCfg.scheme);
		//驾驶员未登陆汇报时间间隔
		CASE_DEF_DWORD(0x0022,pConfig->reportCfg.interval[3].time);
		//休眠时汇报时间间隔
		CASE_DEF_DWORD(0x0027,pConfig->reportCfg.interval[2].time);
		//紧急报警时汇报时间间隔
		CASE_DEF_DWORD(0x0028,pConfig->reportCfg.interval[1].time);
		//缺省时间汇报间隔
		CASE_DEF_DWORD(0x0029,pConfig->reportCfg.interval[0].time);
		//缺省距离汇报间隔，单位m
		CASE_DEF_DWORD(0x002C,pConfig->reportCfg.interval[0].mileage);
		//驾驶员未登陆汇报距离间隔
		CASE_DEF_DWORD(0x002D,pConfig->reportCfg.interval[3].mileage);
		//休眠时汇报间隔距离
		CASE_DEF_DWORD(0x002E,pConfig->reportCfg.interval[2].mileage);
		//紧急报警是汇报距离间隔
		CASE_DEF_DWORD(0x002F,pConfig->reportCfg.interval[1].mileage);
		//拐点补偿角度
		CASE_DEF_DWORD(0x0030,pConfig->reportCfg.angle);
		//电子围栏半径
		CASE_DEF_WORD(0x0031,pConfig->reportCfg.radius);
		//监控平台电话号码
		CASE_DEF_STRING(0x0040,pConfig->phoneNumCfg.centernum,64);
		//复位电话号码
		CASE_DEF_STRING(0x0041,pConfig->phoneNumCfg.rebootnum,64);
		//恢复出厂设置电话号码
		CASE_DEF_STRING(0x0042,pConfig->phoneNumCfg.resetnum,64);
		//控制平台SMS电话号码
		CASE_DEF_STRING(0x0043,pConfig->phoneNumCfg.centersmsnum,64);
		//接收终端SMS文本报警电话号码
		CASE_DEF_STRING(0x0044,pConfig->phoneNumCfg.alarmsmsnum,64);
		//终端接听电话策
		CASE_DEF_DWORD(0x0045,pConfig->phoneNumCfg.answerway);
		//每次通话时间
		CASE_DEF_DWORD(0x0046,pConfig->phoneNumCfg.calltime);
		//当月最长通话时间
		CASE_DEF_DWORD(0x0047,pConfig->phoneNumCfg.monthtime);
		//监听电话
		CASE_DEF_STRING(0x0048,pConfig->phoneNumCfg.monitornum,64);
		//监管平台特权短信号码
		CASE_DEF_STRING(0x0049,pConfig->phoneNumCfg.supersmsnum,64);
		//报警屏蔽字
		CASE_DEF_DWORD(0x0050,pConfig->alarmMaskCfg.alarmmask);
		//报警发送文本SMS开关
		CASE_DEF_DWORD(0x0051,pConfig->alarmMaskCfg.sms);
		//报警拍摄开关
		CASE_DEF_DWORD(0x0052,pConfig->alarmMaskCfg.snap);
		//报警拍摄存储标志
		CASE_DEF_DWORD(0x0053,pConfig->alarmMaskCfg.snapsave);
		//关键标志位
		CASE_DEF_DWORD(0x0054,pConfig->alarmMaskCfg.keymask);
		//最高速度
		CASE_DEF_DWORD(0x0055,pConfig->alarmSpeed.speed);
		//超速持续时间
		CASE_DEF_DWORD(0x0056,pConfig->alarmSpeed.time);
		//连续驾驶时间门限
		CASE_DEF_DWORD(0x0057,pConfig->alarmFatigue.drivingtime);
		//当天累计驾驶时间门限
		CASE_DEF_DWORD(0x0058,pConfig->alarmFatigue.totaltime);
		//最小休息时间
		CASE_DEF_DWORD(0x0059,pConfig->alarmFatigue.resttime);
		//最长停车时间
		CASE_DEF_DWORD(0x005A,pConfig->alarmParking.time);
		//超速报警预警差值
		CASE_DEF_WORD(0x005B,pConfig->alarmSpeed.warning);
		//疲劳驾驶预警差值
		CASE_DEF_WORD(0x005C,pConfig->alarmFatigue.warning);
		//碰撞报警参数设置
		case 0x005D:
			{
				WORD value = GetCollisionParam(&pConfig->alarmCollision);
				ExchangeWord(pData,value);
				SetCollisionParam(&pConfig->alarmCollision,value);
			}
			break;
		//侧翻报警参数设置
		CASE_DEF_WORD(0x005E,pConfig->alarmRollover.angle);
		//定时拍照控制
		case 0x0064:
			{
				DWORD value = GetSnapCtrl(&pConfig->snapCtrl);
				ExchangeDWord(pData,value);
				SetSnapCtrl(&pConfig->snapCtrl,value);
			}
			break;
		//定距离拍照控制
		case 0x0065:
			{
				DWORD value = GetSnapCtrl2(&pConfig->snapCtrl);
				ExchangeDWord(pData,value);
				SetSnapCtrl2(&pConfig->snapCtrl,value);
			}
			break;
		//图像视频质量
		CASE_DEF_DWORD(0x0070,pConfig->picQuality.quality);
		//亮度
		CASE_DEF_DWORD(0x0071,pConfig->picColor.luminance);
		//对比度
		CASE_DEF_DWORD(0x0072,pConfig->picColor.contrast);
		//饱和度
		CASE_DEF_DWORD(0x0073,pConfig->picColor.saturation);
		//色度
		CASE_DEF_DWORD(0x0074,pConfig->picColor.chroma);
		//车辆里程表读数
		CASE_DEF_DWORD(0x0080,pConfig->VehInfoCfg.mileage);
		//车辆所属省域ID
		CASE_DEF_WORD(0x0081,pConfig->VehInfoCfg.provinceid);
		//车辆所属城市ID
		CASE_DEF_WORD(0x0082,pConfig->VehInfoCfg.cityid);
		//车牌号码
		CASE_DEF_STRING(0x0083,pConfig->VehInfoCfg.license,16);
		//车牌颜色
		CASE_DEF_BYTE(0x0084,pConfig->VehInfoCfg.platecolor);
		//GNSS定位模式
		CASE_DEF_BYTE(0x0090,pConfig->gnssDetail.enable);
		//GNSS波特率
		CASE_DEF_BYTE(0x0091,pConfig->gnssDetail.baudtate);
		//GNSS模块详细定位数据输出频率
		CASE_DEF_BYTE(0x0092,pConfig->gnssDetail.outputFreq);
		//GNSS模块详细定位数据采集频率，单位为秒，默认为1。
		CASE_DEF_DWORD(0x0093,pConfig->gnssDetail.sampleFreq);
		//GNSS模块详细定位数据上传方式
		CASE_DEF_BYTE(0x0094,pConfig->gnssDetail.uploadmode);
		//GNSS模块详细定位数据上传设置：
		CASE_DEF_DWORD(0x0095,pConfig->gnssDetail.uploadset);
		//CAN总线通道1 采集时间间隔(ms)，0表示不采集
		CASE_DEF_DWORD(0x0100,pConfig->canbusUpload.sample[0]);
		//CAN总线通道1 上传时间间隔(s)，0表示不上传 
		CASE_DEF_WORD(0x0101,pConfig->canbusUpload.upload[0]);
		//CAN总线通道2 采集时间间隔(ms)，0表示不采集 
		CASE_DEF_DWORD(0x0102,pConfig->canbusUpload.sample[1]);
		//CAN总线通道2 上传时间间隔(s)，0表示不上传 
		CASE_DEF_WORD(0x0103,pConfig->canbusUpload.upload[1]);
		//CAN总线ID单独采集设置：
		CASE_DEF_BYTES(0x0110,pConfig->canbusUpload.aloneSample.set,8);
		//高级驾驶辅助系统参数
		CASE_DEF_BYTES(0xF364,pConfig->cfg_store,len);
		//驾驶员状态检测系统参数
		CASE_DEF_BYTES(0xF365,pConfig->cfg_store,len);
		//音视频参数设置	
		CASE_DEF_BYTES(0x0075,pConfig->cfg_store,len);
		//音视频通道列表设置
		CASE_DEF_BYTES(0x0076,pConfig->cfg_store,len);
		//单独视频通道参数设置
		CASE_DEF_BYTES(0x0077,pConfig->cfg_store,len);
		//特殊报警录像参数设置
		CASE_DEF_BYTES(0x0079,pConfig->cfg_store,len);
		//视频相关报警屏蔽字
		CASE_DEF_BYTES(0x007A,pConfig->cfg_store,len);
		//图像分析报警参数设置
		CASE_DEF_BYTES(0x007B,pConfig->cfg_store,len);
		//终端休眠唤醒模式设置
		CASE_DEF_BYTES(0x007C,pConfig->cfg_store,len);
		
		default:
			ret = 1;
			break;
	}
	return ret;
}

void CJT808ParamBase::ExchangeByte(char*& pData,BYTE& value)
{
	if(flag)
	{
		pData[0] = 1;
		pData[1] = (char)value;
	}
	else
	{
		value = (BYTE)pData[1];
	}

}

void CJT808ParamBase::ExchangeWord(char*& pData,WORD& value)
{
	WORD* p = (WORD*)(pData+1);
	if(flag)
	{
		pData[0] = 2;
		p[0] = htons(value);
	}
	else
	{
		value = ntohs(p[0]);
	}
}

void CJT808ParamBase::ExchangeDWord(char*& pData,DWORD& value)
{
	DWORD* p = (DWORD*)(pData+1);
	if(flag)
	{
		pData[0] = 4;
		p[0] = htonl(value);

		DBG_LEVEL_6("len:%d value:%08x ", pData[0], p[0]);
	}
	else
	{
		value = ntohl(p[0]);
	}

	DBG_LEVEL_6("value:%04x ", value);
}

void CJT808ParamBase::ExchangeString(char*& pData,char* value,int max)
{
	BYTE l;
	if(flag)
	{
		l = strlen(value)+1;
		if(l>max)
		{
			l = max;
		}
		pData[0] = l;
		memcpy(&pData[1],value,l-1);
		pData[l-1] = '\0';
	}
	else
	{
		l = (BYTE)pData[0];
		if(l>max)
		{
			l = max;
		}
		memcpy(value,pData,l-1);
		value[l-1] = '\0';
	}
}

void CJT808ParamBase::ExchangeBCD(char*& pData,BCD* value,int len)
{
	if(flag)
	{
		pData[0] = (BYTE)len;
		memcpy(&pData[1],value,len);
	}
	else
	{
		memcpy(value,&pData[1],len);
	}
}

