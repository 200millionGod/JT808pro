/*************************************************************
 * Copyright			: 
 * FileName				: process_alarm.cpp
 * Description			: process_alarm.cpp implement
 * Author		        : chas
 * Created on			: Jan 7, 2019
 *************************************************************/

#include "../inc/TRML_alarm.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <list>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "../inc/TRML_capture.h"
#include "../inc/TRML_jt808.h"
#include "../inc/TRML_util.h"
#include "../inc/debug_log.h"
#include "../inc/TRML_third_party.h"

#include "TRML_dbus.h"
#include "JT808App.h"

const static char* CAPTURE_HOME_DEFAULT = 	"/tmp/capture";
static int  _auth_status = 0;

int get_dsm_image_config(int event, int& interval, int platform)
{
    int image_num = 0;
    switch(event)
    {
		case 0x1003:
		case 0x1004:
		case 0x100A:
		case 0x1011:
			image_num = g_mgr.config.dsm_config.fatigue.snap_num;
			interval = g_mgr.config.dsm_config.fatigue.snap_interval;
            break;
        case 0x1007:
        	image_num = g_mgr.config.dsm_config.telephone.snap_num;
            interval = g_mgr.config.dsm_config.telephone.snap_interval;
        	break;
        case 0x100B:
        case 0x1015:
			image_num = g_mgr.config.dsm_config.smoking.snap_num;
			interval = g_mgr.config.dsm_config.smoking.snap_interval;
			break;
        case 0x100C:
		case 0x1009:
		case 0x1013:
            image_num = g_mgr.config.dsm_config.distraction.snap_num;
            interval = g_mgr.config.dsm_config.distraction.snap_interval;
        	break;
        case 0x1001:
        case 0x1006:
        	image_num = g_mgr.config.dsm_config.abnormal.snap_num;
            interval = g_mgr.config.dsm_config.abnormal.snap_interval;
			break;
		case 0x1101:
		case 0x1102:
			image_num = g_mgr.config.dsm_config.onehand_sw.snap_num;
            interval = g_mgr.config.dsm_config.onehand_sw.snap_interval;
        	break;
		case 0x1014:
			image_num = g_mgr.config.dsm_config.sungclass.snap_num;
            interval = g_mgr.config.dsm_config.sungclass.snap_interval;
        	break;
		case 0x1012:
			image_num = g_mgr.config.dsm_config.belt.snap_num;
            interval = g_mgr.config.dsm_config.belt.snap_interval;
        	break;
		case 0x1103:
			image_num = g_mgr.config.dsm_config.handsoff_sw.snap_num;
            interval = g_mgr.config.dsm_config.handsoff_sw.snap_interval;
        	break;
        case 0x0802:
        	image_num = g_mgr.config.dsm_config.basicConfig.autosnap.snap_num;
            interval = g_mgr.config.dsm_config.basicConfig.autosnap.snap_interval;
        	break;
        case 0x100F:
        	image_num = 1;
        	interval = 0;
        	break;
//		case 0x1206://*auth
//		case 0x1207:
//		case 0x1208:
//		case 0x1209:
//		case 0x120A:
		case 0x0803:
        	image_num = 1;
        	interval = 5;
        	break;
        default:
            image_num = 0;
            break;
    }
    return image_num;
}


int get_adas_image_config(int event, int& interval, int platform)
{
    int image_num = 0;
    switch(event)
    {
    	case ADAS_VDC_2_LARM:
		case ADAS_VDC_1_LARM:
			image_num = g_mgr.config.adas_config.hmw.snap_num;
			interval = g_mgr.config.adas_config.hmw.snap_interval;	
			break;

		case ADAS_FCW_ALARM:
			image_num = g_mgr.config.adas_config.fcw.snap_num;
			interval = g_mgr.config.adas_config.fcw.snap_interval;
			break;
		case ADAS_LDW_L_ALARM:
		case ADAS_LDW_R_ALARM:
			image_num = g_mgr.config.adas_config.ldw.snap_num;
			interval = g_mgr.config.adas_config.ldw.snap_interval;
			break;
		case ADAS_AAW_ALARM:
					
			image_num = g_mgr.config.adas_config.basicConfig.autosnap.snap_num;
            interval = g_mgr.config.adas_config.basicConfig.autosnap.snap_interval;
            break;
       
        default:
            image_num = 0;
            break;
    }
    return image_num;
}

