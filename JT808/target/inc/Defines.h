#ifndef _GNSS_NET_DEFINE_H_
#define _GNSS_NET_DEFINE_H_
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "CommDefine.h"
#include "ConfigCommon.h"
#include "DataCommon.h"
#include "Comm1078Params.h"
#include <vector>
using namespace std;
//#include "ConfigCommon.h"
//#include "DataCommon.h"

#define GNSS_NET_PROC_MSG_HEAD_LEN			17
#define GNSS_NET_PROC_MSG_SUB_HEAD_LEN		16
#define GNSS_NET_PROC_CHECK_SUM_LEN			1

#pragma pack(4)
/* IC卡读卡结果 */
typedef struct
{
	/* 状态 :0x01--IC卡插入,0x02--IC卡拔出*/
	BYTE 		status;
	/* IC卡读取结果:0x00-成功,0x01--认证未通过,0x02--卡锁定,
		0x03--卡拔出,0x04--数据校验错误
	*/
	BYTE 		result;
	/* 读卡时间 */
	BCD			time[6];
}IC_Read_Info;

/**************************终端参数设置**********************************************/
typedef struct
{
	/* 网络通信配置 */
	Net_Communication_Attr 	commCfg;
	/* 中心服务器配置 */
	Center_Server_Attr 		centerSrvCfg;
	/* IC卡认证服务器 */
	IC_Server_Attr			icSrvCfg;
	/* 位置汇报配置 */
	Gnss_Report_Attr       	reportCfg;
	/* 电话相关参数设置 */
	Phone_Number_Attr       phoneNumCfg;
	/* 报警掩码设置 */
	Alarm_Mask_Attr      	alarmMaskCfg;
	/* 报警条件配置 */
	Speed_Alarm_Attr		alarmSpeed;
	Fatigue_Alarm_Attr		alarmFatigue;
	Parking_Alarm_Attr		alarmParking;
	Collision_Alarm_Attr	alarmCollision;
	Rollover_Alarm_Attr		alarmRollover;
	/* 图象抓拍控制 */
	Snap_Ctrl_Attr			snapCtrl;
	/* 图象质量配置 */
	Pic_Quality_Attr		picQuality;
	/* 图象颜色配置 */
	Pic_Color_Attr			picColor;
	/* 终端信息配置 */
	Vehicle_Info   			VehInfoCfg;
	/* GNSS详细信息配置 */
	Gnss_Detail_Attr		gnssDetail;
	/* CAN数据上传配置 */
	CAN_Upload_Attr			canbusUpload;
	/* ADAS params */
	Adas_Attr				AdasCfg;
	/* DSM params */
	Dsm_Attr				DsmCfg;
	/*终端音视频参数设置*/
	Video_Attr              VideoCfg;
	/*参数临时存储*/
	BCD                    cfg_store[128];
}JT808_Config_S;

typedef struct
{
	/* 网络通信配置 */
	Net_Communication_Attr 	commCfg;
	/* 中心服务器配置 */
	Center_Server_Attr 		centerSrvCfg;
	/* IC卡认证服务器 */
	IC_Server_Attr			icSrvCfg;
}NetWork_Param_S;

///////////////////////信息处理////////////////////////////////////////////
/*短消息标志*/
enum SMS_MASK_E
{
	SMS_MASK_ALARM  	= 0x1,          //EMERGENCY
	SMS_MASK_RESERVED 	= 0x2,          //Reservation
	SMS_MASK_SCREEN     = 0x4,          //SCREEN
	SMS_MASK_TTS    	= 0x8,          //TTS
	SMS_MASK_LED    	= 0x10,         //LED
	SMS_MASK_CANBUS   	= 0x20,         //can fault
};
	
/*操作类型*/
enum InfoOp  
{
	INFO_OP_DEL_ALL = 0x0,          //删除终端现有项
	INFO_OP_UPDATE       ,          //跟新
	INFO_OP_APPEND       ,          //追加
	INFO_OP_MODIFY       ,          //修改
	INFO_OP_UNDEFINED    ,
};

