/*************************************************************
 * Copyright			: 
 * FileName				: util.cpp
 * Description			: util.cpp implement
 * Author		        : chas
 * Created on			: Jan 9, 2019
 *************************************************************/
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>
#include "../inc/TRML_util.h"
#include "../inc/TRML_jt808.h"
#include "../inc/debug_log.h"
#if 0
int getGpsInfo(struGPSData& gpsData)
{
	time_t ts = time(NULL);
    
	gpsData.cGpsStatus = 'A';
	gpsData.usSpeed = ((unsigned short) g_mgr.n_current_speed) * 100; /*速度值*/
#if 0
	gpsData.cLatitudeDegree = g_mgr.gps_info.latitude_degree; /*纬度值的度*/
	gpsData.cLatitudeCent = g_mgr.gps_info.latitude_cent; /*纬度值的分*/
	gpsData.cLongitudeDegree = g_mgr.gps_info.longitude_degree; /*经度值的度*/
	gpsData.cLongitudeCent = g_mgr.gps_info.longitude_cent; /*经度值的分*/
	gpsData.lLatitudeSec = g_mgr.gps_info.latitude_sec; /*纬度值的秒*/
	gpsData.lLongitudeSec = g_mgr.gps_info.longitude_sec; /*经度值的秒*/
#else
	if ( (unsigned int)ts > g_mgr.gps_info.timestamp + 60 )
    {
    	gpsData.fLatitude		= 0;
		gpsData.fLongitude		= 0;
		gpsData.usGpsAngle		= 0;//240; /*gps夹角*/
    }else{
		gpsData.fLatitude		= g_mgr.gps_info.latitude * 1000000;
		gpsData.fLongitude		= g_mgr.gps_info.longitude * 1000000;
		gpsData.usGpsAngle		= g_mgr.gps_info.magnetic;//240; /*gps夹角*/
	}
	
	
	gpsData.GPSSatelliteNum = g_mgr.gps_info.satelliteNum;
	gpsData.GpsLocation		= g_mgr.gps_info.gpslocation;
	gpsData.BeiDouPosition	= g_mgr.gps_info.BDlocation;
	gpsData.Altitude		= g_mgr.gps_info.altitude;
	gpsData.SignalIntensity	= g_mgr.box_info.sig_strength;
	gpsData.status			= g_mgr.gps_info.status;
	
	if (gpsData.fLatitude)
	{
		gpsData.status |= (1 << 1);//定位
		gpsData.status &= ~(1<<2);//北纬
		gpsData.status &= ~(1<<3);//东经
	}
#endif

	return 0;
}
#endif
int get_devid(char *devid, int len)
{
	FILE *fp = fopen(DEFAULT_DEVID_PATH, "r");

	if(NULL == fp)
	{
		DBG_LEVEL_6("fopen error[%s]", strerror(errno));
		return -1;
	}

	if(fp)
	{
		fgets(devid, len, fp);
	}

	if(fp)
	{
		fclose(fp);
	}

	return 0;
}

int get_terminalInfo(char *devid, Terminal *term, int len, const char *pre_sim, int pre_sim_len)
{
	if(devid == NULL || term == NULL || pre_sim == NULL){
		return -1;
	}

	int offset = 0;

	memset(term, 0, sizeof(Terminal));
	strncpy(term->devID, devid, len);
	printf("term->devID = |%s|\n", term->devID);
#if 0
	if((offset = len - sizeof(term->devID) + 2) > 0)
	{
		strncpy(term->devID, devid, 1);
		strncpy(term->devID + 1, devid + offset, sizeof(term->devID) - 2);
		DBG_LEVEL_6("devid[%s]", term->devID);
	}
#endif	
	if(len  > 7)
	{
		strncpy(term->phone_num, pre_sim, pre_sim_len);
		memcpy(term->phone_num + pre_sim_len, term->devID, 7);
		term->phone_num[13] = '\0';
		DBG_LEVEL_6("phone num:%s", term->phone_num);
	}

	if((offset = sizeof(term->devType) - len) > 0)
	{
		memset(term->devType, 0, sizeof(term->devType));
		strncpy(term->devType, DEFAULT_DEVTYPE, strlen(DEFAULT_DEVTYPE));
		
		DBG_LEVEL_6("devType[%s]", term->devType);
	}

	return 0;
}

int convert_GpsInfo(double value, char *degree, char *cent, long *sec)
{
	double tmp;
	*degree = (int) value;
	tmp = (value - (int) value) * 60;
	*cent = (char) tmp;
	tmp = (tmp - (int) tmp) * 60;
	*sec = (char) tmp;

	DBG_LEVEL_6("degree[%d] cent[%d] sec[%d]", (int) *degree, (int) *cent, (int) *sec);
	return 0;
}

int setPthreadStackSize(pthread_attr_t *attr, int stacksize)
{
	int pret = -1;

	pret = pthread_attr_init(attr);
	if(pret != 0)
	{
		return -1;
	}

	pret = pthread_attr_setstacksize(attr, stacksize);
	if(pret != 0)
	{
		return -1;
	}

	return 0;
}

void create_test_GpsInfo(struGPSData& gpsData)
{
//	unsigned short ran = rand() % (21) - 10;
//	double ran_value = ran / 10.0;
//	printf("create_test_GpsInfo[%d] ran_value[%f]\n", ran, ran_value);

	gpsData.cGpsStatus = 'A';
	gpsData.usSpeed    = 8000; /*速度值*/
	gpsData.fLatitude  = 22.570593 * 1000000;
	gpsData.fLongitude = 113.861923 * 1000000;
	gpsData.usGpsAngle = 240; /*gps夹角*/
}

int set_logfile(void)
{
	int id;
	time_t curtime = 0;
	char strTime[64] = { 0 };
//	char dir_name[64] = { 0 };

	if(g_mgr.log_file_path != NULL)
	{
		id = rand() % 1000;

		curtime = time(NULL);

		strftime(strTime, sizeof(strTime), "%H_%M_%S", localtime(&curtime));

		sprintf(g_mgr.log_file_path, "%s/jt808_%s_no%d", g_mgr.log_file_path, strTime, id);

		DBG_LEVEL_6("dir_name[%s]", g_mgr.log_file_path);

		return 0;
	}

	return -1;
}
