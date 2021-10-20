#ifndef __PARAM_INTERFACE_H__
#define __PARAM_INTERFACE_H__

#include <stdio.h>
#include <stdarg.h>
#include <list>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "Interface.h"
#include "TRML_tinyxml2.h"

#define XML_ROOT   				"CFG"
#define XML_SUBROOT_CONTROL		"Control"
#define XML_GPS_REPORT_TIME		"gps_report_time"
#define XML_ENABLE				"enable"
#define XML_SUBROOT_TERMINAL    "Terminal"
#define XML_TERMINAL_FLAG		 "flag"
#define XML_TERMINAL_PHONE	 	 "phone"
#define XML_TERMINAL_DEVID		 "dev_id"
#define XML_TERMINAL_DEVTYPE	 "dev_type"

#define XML_SUBROOT_DEV_PARAM       "DevParam"
#define XML_DEV_PARAM_PRE_SIM       "PreSim"
#define XML_DEV_PARAM_OVER_SPEED    "OverSpeed"
#define XML_DEV_PARAM_VIDEOENABLE   "videoEnable"
#define XML_DEV_PARAM_IMGENABLE     "imgEnable"

#define XML_SUBROOT_PLATFORM     "Platform"
#define XML_PLATFORM_SESSION     "Session"
#define XML_PLATFORM_DSM         "DSM"
#define XML_PLATFORM_ADAS        "ADAS"
#define XML_SESSION_SERVER_URL   "url"
#define XML_SESSION_SERVER_PORT  "port"
#define XML_SUB_BASIC            "Basic"
#define XML_BASIC_SPEED_THRES	 "speed_thres"
#define XML_BASIC_ALARM_VOLUME	 "alarm_volume"
#define XML_BASIC_AUTO_SNAP		 "auto_snap"
#define XML_BASIC_TACTICS    	 "tactics"
#define XML_BASIC_INTERVAL_TIME  "interval_time"
#define XML_BASIC_INTERVAL_DIS   "interval_distance"
#define XML_BASIC_SNAP_NUM       "snap_num"
#define XML_BASIC_SNAP_INTERVAL  "snap_interval"
#define XML_BASIC_PHOTO_RES	     "photo_res"
#define XML_BASIC_VIDEO_RES      "video_res"

#define XML_SUB_ALARM            "Alarm"
#define XML_ALARM_FATIGUE		 "Fatigue"
#define XML_ALARM_TELEPHONE		 "Telephone"
#define XML_ALARM_SMOKING	     "Smoking"
#define XML_ALARM_DISTRACTION    "Distraction"
#define XML_ALARM_ABNORMAL		 "Abnormal"
#define XML_ALARM_ONEHAND_SW	 "OnehandSW"
#define XML_ALARM_HANDSOFF_SW	 "HandsoffSW"
#define XML_ALARM_SUNGCLASS		 "Sungclass"
#define XML_ALARM_BELT			 "Belt"

#define XML_ALARM_IDENTIFY		 "Driver_Identify"
#define XML_ALARM_LDW            "ldw"
#define XML_ALARM_FCW            "fcw"
#define XML_ALARM_HMW            "hmw"
#define XML_ALARM_LEVEL_ONE		 "level1"
#define XML_ALARM_LEVEL_TWO		 "level2"
#define XML_ALARM_SPEED_THRES	 "speed_thres"
#define XML_ALARM_REC_TIME		 "rec_time"
#define XML_ALARM_SNAP_NUM       "snap_num"
#define XML_ALARM_UP_VIDEO       "up_video"
#define XML_ALARM_SNAP_INTERVAL  "snap_interval"
#define XML_ALARM_SMOKE_INTERVAL "smoking_interval"
#define XML_ALARM_PHONE_INTERVAL "phone_interval"