unsigned int get_video_enable(int event)
{
	unsigned int video_enable = 0;

	switch(event)
	{
		case 0x0101://车距过近报警 //level 2
			video_enable = g_mgr.config.adas_config.video_enable.hwm_level2;
			break;

		case 0x0103://车距过近报警 //level 1
			video_enable = g_mgr.config.adas_config.video_enable.hwm_level1;
			break;
		case 0x0102://前向碰撞报警
			video_enable = g_mgr.config.adas_config.video_enable.TTC;
			break;

		case 0x0201://车道偏离报警
			video_enable = g_mgr.config.adas_config.video_enable.ldw_left;
			break;

		case 0x0202://车道偏离报警
			video_enable = g_mgr.config.adas_config.video_enable.ldw_right;
			break;

		case 0x1003: //三级疲劳驾驶报警//warn_lvl1 close eyes
			video_enable = g_mgr.config.dsm_config.video_enable.level1;
			break;
        case 0x1011: //分神驾驶报警//yawn_lvl2
        	video_enable = g_mgr.config.dsm_config.video_enable.yawn;
			break;
        //case 0x1004: //二级疲劳驾驶报警//warn_lvl2
        case 0x100A: //一级疲劳驾驶报//yawn
        	video_enable = g_mgr.config.dsm_config.video_enable.yawn;
			break;

		case 0x1007: //接打电话报警
			video_enable = g_mgr.config.dsm_config.video_enable.phone;
			break;

		case 0x1015: //抽烟报警
			video_enable = g_mgr.config.dsm_config.video_enable.smoking;
			break;

		case 0x1009://headdown
			video_enable = g_mgr.config.dsm_config.video_enable.lookdown;
			break;
		case 0x1013://headup
			video_enable = g_mgr.config.dsm_config.video_enable.lookup;
			break;
		case 0x100C: //分神驾驶报警//look around
			video_enable = g_mgr.config.dsm_config.video_enable.lookaround;
			break;

		case 0x1014: //sunglass
			video_enable = g_mgr.config.dsm_config.video_enable.sunglass;
			break;

		case 0x1012: //belt
			video_enable = g_mgr.config.dsm_config.video_enable.belt;
			break;

		case 0x1001: //驾驶员异常//facemissing 离岗
			video_enable = g_mgr.config.dsm_config.video_enable.facemissing;
			break;
		case 0x1006: //摄像头遮挡
			video_enable = g_mgr.config.dsm_config.video_enable.facemissing;
			break;
		case 0x100F: //driver change
			video_enable = 0;
			break;
		case 0x1101:
			video_enable = 0;
			break;
		case 0x1102://One-handed steering wheel alarm
			video_enable = 0;
			break;
		case 0x1103://Hands off steering wheel alarm
			video_enable = 0;
			break;
	}

	return video_enable;
}
static int rdt_alarm_convert(unsigned char dev, unsigned int cur_speed, int event, int &alarm_type, int &level)
{
    int report_flag = 1;
	DBG_LEVEL_6("dev:%02x event:%04x-", dev,  event);
    level = 0x01;
    if ( dev == ADAS_DEV_ID )
    {
        switch(event)
        {
            case 0x0101://车距过近报警 //level 2
            	alarm_type = 0x03;
                break;
            case 0x0103://车距过近报警 //level 1
            	alarm_type = 0x03;
                break;
            case 0x0102://前向碰撞报警
            	alarm_type = 0x01;
                break;
            case 0x0201://车道偏离报警
            	alarm_type = 0x02;
                break;
            case 0x0202://车道偏离报警
            	alarm_type = 0x02;
                break;
            case 0x0801://主动抓拍事件
            	alarm_type = 0x11;
                break;
            default:
                report_flag = 0;
                break;
        }
    }
    else if ( dev == DSM_DEV_ID )
    {
        switch(event)
		{
		case 0x1003: //三级疲劳驾驶报警//warn_lvl1 close eyes
        case 0x1011: //分神驾驶报警//yawn_lvl2
        //case 0x1004: //二级疲劳驾驶报警//warn_lvl2
        case 0x100A: //一级疲劳驾驶报//yawn
            alarm_type = 0x01;
            if(cur_speed > g_mgr.config.dsm_config.fatigue.speed_thres){
                level = 0x02;
            }
			break;
		case 0x1007: //接打电话报警
			alarm_type = 0x02;
            if(cur_speed > g_mgr.config.dsm_config.telephone.speed_thres){
                level = 0x02;
            }
			break;
		//case 0x100B://抽烟报警
		case 0x1015: //抽烟报警
			alarm_type = 0x03;
            if(cur_speed > g_mgr.config.dsm_config.smoking.speed_thres){
                level = 0x02;
            }
			break;
		case 0x1009://headdown
		case 0x1013://headup
		case 0x100C: //分神驾驶报警//look around
			alarm_type = 0x04;
            if(cur_speed > g_mgr.config.dsm_config.distraction.speed_thres){
                level = 0x02;
            }
			break;
			
		case 0x1014: //sunglass
			alarm_type = 0xDA;
            if(cur_speed > g_mgr.config.dsm_config.sungclass.speed_thres){
                level = 0x02;
            }
			break;
			
		case 0x1012: //belt
			alarm_type = 0xD7;
            if(cur_speed > g_mgr.config.dsm_config.belt.speed_thres){
                level = 0x02;
            }
			break;
			
		case 0x1001: //驾驶员异常//facemissing 离岗
		case 0x1006: //摄像头遮挡
			alarm_type = 0x05;
            level = 0x02;
			break;
		/*case 0x1006: //摄像头遮挡
			alarm_type = 0xD8;
            level = 0x02;
			break;*/
		case 0x0802: //主动抓拍事件
			alarm_type = 0x10;
			break;
		case 0x100F: //driver change
			alarm_type = 0x11;
			break;
//		case 0x1206:
//		case 0x1207:
//		case 0x1208:
//		case 0x1209:
//		case 0x120A:
		case 0x0803:
			alarm_type = 0xE1;
			break;
		case 0x1101:
		case 0x1102://One-handed steering wheel alarm
			alarm_type = 0xDB;
			if(cur_speed > g_mgr.config.dsm_config.onehand_sw.speed_thres){
                level = 0x02;
            }
			break;
		case 0x1103://Hands off steering wheel alarm
			alarm_type = 0xDC;
			if(cur_speed > g_mgr.config.dsm_config.handsoff_sw.speed_thres){
                level = 0x02;
            }
			break;
		default:
			report_flag = 0;
			break;
		}
    } else {
        report_flag = 0;
    }

    return report_flag;
}

