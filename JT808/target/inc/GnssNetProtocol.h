#ifndef _GNSS_MOC_NET_PROTOCOL_H_
#define _GNSS_MOC_NET_PROTOCOL_H_

#include <time.h>
#include "Defines.h"


#pragma pack(1)




///////////////////////////////////////////////中交 2012/12/08//////////////////////////////////////////////////////////////

#define NET_CMD_HEADFLAG			"~"
#define NET_CMD_TAILFLAG			"~"
#define NET_CMD_FLAG                "~"
#define NET_PKG_MSG_BODY_LEN        1020

#define NET_CMD_FLAGLEN             1       //strlen("~")    
#define NET_CMD_HEADFLAGLEN		    1		//strlen("~")
#define NET_CMD_TAILFLAGLEN		    1		//strlen("~")



enum tagNET_CMD_TYPE_E
{
	NET_CMD_REPLY     		    = 0x0001,       //终端通用应答
	NET_CMD_CMS_REPLY  			= 0x8001,	    //平台通用应答
	NET_CMD_HEARTBEAT           = 0x0002,		//终端心跳         
	NET_CMD_REGISTER 		    = 0x0100,		//终端注册
	NET_CMD_REGISTER_REPLY      = 0x8100,		//终端注册应答
	NET_CMD_UNREGISTER 		    = 0X0003,		//终端注销
	NET_CMD_AUTHORIZE   		= 0X0102,		//终端鉴权
	NET_CMD_CONFIG_SET  		= 0X8103,		//设置终端参数
	NET_CMD_CONFIG_GET  		= 0X8104,		//查询终端参数
	NET_CMD_CONFIG_GET_REPLY 	= 0X0104,		//查询终端参数应答
	NET_CMD_CTRL           		= 0X8105,		//终端控制

	NET_CMD_POS_INFO_UP         = 0X0200,		//位置信息上报
	NET_CMD_POS_INFO_GET   		= 0X8201,		//位置信息查询
	NET_CMD_POS_INFO_GET_REPLY  = 0X0201,		//位置信息查询应答
	NET_CMD_LOCATION_TRACKING   = 0X8202, 		//临时位置跟踪控制
	NET_CMD_TEXT_INFO_DOWN      = 0X8300,       //文本信息下发
	NET_CMD_EVENT_SET           = 0X8301,       //事件设置
	NET_CMD_EVENT_UP            = 0X0301,		//事件报告
	NET_CMD_ASK_DOWN            = 0X8302,		//提问下方
	NET_CMD_ASK_REPLY           = 0X0302,		//提问应答
	NET_CMD_INFO_OD_MENU_SET    = 0X8303,		//信息点播菜单设置
	NET_CMD_INFO_OD_CANCEL      = 0X0303,       //信息点播取消