//load terminal config
#define XML_SUBROOT_COMMCFG			"NetCommunication"
#define XML_COMMCFG_HEARTBEAT		"heart_beat"
#define XML_COMMCFG_TCP_TIMEOUT		"tcp_timeout"
#define XML_COMMCFG_TCP_TRYTIMES	"tcp_trytimes"
#define XML_COMMCFG_UDP_TIMEOUT		"udp_timeout"
#define XML_COMMCFG_UDP_TRYTIMES	"udp_trytimes"
#define XML_COMMCFG_SMS_TIMEOUT		"sms_timeout"
#define XML_COMMCFG_SMS_TRYTIMES	"sms_trytimes"
#define XML_COMMCFG_AUTHEN_KEY		"authen_key"
#define XML_COMMCFG_PHONE_NUM		"phone_num"

#define XML_SUBROOT_CENTERSRVCFG		"CenterServer"
#define XML_CENTERSRVCFG_MAIN_APN		"main_apn"
#define XML_CENTERSRVCFG_MAIN_USERNAME	"main_username"
#define XML_CENTERSRVCFG_MAIN_PSW		"main_pwd"
#define XML_CENTERSRVCFG_MAIN_ADDR		"main_addr"
#define XML_CENTERSRVCFG_BACK_APN		"back_apn"
#define XML_CENTERSRVCFG_BACK_USERNAME	"back_username"
#define XML_CENTERSRVCFG_BACK_PSW		"back_psw"
#define XML_CENTERSRVCFG_BACK_ADDR		"back_addr"
#define XML_CENTERSRVCFG_TCP_PORT		"tcp_port"
#define XML_CENTERSRVCFG_UDP_PORT		"udp_port"

#define XML_SUBROOT_ICSERVER	"ICServer"
#define XML_ICSERVER_MAIN_SERV	"main_serv"
#define XML_ICSERVER_BACK_SERV	"back_serv"
#define XML_ICSERVER_TCP_PORT	"IC_server_tcp_port"
#define XML_ICSERVER_UDP_PORT	"IC_server_udp_port"

#define XML_SUBROOT_GNSSREPORT			"GnssReport"
#define XML_GNSSREPORT_TACTICS			"tactics"
#define XML_GNSSREPORT_SCHEME			"scheme"
#define XML_GNSSREPORT_INTERVAL			"interval"
#define XML_GNSSREPORT_ANGLE			"angle"
#define XML_GNSSREPORT_RADIUS			"radius"
#define XML_SUB_DEFAULT					"default"
#define XML_SUB_ALARM_T					"alarm"
#define XML_SUB_DORMANT					"dormant"
#define XML_SUB_DRIVER_DEPARTURE		"driver_departure"
#define XML_DEFAULT_TIME				"time"
#define XML_DEFAULT_MILEAGE				"mileage"
#define XML_ALARM_T_TIME				"time"
#define XML_ALARM_T_MILEAGE				"mileage"
#define XML_DORMANT_TIME				"time"
#define XML_DORMANT_MILEAGE				"mileage"
#define XML_DRIVER_DEPARTURE_TIME		"time"
#define XML_DRIVER_DEPARTURE_MILEAGE	"mileage"

#define XML_SUBROOT_PHONENUMBER			"PhoneNumber"
#define XML_PHONENUMBER_MONITOR_NUM		"monitor_num"
#define XML_PHONENUMBER_SUPER_SMS_NUM	"super_sms_num"
#define XML_PHONENUMBER_CENTER_NUM		"center_num"
#define XML_PHONENUMBER_RESET_NUM		"reset_num"
#define XML_PHONENUMBER_REBOOT_NUM		"reboot_num"
#define XML_PHONENUMBER_CENTER_SMS_NUM	"center_sms_num"
#define XML_PHONENUMBER_ALARM_SMS_NUM	"alarm_sms_num"
#define XML_PHONENUMBER_ANSWER_WAY		"answer_way"
#define XML_PHONENUMBER_CALL_TIME		"call_time"
#define XML_PHONENUMBER_MONTH_TIME		"month_time"

#define XML_SUBROOT_ALARMMASK		"AlarmMask"
#define XML_ALARMMASK_ALARM_MASK	"alarm_mask"
#define XML_ALARMMASK_SMS			"sms"
#define XML_ALARMMASK_SNAP			"snap"
#define XML_ALARMMASK_SNAP_SAVE		"snap_save"
#define XML_ALARMMASK_KEY_MASK		"key_mask"