static void get_BCD_time(BCD *bcd_time, time_t pts)
{
	tm *ptm = localtime(&pts);
	bcd_time[0] = (((ptm->tm_year - 100) / 10) << 4) + (ptm->tm_year % 10);
	bcd_time[1] = (((ptm->tm_mon + 1) / 10) << 4) + ((ptm->tm_mon + 1) % 10);
	bcd_time[2] = ((ptm->tm_mday / 10) << 4) + (ptm->tm_mday % 10);
	bcd_time[3] = ((ptm->tm_hour / 10) << 4) + (ptm->tm_hour % 10);
	bcd_time[4] = ((ptm->tm_min / 10) << 4) + (ptm->tm_min % 10);
	bcd_time[5] = ((ptm->tm_sec / 10) << 4) + (ptm->tm_sec % 10);
}

static const char* __build_file_name(int dev, char* file_name, int event,int image_type, time_t pts, int index)
{
	unsigned int high_event = ((event >> 8) & 0xFF);
//	unsigned int low_event = (event & 0xFF);
	tm* p_tm = localtime(&pts);
	int len = sprintf(file_name, CAPTURE_HOME_DEFAULT);

    if ( file_name[len-1] != '/' )
    {
        file_name[len++] = '/';
    }
    if ( image_type == 0 )/*picture*/
    {
	    //len += sprintf(file_name+len,"d1_");
	}

    switch (high_event)
    {
        case 0x01:
            len += sprintf(file_name+len, "fcw_");
            break;
        case 0x02:
            len += sprintf(file_name+len, "ldw_");
            break;
        case 0x04:
            len += sprintf(file_name+len, "urgent_");
            break;
        case 0x08:
			len += sprintf(file_name+len, "manual_");
            break;
        case 0x10:
            len += sprintf(file_name+len, "dfw_");
            break;
		case 0x11:
			len += sprintf(file_name+len,"hod_");
            break;
		case 0x12:
			len += sprintf(file_name+len,"local_auth_");
			break;
		/*case 0x12:
			if ((low_event == AUTH_DRIVER_TRIGGER) || (low_event == AUTH_TIMER_TRIGGER)
					|| (low_event == AUTH_SPEED_TRIGGER) || (low_event == AUTH_DVR_IC_TRIGGER) || (low_event == AUTH_PLATFORM_TRIGGER)) {
				len += sprintf(file_name + len, "platform_auth_");
			} else if (ALARM_NONIE == low_event) {
				len += sprintf(file_name + len, "capture_");
			} else {
				len += sprintf(file_name+len,"device_auth_");
			}
            break;
		*/
    }

	//snapshot是写死的，只能抓manual_01_的图片
    /*if(event== 0x0802)
    {
    	event = 0x0801;
    }*/

    len += sprintf(file_name+len,"%02d_",event&0xFF);
    len += sprintf(file_name+len,"%04d%02d%02d_%02d%02d%02d", \
        p_tm->tm_year+1900,p_tm->tm_mon+1,p_tm->tm_mday,p_tm->tm_hour,p_tm->tm_min,p_tm->tm_sec);

	if(image_type == 2)
	{
		if(dev == 0x65){
			if (0x11 == high_event){
				len += sprintf(file_name + len, "_hod.mp4");
			}else{
            	len += sprintf(file_name + len, "_dsm.mp4");
			}
        } else if(dev == 0x64) {
            len += sprintf(file_name + len, "_adas.mp4");
        }
	}
	else
	{

		if (dev == 0x65) {
			if (0x11 == high_event) {
				len += sprintf(file_name + len, "_hod");
				if (index > 0) {
					len += sprintf(file_name + len, "_%02d", index & 0xFF);
				}
				len += sprintf(file_name + len, ".jpg");	
			} 
			else if (0x12 == high_event)
			{
				len += sprintf(file_name+len,".jpg");
			}
			else
			{
				if(index > 0)
				{
					len += sprintf(file_name + len, "_%02d", index & 0xFF);
				}

				len += sprintf(file_name + len, "_dsm.jpg");
			}
		}
		else if(dev == 0x64) {
			if(index > 0)
			{
				len += sprintf(file_name + len, "_%02d", index & 0xFF);
			}

			len += sprintf(file_name + len, "_adas.jpg");
		}
            
	}

    return file_name;
}