/* 透传数据类型 */
enum TRANSPARENT_TYPE_E 
{
	TRANS_GNSS_DATA 	= 0x00,
	TRANS_IC_INFO 		= 0x0B,
	TRANS_Serail1_DATA 	= 0x41,
	TRANS_Serail2_DATA 	= 0x42,
	TRANS_USER_DEF 		= 0xF0
};


typedef struct
{
	DWORD 	mediaId;	//多媒体ID
	BYTE 	type;		// 多媒体类型0:图像;1:音频; 2:视频
	char 	fileName[128];	//满足条件的文件名
	BYTE 	chn;		// 通道id
	BYTE 	event;		// 数据项编码0:平台下发命令;1:定时动作;2:抢劫报警触发;3:碰撞侧翻报警触发;其他保留
	char btime[6];					 // 起始时间 YY-MM-DD-hh-mm-ss
	char etime[6];				     // 结束时间
}IdxCmdRes;


/*储存多媒体数据检索*/
typedef struct
{
	BYTE type;			// 多媒体类型0:图像;1:音频; 2:视频
	BYTE chn;			// 通道id
	BYTE event;			// 数据项编码0:平台下发命令;1:定时动作;2:抢劫报警触发;3:碰撞侧翻报警触发;其他保留
	char btime[6];		// 起始时间 YY-MM-DD-hh-mm-ss
	char etime[6];		// 结束时间
	WORD totalNum;		// 多媒体数据项总数
}NetMdIdxCmd;

/*单条储存多媒体数据检索*/
typedef struct
{
	DWORD 	mediaId;	//多媒体ID
	BYTE	deleteFlag;	//删除标志
}SingleNetMdIdxCmd;

#if 0
/*储存多媒体数据上传命令*/
typedef struct
{
	BYTE type;					     // 多媒体类型0:图像;1:音频; 2:视频
	BYTE chn;						 // 通道id
	BYTE event;						 // 数据项编码
	char btime[6];					 // 起始时间 YY-MM-DD-hh-mm-ss
	char etime[6];				     // 结束时间
	BYTE mark;						 // 删除标志0:保留, 1:删除
}NetMdUploadCmd;
#else
typedef struct
{
	DWORD mId;                        // 多媒体ID
	BYTE type;					     // 多媒体类型0:图像;1:音频; 2:视频
	BYTE enctype;					 //	多媒体编码信息
	BYTE chn;						 // 通道id
	BYTE event;						 // 数据项编码
	char btime[6];					 // 起始时间 YY-MM-DD-hh-mm-ss
	char etime[6];				     // 结束时间
	BYTE mark;						 // 删除标志0:保留, 1:删除
	char filename[256];				 // 多媒体文件名
	char reserve[3];
}NetMdUploadCmd;

/*多媒体重传*/
typedef struct
{
	int media_id;
	BYTE retryPackCnt;
	short retryIdList[256];
}ResendMediaDataCmd;

#endif

/*录音命令*/
typedef struct
{
	 BYTE onoff;                      // 0:off, 1:on
	 WORD lasttime;                   // 录音时长
	 BYTE issave;                     // 0:实时上传, 1:保存
	 BYTE voicefreq;                  // 采样率,0:8k,  1:11k, 2:23k, 3:32k, 其他保留
}NetRecordCmd;

/*摄像头立即拍摄命令*/
typedef struct
{
	WORD wSerialNumber;				 //序列号
	BYTE chn;                        // 通道id
	WORD cmd;                        // 拍摄命令
	WORD period;                     // 拍摄间隔录像时间
	BYTE issave;                     // 保存标志1:保存；0:实时上传

	BYTE res;                		 // 分辨率
	BYTE quality;                    // 图像视频质量
	BYTE brightness;                 // 亮度
	BYTE contrast;					 // 对比度
	BYTE saturation;                 // 饱和度
	BYTE chroma;                     // 色度
}NetMdShotCmd;