#define XML_SUBROOT_ALARMSPEED	"AlarmSpeed"
#define XML_ALARMSPEED_SPEED	"speed"
#define XML_ALARMSPEED_TIME		"time"
#define XML_ALARMSPEED_WARNING	"warning"

#define XML_SUBROOT_FATIGUEALARM		"FatigueAlarm"
#define XML_FATIGUEALARM_DRIVING_TIME	"driving_time"
#define XML_FATIGUEALARM_TOTAL_TIME		"total_time"
#define XML_FATIGUEALARM_REST_TIME		"rest_time"
#define XML_FATIGUEALARM_WARNING		"warning"

#define XML_SUBROOT_PARKINGALARM	"ParkingAlarm"
#define XML_PARKINGALARM_TIME		"time"

#define XML_SUBROOT_COLLISIONALARM		"CollisionAlarm"
#define XML_COLLISIONALARM_TIME			"time"
#define XML_COLLISIONALARM_ACCELERATION	"acceleration"

#define XML_SUBROOT_ROLLOVERALARM		"RolloverAlarm"
#define XML_ROLLOVERALARM_ANGLE			"angle"

#define XML_SUBROOT_SNAPCTRL			"SnapCtrl"
#define XML_SNAPCTRL_TIME				"time"
#define XML_SNAPCTRL_MILEAGE			"mileage"
#define XML_SNAPCTRL_TIME_UNIT			"time_unit"
#define XML_SNAPCTRL_MILEAGE_UNIT		"mileage_unit"
#define XML_SNAPCTRL_TIME_INTERVAL		"time_interval"
#define XML_SNAPCTRL_MILEAGE_INTERVAL	"mileage_interval"
#define XML_TIME_MASK			"mask"
#define XML_TIME_SAVE_MASK		"save_mask"
#define XML_MILEAGE_MASK		"mask"
#define XML_MILEAGE_SAVE_MASK	"save_mask"

#define XML_SUBROOT_PICQUALITY	"PicQuality"
#define XML_PICQUALITY_QUALITY	"quality"

#define XML_SUBROOT_PICCOLOR	"PicColor"
#define XML_PICCOLOR_LUMINANCE	"luminance"
#define XML_PICCOLOR_CONTRAST	"contrast"
#define XML_PICCOLOR_SATURATION	"saturation"
#define XML_PICCOLOR_CHROMA		"chroma"

#define XML_SUBROOT_VEHICLEINFO			"VehicleInfo"
#define XML_VEHICLEINFO_MILEAGE			"mileage"
#define XML_VEHICLEINFO_PROVINCE_ID		"province_id"
#define XML_VEHICLEINFO_CITY_ID			"city_id"
#define XML_VEHICLEINFO_LICENSE			"license"
#define XML_VEHICLEINFO_PLATE_COLOR		"plate_color"
#define XML_VEHICLEINFO_COEFFICIENT		"coefficient"
#define XML_VEHICLEINFO_VIN				"vin"
#define XML_VEHICLEINFO_VEHICLE_TYPE	"vehicle_type"
#define XML_VEHICLEINFO_NVEHICLE_TYPE	"nvehicle_type"

#define XML_SUBROOT_GNSSDETAIL		"GnssDetail"
#define XML_GNSSDETAIL_ENABLE		"enable"
#define XML_GNSSDETAIL_BAUDTATE		"baudtate"
#define XML_GNSSDETAIL_OUTPUT_FREQ	"output_freq"
#define XML_GNSSDETAIL_UPLOAD_MODE	"upload_mode"
#define XML_GNSSDETAIL_SAMPLE_FREQ	"sample_freq"
#define XML_GNSSDETAIL_UPLOAD_SET	"upload_set"

#define XML_SUBROOT_CANUPLOAD		"CanUpload"
#define XML_CANUPLOAD_SAMPLE1		"sample1"
#define XML_CANUPLOAD_SAMPLE2		"sample2"
#define XML_CANUPLOAD_UPLOAD1		"upload1"
#define XML_CANUPLOAD_UPLOAD2		"upload2"
#define XML_CANUPLOAD_ALONE_SAMPLE	"alone_sample"
#define XML_ALONE_SAMPLE_SET		"set"