static int s_alarm_seq = 0;
static time_t s_last_alarm_pts = 0;

static void __create_alarm(RDT_EVENT_S evt, Alarm_S& stAlarm)
{
	if(g_mgr.test_mode){
		create_test_GpsInfo(stAlarm.gpsData);
	} else {
		getGpsInfo(stAlarm.gpsData);
	}

	if (s_last_alarm_pts == evt.pts){
		s_alarm_seq ++;
	}else{
		s_last_alarm_pts = evt.pts;
		s_alarm_seq = 0;
	}

	stAlarm.sequence = s_alarm_seq;
	stAlarm.nDevType = evt.dev;
	if ( evt.dev == ADAS_DEV_ID ){
		stAlarm.adas_alarm.nType         = evt.alarm_type;
		stAlarm.adas_alarm.nLevel        = evt.level;
		stAlarm.adas_alarm.speedFront    = 0;
		stAlarm.adas_alarm.distanceFront = 0;
		
		if (ADAS_LDW_L_ALARM == evt.event ){
			stAlarm.adas_alarm.deviationType = 0x01;
		}else if(ADAS_LDW_R_ALARM == evt.event ){
			stAlarm.adas_alarm.deviationType = 0x02;
		}else{
			stAlarm.adas_alarm.deviationType = 0;
		}
		
		stAlarm.adas_alarm.roadFlag      = 0;
		stAlarm.adas_alarm.roadFlagData  = 0;
	}else if ( evt.dev == DSM_DEV_ID ){
		stAlarm.dsm_alarm.nType = evt.alarm_type;
		stAlarm.dsm_alarm.nLevel = evt.level;
		stAlarm.dsm_alarm.nTiredLevel = 0x05;
	}

	
	char datetime[6] = { 0 };
	get_BCD_time((BCD *) &datetime[0], evt.pts);
	memcpy(&stAlarm.datetime[0], &datetime[0], 6);

	File_Info_s stInfo;
	char filename[128];
	time_t cap_ts = evt.pts;
    stAlarm.fileList.clear();
	if(g_mgr.dev_param.imageEnable)
	{
		for(int i = 0;i < evt.image_num;i++)
		{
			memset(&stInfo, 0, sizeof(File_Info_s));
			memset(filename, 0, sizeof(filename));
			stInfo.nIndex = i + 1;
			stInfo.nType = FILE_TYPE_IMAGE;
			if(evt.image_type == 0)
			{
				cap_ts += (i * evt.interval) / 1000;
			}

			__build_file_name(stAlarm.nDevType, filename, evt.event, evt.image_type[i], cap_ts, i);
			DBG_LEVEL_6("[index:%d filename:%s]", i, filename);

			memcpy(stInfo.szFileName, filename, strlen(filename));
			stAlarm.fileList.push_back(stInfo);
		}
	}

	if(evt.video_enable)
	{
		memset(&stInfo, 0, sizeof(File_Info_s));
		memset(filename, 0, sizeof(filename));
		stInfo.nIndex = 0;
		stInfo.nType = FILE_TYPE_VIDEO;

		__build_file_name(stAlarm.nDevType, filename, evt.event, 0x02, cap_ts, 0);
		if (0x11 == (( evt.event >> 8 ) & 0xFF)){
//			gdbus_if_record_small_video(filename, cap_ts, 5, 5);
		}
	
		memcpy(stInfo.szFileName, filename, strlen(filename));
		stAlarm.fileList.push_back(stInfo);

		DBG_LEVEL_6("[filename:%s]", filename);
	}
}