/*****************************消息类型*********************************************/
/*消息基类*/
typedef struct 
{
	WORD wId;                			  //消息id
	WORD wSerialNumber;      			  //消息流水号
	BYTE wVer;							  //协议版本号
	WORD wPacketTotal;       			  //封包总数
	WORD wPacketSn;          			  //包序列号
	time_t tLastVisit;                    //最近一次访问的时间
	WORD wTotalLen;		     			  //整个消息长度
}NetMsg, *PNetMsg;

/*接收消息*/
struct NetRcvMsg : NetMsg
{
	WORD wEncryptType;                    //数据加密的方式
	BYTE nCheckCode;          			  //检验码	
	WORD wMsgLen;			              //消息体内容长度
	char* pData;			              //消息体内容
};

/*发送消息*/
struct NetSendMsg : NetMsg
{
	time_t tTimeout;                      //超时时间间隔
	WORD wReSendTimes;       			  //可以重发次数
	char* pData;			  			  //整个消息内容
};

#pragma pack()

typedef enum {						
	MSK_EMERGENCY_BUTTON	= 0x00000001,  	//紧急报警
	MSK_OVER_SPEED			= 0x00000002,	//超速报警
	MSK_TIRE_DRIVING		= 0x00000004,	//疲劳驾驶
	MSK_PREPARATION			= 0x00000008,	//危险预警
	MSK_GNSS_FAILED			= 0x00000010,	//GNSS模块发生故障
	MSK_ANTENNA_OFF			= 0x00000020,	//GNSS 天线未接或被剪断
	MSK_ANTENNA_SHORTCIRCURT= 0x00000040,	//GNSS 天线短路
	MSK_MASTER_POWER_LOW	= 0x00000080,	//终端主电源欠压
	MSK_MASTER_POWER_STOP	= 0x00000100,	//终端主电源掉电
	MSK_LCD_FAILED			= 0x00000200,	//终端LCD 或显示器故障
	MSK_TTS_FAILED			= 0x00000400,	//TTS 模块故障
	MSK_CAMERA_FAILED		= 0x00000800,	//摄像头故障
	MSK_RFID_MODE_FAILED	= 0x00001000,	//道路运输证IC 卡模块故障
	MSK_OVERSPEED_WARING	= 0x00002000,	//超速预警
	MSK_TIREDRIVING_WARING	= 0x00004000,	//疲劳驾驶预警
	MSK_DRIVING_TIMEOUT		= 0x00040000,	//当天累计驾驶超时
	MSK_PARK_TIMEOUT		= 0x00080000,	//超时停车报警
	MSK_INOUT_REGION		= 0x00100000,	//进出区域报警
	MSK_INOUT_PATH			= 0x00200000,	//进出线路报警
	MSK_SEG_TIMEOUT			= 0x00400000,	//路段行驶时间不足/过长
	MSK_LEAVE_PATH			= 0x00800000,	//路线偏离报警
	MSK_VSS_FAILED			= 0x01000000,	//车辆VSS 故障
	MSK_OIL_FAILED			= 0x02000000,	//车辆油量异常
	MSK_BE_ROB				= 0x04000000,	//车辆被盗(通过车辆防盗器)
	MSK_ABNORMAL_FIRE		= 0x08000000,	//车辆非法点火
	MSK_ABNORMAL_MOVE		= 0x10000000,	//车辆非法位移
	MSK_VEHICLE_COLLIDE		= 0x20000000,	//碰撞预警
	MSK_VEHICLE_WELTER		= 0x40000000,	//侧翻预警
	MSK_ABNORMAL_OPENDOOR	= 0x80000000,  	//非法开门报警（终端未设置区域时，不判断非法开门）	
} AlarmMaskBit;

/* 超速报警附加信息 */
struct Alarm_Speed_S
{
	/* 位置类型:0--无特定位置;1--圆型区域;2--矩形区域;3--多边形区域;4--路段 */
	char position;
	char reserved[3];
	/* 区域或路段ID */
	int id;		
};

/* 进出区域或线路附加信息*/
struct Alarm_InOut_S
{
	/* 位置类型:0--无特定位置;1--圆型区域;2--矩形区域;3--多边形区域;4--路段 */
	char position;
	/* 进出方向:0--进,1--出 */
	char inout;
	char reserved[2];
	/* 区域或线路ID */
	int id;
};

