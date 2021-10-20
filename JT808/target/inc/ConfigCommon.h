#ifndef _CONFIG_COMMON_H
#define _CONFIG_COMMON_H

#include "CommDefine.h"

#define AV_CHANNEL_MAX	8

/**************************  本地配置参数 **************************/
/* 基本设置结构体定义*/
typedef struct 
{
	/* 系统语言0--简体中文,1--英语*/
	BYTE	language;
	/*日期类型,0--年月日, 1--日月年,2--月日年,(default:0)*/
	BYTE	DateType;
	/*校时模式, 0:禁止开机自动校时,1,开机gps校时(default:1)*/
	BYTE	TimingMode;
	/*时区,值0-23*/
	BYTE 	TimeZone;
	/*校时时间点，单位秒*/
	DWORD 	timingtime;
	/*default: 1min*/
	DWORD	OprTimeOut;		
	/* 设备安装时间 */
	DWORD 	InstallTime;
	/*线路号*/
	char 	LineNum[12];
}BASE_SETUP_S;

/* 夏令时 */
typedef struct
{
	/* 0关闭，1开启 */
	BYTE 	onoff;
	/* 开始月份，1开始计数 */
	BYTE	start_month;
	/* 开始日 */
	BYTE	start_day;
	/* 结束月份 */
	BYTE	end_month;
	/* 结束日 */
	BYTE	end_day;
	/* 保留字 */
	BYTE	reserved[3];
}DST_SET_S;

/*开关机设置*/
typedef struct
{
	/* 1--点火模式, 2--定时模式,3--休眠模式,default: 1 */
	BYTE	PowerMode;
	/*延时开机开关, 0--关,1--开, default: 1*/
	BYTE	DelaySwitch;
	/*延时关机时间,3min~1440min, default: 5min*/
	WORD	DelayTime;
	/*定时开机时间(单位秒),hour*3600+min*60+sec*/
	DWORD	PowerOnTime;
	/*定时关机时间(单位秒),hour*3600+min*60+sec*/
	DWORD	PowerOffTime;		
}SYSTEM_POWER_SET_S;

/* 监视画面设置 */
typedef struct
{
	/* 视频预览模式 :0--单通道,1--多通道*/
	BYTE	OutputMode;
	/*通道编号, 1~4 (channel1~channel4) (单通道模式使用)*/
	BYTE	ChlNum;		    
	/*视频预览OSD叠加掩码,对应位为1则不显示该信息,bit0--时间,bit1--日期,bit2--通道名称,*/
	WORD	showmask;
}MONITOR_SET_S;

/* 本地网络设置 */
typedef struct
{
	/*ip地址*/
	DWORD	IpAddr;
	/*子网掩码*/
	DWORD 	NetMask;
	/*网关*/
	DWORD 	GateWay;
	/*DNS服务器地址*/
	DWORD 	dns[2];	
}LOCAL_NETWORK_S;

/* WIFI设置 */
typedef struct
{
	/*1--启用，0--不启用，default--0*/
	BYTE	isOpen;
	/*1--密码开启，0--密码关闭*/
	BYTE	EncryptSw;
	/*0--开放式，1--共享式，2--WPA, 3--WPA-PSK*/
	BYTE	AuthMode;
	/*0--NONE, 1--WEP, 2--TKIP, 3--AES, */
	BYTE	Encrypt;
	/*SSID*/
	char	SSID[20];
	/*密码*/
	char	Pwd[16];
	/*IP 地址*/
	DWORD	IpAddr;
	/*子网掩码*/
	DWORD	SubNet;	
	/*网关*/
	DWORD	GateWay;
}WIFI_SETUP_S;

/* 拨号设置 */
typedef struct
{
	/*1--启用3g/4g模块，0---不启用,default--1*/
	BYTE	isOpen;
	/*0--WCDMA, 1--EVDO, 2--Td-scdma, 3--LTE-TDD, 4--LTE-FDD*/
	BYTE	DialType;
	/* 保留字 */
	BYTE	reserved;
	/*1--启用拨号，0--不启用拨号*/
	BYTE	DialOpen;
	/*接入点设置*/
	char	APN[40];
	/*中心号码设置*/
	char	CenterNum[20];
	/*用户名*/
	char	UserName[16];
	/*用户密码*/
	char	UserPwd[16];
}DIALUP_SETUP_S;