#define XML_SUBROOT_VIDEOATTR		"VideoAttr"
#define XML_VIDEOATTR_AI_VI_CONF	"Ai_Vi_Conf"
#define XML_VIDEOATTR_AI_VI_CHN		"Ai_Vi_Chn"
#define XML_VIDEOATTR_SINGLE_CHN	"Single_Chn"
#define XML_VIDEOATTR_SPECIAL_ALARM	"Special_Alarm"
#define XML_VIDEOATTR_VIDEO_ALARM	"Video_Alarm"
#define XML_VIDEOATTR_DAILY_AWAKE	"Daily_Awake"
#define XML_AI_VI_CONF_CODING_MODE				"coding_mode"
#define XML_AI_VI_CONF_RESOLUTION				"resolution"
#define XML_AI_VI_CONF_KEY_FRAME_INTER			"key_frame_inter"
#define XML_AI_VI_CONF_FRAME_RATE				"frame_rate"
#define XML_AI_VI_CONF_BIT_RATE					"bit_rate"
#define XML_AI_VI_CONF_STORE_CODING_MODE		"store_coding_mode"
#define XML_AI_VI_CONF_STORE_RESOLUTION			"store_resolution"
#define XML_AI_VI_CONF_STORE_KEY_FRAME_INTER	"store_key_frame_inter"
#define XML_AI_VI_CONF_STORE_FRAME_RATE			"store_frame_rate"
#define XML_AI_VI_CONF_STORE_BIT_RATE			"store_bit_rate"
#define XML_AI_VI_CONF_OSD_SET					"osd_set"
#define XML_AI_VI_CONF_ENABLE_AUDIO				"enable_audio"
#define XML_AI_VI_CHN_CHN_TOTAL_NUM	"chn_total_num"
#define XML_AI_VI_CHN_AUDIO_NUM		"audio_num"
#define XML_AI_VI_CHN_VIDEO_NUM		"video_num"
#define XML_SINGLE_CHN_CHN_NUM	"chn_num"
#define XML_SPECIAL_ALARM_VIDEO_STORAGE_THRES	"video_storage_thres"
#define XML_SPECIAL_ALARM_VIDEO_DURATION		"video_duration"
#define XML_SPECIAL_ALARM_START_TIME			"start_time"
#define XML_VIDEO_ALALRM_NUM_OF_CARRIES	"num_of_carries"
#define XML_VIDEO_ALALRM_FATIGUE_THRES	"fatigue_thres"
#define XML_DAILY_AWAKE_AWAKE_MODE		"awake_mode"
#define XML_DAILY_AWAKE_AWAKE_TYPE		"awake_type"
#define XML_DAILY_AWAKE_SET_AWAKE_TIME	"set_awake_time"
#define XML_DAILY_AWAKE_FLAG			"flag"
#define XML_SUB_PERIOD1_START_TIME	"period1_start_time"
#define XML_SUB_PERIOD1_CLOSE_TIME	"period1_close_time"
#define XML_SUB_PERIOD2_START_TIME	"period2_start_time"
#define XML_SUB_PERIOD2_CLOSE_TIME	"period2_close_time"
#define XML_SUB_PERIOD3_START_TIME	"period3_start_time"
#define XML_SUB_PERIOD3_CLOSE_TIME	"period3_close_time"
#define XML_SUB_PERIOD4_START_TIME	"period4_start_time"
#define XML_SUB_PERIOD4_CLOSE_TIME	"period4_close_time"
#define XML_PERIOD_TIME_HOUR		"hour"
#define XML_PERIOD_TIME_MINUTES		"minutes"

//load tts config
#define XML_TTS_ROOT   				"ROOT"
#define XML_TTS_KEY   				"Key"
#define XML_TTS_API_KEY   			"api_key"
#define XML_TTS_SECRET_KEY  		"secret_key"
#define XML_TTS_APP_ID   			"app_id"
#define XML_TTS_CFG   				"Cfg"
#define XML_TTS_SPD   				"spd"
#define XML_TTS_PIT   				"pit"
#define XML_TTS_VOL   				"vol"
#define XML_TTS_PER   				"per"