/* 路段行驶时间不足或过长附加信息*/
struct Alarm_RtSecTm_S
{
	/* 路段ID*/
	int id;
	/* 路段行驶时间*/
	int time;
	/* 结果:0--不足,1--过长*/
	char result;
	char reserved[3];
};

struct Alarm_Flag_S
{
	/*终端ID*/
	BYTE	id[7];
	/*YY-MM-DD-hh-mm-ss GMT+8*/
	BCD		datetime[6];
	/*同一时间点报警的序号，从0循环累加*/
	BYTE	sequence;
	/*附件数量，标识该报警对应的附件数量*/
	BYTE	attachment_num;
	/*预留*/
	BYTE	reserved;
};


#define  FILE_DATA_UNIT_SIZE		(1024)
#define  FILE_NAME_LEN				(50)
#define  FILE_NAME_LEN_MAX			(128)
struct File_Data_S
{
	union {
		DWORD dFlag;
		BYTE  bFlag[4];
	};
	BYTE filename[50];
	DWORD offset;
	DWORD length;
	char data[FILE_DATA_UNIT_SIZE];
};


struct File_Info_s {
    short nIndex; //序号从1开始
    short nType; //类型，音频、视频、文本等
    char szFileName[128]; //带完整路径
};

/*dsm 报警附加信息*/
struct  Alarm_Dsm_S
{
	/*报警ID*/
	DWORD	id;
	/*开始 结束标识 0x0:不可用 0x1:开始 0x2:结束, 仅针对有开始/结束的报警*/
	BYTE	flag;
	/*报警类型*/
	BYTE	type;
	/*报警级别 0x01:1级 0x02:2级*/
	BYTE	level;
	/*疲劳程度1~10  报警类型为0x01时有效*/
	BYTE	tired_level;
	BYTE	reserved[4];
	/*速度 Km/h, 0~250*/
	BYTE	speed;
	/*海拔高度 m*/
	WORD	altitude;
	/*纬度 以度为单位剩以10的6次方，精确到百万分之一度*/
	double	latitude;
	/*经度 以度为单位剩以10的6次方，精确到百万分之一度*/
	double	longitude;
	/*YY-MM-DD-hh-mm-ss GMT+8*/
	BCD		datetime[6];
	/*车辆状态*/
	WORD	vechile_status;
	/*报警标识号*/
	Alarm_Flag_S	alarm_flag;
	/*face auth result*/
	BYTE result;/*0:succ;1:fail;2:timeout 3:disable*/
	BYTE similarity;
	WORD similar_degree;
	BYTE cmp_type;//0:plug-in card;1:timer;2:start;3:leave;4:trigger
	BYTE dirver_id_len;
	char dirver_id[32];
	
};

struct Alarm_Adas_S
{
	/*报警ID*/
	DWORD	id;
	/*开始 结束标识 0x0:不可用 0x1:开始 0x2:结束, 仅针对有开始/结束的报警*/
	BYTE	flag;
	/*报警类型*/
	BYTE	type;
	/*报警级别 0x01:1级 0x02:2级*/
	BYTE	level;
	/*前车车速 单位：Km/h. 范围0~250，仅报警类型为0x01和0x02时有效*/
	BYTE	speedFront;
	/*前车/行人距离 单位：100ms. 范围0~100，仅报警类型为0x01、0x02和0x04时有效*/
	BYTE	distanceFront;
	/*偏离类型 0x01:左侧偏离   0x02:右侧偏离 仅报警类型为0x02时有效*/
	BYTE	deviationType;
	/*道路标志识别类型 0x01:限速标志 0x02:限高标志 0x03:限重标识*/
	BYTE	roadFlag;
	/*识别到道路标识数据*/
	BYTE	roadFlagData;
		/*速度 Km/h, 0~250*/
	BYTE	speed;
	/*海拔高度 m*/
	WORD	altitude;
	/*纬度 以度为单位剩以10的6次方，精确到百万分之一度*/
	double	latitude;
	/*经度 以度为单位剩以10的6次方，精确到百万分之一度*/
	double	longitude;
	/*YY-MM-DD-hh-mm-ss GMT+8*/
	BCD		datetime[6];
	/*车辆状态*/
	WORD	vechile_status;
	/*报警标识号*/
	Alarm_Flag_S	alarm_flag;
};