int add_capture_list(int dev, int event, unsigned int ts, int& interval)
{
	int img_num = 0;
	int platform = 0;
	int i = 0;

	if(dev == DSM_DEV_ID)
	{
		img_num = get_dsm_image_config(event, interval, platform);
	}else if ( ADAS_DEV_ID == dev){
		img_num = get_adas_image_config(event, interval, platform);
	}

	interval = interval * 100;

	if(img_num > 0)
	{
		Capture_S cap;
		for(i = 0;i < img_num - 1;i++)
		{
			cap.event = event;
			cap.dev = dev;
			cap.event_pts = ts;
			cap.num = img_num;
			cap.interval = interval;
			cap.index = i + 1;
			cap.pts = get_cur_pts();

			pthread_mutex_lock(&g_mgr.g_capture_mutex);
			g_mgr.g_capture_list.push_back(cap);
			pthread_mutex_unlock(&g_mgr.g_capture_mutex);
		}
	}
	return img_num;
}

int handle_face_auth_result(FaceCmpResult *result, int event)
{
	Alarm_S stAlarm;
	char filename[128];
	char datetime[6] = { 0 };
	File_Info_s stInfo;
	get_BCD_time((BCD *) &datetime[0], result->pts);

	memcpy(&stAlarm.datetime[0], &datetime[0], 6);

	stAlarm.fileList.clear();
	memset(&stInfo, 0, sizeof(File_Info_s));
	memset(filename, 0, sizeof(filename));

	if (0 == result->result || 1 == result->result) {
		
		stInfo.nIndex = 1;
		stInfo.nType = FILE_TYPE_IMAGE;
		
		__build_file_name(DEV_DSM, filename, event, 1, result->pts, 0);
		DBG_LEVEL_6("filename:%s-", filename);
		
		memcpy(stInfo.szFileName, filename, strlen(filename));
		
		usleep(1000 * 1000);
	}

	stAlarm.fileList.push_back(stInfo);

	if(g_mgr.test_mode){
		create_test_GpsInfo(stAlarm.gpsData);
	} else {
		getGpsInfo(stAlarm.gpsData);
	}
	stAlarm.nDevType					= DEV_DSM;  //Type: DSM alarm
	stAlarm.dsm_alarm.nType				= 0xE2;
	stAlarm.dsm_alarm.result			= result->result;
	stAlarm.dsm_alarm.similarity		= result->similarity;
	stAlarm.dsm_alarm.similar_degree	= result->similar_degree;
	stAlarm.dsm_alarm.cmp_type			= result->cmp_type;
	stAlarm.dsm_alarm.dirver_id_len		= result->ID_len;
	memcpy(stAlarm.dsm_alarm.dirver_id, result->ID, result->ID_len);

	DBG_LEVEL_6("-- authen upload alarm event[0x%02x]", stAlarm.dsm_alarm.nType);
	JT808_APP_ReportAlarm(&stAlarm);
	
	return 0;
}