class CParamInterface : public InterFace
{
public:
	bool mediaUpload;
	CParamInterface();
	~CParamInterface();
	JT808_Config_S *terminal_cfg;
	/********************************************************
	*函数功能:  load terminal param
	*输入参数: 
	* @param           pLog       日志内容?
	* 输出参数: 
	* @param  
	* 返回参数: 
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	DWORD Get_terminal_param(JT808_Config_S *cfg);

	/********************************************************
	*函数功能:  get snap ctrl cfg
	*输入参数: 
	* @param          time:定时拍照 mileage:定距拍照
	* 输出参数: 
	* @param  
	* 返回参数: 
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/

	DWORD Get_SnapCtrlCfg(Snap_Ctrl_Attr *SnapCtrl);
	
	/********************************************************
	*函数功能:  网络模块日志输出
	*输入参数: 
	* @param           pLog       日志内容?
	* 输出参数: 
	* @param  
	* 返回参数: 
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void PutOutLog(const char * fmt, ...);

	
	/***********************************************************
	*函数功能:  设置终端参数
	*输入参数: 
	* @param           sysCfg  终端配置
	* 输出参数: 
	* 返回参数: 
	* 使用说明:返回0:成功，1:失败
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SetSysCfg(JT808_Config_S *sysCfg);


	/***********************************************************
	*函数功能: 获取终端参数
	*输入参数: 
	* @param           sysCfg  终端配置
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 返回0:成功，1:失败
	* 参考函数:
	* 创建作者:
	************************************************************/
	int GetSysCfg(JT808_Config_S *sysCfg );
	
	/***********************************************************
	* 函数功能:  获取鉴权码
	* 输入参数: 
	* 输出参数: 
	* @param  szACode    鉴权码
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int GetAuthorizeCode(char* szACode);
	
		
	/***********************************************************
	*函数功能:  获取终端手机号
	*输入参数: 
	* 输出参数: 终端手机号
	* @param  
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int GetPhoneNumber(unsigned char* szPhoneNum);
	/***********************************************************
	*函数功能:  设置终端手机号
	*输入参数: 
	* @param      终端手机号,12位，不足前面补0
	* 输出参数: 
	* @param  
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void SetPhoneNumber(const unsigned char* szPhoneNum);

	/***********************************************************
	*函数功能:  获取驾驶员信息
	*输入参数: 
	* @param 
	* 输出参数: 
	* 返回参数: 0:成功，-1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int GetDriverInfo(IC_Read_Info* pICInfo,Driver_Info* pDriverInfo);

	/***********************************************************
	*函数功能: 设置车辆信息
	*输入参数: 
	* @param    pVehicle 
	* 输出参数: 
	* 返回参数: 无
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void SetVehicleInfo(const Vehicle_Info* pVehicle);

	/***********************************************************
	*函数功能: 获取车辆信息
	*输入参数: 
	*输出参数: 
		@param    pVehicle 
	* 返回参数: 0:成功，-1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int GetVehicleInfo(Vehicle_Info* pVehicle);
	
	/***********************************************************
	*函数功能 :  获取网络通讯参数
	*输入参数 : 
	* @param			 
	* 输出参数: 
	* 返回参数:  重传次数
	* 使用说明:  无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int GetNetworkParam(Net_Communication_Attr* pParam);
		
	
	/***********************************************************
	*函数功能 :  获取服务器地址
	*输入参数 : 
	* @param		int nType	 :0--中心服务器,1--认证服务器
	* 输出参数: 
	* 返回参数:  
	* 使用说明:  无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int GetServerAddr(int &nType, char* pAddr, int &port);
	void SetServerAddr(int nType, const char* pAddr, int port);
	/***********************************************************
	*函数功能:	获取终端属性
	*输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void GetTerminalAttr(Terminal_Attr* pTermAttr);
	void SetTerminalAttr(const Terminal_Attr& stTermAttr);

	void GetGnssData(void* pData);
	
	/***********************************************************
	* 函数功能: 车辆控制
	* 输入参数: 
	* @param   mark      0:车门解锁 1:车门加锁    
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void VehiDoorCtrlSet(unsigned char mark);

	
	/***********************************************************
	*函数功能 :  软件升级
	*输入参数 : 
	* 输出参数:   
	* 返回参数:  
	* 使用说明:  0: 成功， 1:失败
	* 参考函数:
	* 创建作者:
	************************************************************/
	int Upgrade(const char* pVersion,void* pData, int len);