struct AlarmData_s
{
	/* 报警标志 */
	int alarm;
	/* 设备状态 */
	int status;
	/* Gnss信息 */
	struGPSData gnss;
	/* 车辆来源的速度 */
	int vehichleSpeed;
	/* 超速报警附加信息 */
	Alarm_Speed_S	speed;
	/* 进出区域附加信息 */
	Alarm_InOut_S	areaIO;
	/* 进出线路附加信息 */
	Alarm_InOut_S	routeIO;
	/* 路段行驶时间不足或过长附加信息 */
	Alarm_RtSecTm_S	rtSecTm;
	/*扩展报警类型,若无，必须设置为EXTEND_ALARM_NONE*/
	int extend_type;
	//报警信息
	union{
		Alarm_Dsm_S dsm_alarm;
		Alarm_Adas_S adas_alarm; 
	};
};

struct File_Server_Config
{
	vector<File_Info_s> fileList;
	/*服务器地址*/
	BYTE szAddr[64];
	/*TCP 端口*/
	WORD nTcpPort;
	/*UDP 端口*/
	WORD nUdpPort;
	/*报警标识号*/
	Alarm_Flag_S	alarm_flag;
	/*报警编号*/
	BYTE alarmCode[32];
	BYTE reserved[16];
	bool bReSend;
	int  nResetIndex;
	int nDevType; /*0x64: adas, 0x65:dsm, 0x66:TPT, 0x67:BSD*/
	int nAlarmType;
};


enum NetFrameType_E
{
	AV_FRAME_I = 0,
	AV_FRAME_P,
	AV_FRAME_B,
	AV_FRAME_A,
};

//一个视频帧或音频帧在缓冲池的信息结构体
struct FrameInfo_S
{
	int				frmLen; /*此帧的有效数据长度*/
	long long		pts;	/*时间戳*/
	unsigned char	flag;	/*I 帧, P 帧, 音频帧*/
	unsigned char	hour;	/*产生此帧的时间*/
	unsigned char	min;
	unsigned char	sec;
	char			szData[0];	/*音视频数据帧*/
};

enum {
	DEV_NONE = 0,
	DEV_ADAS = 0x64,	/*高级辅助驾驶*/
	DEV_DSM = 0x65,		/*驾驶员状态监测*/
	DEV_TPT = 0x66, 	/*胎压监测*/
	DEV_BSD = 0x67,		/*盲区监测*/
};

enum {
	FILE_TYPE_IMAGE = 0x00,
	FILE_TYPE_AUDIO = 0x01,
	FILE_TYPE_VIDEO = 0x02,
	FILE_TYPE_TEXT = 0x03,
	FILE_TYPE_OTHER = 0x05,
};

struct FileInfoS
{
	BYTE src_file[FILE_NAME_LEN_MAX];
	BYTE name[FILE_NAME_LEN_MAX];
	BYTE name_len;
	BYTE type;
	off_t size;
};

struct DSM_Alarm_Data_S {
    int nType;    //报警类型
    int nLevel;   //报警级别
    int nTiredLevel; //疲劳等级

	/*face auth result*/
	BYTE result;/*0:succ;1:fail;2:timeout 3:disable*/
	BYTE similarity;
	WORD similar_degree;
	BYTE cmp_type;//0:plug-in card;1:timer;2:start;3:leave;4:trigger
	BYTE dirver_id_len;
	char dirver_id[32];
};