	NET_CMD_INFO_SERVICE        = 0X8304,       //信息服务
	NET_CMD_TEL_CALLBACK        = 0X8400,		//电话回拨
	NET_CMD_TELBOOK_SET         = 0X8401,		//设置电话本
	NET_CMD_VEHICLE_CTRL        = 0X8500,		//车辆控制
	NET_CMD_VEHICLE_CTRL_REPLY  = 0X0500,		//车辆控制应答
	NET_CMD_CIRCLE_AREA_SET     = 0X8600,		//设置圆形区域
	NET_CMD_CIRCLE_AREA_DEL     = 0X8601,		//删除圆形区域
	NET_CMD_RECT_AREA_SET       = 0X8602,		//设置矩形区域
	NET_CMD_RECT_AREA_DEL       = 0X8603,		//删除矩形区域
	NET_CMD_POLYGON_AREA_SET    = 0X8604,		//设置多边形区域
	NET_CMD_POLYGON_AREA_DEL    = 0X8605,		//删除多边形区域
	NET_CMD_PATH_SET            = 0X8606,		//设置路线
	NET_CMD_PATH_DEL            = 0X8607,		//删除路线
	NET_CMD_DRIVING_DATA_PICK   = 0X8700,		//行驶记录仪数据采集命令
	NET_CMD_DRIVING_DATA_UP     = 0X0700,		//行驶记录仪数据上传
	NET_CMD_DRIVING_PARAM_DOWN  = 0X8701,		//行驶记录仪参数下传命令
	NET_CMD_ELECT_BILL_UP       = 0X0701,		//电子运单上报
	NET_CMD_DRIVER_IDINFO_UP    = 0X0702,		//驾驶员身份信息采集上报
	NET_CMD_MULTIMEDIA_EVENT_UP = 0X0800,		//多媒体事件信息上报
	NET_CMD_MUTIMEDIA_DATA_UP   = 0X0801,		//终端多媒体数据上传
	NET_CMD_MUTIMEDIA_DATA_UP_REPLY			= 0X8800,	//平台多媒体数据上传应答
	NET_CMD_CAMERA_SHOT						= 0X8801,       //摄像头立即拍摄命令 
	NET_CMD_CAMERA_SHOT_REPLY				= 0X0805,		//摄像头立即拍摄命令应答 
	NET_CMD_STORE_MUTLIMEDIA_SEARCH			= 0X8802,       //储存多媒体数据检索
	NET_CMD_STORE_MUTLIMEDIA_SEARCH_REPLY	= 0X0802,		  //储存多媒体数据检索应答
	NET_CMD_STORE_MUTLIMEDIA_UP				= 0X8803,				  //储存多媒体数据上传
	NET_CMD_RECORD_SOUND_START				= 0X8804,				  //录音开始命令
	NET_CMD_DATA_DOWN_TAANSPARENT_TRANS		= 0X8900,		  //数据下行透传
	NET_CMD_DATA_UP_TAANSPARENT_TRANS		= 0X0900,		  //数据上行透传
	NET_CMD_DATA_COMPRESS_UP				= 0X0901,                 //数据压缩上报
	NET_CMD_CMS_RSA							= 0X8A00,	              //平台RSA 公钥
	NET_CMD_RSA								= 0X0A00,				  // 终端RSA公钥

	NET_CMD_CMS_DOWN_RESERVED_BEG			= 0X8F00,               //平台下行消息保留
	NET_CMD_CMS_DOWN_RESERVED_END			= 0X8FFF,
			
	NET_CMD_UP_RESERVED_BEG					= 0X0F00,               //终端上行消息保留
	NET_CMD_UP_RESERVED_END					= 0X0FFF,

    //新部标协议增加部分---待实现
	NET_CMD_SOFTWARE_UPGRADE				= 0x8108,   		//下发终端升级包
	NET_CMD_SOFTWARE_UPGRADE_REPLY			= 0x0108,  	 //终端升级结果通知
	NET_CMD_ALARM_CONFIRM					= 0x8203,   			//人工确认报警消息
	NET_CMD_DRIVER_INFO_REQUEST				= 0x8702,    	//上报驾驶员身份信息请求
	NET_CMD_DRIVER_INFO_REPLY				= 0x0702 ,     	//驾驶员身份信息采集上报
	NET_LOCATION_DATA_BATCH_UPLOAD			= 0x0704,   	//定位数据批量上传
	NET_CANBUS_DATA_BATCH_UPLOAD			= 0x0705, 		//CAN 总线数据上传
	NET_CMD_CAMERA_SNAPSHOT_REPLY			= 0x0805,	//摄像头立即拍摄命令应答
	NET_CMD_SINGLE_MUTLIMEDIA_UPLOAD		= 0x8805,	//单条存储多媒体数据检索上传命令
	NET_CMD_ENQUIRY_DEVICE_PARAM			= 0x8106,   		//查询指定终端参数
	NET_CMD_ENQUIRY_DEVICE_PARAM_REPLY		= 0x0104,   //查询指定终端参数应答
	NET_CMD_PACKAGE_TRANS					= 0x8003,   //平台补传分包请求
	NET_CMD_PACKAGE_TRANS_REPLY				= 0X0005,	//终端补传分包请求
	NET_CMD_DEV_PROPERTY					= 0x8107,       		//查询终端属性
	NET_CMD_DEV_PROPERTY_REPLY				= 0x0107,    		//查询终端属性应答