int reportSpeedingAlarm(bool overspeed)
{
	Alarm_Speed_S AlarmSpeed;
	struGPSData gpsData;

	if(g_mgr.test_mode){
		create_test_GpsInfo(gpsData);
	} else {
		getGpsInfo(gpsData);
	}

	AlarmSpeed.position = 0;
	JT808_APP_Report_OverSpeed_Alarm(overspeed, gpsData, AlarmSpeed);

	return 0;
}

int handle_alarm(unsigned int event, unsigned int pts, unsigned char dev)
{
	int image_id[8] = { 0 };
	int image_type[8] = { 0 };
	int image_num = 0;
	int interval = 0;
	int alarm_type = 0;
    int level = 0x01;
	DBG_LEVEL_6("[event:%04x pts:%u]", event, pts);
	
	image_num = add_capture_list(dev, event, pts, interval);
	//int video_enable = 0;
	if(rdt_alarm_convert(dev, (unsigned int)g_mgr.n_current_speed, event, alarm_type, level))
	{
		DBG_LEVEL_6("[alarm_type:%02X]", alarm_type);
		
		pthread_mutex_lock(&g_mgr.g_alarm_mutex);
		Alarm_S stAlarm;
		RDT_EVENT_S evt;
		evt.event = event;
		evt.alarm_type = alarm_type;
        evt.level = level;
		evt.dev = dev;
		evt.pts = pts;
		evt.image_num = image_num;
		evt.interval = interval;
		evt.video_enable = get_video_enable(event);//video_enable;
		DBG_LEVEL_6("video_enable:%d", evt.video_enable);
		for(int i = 0;i < image_num;i++)
		{
			evt.image_type[i] = image_type[i];
			evt.image_id[i] = image_id[i];
		}
		__create_alarm(evt, stAlarm);
		g_mgr.g_alarm_list.push_back(stAlarm);
		pthread_mutex_unlock(&g_mgr.g_alarm_mutex);
	}

    return 0;
}