/* 录像通道设置 */
typedef struct
{
	/*编码开关, 1--on, 0--off (default:1)*/
	BYTE	isOpen;
	/*预览开关, 1--on, 0--off (default:1)*/
	BYTE	isPreview;
	/*是否包含音频,1--have audio, 0--not have audio*/
	BYTE	HaveAudio;
	/*音频格式G726, AAC.  1--G726, 2--AAC (default:1)*/
	BYTE	AudioType;
	/*分辨率, 0-720P,1--D1, 2--HD1, 3--CIF*/	
	BYTE	Resolution;
	/*帧率, 1~25(PAL), 1~30(NTSC)*/
	BYTE	FrameRate;
	/*音频输入增益, (1-15)*/
	BYTE	AIVolume;
	/*画质: 1~8, 1 最清晰，(default:1)*/
	BYTE	Quality;
	/*码流,bitrate 128~4096(unit: k)*/
	BYTE	BitRate;
	/*I帧间隔, 35~1000(unit: k)(未用到)*/
	BYTE	Gop;
	/* 保留字 */
	BYTE	reserved[2];
}ENCODE_CHN_ATTR;

/*音视频编码设置*/
typedef struct
{
	ENCODE_CHN_ATTR	ChlSet[AV_CHANNEL_MAX];
}AV_ENCODE_S;

/* 录像设置相关参数 */
typedef struct
{
	/*制式,0--PAL, 1--NTSC, (default: 0)*/
	BYTE	SysNorm;
	/*录像模式,1--开机录像,2--定时录像,3--报警录像(default:1)*/
	BYTE	RecMode;
	/*自动覆盖,1--开启, 0--关闭,default: 1*/
	BYTE	AutoCover;
	/*打包时间, 1--15min, 2--30min, 3--45min, 4--60min, default:2*/
	BYTE	PacketTime;
	/*镜像录像,1--Mirror recording, 0--off*/
	BYTE	isMirror;
	/*报警最大预录时间，范围10~60, default: 30*/
	BYTE	AlarmValidTm;
	/*报警输出时间，5s~240s，default: 15*/
	BYTE	AlarmOutTm;
	/*报警录像保护天数，1,3,5,7,10,15*/
	BYTE	AlarmProtectTm;
	/*行车记录信息记录，1--on, 0--off*/
	BYTE	RecVehicleInfo;
	/*音频输出增益1-15*/
	BYTE	AOVolume;
	/*报警录像延时时间,30s~1800s,default: 30s*/
	WORD	AlarmDelayTime;
}RECORD_SET_S;

/* 本地录像时间段项目 */
typedef struct
{	
	/*第一时段的开始时间(单位为秒)*/
	DWORD	start_rec1;
	/*第一时段的结束时间(单位为秒)*/
	DWORD 	end_rec1;
	/*第二时段的开始时间(单位为秒)*/
	DWORD 	start_rec2;
	/*第二时段的结束时间(单位为秒)*/
	DWORD 	end_rec2;
	/*第一时段的录像方式0:常规1:事件2:报警3:移动侦测4:镜头遮挡*/
	BYTE	rec1_type;
	/*第二时段的录像方式*/
	BYTE	rec2_type;
	/* 保留字 */
	BYTE	reserved[2];
}REC_TIME_ITEM_S;

/* 录像计划 */
typedef struct
{
	/*下标0至6分别对应星期天到星期六,7对应每一天*/
	REC_TIME_ITEM_S	RecTimer[8];
}REC_TIME_S;

/* 图像数据加密配置参数 */
typedef struct
{
	/* 加密类型(0--null  1--DES  2--3DES 3--AES) */
	DWORD	type;
	/* 加密密钥 */
	char 	cryptogram[32];
	/* 解密密钥 */
	char 	Decryptogram[32];
}REC_ENCRYPTION_S;

/* 子码流设置 */
typedef struct
{
	/*分辨率, 0-720P,1--D1, 2--HD1, 3--CIF*/	
	BYTE	resolution;
	/* 帧率 */
	BYTE	targetFrame;
	/* 峰值码率 */
	WORD	bitRate;
	/* 图象质量 1~8, 1 最清晰*/
	BYTE 	picLevel;
	/* I帧间隔 */
	BYTE 	gop;	
	/* 保留字 */
	BYTE	reserved[2];
}SUB_STREAM_ATTR;

/* 通道抓拍设置参数 */
typedef struct
{
	/*使能: 0--关闭 1--开启*/
	BYTE	isSnap;
	/*分辨率, 0-720P,1--D1, 2--HD1, 3--CIF*/	
	BYTE	Resolution;
	/*画质: 1~8, 1 最清晰*/
	BYTE	Quality;
	/* 保留字 */
	BYTE	reserved;
}CHANNEL_SNAP_S;