	//基于808增加的扩展协议---待实现
	NET_CMD_FTP_INFO_REQUEST				= 0x3030,
	NET_CMD_FTP_INFO_REPLY					= 0xB030,
	NET_CMD_UP_DONE_EVENT					= 0x3031,
	NET_CMD_CAR_INFO_QUERY					= 0x4040,

	NET_CMD_FILE_SERVER_INFO				= 0x9208,				  //平台下发附件上传指令
	NET_CMD_ALARM_ATTACHMENT_INFO_UPLOAD	= 0x1210,		 //报警附件信息上传指令
	NET_CMD_FILE_INFO_UPLOAD				= 0x1211,				//报警附件信息上传完成指令
	NET_CMD_FILE_UPLOAD_FINISHED			= 0x1212,				//报警附件信息上传完成指令
	NET_CMD_FILE_UPLOAD_FINISHED_REPLY		= 0x9212,		//报警附件信息上传完成应答

	/*Face Recognition Protocol.*/
	NET_CMD_FACE_CMP_RESULT_UP				= 0x0E10,//人脸比对结果上报
	NET_CMD_FACE_CMP_RESULT_UP_REPLY		= 0x8E10,//人脸比对结果上报应答
	NET_CMD_FACE_PIC_PUBLISH				= 0x8E11,//人脸图片下发
	NET_CMD_FACE_PIC_DOWNLOAD_REPLY			= 0x0E11,//人脸图片下载应答
	NET_CMD_FACE_INFO_QUERY					= 0x8E12,//人脸信息查询
	NET_CMD_FACE_INFO_QUERY_REPLY			= 0x0E12,//人脸信息查询应答
	NET_CMD_FACE_CMP_TRIGGER				= 0x8E13,//face compare active triggering
	NET_CMD_FACE_CMP_RESULT_PUB_REPLY		= 0x8E14,//人脸比对结果下发应答
	NET_CMD_FACE_AUTH_ACTIVE_SNAP			= 0x8E15,//人脸认证照片主动抓拍	
	NET_CMD_FACE_AUTH_UPLOAD_REPLY			= 0x8E16,//Upload Face Authentication Picture Response
	NET_CMD_FACE_AUTH_UPLOAD				= 0x0E16,//Upload Face Authentication Picture
	/*End.*/
	
	/*Audio-Video Communication.*/
	NET_CMD_AV_CONF_GET               	    = 0x9003,//查询终端音视频属性
	NET_CMD_AV_CONF_UPLOAD               = 0x1003,//终端上传音视频属性
	NET_CMD_AV_UPLOAD_PASSENGER_NUM      = 0x1005,//终端上传乘客流量
	NET_CMD_AV_REQ_STREAM                = 0x9101,//实时音视频传输请求
	NET_CMD_AV_CTRL_TRANS                = 0x9102,//音视频实时传输控制
	NET_CMD_AV_TRANS_STATUS              = 0X9105,//实时音视频传输状态通知
	NET_CMD_AV_QUE_RESOURCE_LIST         = 0x9205,//查询视频资源列表
	NET_CMD_AV_UPLOAD_RESOURCE_LIST      = 0x1205,//终端上传音视频资源列表
	NET_CMD_AV_REQ_PLAYBACK              = 0x9201,//平台下发远程录像回放请求
	NET_CMD_AV_PLAYBACK_CTRL             = 0x9202,//平台下发远程录像回放控制
	NET_CMD_AV_UPLOAD_FILE               = 0x9206,//文件上传指令
	NET_CMD_AV_UPLOAD_FILE_FINISH        = 0x1206,//文件上传完成通知
	NET_CMD_AV_UPLOAD_FILE_CTRL          = 0x9207,//文件上传控制
	/*End.*/
};