	/***********************************************************
	*函数功能 :  临时位置跟踪
	*输入参数 : 
	* 输出参数:   
	* 返回参数:  
	* 使用说明:  0: 成功， 1:失败
	* 参考函数:
	* 创建作者:
	************************************************************/
	int LocationTracking(int interval, int time);

	/***********************************************************
	* 函数功能: 平台清除报警标志位接口
	* 输入参数: 
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int ConfirmAlarm(unsigned int alarmMask);
	
	/***********************************************************
	* 函数功能:短消息处理
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 0:成功, 1:失败
	* 使用说明:
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SMSProc(const char* pSMS,SMS_MASK_E mask);

	/***********************************************************
	* 函数功能:事件菜单设置
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 0:成功, 1:失败
	* 使用说明:
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SetEventMenu(InfoOp opr,Event_Menu_Item* pItem,int num);

	/***********************************************************
	* 函数功能:提问
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 0:成功, 1:失败
	* 使用说明:
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SetAskAnswer(const char* pQustion,SMS_MASK_E mask,Event_Menu_Item* pItem,int num);
	
	/***********************************************************
	* 函数功能:信息点播菜单设置
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 0:成功, 1:失败
	* 使用说明:
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SetMsgMenu(InfoOp opr,Msg_Menu_Item* pItem,int num);

	
	/***********************************************************
	* 函数功能:信息服务
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 0:成功, 1:失败
	* 使用说明:
	* 参考函数:
	* 创建作者:
	************************************************************/
	int MessageService(int nType,char* pMessage);
	