/* 抓拍设置 */
typedef struct
{
	CHANNEL_SNAP_S 	ChlSet[AV_CHANNEL_MAX];
}PIC_SNAP_ATTR;


/* IO报警通道设置 */
typedef struct
{
	/*IO通道名称*/
	char	Name[16];
	/*1--enable, 0--disable, default: 0*/
	BYTE	Enable;
	/*1--高电平或常开, 0--低电平或常闭,default:1*/
	BYTE	ControlSet;
	/*1--触发录像，0--不触发录像,default:1*/
	BYTE	StartRec;
	/*1--记录日志，0--不记录日志,default:1*/
	BYTE	RecLog;	
	/*绑定视频预览通道 0--关闭,1~8对应通道1~8*/
	BYTE	TrigAVChn;
	/* 保留字 */
	BYTE	reserved[2];
}ALARM_IO_ITEM;

/* IO报警设置 */
typedef struct
{
	ALARM_IO_ITEM		AlarmSets[8];	/*下标0~3 为报警输入,下标4~5为报警输出*/
}ALARM_IO_S;

//温度设置结构体定义
typedef struct
{
	/*0--摄氏度，1--华氏度*/
	BYTE	TempUnit;
	/*0--disable, 1--enable*/
	BYTE	isOpen;
	/*0--record off, 1--record on*/
	BYTE	isRecord;
	/*报警阀值*/
	BYTE	AlarmValue;
}TEMP_SET_S;

/* gsensor初值配置 */
typedef struct
{
	/* X轴 */
	signed short 		xV;
	/* Y轴 */
	signed short 		yV;
	/* Z轴 */
	signed short 		zV;
	/* 精度 */
	signed short 		sensitivity;
}GSENSOR_INIT_S;

typedef struct
{
	/*1--on, 0--off*/
	BYTE	isOpen;
	/*1--record on, 0--record off*/
	BYTE	isRecord;
	/*阀值,放大100倍后的值*/
	WORD	limitvalue;
}SINGLE_SENSOR_SET;

/* 加速度报警设置 */
typedef struct
{
	/*0-x, 1-y,2-z 三个方向的设置*/
	SINGLE_SENSOR_SET	Sensors[3];
}ALARM_GSENSOR_SET;


/* 移动侦测设置 */
typedef struct
{
	/*图象块11*9:bit0~7对应通道1~8,bit值1表示选中 */
	BYTE	DetectMark[99];
	/* 开启:bit0~7对应通道1~8,bit值1表示开启 */
	BYTE	WorkOn;
	/* 灵敏度 :每通道2bit,bit0~15对应通道1~8,*/
	WORD	Sensitivity;
	/* 保留字 */
	BYTE	reserved[2];
}MOTION_DETECTION_S;

/* 安全设置 */
typedef struct
{
	/*密码启动开关,0--关闭，1--启动,(default:1)*/
	BYTE	OnOrOff;
	/* 保留字 */
	BYTE	reserved;
	char	AdminPassword[8]; 	/*管理员密码(6个数字) (default:111111)*/
	char	UserPassword[8];	/*普通用户密码(6个数字) (default:000000)*/
}SECURITY_SET_S;

/*************************************************************************/

/************************* 808协议配置参数 *************************/
/*网络通信配置*/
typedef struct
{
	/* 消息超时应答时间，单位s */
	DWORD	timeout;
	/* 消息重传次数 */
	DWORD	trytimes;
}Msg_Send_Attr;

typedef struct
{
	/* 心跳间隔时间，单位s */
	DWORD	heartbeat;
	/* TCP消息*/
	Msg_Send_Attr	tcp;
	/* UDP消息*/
	Msg_Send_Attr	udp;
	/* SMS消息*/
	Msg_Send_Attr	sms;
	/* 鉴权码 */
	char	authenticationkey[64];
	/* 手机号 */
	BCD		phoneNumber[10];
	/* 保留字 */
	BYTE	reserved[2];
}Net_Communication_Attr;

/* 中心服务器配置 */
typedef struct
{
	/* APN/PPP拨号号码 */
	char	apn[64];
	/* 无线拨号用户名 */
	char	username[32];
	/* 无线拨号密码 */
	char	psw[16];
	/* IP地址或域名 */
	char	addr[64];
}Server_Addr_Attr;

/* 中心服务器 */
typedef struct
{
	/* 主服务器 */
	Server_Addr_Attr	main;
	/* 备份服务器 */
	Server_Addr_Attr	backup;
	/* tcp端口号 */
	DWORD				tcpport;
	/* udp端口号 */
	DWORD				udpport;
}Center_Server_Attr;

