/*************************************************************
 * Copyright			: 
 * FileName				: dbus_if.h
 * Description			: dbus_if.h implement
 * Author		        : chas
 * Created on			: Jan 4, 2019
 *************************************************************/
#ifndef INC_TRML_DBUS_H_
#define INC_TRML_DBUS_H_

#include <time.h>

#define ADAS_DEV_ID        0x64
#define DSM_DEV_ID         0x65

enum  DRIVER_ALARM_TYPE {
	  ALARM_NONIE = 0,
	  /*! for local auth alarm */
	  DVR_IC_TRIGGER,
	  TIMER_TRIGGER,
	  SPEED_TRIGGER,
	  DRIVER_TRIGGER,
	  PLATFORM_TRIGGER,
	  /*! for platform auth trigger to webclient */
	  AUTH_DRIVER_TRIGGER,
	  AUTH_TIMER_TRIGGER,
	  AUTH_SPEED_TRIGGER,
	  AUTH_DVR_IC_TRIGGER,
	  AUTH_PLATFORM_TRIGGER,
	  ALARM_END
};

typedef struct DFSSResult_tag
{
    unsigned int nAlarmType;
    time_t       nTrigTime;         //触发时间
    unsigned int nDrvState;
    unsigned int nAlarmKeepingTime; //保持时间
    unsigned int nCalibState;       //标定状态
    unsigned int nAlarmSpeed;       //报警时车速
    unsigned int nReserved[7];
}DFSSResult;

typedef struct DriverInfoToAIServer_t
{
    char driver_id[64];
	char driver_name[64];
	char pic_path[128];
	char pic_src;
	unsigned int picNo;
	unsigned int picSerial;
	unsigned int picSize;
	unsigned int picMode;
    unsigned int face_left_posX;
	unsigned int face_left_posY;
    unsigned int face_right_posX; 
	unsigned int face_right_posY;
    unsigned int left_eye_centerX;   
	unsigned int left_eye_centerY;   
    unsigned int right_eye_centerX;   
	unsigned int right_eye_centerY;  
    unsigned int nose_centerX;
	unsigned int nose_centerY;
	unsigned int left_mouth_posX;
	unsigned int left_mouth_posY;
	unsigned int right_mouth_posX;
	unsigned int right_mouth_posY;
	
    uint32_t       ts;         //触发时间
}DriverInfoToAIServer;

typedef struct Jt1078ReqStrem_tag
{
	unsigned int      addr_len;//服务器ip地址长度
	char	          addr[32];//服务器ip
	unsigned short    tcp_port;//tcp端口号
	unsigned short    udp_port;//udp端口号
	unsigned int      chn;//逻辑通道号
	unsigned int      data_type;//数据类型。0：音视频；1：视频；2：双向对讲；
	                     //3：监听；4：中心广播；5：透传
	unsigned int      stream_type;//码流类型。0：主码流；1：子码流
	unsigned char     phone_num[20];
}Jt1078ReqStrem;

typedef struct Jt1078CtrlStrem_tag
{
	unsigned int    chn;//逻辑通道号
	unsigned int    control_instrut;//控制指令
	unsigned int    close_av;//关闭音视频类型
	unsigned int    switch_stream;//切换码流类型
}Jt1078CtrlStrem;

typedef struct Jt1078ReqPlay_tag
{
	unsigned int      addr_len;//服务器ip地址长度
	char              addr[32];//实时音视频服务器ip地址
	unsigned short    tcp_port;//tcp端口号
	unsigned short    udp_port;//udp端口号
	unsigned int      chn;//逻辑通道号
	unsigned int      type;//0：音视频；1：音频；2：视频；3：视频或者音视频
	unsigned int      stream_type;//0：主码流或子码流；1：主码流；2：子码流
	unsigned int      store_type;//0：主存储器或灾备；1：主存；2：灾备
	unsigned int      pb_method;//回放方式。0：正常回放；1：快进回放；2：关键帧快退回放；
						//3：关键帧播放；4：单帧上传
	unsigned int      multiple;//快进或者快退倍数。0：无效；1：1； 2：2； 3：4； 4：8； 5：16
	unsigned int      start_ts;//开始时间
	unsigned int      end_ts;//结束时间
	unsigned char     phone_num[20];
}Jt1078ReqPlay;

typedef struct Jt1078CtrlPlay_tag
{
	unsigned int    chn;//
	unsigned int    control;//回放控制。0：开始回放；1：暂停回放；2：结束回放；3：快进回放；
						//4：关键帧快退回放；5：拖动回放；6关键帧播放
	unsigned int    multiple;//快进或者快退倍数。0：无效；1：1倍；2：2倍；3：4倍；4：8倍；5：16倍
	unsigned int    position;//拖动回放位置
}Jt1078CtrlPlay;

//void *dbus_client_start(void *param);
void dbus_client_stop(void);
//void gdbus_if_record_small_video(const char* file_name,time_t timestamp, int32_t pre_dur, int32_t pos_dur);
void dfw_capture_image_alarm(int event, unsigned int ts, int index);
//void dfw_capture_image(uint32_t *ts, uint32_t *speed);
//void ai_server_capture_img(uint32_t *ts, uint32_t *speed);
//int  gdbus_play_audio(const char *wav);
void face_info_query_from_sql(unsigned int *driver_id_num, char **IDList, uint32_t *ts);
//void face_info_req_data_to_sql(DriverInfoToAIServer *driverinfo);
void face_pic_download(unsigned int result, char *name, char *id, uint32_t ts);
//void trigger_face_compare(char *alarm_type, unsigned int ts);
//void face_auth_result(char *driver_name, unsigned char name_len, unsigned int result);

//void jt1078_req_stream(Jt1078ReqStrem *req_av);
//void jt1078_ctrl_stream(Jt1078CtrlStrem *req_av);
//void jt1078_req_playback(Jt1078ReqPlay *req_play);
//void jt1078_ctrl_playback(Jt1078CtrlPlay *ctrl_play);


#endif /* INC_TRML_DBUS_H_ */