	/***********************************************************
	* 函数功能: 电话回拨
	* 输入参数: 
	* @param   nMark           标志0: 普通通话, 1:监听
	* @param   szPhoneNo    电话号码
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int  TelCallback(int nMark, char* pPhoneNo);
	
	/***********************************************************
	* 函数功能:电话本设置
	* 输入参数: 
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int  TelBookSet(InfoOp  op, Address_Book_Item* pAddrBook,int num);

	/***********************************************************
	* 函数功能: 设置圆形区域
	* 输入参数: 
	* @param   opr   区域设置类型0--更新,1--追加,2--修改
	* @param   rgn   参数值
	* @param   num  区域数
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SetRingRgn(int opr,Ring_Region_Attr* rgn,int num);

	
	/***********************************************************
	* 函数功能: 设置矩形区域
	* 输入参数: 
	* @param   opr   区域设置类型0--更新,1--追加,2--修改
	* @param   rgn   参数值
	* @param   num  区域数
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SetRectRgn(int opr,Rectangle_Region_Attr* rgn,int num);

	/***********************************************************
	* 函数功能: 设置圆形区域
	* 输入参数: 
	* @param   rgn	 参数值
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SetPolyRgn(Poly_Region_Attr* rgn);


	/***********************************************************
	* 函数功能: 设置线路区域
	* 输入参数: 
	* @param   rgn	 参数值
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SetRouteRgn(Route_Region_Attr* rgn);

		
	/***********************************************************
	* 函数功能: 删除区域
	* 输入参数: 
	* @param   rgnType   区域类型0--圆形,1--矩形,2--多边形,3--线路
	* @param   pRgnID   区域ID
	* @param   num  区域数
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int DelRgn(int rgnType,DWORD* pRgnID,int num);

	/***********************************************************
	* 函数功能: 行驶记录采集
	* 输入参数: 
	* @param cmd  下传命令字
	* @param data           设置参数
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SetDrivingRecord(int cmd, char* data);

	/***********************************************************
	* 函数功能: 数据下行透传
	* 输入参数: 
	* @param type    	0x00--GNSS模块详细定位数据
					0x01--
	* @param buf      	透传消息
	* @param size     	透传消息大小
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:	无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int DownlinkTrans(TRANSPARENT_TYPE_E type,char* buf,int size);
	
	/***********************************************************
	* 函数功能: 多媒体数据上传应答
	* 输入参数: 
	* 输出参数:
	* @param  id     		     多媒体id
	* @param retryPackCnt      重传包数
	* @param retryIdList          重传id列表
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int MultimediaDataReply(int id, unsigned char  retryPackCnt, short* retryIdList);


	/***********************************************************
	* 函数功能: 存储多媒体检索
	* 输入参数: 
	* 输出参数:
	* @param   mdIdxCmd;     	      多媒体检索信息
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int MdSearchCmd(NetMdIdxCmd mdIdxCmd,    vector <IdxCmdRes> &stResultList);

	/***********************************************************
	* 函数功能: 单条存储多媒体检索
	* 输入参数: 
	* 输出参数:
	* @param   SingleMdIdxCmd;     	  单条多媒体检索信息
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int SingleMdSearchCmd(SingleNetMdIdxCmd SingleMdIdxCmd, NetMdUploadCmd &cmd);
	
	/***********************************************************
	* 函数功能: 存储多媒体上传
	* 输入参数: 
	* param  recordcmd   储存多媒体上传命令信息
	* 输出参数:
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int MdUploadCmd(NetMdUploadCmd& mdUploadCmd);


	/***********************************************************
	* 函数功能: 启动录音
	* 输入参数: 
	* 输出参数:
	* param  recordcmd   录音命令信息
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int MdRecordVoiceCmd(NetRecordCmd& recordCmd);

	/***********************************************************
	* 函数功能: 摄像头立即拍摄
	* 输入参数: 
	* 输出参数:
	* param  mdShotCmd   拍摄信息
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	int MdShotCmd(NetMdShotCmd &mdShotCmd, vector<ShotResult> &resultList);

	void NetSetGnssCmsLinkState(bool state);
	void NetSetGnssCmssAuthorize(bool state);
	void SaveAuthCode(char*m_szAuthCode,int len);

	
	/*****************************************************************************
	 函 数 名  : InterFace.ReqIFrame
	 功能描述  : 请求生成I帧
	 输入参数  : int chn		通道号 
				 int streamType  码流类型:0--主码流,1--子码流
	 输出参数  : 无
	 返 回 值  : virtual
	 调用函数  : 
	 被调函数  : 
	
	*****************************************************************************/
	int ReqIFrame(int chn,int streamType);

	/*****************************************************************************
	 函 数 名  : RegisterAVFrame
	 功能描述  : 注册获取音视频帧钩子
	 输入参数  : void* pUser		  用户参数		
				 pFunPutFrameCallBack pFun	钩子函数
				 int chn					通道,从0计数
				 int streamType 			码流类型:0--主码流,1--子码流,2--音频流
	 输出参数  : 无
	 返 回 值  : 0--成功,1--失败
	 调用函数  : 
	 被调函数  : 
	*****************************************************************************/
	int RegisterAVFrame(void* pUser,pFunPutFrameCallBack pFun,int chn,int streamType);

	/*****************************************************************************
	 函 数 名  : UnRegisterAVFrame
	 功能描述  : 反注册获取音视频帧钩子
	 输入参数  : void* pUser		  用户参数		
			pFunPutFrameCallBack pFun  钩子函数
			int chn 				 通道,从0计数
			int streamType			 码流类型:0--主码流,1--子码流,2--音频流
	 
	 输出参数  : 无
	 返 回 值  : 0--成功,1--失败
	 调用函数  : 
	 被调函数  : 
	*****************************************************************************/
	int UnRegisterAVFrame(void* pUser,int chn,int streamType);

private:
	Terminal_Attr m_stTermAttr;
	Vehicle_Info m_st_VehicleInfo;
	unsigned char m_szPhoneNum[15];
	char m_szServerAddr[64];
	int  m_nPort;
	int m_nType;
};

#endif	/* __PARAM_INTERFACE_H__ */