/* IC卡认证服务器 */
typedef struct
{
	/* 主服务器IP地址或域名 */
	char	main[64];
	/* 备份服务器IP地址或域名 */
	char	backup[64];	//备份服务器IP地址或域名
	/* tcp端口号 */
	DWORD	tcpport;
	/* udp端口号 */
	DWORD	udpport;
}IC_Server_Attr;

/* 位置信息汇报配置参数 */
typedef struct
{
	/* 时间间隔,单位秒,>0 */
	DWORD	time;
	/* 距离间隔,单位米,>0 */
	DWORD	mileage;
}Interval_Attr;

typedef struct
{
	/* 位置信息汇报策略:1--定时,2--定距,3--定时和定距 */
	DWORD 	tactics;
	/* 位置汇报方案:0--根据休眠状态,1--根据登陆状态和休眠 */
	DWORD 	scheme;
	/* 汇报间隔 0--默认,1--报警时,2--休眠时,3--未登陆时*/
	Interval_Attr	interval[4];
	/* 拐点补传角度 ,单位1/100度*/
	DWORD 	angle;
	/* 电子围栏半径 ,单位米*/
	WORD 	radius;
	/* 保留字 */
	WORD 	reserved;
}Gnss_Report_Attr;

/*电话相关参数设置*/
typedef struct
{
	/* 监听电话 */
	char	monitornum[16];
	/* 监管平台特权短信号码 */
	char	supersmsnum[16];
	/* 监控平台电话号码 */
	char 	centernum[16];
	/* 复位电话号码 */
	char	rebootnum[16];
	/* 恢复出厂设置电话号码 */
	char 	resetnum[16];
	/* 控制平台SMS电话号码 */
	char 	centersmsnum[16];
	/* 接收终端SMS文本报警电话号码 */
	char 	alarmsmsnum[16];
	/* 终端接听电话策略，0:自动接听;1:ACC ON 时自动接听，OFF时手动接听 */
	DWORD 	answerway;
	/* 每次通话时间，单位s，0为不允许通话，0xffffffff为不限制 */
	DWORD 	calltime;
	/* 当月最长通话时间，单位同上 */
	DWORD 	monthtime;
}Phone_Number_Attr;

/*报警掩码设置*/
typedef struct
{
	/* 报警屏蔽字*/
	DWORD	alarmmask;
	/* 报警发送文本SMS开关*/
	DWORD 	sms;
	/* 抓拍开关 */
	DWORD	snap;
	/* 报警拍摄存储标志 */
	DWORD 	snapsave;
	/* 关键报警标志位 */
	DWORD	keymask;
}Alarm_Mask_Attr;

/*超速报警配置*/
typedef struct
{
	/* 最高速度，单位km/h*/
	DWORD	speed;
	/* 超速持续时间，单位s */
	DWORD   time;
	/* 超速预警差值 ，单位为1/10Km/h */
	WORD   	warning;
	/* 保留字 */
	WORD 	reserved;
}Speed_Alarm_Attr;

/*疲劳驾驶报警配置*/
typedef struct
{
	/* 连续驾驶时间门限，单位秒*/
	DWORD	drivingtime;
	/* 累计驾驶时间门限，单位秒*/
	DWORD	totaltime;
	/* 休息时间，单位秒 */
	DWORD   resttime;
	/* 疲劳预警差值 ，单位为秒*/
	WORD   	warning;
	/* 保留字 */
	WORD 	reserved;
}Fatigue_Alarm_Attr;

/*超时停车报警配置*/
typedef struct
{
	/* 停车时间门限，单位秒*/
	DWORD	time;
}Parking_Alarm_Attr;

/*碰撞报警配置*/
typedef struct
{
	/* 碰撞时间，单位4ms*/
	WORD	time;
	/* 加速度，单位 0.1g，设置范围在：0-79 之间，默认为10*/
	WORD	acceleration;
}Collision_Alarm_Attr;

/*侧翻报警配置*/
typedef struct
{
	/* 侧翻角度，单位度，默认为30度*/
	WORD	angle;
	/* 保留字 */
	WORD 	reserved;
}Rollover_Alarm_Attr;

/* 图象抓拍控制*/
/*抓拍标志，bit0~7对应通道1~8*/
typedef struct
{
	/*抓拍开关: 1--抓拍，0--不抓拍*/
	WORD	mask;
	/* 存储标志:0-存储，1--上传*/
	WORD	savemask;
	
}Snap_Mask_Attr;

