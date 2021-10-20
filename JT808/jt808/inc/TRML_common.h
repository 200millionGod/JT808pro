#ifndef     _COMMON_H_
#define     _COMMON_H_

#include	<stdio.h>
#include    <stdlib.h>
#include    <stdarg.h>
#include    <string>
#include    <list>
#include    <queue>

using namespace std;

#define MAX_PLATFORM_NUM			 (2)
#define MAX_SESSION_NUM				 (2)

typedef struct DsmVideoEnable_s
{
	unsigned int   level1;
	unsigned int   level2;
	unsigned int   facemissing;
	unsigned int   eyesmissing;
	unsigned int   lookaround;
	unsigned int   lookup;
	unsigned int   lookdown;
	unsigned int   smoking;
	unsigned int   phone;
	unsigned int   yawn;
	unsigned int   talking;
	unsigned int   speeding;
	unsigned int   sunglass;
	unsigned int   belt;
} DsmVideoEnable;

typedef struct IfvVideoEnable_s
{
	unsigned int   hwm_level2;
	unsigned int   TTC;
	unsigned int   hwm_level1;
	unsigned int   ufcw;
	unsigned int   ldw_left;
	unsigned int   ldw_right;
}IfvVideoEnable;

typedef struct AlarmConfig_s
{
	bool level1;
	bool level2;
	unsigned int speed_thres;
	unsigned int rec_time;
	unsigned int snap_num;
	unsigned int snap_interval;
	unsigned int up_video;
} AlarmConfig;

typedef struct AutoSnap_s
{
	unsigned int tactics;
	unsigned int interval_time;
	unsigned int interval_distance;
	unsigned int snap_num;
	unsigned int snap_interval;
} AutoSnap;

typedef struct Session_s
{
    char   		   server_url[64];
	unsigned int   port;
}Session;

typedef struct Basic_Config_S
{
	unsigned int   speed_thres;
	unsigned int   alarm_volume;
	unsigned int   photo_res;
	unsigned int   video_res;
	AutoSnap	   autosnap;
}Basic_Config;

typedef struct DSM_Config_S
{
	unsigned int   smoking_interval;
	unsigned int   phone_interval;
	unsigned int   driver_identify;
	Basic_Config_S basicConfig;
	AlarmConfig    fatigue;
	AlarmConfig    telephone;
	AlarmConfig    smoking;
	AlarmConfig    distraction;
	AlarmConfig    abnormal;
	AlarmConfig    sungclass;
	AlarmConfig    belt;
	AlarmConfig    onehand_sw;	//*One-handed steering wheel alarm
	AlarmConfig    handsoff_sw; //*Hands off steering wheel alarm
	DsmVideoEnable video_enable;
}DSM_Config;

typedef struct ADAS_Config_S
{
	Basic_Config_S basicConfig;
	AlarmConfig    ldw;
	AlarmConfig    fcw;
	AlarmConfig    hmw;
	IfvVideoEnable video_enable;
}ADAS_Config;

typedef struct JT808_CONFIG_S
{  
	Session		session;
	DSM_Config 	dsm_config;
	ADAS_Config adas_config;
}JT808_CONFIG;

#endif
