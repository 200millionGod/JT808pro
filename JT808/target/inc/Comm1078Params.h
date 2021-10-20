#ifndef _COMM_1078_PARAMS_H_
#define _COMM_1078_PARAMS_H_
#include <time.h>
#include <unistd.h>

using namespace std;

/*************************************************************************
*Function:Comm1078Params.h
*Description:1078协议音视频参数定义
*Params in:
*Params out:
*Result:
*author:huangshimei
*date:20190620
**************************************************************************/

typedef enum {
	PAUSE = 0,
	CONTINUE,
	CANCEL,
	START,
}up_file_status;


#pragma pack(4)

typedef struct JT1078_Audio_video_conf_set_s{
	BYTE    coding_mode;//0:CBR;1:VBR;2:ABR
	BYTE    resolution;//0:QCIF;1:CIF;2:WCIF;3:D1;4:WD1;5:720P;6:1080P
	WORD    key_frame_inter;//实时流关键帧间隔
	BYTE    frame_rate;//实时流目标帧率
	DWORD   bit_rate;//实时流目标码率
	BYTE    store_coding_mode;//0:CBR;1:VBR;2:ABR
	BYTE    store_resolution;//0:QCIF;1:CIF;2:WCIF;3:D1;4:WD1;5:720P;6:1080P
	WORD    store_key_frame_inter;//存储流关键帧间隔
	BYTE    store_frame_rate;//存储流目标帧率
	DWORD   store_bit_rate;//存储流目标码率
	WORD    osd_set;//bit0:date;bit1:vehicle num;bit2:逻辑通道号；bit3:经纬度;
	                  //bit4:行驶记录速度；bit5:卫星定位速度；bit6:连续驾驶时间
	BYTE    enable_audio;//0:disable;1:enable
}JT1078_Audio_video_conf_set;

typedef struct channel_control_table_s{
	BYTE    physical_chn;//物理通道号
	BYTE    logical_chn;//逻辑通道号
	BYTE    chn_type;//0:audio_video;1:audio;2:video
	BYTE    connect_ptz;//0:no;1:yes
}channel_control_table;

typedef struct JT1078_Audio_video_chn_set_s{
	BYTE    chn_total_num;//音视频通道总数
	BYTE    audio_num;//音频通道总数
	WORD    video_num;//视频通道总数
	channel_control_table    chn_table[64];
}JT1078_Audio_video_chn_set;

typedef struct channel_video_params_s{
	BYTE    logical_chn;//逻辑通道号
	BYTE    coding_mod;//0:CBR;1:VBR;2:ABR
	BYTE    resolution;//0:QCIF;1:CIF;2:WCIF;3:D1;4:WD1;5:720P;6:1080P
	WORD    key_frame_inter;//实时流目标帧间隔
	BYTE    frame_rate;//实时流目标帧率
	DWORD   bit_rate;//实时流目标码率
	BYTE    store_coding_mode;//0:CBR;1:VBR;2:ABR
	BYTE    store_resolution;//0:QCIF;1:CIF;2:WCIF;3:D1;4:WD1;5:720P;6:1080P
	WORD    store_key_frame_inter;//存储流关键帧间隔
	BYTE    store_frame_rate;//存储流目标帧率
	DWORD   store_bit_rate;//存储流目标码率
	WORD    osd_set;//bit0:date;bit1:vehicle num;bit2:逻辑通道号；bit3:经纬度;
	                  //bit4:行驶记录速度；bit5:卫星定位速度；bit6:连续驾驶时间
}channel_video_params;

typedef struct JT1078_single_chn_control_s{
	BYTE    chn_num;//需要单数设置视频参数的通道数量
	channel_video_params chn_video_param[16];
}JT1078_single_chn_control;

typedef struct JT1078_special_alarm_param_set_s{
	BYTE    video_storage_thres;//特殊报警录像存储阈值
	BYTE    video_duration;//特殊报警录像持续时间
	BYTE    start_time;//特殊报警标识起始时间
}JT1078_special_alarm_param_set;

typedef struct JT1078_alarm_shield_s{
	DWORD   shield;//视频相关屏蔽字
}JT1078_alarm_shield;

typedef struct JT1078_alarm_video_param_s{
	BYTE    num_of_carries;//车辆核载人数
	BYTE    fatigue_thres;//疲劳程度阈值
}JT1078_alarm_video_param;