#if 0
int handle_alarm_test(unsigned int event, unsigned int pts, unsigned char dev)
{
	int image_id[8] = { 0 };
	int image_type[8] = { 0 };
	int image_num = 0;
	//int interval = 0;
	int alarm_type = 0;
    int level = 0x01;
	event = 0x0202;
	alarm_type = 0x02;
	DBG_LEVEL_6("[Func:%s] [line:%d]****[event:%x pts:%u]\n", __func__, __LINE__, event, pts);

	{
		DBG_LEVEL_6("[Func:%s] [line:%d]****[alarm_type:%02X]\n", __func__, __LINE__, alarm_type);
		pthread_mutex_lock(&g_mgr.g_alarm_mutex);
		Alarm_S stAlarm;
		RDT_EVENT_S evt;
		evt.event = event;
		evt.alarm_type = alarm_type;
        evt.level = level;
		evt.dev = dev;
		evt.pts = pts;
		evt.image_num = 3;
		evt.interval = 0;
		evt.video_enable = 1;
		for(int i = 0;i < image_num;i++)
		{
			evt.image_type[i] = image_type[i];
			evt.image_id[i] = image_id[i];
		}

		if(g_mgr.test_mode){
		create_test_GpsInfo(stAlarm.gpsData);
		} else {
			getGpsInfo(stAlarm.gpsData);
		}

		if (s_last_alarm_pts == evt.pts){
		s_alarm_seq ++;
		}else{
			s_last_alarm_pts = evt.pts;
			s_alarm_seq = 0;
		}

	stAlarm.sequence = s_alarm_seq;
	stAlarm.nDevType = evt.dev;
	if ( evt.dev == ADAS_DEV_ID ){
		stAlarm.adas_alarm.nType         = evt.alarm_type;
		stAlarm.adas_alarm.nLevel        = 0x01;
		stAlarm.adas_alarm.speedFront    = 0;
		stAlarm.adas_alarm.distanceFront = 0;
		
		if (ADAS_LDW_L_ALARM == evt.event ){
			stAlarm.adas_alarm.deviationType = 0x01;
		}else if(ADAS_LDW_R_ALARM == evt.event ){
			stAlarm.adas_alarm.deviationType = 0x02;
		}else{
			stAlarm.adas_alarm.deviationType = 0;
		}
		
		stAlarm.adas_alarm.roadFlag      = 0;
		stAlarm.adas_alarm.roadFlagData  = 0;
	}

	
	char datetime[6] = { 0 };
	get_BCD_time((BCD *) &datetime[0], evt.pts);
	memcpy(&stAlarm.datetime[0], &datetime[0], 6);

	File_Info_s stInfo;
	char filename[128];
	time_t cap_ts = evt.pts;
    stAlarm.fileList.clear();
	if(g_mgr.dev_param.imageEnable)
	{
		for(int i = 0;i < evt.image_num;i++)
		{
			memset(&stInfo, 0, sizeof(File_Info_s));
			memset(filename, 0, sizeof(filename));
			stInfo.nIndex = i + 1;
			stInfo.nType = FILE_TYPE_IMAGE;
			if(evt.image_type == 0)
			{
				cap_ts += (i * evt.interval) / 1000;
			}

			if (i == 0){
				memcpy(filename, "/tmp/capture/ldw_02_20190617_160236_adas.jpg", sizeof("/tmp/capture/ldw_02_20190617_160236_adas.jpg"));
			}

			if (i == 1){
			
				memcpy(filename, "/tmp/capture/ldw_02_20190617_160236_01_adas.jpg", sizeof("/tmp/capture/ldw_02_20190617_160236_01_adas.jpg"));
			}

			
			if (i == 2){
			
				memcpy(filename, "/tmp/capture/ldw_02_20190617_160236_02_adas.jpg", sizeof("/tmp/capture/ldw_02_20190617_160236_02_adas.jpg"));
			}
			DBG_LEVEL_6("[Func:%s] [line:%d]****[index:%d filename:%s]", __func__, __LINE__, i, filename);
			
			memcpy(stInfo.szFileName, filename, strlen(filename));
			stAlarm.fileList.push_back(stInfo);
		}
	}

	if(evt.video_enable)
	{
		memset(&stInfo, 0, sizeof(File_Info_s));
		memset(filename, 0, sizeof(filename));
		stInfo.nIndex = 0;
		stInfo.nType = FILE_TYPE_VIDEO;
		memcpy(filename, "/tmp/capture/ldw_02_20190617_160236_adas.mp4", sizeof("/tmp/capture/ldw_02_20190617_160236_adas.mp4"));

		//__build_file_name(stAlarm.nDevType, filename, evt.event, 0x02, cap_ts, 0);
		if (0x11 == (( evt.event >> 8 ) & 0xFF)){
//			gdbus_if_record_small_video(filename, cap_ts, 5, 5);
		}
	
		memcpy(stInfo.szFileName, filename, strlen(filename));
		stAlarm.fileList.push_back(stInfo);
		DBG_LEVEL_6("[Func:%s] [line:%d]****[filename:%s]", __func__, __LINE__, filename);
	}
		g_mgr.g_alarm_list.push_back(stAlarm);
		pthread_mutex_unlock(&g_mgr.g_alarm_mutex);
	}

    return 0;
}
#endif

bool handle_snapshot(int dev, unsigned int event, unsigned int *pts, char *filename)
{
    if(filename == NULL){
        return false;
    }

//	uint32_t speed = 0;
	if (0x1200 == event) {
//		ai_server_capture_img(pts, &speed);
	} else {
//		dfw_capture_image(pts, &speed);
	}
    __build_file_name(dev, filename, event, 1, (time_t)*pts, 0);

    return true;
}

static bool s_bOverSpeed_Flag = false;
int handle_speed(int speed)
{
	g_mgr.n_current_speed = speed;
	bool bOverSpeed_Flag = false;

	if(speed == g_mgr.dev_param.overspeed)
	{
//		gdbus_play_audio(WAV_PATH WAV_SPEED_ATTEN);
	}

	if(speed > g_mgr.dev_param.overspeed)
	{
//		gdbus_play_audio(WAV_PATH WAV_SPEEDING);
		bOverSpeed_Flag = true;
	}
	else
	{
		bOverSpeed_Flag = false;
	}

	if(s_bOverSpeed_Flag != bOverSpeed_Flag)
	{
		reportSpeedingAlarm(bOverSpeed_Flag);
		s_bOverSpeed_Flag = bOverSpeed_Flag;
	}

	return 0;
}
#if 0
static int overSpeedHandler(void* user_data)
{
	static int cnt = 0;
	if(g_mgr.n_current_speed > g_mgr.dev_param.overspeed && (cnt++ % 300 == 0))
	{
//		gdbus_play_audio(WAV_PATH WAV_SPEEDING);
		DBG_LEVEL_6("cnt[%d] s_bOverSpeed_Flag[%d]", cnt, s_bOverSpeed_Flag);
	}
	else if(s_bOverSpeed_Flag == false)
	{
		cnt = 0;
	}

	return 1;
}
#endif
void overSpeedService(void)
{
//	g_timeout_add(50, overSpeedHandler, NULL);
}