struct ADAS_Alarm_Data_S {
    int nType;    //报警类型
    int nLevel;   //报警级别
    /*前车车速 单位：Km/h. 范围0~250，仅报警类型为0x01和0x02时有效*/
	int	speedFront;
	/*前车/行人距离 单位：100ms. 范围0~100，仅报警类型为0x01、0x02和0x04时有效*/
	int	distanceFront;
	/*偏离类型 0x01:左侧偏离   0x02:右侧偏离 仅报警类型为0x02时有效*/
	int	deviationType;
	/*道路标志识别类型 0x01:限速标志 0x02:限高标志 0x03:限重标识*/
	int	roadFlag;
	/*识别到道路标识数据*/
	int	roadFlagData;
};

struct Alarm_S {
    int nDevType; /*0x64: adas, 0x65:dsm, 0x66:TPT, 0x67:BSD*/
    char datetime[6]; //bcd格式的时间
    unsigned short sequence;
    struGPSData gpsData;
    vector<File_Info_s> fileList;
    union{
        DSM_Alarm_Data_S dsm_alarm;
        ADAS_Alarm_Data_S adas_alarm;
    };
};

struct FacePic_s{
	WORD	MsgSerialNum;
	unsigned int opsMode;
	char	driver_id[64];
	char	driver_name[64];
	char	pic_url[512];
	char	pic_src;
	char	pic_type ;
	DWORD	pic_size;
	char	pic_name[128];
	WORD	up_left_coordi[2];	
	WORD	down_right_coordi[2];
	WORD	left_eye_center_coordi[2];
	WORD	right_eye_center_coordi[2];
	WORD	nose_coordi[2];
	WORD	left_mouth_coordi[2];
	WORD	right_mouth_coordi[2];
	char	pic_serial;
	char	pic_total;
	time_t  cur_time;
};

struct Files_Map_S {
	int  nDevType;
	int  nAlarmType;
	time_t time;
	BYTE bcdTime[6];
	BYTE nSeq;
	vector<File_Info_s> fileList;
};

struct ReSendData_s
{
	DWORD nOffset;
	DWORD nLen;
};

struct File_Finish_Respone_s
{
	char szFileName[FILE_NAME_LEN];
	int  nType;
	int  nResult;
	int  nReSendPackageNum;
	vector<ReSendData_s> reSendDataList;
};

struct FileUploadReply_S {
	bool bReply;
	char szFileName[FILE_NAME_LEN_MAX];
};

struct ComReply_S {
	unsigned int nSerailNum;
	bool bReply;
};

#define 	START_RECORD_VIDEO	(0xFFFF)
#define 	STOP_SHOT			(0x00)
#define 	SNAP_FACE_AUTH		(0x11)
#define		PICTRUE_TYPE_JPEG	0
#define		PICTRUE_TYPE_PNG	1

#define 	CHN_NUM		(5)

typedef struct SingleFileData_t{
	int media_id;
	int packetid;
	char data[1000];	//分包数据
}SingleFileData;


struct ShotResult{
	int media_id;
	char filename[256];
	time_t pts;

	DWORD mId;						 // 多媒体ID
	BYTE type;						 // 多媒体类型0:图像;1:音频; 2:视频
	BYTE enctype;					 // 多媒体编码信息
	BYTE chn;						 // 通道id
	BYTE event; 					 // 数据项编码
	char btime[6];					 // 起始时间 YY-MM-DD-hh-mm-ss
	char etime[6];					 // 结束时间
	BYTE mark;						 // 删除标志0:保留, 1:删除
};

struct FacePictureInfo{
	BYTE pic_url_len;
	BYTE pic_url[256];
	BYTE pic_name_len;
	char pic_name[128];
	BYTE pic_type;//00:JPG  01:PNG 02:others
	DWORD pic_size;
	WORD up_left_coordi[2];	
	WORD down_right_coordi[2];
	WORD left_eye_center_coordi[2];
	WORD right_eye_center_coordi[2];
	WORD nose_coordi[2];
	WORD left_mouth_coordi[2];
	WORD right_mouth_coordi[2];
};

struct FaceInfo{
	BYTE face_id_len;
	char face_id[64];
	BYTE name_len;
	char name[64];
	BYTE download_proc;//0:ftp 1:http
	