typedef struct
{
	/* 定时抓拍控制 */
	Snap_Mask_Attr	time;
	/* 定距抓拍控制 */
	Snap_Mask_Attr	mileage;
	/* 定时时间单位，0--秒(小于5秒按5秒处理)，1--分 */
	BYTE	timeunit;
	/* 定距距离单位，0--米(小于100米按100米处理)，1--公里 */
	BYTE	mileageunit;
	/* 定时时间间隔 */
	WORD 	timeInterval;
	/* 定距距离间隔 */
	WORD 	mileageInterval;	
	/* 保留字 */
	WORD 	reserved;	
}Snap_Ctrl_Attr;

/* 图象质量配置 */
typedef struct
{
	/* 图像视频质量，1~10，1最好 */
	DWORD	quality;
}Pic_Quality_Attr;

/* 图象颜色配置 */
typedef struct
{
	/* 亮度,0~255 */
	DWORD luminance;
	/* 对比度,0~127 */
	DWORD contrast;
	/* 饱和度，0~127 */
	DWORD saturation;
	/* 色度，0~255 */
	DWORD chroma;
}Pic_Color_Attr;

/*车辆信息配置*/
typedef struct
{
	/* 车辆里程表读数，单位1/10km */
	DWORD 	mileage;
	/* 车辆所属省域ID */
	WORD 	provinceid;
	/* 车辆所属城市ID */
	WORD 	cityid;
	/* 机动车号牌 */
	char	license[16];
	/* 车牌颜色,按照JT/T415-2006的5.4.12 */
	BYTE 	platecolor;
	/* 保留字 */
	BYTE	reserved;
	/* 车辆系数K值 */
	WORD	coefficient;
	/* VIN */
	char	vin[20];
	/* 车辆类型 */
	char	vehicletype[16];
	/* 机动车号牌分类 */
	DWORD 	nVehicletype;
}Vehicle_Info;

/* Gnss详细信息设置 */
typedef struct
{
	/* Gnss定位模式 ,bit0--gps,bit1--北斗,bit2--GLONASS,bit3--Galileo*/
	BYTE	enable;
	/* 波特率:0--4800,1--9600,2--19200,3--38400,4--57600,5--115200 */
	BYTE	baudtate;
	/* 详细定位数据输出频率 :0--500ms,1--1s,2--2s,3--3s,4--4s*/
	BYTE	outputFreq;
	/* 上传方式:0--不上传,0x01--时间间隔,0x02--距离间隔,
		0x0B--按累计时间,0x0C--按累计距离,0x0D--按累计条数*/
	BYTE	uploadmode;
	/* 详细定位数据采集频率,单位为秒，默认1 */
	DWORD	sampleFreq;
	/*上传方式为0x01,0x0B--单位秒,0x02,0x0C--单位米,0x0D--单位条*/
	DWORD	uploadset;
	
}Gnss_Detail_Attr;

/* CAN数据上传 */
/* 单独采集设置 */
typedef struct
{
	/* bit63~bit32--采集时间间隔(ms),0表示不采集,
		bit31--CAN通道号[0,1],
		bit30--帧类型,0标准,1扩展,
		bit29--采集方式,0原始数据,1采集区间计算值
		bit28~bit0--CAN总线ID*/
	BYTE	set[8];
	
}CAN_Sample_Attr;

typedef struct
{
	/*CAN1,CAN2采集时间间隔(ms),0表示不采集*/
	DWORD	sample[2];
	/* CAN1,CAN2上传时间间隔(S) ,0表示不上传*/
	WORD	upload[2];
	/*单独采集设置,*/
	CAN_Sample_Attr	aloneSample;
}CAN_Upload_Attr;