/*透传类型*/
enum TRANS_TYPES
{
	POS_DATA  = 0x00,	//GPS /北斗定位数据
	DRIVER_INFO =0x0B,   //驾驶员资格证信息
	USART1_TRANS=0x41,   // 串口1数据
	USART2_TRANS=0x42,   // 串口2数据
	USER_CUSTOM=0x0F,	//用户自定义
};


/* 中心回复状态*/
enum tagNET_CMD_REPLY_E
{
	NET_CMD_REPLY_SUCCESS= 0,	//应答成功
	NET_CMD_REPLY_FAILURE,      //应答失败
	NET_CMD_REPLY_MSGERR ,      // 应答消息错误
	NET_CMD_REPLY_NOTSUPPORT,   // 不支持
	NET_CMD_REPLY_ALARM_VERIFY, // 报警处理确认
};

enum tagNET_REGISTER_REPLY_E
{

	SERVER_REPLY_REGISTER_SUCCESS= 0,  //注册成功
	SERVER_REPLY_VEHICLE_REGISTERED,   //车辆已被注册
	SERVER_REPLY_NO_VEHICLE ,	   	   //数据库中无该车辆
	SERVER_REPLY_TERMINAL_REGISTERED,  //终端已被注册
	SERVER_REPLY_NO_TERMINAL,  		   // 数据库中无该终端

};

enum tagNET_DRIVING_DATA_TYPE_E
{
	NET_RECORD_DRIVER_INFO         =  0x1,  // 司机信息
	NET_RECORD_REALTIME_CLOCK      =  0x2,  // 记录仪实时时钟
	NET_RECORD_15DAY_MILEAGE       =  0x3,  // 360小时累计行驶里程
	NET_RECORD_SENSOR_ROUND        =  0x4,  // 车辆特征系数
	NET_RECORD_15DAY_SPEED         =  0x5,  //  360 小时内行驶速度
	NET_RECORD_VEHICLE_INFO        =  0x6,  // 车辆信息
	NET_RECORD_STOP_DATA           =  0x7,  // 记录仪事故疑点数据
	NET_RECORD_2DAY_MILEAGE        =  0x8,  // 最近2个工作日内累计行驶里程
	NET_RECORD_2DAY_SPEED          =  0x9,  // 最近2个工作日内累计行驶速度
	NET_RECORD_TIMEOUT             =  0x11, // 超时驾驶记录

};

/*消息头*/
typedef struct 
{
	WORD wId;                  //消息id
	WORD wProperty;			   //消息体属性
	BYTE wVer;				   //协议版本号
	BYTE bTermMobileNo[10]; 	   //终端手机号
	WORD wSerialNumber;        //消息流水号
}__attribute__((packed)) NetMsgHead;


/*消息包 封装项*/
typedef struct 
{
	WORD wPacketTotal;        // 封包总数
	WORD wSerialNuber;        // 包序列号
}__attribute__((packed)) NetMsgHeadPack;



/* 消息体属性*/
typedef union
{
  struct
   {
    WORD
	     wMsgLen:10,          //消息体的长度
	     wEncryptType:3,      //数据加密的方式
	     wPack:1,		      //分包
	     wVer:1,			  //版本标志
	     wReserved:1;          //保留   
   } m_MsgAttr;
  
   WORD MsgAttr;
   
} NET_MSG_ATTR_S;


/*中心回复*/
typedef struct tagNET_MSG_CMS_REPLY_S
{
	WORD wSerialNumber;       // 消息流水号
	WORD wId;                 // 消息id
	BYTE bResult;             // 结果
}__attribute__((packed)) NET_MSG_CMS_REPLY_S;


#endif