static void *upload_pthread(void *param)
{
	Alarm_S stAlarm;
	bool bUpdate = false;

	while(g_mgr.g_upload_status)
	{
		pthread_mutex_lock(&g_mgr.g_alarm_mutex);

		if(g_mgr.g_alarm_list.size() > 0)
		{
			auto iter = g_mgr.g_alarm_list.begin();
			stAlarm = *iter;
			g_mgr.g_alarm_list.erase(iter);
			bUpdate = true;
		}

		pthread_mutex_unlock(&g_mgr.g_alarm_mutex);

		if(bUpdate)
		{
			DBG_LEVEL_6("upload alarm event[0x%02x]", stAlarm.dsm_alarm.nType);
			JT808_APP_ReportAlarm(&stAlarm);
			bUpdate = false;
		}
		usleep(20 * 1000);
	}

	return NULL;
}

static void *authen_pthread(void *param) {
    Alarm_S stAlarm;
	RDT_EVENT_S rtEvt;
//	uint32_t speed = 0;
	uint32_t pts = 0;
	char filename[128];
	char datetime[6] = { 0 };
	File_Info_s stInfo;

    while(_auth_status)
    {
		if (g_mgr.g_auth_list.size() > 0) {
			pthread_mutex_lock(&g_mgr.g_auth_mutex);
			auto iter = g_mgr.g_auth_list.begin();
			rtEvt = *iter;
			g_mgr.g_auth_list.erase(iter);
			pthread_mutex_unlock(&g_mgr.g_auth_mutex);
		} else{
			usleep(500 * 1000);
			continue;
		}
		
		get_BCD_time((BCD *) &datetime[0], rtEvt.pts);
		memcpy(&stAlarm.datetime[0], &datetime[0], 6);

		stAlarm.fileList.clear();

		for (int i = 0; i < rtEvt.image_num; i++) {
			memset(&stInfo, 0, sizeof(File_Info_s));
			memset(filename, 0, sizeof(filename));
			stInfo.nIndex = i + 1;
			stInfo.nType = FILE_TYPE_IMAGE;

//			dfw_capture_image(&pts, &speed);
			//指定event为0x0801为手动模式抓图
			__build_file_name(rtEvt.dev, filename, 0x0801, 1, pts, 0);
			
			DBG_LEVEL_6("***[filename:%s]", filename);
			
			memcpy(stInfo.szFileName, filename, strlen(filename));
			stAlarm.fileList.push_back(stInfo);
			usleep(1000 * 1000);
		}

		if(g_mgr.test_mode){
			create_test_GpsInfo(stAlarm.gpsData);
		} else {
			getGpsInfo(stAlarm.gpsData);
		}
		stAlarm.nDevType = rtEvt.dev;  //Type: DSM alarm
		stAlarm.dsm_alarm.nType = rtEvt.alarm_type;
		stAlarm.dsm_alarm.nLevel = rtEvt.level;

		DBG_LEVEL_6("authen upload alarm event[0x%02x]", stAlarm.dsm_alarm.nType);
		
		JT808_APP_ReportAlarm(&stAlarm);
		usleep(100 * 1000);
		
	}
	return NULL;
}

int uploadAlarmPthread_start(void)
{
	int ret = 0;
	pthread_t upload_tid;
	g_mgr.g_upload_status = 1;

	ret = pthread_create(&upload_tid, 0, upload_pthread, NULL);
	return ret;
}

int faceAuthenEvent_start(void)
{
    int ret = 0;
    pthread_t authen_tid;
    _auth_status = 1;

    ret = pthread_create(&authen_tid, 0, authen_pthread, NULL);
    return ret;
}