typedef struct
{
	/*报警判断速度阈值*/
	BYTE AlarmThres;
	/*Alarm alert volume*/
	BYTE AlarmVolume;
	/*主动拍照策略*/
	BYTE ActiveSnap;/*0x0:disable 0x1:timing 0x2:mileage*/
	/*主动定时拍照时间间隔*/
	WORD TimeThres;
	/*主动定距拍照时间间隔*/
	WORD MileageThres;
	/*单词主动拍照张数*/
	BYTE SingleSnapNum;
	/*单次主动拍照时间间隔*/
	BYTE SingleSnapInter;
	/*拍照分辨率*/
	BYTE PicResolution;
	/*视频录制分辨率*/
	BYTE VedioResolution;
	/*报警使能*/
	DWORD AlarmEnble;/*0：关闭 1：打开 bit0:障碍检测一级报警；bit1：障碍检测二级报警
						* bit2：频繁变道一级报警； bit3：频繁变道二级报警；
						* bit4：车道偏离一级报警；bit5：车道偏离二级报警；
						*bit6：前向碰撞一级报警；bit7：前向碰撞二级报警；
						*bit8：行人碰撞一级报警； bit9：行人碰撞二级报警；
						*bit10：车距过近一级报警； bit11：车距过近二级报警
						*bit16：道路标志超限报警*/
	/*事件使能*/
	DWORD EventEnable;/*0：关闭 1：打开 bit0：道路标志识别； bit1：主动拍照*/
	/*预留*/
	BYTE Reserve;
	/*OBS:Obstacle Waring 障碍物报警*/
	/*障碍物报警距离阈值*/
	BYTE OWDistanceThres;
	/*障碍物报警分级速度阈值*/
	BYTE OWSpeedThres;
	/*障碍物报警视频录制长度*/
	BYTE OWRecordingTime;
	/*障碍物报警拍照张数*/
	BYTE OWSnapNum;
	/*障碍物报警拍照间隔*/
	BYTE OWSnapThres;
	/*FDW:Frequent diversion warning.频繁变道报警*/
	/*频繁变道报警判断时间段*/
	BYTE FDWAlarmTime;
	/*频繁变道报警判断次数*/
	BYTE FDWTimes;
	/*频繁变道报警分级速度阈值*/
	BYTE FDWSpeedThres;
	/*频繁变道报警视频录制前后时间*/
	BYTE FDWRecordingTime;
	/*频繁变道报警拍照张数*/
	BYTE FDWSnapNum;
	/*频繁变道报警拍照间隔*/
	BYTE FDWSnapInterval;
	/*LDW:Lane deviation warning*/
	/*车道偏离分级速度阈值*/
	BYTE LDWSpeedThres;
	/*车道偏离前后视频录制时长*/
	BYTE LDWRecordingTime;
	/*车道偏离报警拍照张数*/
	BYTE LDWSnapNum;
	/*车道偏离报警拍照间隔*/
	BYTE LDWSnapInterval;
	/*FCW:Forward collision warning .前向碰撞报警*/
	/*前向碰撞报警时间阈值*/
	BYTE FCWAlarmThres;
	/*前向碰撞报警分级速度阈值*/
	BYTE FCWSpeedThres;
	/*前向碰撞前后视频录制时长*/
	BYTE FCWRecordingTime;
	/*前向碰撞报警拍张张数*/
	BYTE FCWSnapNum;
	/*前向碰撞报警拍照间隔*/
	BYTE FCWSnapInterval;
	/*PCW:Pedestrian collision warning.行人碰撞报警*/
	/*行人碰撞报警时间阈值*/
	BYTE PCWAlarmThres;
	/*行人碰撞报警分级速度阈值*/
	BYTE PCWSpeedThres;
	/*行人碰撞报警前后视频录制时长*/
	BYTE PCWRecordingTime;
	/*行人碰撞报警拍照数量*/
	BYTE PCWSnapNum;
	/*行人碰撞报警拍照间隔*/
	BYTE PCWSnapInterval;
	/*DMW:Vehicle Distance Monitoring warning 车距监控报警*/
	/*车距监控报警距离阈值*/
	BYTE DMWDistanceThres;
	/*车距监控报警分级速度阈值*/
	BYTE DMWSpeedThres;
	/*车距监控报警前后视频录制时长*/
	BYTE DMWRecordingTime;
	/*车距监控报警拍照数量*/
	BYTE DMWSnapNum;
	/*车距监控报警拍照间隔*/
	BYTE DMWSnapInterval;
	/*RSR: Road sign Recognition.道路标志识别*/
	/*道路标志识别拍照张数*/
	BYTE RSRSnapNum;
	/*道路标志识别拍照间隔*/
	BYTE RSRSnapInterval;
	/*保留字段*/
	BYTE inter[4];
	
}Adas_Attr;