	BYTE pic_src;//0:local 1:third party
	BYTE pic_num;
	struct FacePictureInfo pic_info[8];
};

typedef struct FacePicPublish_t{
	WORD wSerialNumber;
	BYTE cmd_type;//0x0:add, 0x1:delete, 0x2:delete the item, 0x3:update
	BYTE list_num;//driver number

	struct FaceInfo face_info[8];//face list details.
}FacePicPublish;

typedef struct FacePicDownloadReply_t{
	WORD serial;
	BYTE result;
	BYTE download_num;
	BYTE cur_download;
	BYTE face_id_len;
	BYTE face_id[64];	
}FacePicDownloadReply;

typedef struct FaceCmpResult_t{
	BYTE result;
	BYTE similarity;
	WORD similar_degree;
	BYTE cmp_type;//0:plug-in card;1:timer;2:start;3:leave;4:trigger
	BYTE ID_len;
	char ID[64];
	BYTE pic_type;
	char pic_name[128];
	uint32 pts;
}FaceCmpResult;

typedef struct FaceIdInfo_t{
	BYTE Id_len;
	BYTE Id[64];	
}FaceIdInfo;

typedef struct FaceIDInfo_t{
	BYTE face_num;
	FaceIdInfo face_info[24];		
}FaceIDInfo;

struct PackageSendMsg {
	char strEvPackage[2048];
	WORD nPackageSize;
	WORD nSerialNum;
};

typedef struct FaceCmpPubResult_t {
	BYTE result;
	BYTE similarity;
	WORD similar_degree;
	BYTE ID_len;
	char ID[64];
} FaceCmpPubResult;

typedef struct UploadFacePic_t{
	WORD serial;
	bool PicUpdate;
	char pic_type;//0:jpeg 1:png
	char pic_name[128];	
	
	unsigned int ts;//timestamp
	pthread_mutex_t Upload_mutex;
}UploadFacePic;

typedef struct RetransmitPacket_t{
	WORD RetransmitTotal;
	WORD RetransmitId[32];
}RetransmitPacket;

typedef struct TextInfo_t{
	BYTE flag;
	char text[1024];
}TextInfo;

typedef struct TTSInfo_t{
	char  app_id[32];
	char  api_key[64];
	char  secrect_key[64];
	char  spd[4];
	char  pit[4];
	char  vol[4];
	char  per[4];
}TTSInfo;


typedef void (*SetTerminalParam_CallBack)(JT808_Config_S *cfg);

////////////////////////////////	 获取视频数据回调函数指针
typedef void(*pFunPutFrameCallBack)(void * pUser, FrameInfo_S* pFrameInfo,void* pData);
////////////////////////////////	 抓拍回调函数
typedef bool (*SnapShot_Callback)(int dev, unsigned short cmd, unsigned short period, unsigned int *pts, char *file_name);
typedef bool (*FaceSnap_Callback)(int dev, unsigned int *pts, char *file_name);
typedef void (*GetGPS_Callback)(struGPSData *gpsData);
typedef void (*FacePic_Callback)(FacePicPublish *facePicDate);
typedef void (*FacePicCmpRes_Callback)(FaceCmpPubResult *fCmpPbData);
typedef void (*FaceInfo_Callback)(void *pdata);
typedef void (*TriggerFaceCmp_Callback)(void *pdata);
typedef void (*VideoCfgGet_Callback)(void *pdata);
typedef void (*VideoReqStream_Callback)(void *pdata);
typedef void (*VideoTransCtrl_Callback)(void *pdata);
typedef void (*VideoQueSource_Callback)(void *pdata, void *upload_list);
typedef void (*VideoReqPlayback_Callback)(void *pdata, void *upload_list);
typedef void (*VideoPlaybackCtrl_Callback)(void *pdata);
typedef void (*VideoUploadFile_Callback)(void *pdata);
typedef void (*VideoCtrlUpload_Callback)(void *pdata);
typedef void (*TextInfo_Callback)(void *pdata);

#endif	/* _GNSS_NET_DEFINE_H_ */