typedef struct JT1078_daily_awake_param_s{
	BYTE    flag;//定时唤醒启用标志 bit0:period1;bit1:period2;bit2:period3;bit3:period4
	BCD     period1_start_time[2];//时间段1唤醒时间
	BCD     period1_close_time[2];//时间段1关闭时间
	BCD     period2_start_time[2];//时间段2唤醒时间
	BCD     period2_close_time[2];//时间段2关闭时间
	BCD     period3_start_time[2];//时间段3唤醒时间
	BCD     period3_close_time[2];//时间段3关闭时间
	BCD     period4_start_time[2];//时间段4唤醒时间
	BCD     period4_close_time[2];//时间段4关闭时间
}JT1078_daily_awake_param;

typedef struct JT1078_awake_param_s{
	BYTE	awake_mode;//休眠唤醒模式
	BYTE	awake_type;//唤醒条件类型
	BYTE	set_awake_time;//定时唤醒日设置
	JT1078_daily_awake_param awake_params;
}JT1078_awake_param;

typedef struct JT1078_transmission_control_s{
	BYTE    logical_chn;//逻辑通道号
	BYTE    control_instruction;//控制指令。0：关闭音视频传输指令；1：切换码流；
	                              //2：暂停；3：恢复暂停前；4：关闭双向对讲
	BYTE    close_type;//
	BYTE    switch_type;//
}JT1078_transmission_control;

#pragma pack()


/*音视频传输控制*/
typedef struct VideoGetParam_t{
	BYTE    audio_coding;//输入音频编码方式
	BYTE    audio_chn_num;//输入音频声道数
	BYTE    audio_frequency;//输入音频采样率
	BYTE    audio_sampling_bits;//输入音频采样位数
	WORD    audio_frame_len;//音频帧长度
	BYTE    support_audio;//是否支持音频输出
	BYTE    video_coding;//视频编码方式
	BYTE    max_audio_num;//终端支持的最大音频物理通道数量
	BYTE    max_video_num;//终端支持的最大视频物理通道数量
}VideoGetParam;

/*音视频实时传输请求*/
typedef struct VideoServerParamSet_t{
	BYTE    server_addr_len;//服务器ip地址长度
	char	server_addr[32];//服务器ip
	WORD    tcp_port;//tcp端口号
	WORD    udp_port;//udp端口号
	BYTE    logical_chn;//逻辑通道号
	BYTE    data_type;//数据类型。0：音视频；1：视频；2：双向对讲；
	                     //3：监听；4：中心广播；5：透传
	BYTE    stream_type;//码流类型。0：主码流；1：子码流
}VideoServerParamSet;

/*上传乘客流量*/
typedef struct VideoUploadCarries_t{
	BCD     start_time[6];//起始时间
	BCD     end_time[6];//结束时间
	WORD    boarders_num;//上车人数
	WORD    down_num;//下车人数
}VideoUploadCarries;

/*音视频实时传输控制*/
typedef struct VideoTransCtrl_t{
	BYTE    logic_chn;//逻辑通道号
	BYTE    control_instrut;//控制指令
	BYTE    close_av;//关闭音视频类型
	BYTE    switch_stream;//切换码流类型
}VideoTransCtrl;

/*实时音视频传输状态通知*/
typedef struct VideoTransSta_t{
	BYTE    logic_chn;//逻辑通道号
	BYTE    packet_loss;//丢包率
}VideoTransSta;

/*查询资源列表*/
typedef struct VideoResource_t{
	WORD    serial;//查询指令流水号
	BYTE    logic_chn;//逻辑通道号
	BCD     start_time[6];//开始时间
	BCD     end_time[6];//结束时间
	DWORD   alarm_flag1;//告警标记
	DWORD   alarm_flag2;
	BYTE    type;//音视频资源类型.0:音视频；1：音频；2：视频；3：视频或音频
	BYTE    stream_type;//码流类型。0：所有码流，1：主码流；2：子码流
	BYTE    store_type;//存储类型。0：所有存储器；1：主存储器；2：灾备存储器
	DWORD   file_size;
}VideoResource;

/*终端上传音视频资源列表*/
typedef struct VideoUploadResInfo_t{
	WORD    serial;//对应查询指令流水号
	DWORD   list_num;//音视频资源总数
	VideoResource *souce_info;//音视频资源列表
}VideoUploadResInfo;