typedef struct
{
	/*报警判断速度阈值*/
	BYTE AlarmThres;
	/*Alarm alert volume*/
	BYTE AlarmVolume;
	/*主动拍照策略*/
	BYTE ActiveSnap;/*0x0:disable 0x1:timing 0x2:mileage 0x03:card*/
	/*主动定时拍照时间间隔*/
	WORD TimeThres;
	/*主动定距拍照时间间隔*/
	WORD MileageThres;
	/*单词主动拍照张数*/
	BYTE SingleSnapNum;
	/*单次主动拍照时间间隔*/
	BYTE SingleSnapInter;
	/*拍照分辨率*/
	BYTE PicResolution;
	/*视频录制分辨率*/
	BYTE VedioResolution;
	/*报警使能*/
	DWORD AlarmEnble;/*0：关闭 1：打开 bit0:疲劳驾驶一级报警；bit1：疲劳驾驶二级报警
						* bit2：接打电话一级报警； bit3：接打电弧二级报警；
						* bit4：抽烟一级报警；bit5：抽烟二级报警；
						*bit6：分神驾驶一级报警；bit7：分神驾驶二级报警；
						*bit8：驾驶员异常一级报警； bit9：驾驶员异常二级报警；*/
	/*事件使能*/
	DWORD EventEnable;/*0：关闭 1：打开 bit0：驾驶员更换时间； bit1：主动拍照*/
	/*抽烟报警判断时间阈值*/
	WORD SmokeInter;
	/*打电话报警判断时间阈值*/
	WORD PhoneInter;
	/*保留*/
	BYTE inter[3];
	/*FDW:Fatigue driving warning.疲劳驾驶*/
	/*疲劳驾驶分级速度阈值*/
	BYTE FDWSpeedThres;
	/*疲劳驾驶分级视频录制时长*/
	BYTE FDWRecordingTime;
	/*疲劳驾驶拍照张数*/
	BYTE FDWSnapNum;
	/*疲劳驾驶抓拍间隔*/
	BYTE FDWSnapInter;
	/*打电话报警分级速度阈值*/
	BYTE PhoneSpeedThres;
	/*打电话报警视频录制时长*/
	BYTE PhoneRecordingTime;
	/*打电话拍照抓拍张数*/
	BYTE PhoneSnapNum;
	/*打电话拍照间隔*/
	BYTE PhoneSnapInter;
	/*抽烟报警分级速度阈值*/
	BYTE SmokeSpeedThres;
	/*抽烟报警视频录制时长*/
	BYTE SmokeRecordingTime;
	/*抽烟报警拍照张数*/
	BYTE SmokeSnapNum;
	/*抽烟报警拍照间隔*/
	BYTE SmokeSnapInter;
	/*DDW :分神驾驶报警：Distracted driving Warning*/
	/*分神驾驶报警分级速度阈值*/
	BYTE DDWSpeedThres;
	/*分神驾驶报警视频录制时长*/
	BYTE DDWRecordingTime;
	/*分神驾驶报警拍张张数*/
	BYTE DDWSnapNum;
	/*分神驾驶报警拍照价格*/
	BYTE DDWSnapInter;
	/*ADW:Abnormal driving behavior warning.驾驶行为异常报警*/
	BYTE ADWSpeedThres;
	/*驾驶员行为异常视频录制时长*/
	BYTE ADWRecordingTime;
	/*驾驶员行为异常拍照张数*/
	BYTE ADWSnapNum;
	/*驾驶员行为异常拍照间隔*/
	BYTE ADWSnapInter;
	/*驾驶员身份识别触发*/
	BYTE TriggerAuth;/*0x00:不开启 0x01：定时触发 0x02：定距触发 0x03：插卡开始行使触发*/
	BYTE inter1[3];
}Dsm_Attr;


/*************************************************************************/

/************************* 808电子围栏设置 *************************/
/*表1	区域属性定义 */
/*
---------------------------------------------
|	位	|			标志					|
|-------|------------------------------------
|	0	|1:根据时间						|
|-------|------------------------------------
|	1	|1:限速							|
|-------|------------------------------------
|	2	|1:进区域报警给驾驶员		|
|-------|------------------------------------
|	3	|1:进区域报警给平台		|
|-------|------------------------------------
|	4	|1:出区域报警给驾驶员		|
|-------|------------------------------------
|	5	|1:出区域报警给平台		|
|-------|------------------------------------
|	6	|0:北纬1:南纬					|
|-------|------------------------------------
|	7	|0:东经1:西经					|
|-------|------------------------------------
|	8	|0:允许开门1:禁止开门		|
|-------|------------------------------------
|   9-13	|保留								|
|-------|------------------------------------
|	14	|1:进区域关闭通讯模块		|
|-------|------------------------------------
|	15	|1:进区域采集GNSS详细信息|
---------------------------------------------
*/

typedef struct
{
	/* 以度为单位的的纬度值乘以10的6次方,精确到百万分之一度*/
	DWORD	lattitude;
	/* 以度为单位的的经度值乘以10的6次方,精确到百万分之一度*/
	DWORD	longtitude;
}Gnss_Point;

