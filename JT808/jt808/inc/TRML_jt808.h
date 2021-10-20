/*************************************************************
 * Copyright			: 
 * FileName				: jt808.h
 * Description			: jt808.h implement
 * Author		        : chas
 * Created on			: Jan 7, 2019
 *************************************************************/
#ifndef INC_TRML_JT808_H_
#define INC_TRML_JT808_H_
//#include "xcam_iface.h"
#include "JT808App.h"
#include "TRML_common.h"
#include "TRML_dbus.h"


#define CONFIG_FILE_PATH          	 "./jtt808.xml"
#define MAJOR_VERSION				 (0)
#define MINOR_VERSION				 (0)
#define BUILD_VERSION				 (1)
#define ADAS_DEV_ID        		 	 0x64
#define DSM_DEV_ID                   0x65

#define ADAS_VDC_2_LARM               0X0101    //Alarm for Close Distance.level 2
#define ADAS_FCW_ALARM                0X0102    //Forward collision warning
#define ADAS_VDC_1_LARM               0X0103    //Alarm for Close Distance level 1

#define ADAS_LDW_L_ALARM             0X0201    //Left lane departure warning
#define ADAS_LDW_R_ALARM             0X0202    //Right lane departure warning
#define ADAS_AAW_ALARM               0X0801    //Active capture alarm

#define DSM_AAW_ALARM               0X0802    //Active capture alarm

typedef enum {
	MANUAL = 0,
	DFSS_ALARM,
	LDW_ALARM,
	FCW_ALARM,
	ZEBRA_ALARM,
	URGENT_ALARM,
	MISC_ALARM,			
	IMU_ALARM,
    BSD_ALARM,
}ALARM_TYPE;

typedef enum {
	LDW_ALM_NONE = 0,
	LDW_LEFT_WARMING,	//< left turn warning
	LDW_RIGHT_WARMING	//< right turn warning
}LDW_ALARMTYPE;


typedef struct gps_info_t
{
	unsigned int timestamp;
	char latitude_degree;
	char latitude_cent;
	long latitude_sec;
	char longitude_degree;
	char longitude_cent;
	long longitude_sec;
	double latitude;
	double longitude;
	double magnetic;
	double speed;
	char satelliteNum;			/*Number of GPS positioning satellites*/
	char gpslocation;			/*0:disable 1:enable*/
	char BDlocation;			/*0:disable 1:enable*/
	unsigned short altitude;     /**/
	unsigned int status;
	//unsigned short gpsAngle;
} GPS_info;

typedef struct Box_4G_info_t
{
	char			phone_num[11];
	char			IMME[6];
	char			ICCID[10];
	unsigned int	sig_strength;
	unsigned int	timestamp;
}Box_4G_Info;

struct Capture_S
{
    int             event;
	int		        dev;
    unsigned int    event_pts;
    int             num;
    int             interval;
    int             index;
    long long int   pts;
};

struct RDT_EVENT_S
{
    int     event;
    int 	alarm_type;
    int     level;
    time_t  pts;
	int		dev;
	int		image_type[8];
    int     image_id[8];
    int     image_num;
    int     interval;
	int 	video_enable;
};

struct RDT_IMAGE_S
{
    int     event;
    time_t  pts;
	int		dev;
	int		image_type;
    int     image_id;
    int     image_index;
    int     interval;
    int     flag;
    int     file_size;
    time_t  send_pts;
};

typedef struct Terminal_s
{
	unsigned int nFlag;
	char phone_num[36];
	char devID[30];
	char devType[36];
	char devIMEI[30];
}Terminal;

typedef struct Device_Control_Info
{
	bool bEnable;
	int nGPSReportTime;
}Control_Info;

typedef struct Device_Param_S
{
    char    pre_sim[5];
    int     overspeed;
    int     videoEnable;
    int     imageEnable;
}Device_Param;

typedef std::list<Capture_S>     TRML_CAPTURE_LIST;
typedef std::list<Alarm_S>    	 TRML_ALARM_LIST;
typedef std::list<RDT_EVENT_S>   TRML_AUTH_LIST;
typedef std::list<FacePic_s>     TRML_FacePic_LIST;
typedef std::list<VideoUpLoad>   TRML_Video_LIST;

typedef struct
{
	char*			   configure_file_path;
	char*			   log_file_path;
	int 			   g_upload_status;
	int 			   g_capture_status;
	int 			   g_upordown_status;
	int 			   test_mode;
    int                n_current_speed;
	TRML_AUTH_LIST     g_auth_list;
	TRML_CAPTURE_LIST  g_capture_list;
    TRML_ALARM_LIST    g_alarm_list;
	TRML_FacePic_LIST  g_facepic_list;
	TRML_Video_LIST    g_videofile_list;
	pthread_mutex_t    g_capture_mutex;
	pthread_mutex_t    g_alarm_mutex;
	pthread_mutex_t    g_auth_mutex;
	pthread_mutex_t    g_facepic_mutex;
	pthread_mutex_t    g_videofile_mutex;
    GPS_info           gps_info;
    Device_Param       dev_param;
    Terminal           terminal;
    JT808_CONFIG       config;
	Control_Info	   control_info;
	Box_4G_Info		   box_info;
	JT808_Config_S     terminal_cfg;
	TTSInfo            tts_info;
	char phone_num[12];
} JTT808_mgr;

extern JTT808_mgr g_mgr;
#endif /* INC_TRML_JT808_H_ */