/*终端上传音视频资源列表*/
typedef struct VideoList_t{
	BCD     start_time[6];//开始时间
	BCD     end_time[6];//结束时间
	DWORD   file_size;
	char    file_name[32];
	BYTE    chn;
}VideoList;


/*录像回放请求*/
typedef struct VideoPlayback_t{
	WORD    serial;//流水号
	BYTE    addr_len;//服务器ip地址长度
	char    addr[32];//实时音视频服务器ip地址
	WORD    tcp_port;//tcp端口号
	WORD    udp_port;//udp端口号
	BYTE    logic_chn;//逻辑通道号
	BYTE    type;//0：音视频；1：音频；2：视频；3：视频或者音视频
	BYTE    stream_type;//0：主码流或子码流；1：主码流；2：子码流
	BYTE    store_type;//0：主存储器或灾备；1：主存；2：灾备
	BYTE    pb_method;//回放方式。0：正常回放；1：快进回放；2：关键帧快退回放；
						//3：关键帧播放；4：单帧上传
	BYTE    multiple;//快进或者快退倍数。0：无效；1：1； 2：2； 3：4； 4：8； 5：16
	BCD     start_time[6];//开始时间
	BCD     end_time[6];//结束时间
}VideoPlayback;

/*录像回放控制*/
typedef struct VideoPlaybackControl_t{
	BYTE    logic_chn;//
	BYTE    control;//回放控制。0：开始回放；1：暂停回放；2：结束回放；3：快进回放；
						//4：关键帧快退回放；5：拖动回放；6关键帧播放
	BYTE    multiple;//快进或者快退倍数。0：无效；1：1倍；2：2倍；3：4倍；4：8倍；5：16倍
	BCD     pb_position[6];//拖动回放位置
}VideoPlaybackControl;

/*上传录像文件*/
typedef struct VideoUpFile_t{
	WORD    serial;//流水号
	BYTE    addr_len;//ftp服务器地址长度
	char    addr[32];//ftp服务器地址
	WORD    ftp_port;//ftp端口
	BYTE    name_len;//ftp长度
	char    user_name[32];//ftp用户名
	BYTE    pwd_len;//ftp长度
	char    user_pwd[32];//ftp密码
	BYTE    path_len;//文件上传路径长度
	char    path[128];//文件上传路径
	BYTE    logic_chn;//逻辑通道号
	BCD     start_time[6];//开始时间
	BCD     end_time[6];//结束时间
	DWORD   alarm_flag1;//告警标记
	DWORD   alarm_flag2;//
	BYTE    type;//音视频资源类型；0：音视频；1：音频；2：视频；3：音频或视频
	BYTE    stream_type;//码流类型。0：主码流或者子码流；1：主码流；2：子码流
	BYTE    store_type;//存储位置。0：主存储器或灾备存储器；1：主存；2：灾备
	BYTE    task;//任务执行条件。bit0:wifi ；bit1:lan; bit2:3G~4G
}VideoUpFile;

/*上传录像文件*/
typedef struct VideoUpLoad_t{
	char    addr[32];//ftp服务器地址
	WORD    ftp_port;//ftp端口
	char    user_name[32];//ftp用户名
	char    user_pwd[32];//ftp密码
	char    file_name[128];//文件名
	char    local_path[128];//文件本地路径
	char    target_path[128];//文件上传路径
	char    total;//文件总数
	char    num;//当前上传的第几个文件
	char    msg_serial;//消息序列号
	char    up_status;
}VideoUpLoad;


/*文件上传完成通知*/
typedef struct VideoUpFileResult_t{
	WORD    serial;//对应平台文件上传消息的流水号
	BYTE    result;//结果。0：成功；1：失败
}VideoUpFileResult;

/*上传文件控制*/
typedef struct VideoUpFileCtrl_t{
	WORD    serial;//对应平台文件上传消息的流水号
	BYTE    ctrl;//0:暂停；1：继续；2：取消
}VideoUpFileCtrl;

typedef struct Video_Attr_s{
	JT1078_Audio_video_conf_set     ai_vi_attr;
	JT1078_Audio_video_chn_set      ai_vi_chn;
	JT1078_single_chn_control       single_chn_ctl;
	JT1078_special_alarm_param_set  special_alarm;
	JT1078_alarm_shield             alarm_shield;
	JT1078_alarm_video_param        video_alarm;
	JT1078_awake_param              awake_attr;
}Video_Attr;


#endif	/* _COMM_1078_PARAMS_H_ */