typedef struct
{
	/* 区域ID */
	DWORD id;
	/* 区域属性 ,见表1*/
	WORD attr;
	/* 开始时间 */
	BCD stime[6];
	/* 结束时间 */
	BCD etime[6];
	/*最高速度值,单位km/h*/
	WORD speedmax;
	/* 超速持续时间,单位秒*/
	BYTE overspeedtime;
	/* 保留字 */
	BYTE reserved[3];
}Region_Base_Attr;//size:24

/* 圆形区域项*/
typedef struct
{
	/* 电子围栏属性 */
	Region_Base_Attr attr;
	/* 中心点*/
	Gnss_Point point;
	/* 半径 */
	DWORD rd;
}Ring_Region_Attr;

/* 矩形区域项*/
typedef struct
{
	/* 电子围栏属性 */
	Region_Base_Attr	attr;
	/* 左上点*/
	Gnss_Point leftup;
	/* 右下点*/
	Gnss_Point rightdown;
}Rectangle_Region_Attr;

/* 多边形区域项*/
typedef struct
{
	/* 电子围栏属性 */
	Region_Base_Attr attr;
	/*顶点数*/
	WORD	num;
	/* 保留字 */
	WORD 	reserved;
	/* 顶点列表 */
	Gnss_Point	node[0];
}Poly_Region_Attr;

/*************************************************************************/


/************************* 808协议线路设置 *************************/
/*表2	路线属性定义 */
/*
---------------------------------------------
|	位	|			标志					|
|-------|------------------------------------
|	0	|1:根据时间 					|
|-------|------------------------------------
|	1	|1:限速 						|
|-------|------------------------------------
|	2	|1:进区域报警给驾驶员		|
|-------|------------------------------------
|	3	|1:进区域报警给平台 	|
|-------|------------------------------------
|	4	|1:出区域报警给驾驶员		|
|-------|------------------------------------
|	5	|1:出区域报警给平台 	|
|-------|------------------------------------
|	6-15	|保留								|
---------------------------------------------
*/

/*表3	路段属性定义 */
/*
---------------------------------------------
|	位	|			标志					|
|-------|------------------------------------
|	0	|1:行驶时间 					|
|-------|------------------------------------
|	1	|保留								|
|-------|------------------------------------
|	2	|0:北纬1:南纬					|
|-------|------------------------------------
|	3	|0:东经1:西经					|
|-------|------------------------------------
|	4-7 |保留								|
---------------------------------------------
*/
/* 线路拐点项信息 */
typedef struct
{
	/* 拐点ID */
	DWORD nodeid;
	/* 路段ID */
	DWORD Sectionid;
	/* 经纬度 */
	Gnss_Point point;
	/* 路段宽度 ,单位米,路段为该拐点到下一拐点*/
	BYTE width;
	/* 路段属性 ,见表3*/
	BYTE attr;
	/* 路段行驶过长阀值,单位秒*/
	WORD drivingmax;
	/*路段行驶不足阀值,单位秒*/
	WORD drivingmin;	
	/*路段最高速度,单位km/h */
	WORD speedmax;
	/* 路段超速持续时间,单位秒 */
	BYTE overspeedtime;
	/* 保留字 */
	BYTE reserved[3];
}Line_Node_Attr;

/* 线路配置项*/
typedef struct
{
	/* 路线ID */
	DWORD id;
	/*路线属性 ,见表2*/
	WORD attr;
	/* 开始时间 */
	BCD stime[6];
	/* 结束时间 */
	BCD etime[6];
	/* 线路总拐点数*/
	WORD num;
	/* 拐点项 */
	Line_Node_Attr node[0];
}Route_Region_Attr;

/*************************************************************************/

/************************* 808协议电话设置 *************************/
/* 电话本配置项 */
typedef struct
{
	/* 标志 1--呼出,2--呼入,3--呼入/呼出*/
	DWORD flag;
	/* 电话号码 */
	char phone[16];
	/*联系人*/
	char name[16];
}Address_Book_Item;
/*************************************************************************/

/************************* 808协议事件设置 *************************/
/* 事件设置项 */
typedef struct
{
	/*事件ID*/
	BYTE id;
	/* 保留字 */
	BYTE reserved[3];
	/*事件内容*/
	char event[32];
}Event_Menu_Item;
/*************************************************************************/

/************************* 808协议点播菜单 *************************/
/* 信息点播菜单项 */
typedef struct
{
	/*信息类型*/
	BYTE type;
	/* 保留字 */
	BYTE reserved[3];
	/*信息名称*/
	char msgname[32];
}Msg_Menu_Item;

/*************************************************************************/

#endif //_CONFIG_COMMON_H

