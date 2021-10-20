#ifndef _PARAMSCONFIG_H_
#define _PARAMSCONFIG_H_

#include "TRML_common.h"
#include "TRML_jt808.h"
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
#define XML_TERMINAL_DEVIMEI	 "imei"

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
#define XML_CENTERSRVCFG_MAIN_PSW		"main_psw"
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
#define XML_ICSERVER_TCP_PORT	"tcp_port"
#define XML_ICSERVER_UDP_PORT	"udp_port"

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
#define XML_PERIOD1_START_TIME_HOUR		"hour"
#define XML_PERIOD1_START_TIME_MINUTES	"minutes"
#define XML_PERIOD2_START_TIME_HOUR		"hour"
#define XML_PERIOD2_START_TIME_MINUTES	"minutes"
#define XML_PERIOD3_START_TIME_HOUR		"hour"
#define XML_PERIOD3_START_TIME_MINUTES	"minutes"
#define XML_PERIOD4_START_TIME_HOUR		"hour"
#define XML_PERIOD4_START_TIME_MINUTES	"minutes"

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

class XMLParse
{

public:
	XMLParse();
	~XMLParse();
public:
	bool getInfo(const char *pXmlfile, Control_Info& stControlInfo, JT808_CONFIG& config, Terminal &stTerm, Device_Param& dev_param);
	
	bool Load_tts_param(const char *pXmlfile, TTSInfo *cfg);
	
	void SetMgrXml(const char *pXmlfile, const JTT808_mgr &stJtt808_mgr);
	void SetTerXml(const char *pXmlfile, const JT808_Config_S *cfg);
	int LoadTerminalParam(const char* pXmlfile, JT808_Config_S& cfg);
	
	int LoadDefaultParam(const char* pXmlfile, JT808_Config_S* cfg);
private:
    void parse_basic_config(tinyxml2::XMLElement *pBasicConfigNode, Basic_Config& config);
    void param_auto_snap_config(tinyxml2::XMLElement *pAutoSnapNode, AutoSnap& auto_snap);
    void parse_dsm_alarm_config(tinyxml2::XMLElement *pDSMAlarmNode, DSM_Config& config);
    void parse_adas_alarm_config(tinyxml2::XMLElement *pADASAlarmNode, ADAS_Config& config);
    void parse_alarm_config(tinyxml2::XMLElement *pAlarmNode, AlarmConfig& config);
    void parse_terminal_attr(tinyxml2::XMLElement *pterminalNode, Terminal& terminal);
    void parse_device_param(tinyxml2::XMLElement *pDevParamNode, Device_Param& dev_param);
	void parse_interval_config(tinyxml2::XMLElement *pIntervalConfigNode, Interval_Attr& interval);
	void parse_snapctl_time_config(tinyxml2::XMLElement *pSnapctlConfigNode, Snap_Mask_Attr& time);
	void parse_snapctl_mileage_config(tinyxml2::XMLElement *pSnapctlConfigNode, Snap_Mask_Attr& mileage);
	void parse_videoattr_ai_vi_config(tinyxml2::XMLElement *pVideoAttrConfigNode, JT1078_Audio_video_conf_set& ai_vi_conf);
	void parse_videoattr_ai_vi_chn_config(tinyxml2::XMLElement *pAiViChnConfigNode, JT1078_Audio_video_chn_set& ai_vi_chn);
	void parse_videoattr_single_chn_config(tinyxml2::XMLElement *pSingleChnConfigNode, JT1078_single_chn_control& single_chn_ctl);
	void parse_videoattr_special_alarm_config(tinyxml2::XMLElement *pSpecialAlarmConfigNode, JT1078_special_alarm_param_set& special_alarm);
	void parse_videoattr_video_alarm_config(tinyxml2::XMLElement *pVideoAlarmConfigNode, JT1078_alarm_video_param& video_alarm);
	void parse_videoattr_daily_awake_config(tinyxml2::XMLElement *pDailyAwakeConfigNode, JT1078_awake_param& awake_attr);
	void param_period1_startTime_config(tinyxml2::XMLElement *period1StartTimeConfigNode, JT1078_daily_awake_param& awake_params);
	void param_period1_closeTime_config(tinyxml2::XMLElement *period1CloseTimeConfigNode, JT1078_daily_awake_param& awake_params);
	void param_period2_startTime_config(tinyxml2::XMLElement *period2StartTimeConfigNode, JT1078_daily_awake_param& awake_params);
	void param_period2_closeTime_config(tinyxml2::XMLElement *period2CloseTimeConfigNode, JT1078_daily_awake_param& awake_params);
	void param_period3_startTime_config(tinyxml2::XMLElement *period3StartTimeConfigNode, JT1078_daily_awake_param& awake_params);
	void param_period3_closeTime_config(tinyxml2::XMLElement *period3CloseTimeConfigNode, JT1078_daily_awake_param& awake_params);
	void param_period4_startTime_config(tinyxml2::XMLElement *period4StartTimeConfigNode, JT1078_daily_awake_param& awake_params);
	void param_period4_closeTime_config(tinyxml2::XMLElement *period4CloseTimeConfigNode, JT1078_daily_awake_param& awake_params);
};

#endif
