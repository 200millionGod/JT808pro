/*************************************************************
 * Copyright			: 
 * FileName				: dbus_if.cpp
 * Description			: dbus_if.cpp implement
 * Author		        : chas
 * Created on			: Jan 3, 2019
 *************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <math.h>

//#include "xcam_iface.h"
#include "../inc/TRML_parse.h"
#include "../inc/TRML_dbus.h"
#include "../inc/TRML_alarm.h"
#include "../inc/TRML_jt808.h"
#include "../inc/TRML_util.h"
#include "../inc/TRML_driver.h"
#include "../inc/TRML_video_ctrl.h"
#include "../inc/debug_log.h"
#if 0
const static char* DBUS_NAME_BASE		= "com.xcam.";
const static char* DBUS_PATH_BASE		= "/com/xcam/";
const static char* DBUS_NAME_GPS		= "gps";
const static char* DBUS_NAME_DFSS		= "dfss";
const static char* DBUS_NAME_IDSP		= "idsp";

const static char* DBUS_NAME_V4GBOX		= "v4gbox";
const static char* DBUS_NAME_VEHICLE	= "vehiclestatus";
const static char* DBUS_NAME_APLAYD		=  "aplayd";
const static char* DBUS_NAME_IMGCAP		=   "imgcap";
const static char* DBUS_NAME_JT808		=	"jt808";
const static char* DBUS_NAME_AI_SERVER	=	"ai_server";
const static char* DBUS_NAME_HOD		=	"hod";
//const static char* DBUS_NAME_JT1078		=	"jt1078";
const static char* DBUS_NAME_diskmgr	=	"diskmgr";
const static char* DBUS_NAME_VCOMST		= "vcom_st";

const static char* SIGNAL_PROPERTIES_CHANGED = "g-properties-changed";
const static char* SIGNAL_JT808_SET          = "handle-set";
const static char* SIGNAL_JT808_GET          = "handle-get";

static string s__key_gps				= "GPRMC";
static string s__key_gps_gpgga			= "GPGGA";
static string s__key_gps_gpsex			= "GPSEX";
static string g__key_dfss				= "DFSSResult";
static string g__key_iccid				= "ICCID";
static string g__key_sig_strength		= "CSQ";
static string g__key_alarm_type			= "AlarmType";
static string g__key_facecmp_result		= "FaceCmpResult";
static string g__key_trigger_faceauth	= "AlarmType";
static string g__key_sd_status			= "sd_status";
static string g__key_acc				= "acc";

//static XCamIfv*		IFVproxy		    = NULL;
//static XCamDfss*	g_proxy_dfss	    = NULL;
//static XCamGps*		g_proxy_gps		    = NULL;
//static XCamAplayd*	g_proxy_aplayd	    = NULL;
//static XCamImgcap*	g__proxy_imgcap	    = NULL;
//static GMainLoop*	g__main_loop	    = NULL;
//static XCamV4gbox*	g_v4gbox_info	    = NULL;
//static XCamJt808*	g_proxy_jt808	    = NULL;
//static XCamHod*	    g_proxy_hod		    = NULL;
//static XCamJt1078*	g_proxy_jt1078	    = NULL;
//static XCamDiskmgr*	g_proxy_disk_status	= NULL;
//static XCamVcom_st* g_proxt_vcomst  = NULL;

//static XCamVehiclestatus*	g_proxy_vehicle		= NULL;
//static XCamAi_server*		g_proxy_ai_server	= NULL;
#define JTT808_CONFIG_PATH	"/usr/local/etc/jtt808.xml"

void dbus_ae_open(void)
{
	GError *error = NULL;
	string bus_name(DBUS_NAME_BASE);
	string bus_path(DBUS_PATH_BASE);

	bus_name += DBUS_NAME_IDSP;
	bus_path += DBUS_NAME_IDSP;
	XCamIdsp* proxy = xcam_idsp_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, bus_name.c_str(), bus_path.c_str(), NULL, &error);
	if(proxy)
	{
		xcam_idsp_call_enable_ae_sync(proxy, 2, 1, NULL, NULL);
		g_object_unref(proxy);
	}
}

void report_dfss_alarm(int event, time_t pts)
{
	if(0x100B == event)
	{
		//open ae
//		printf("start open ae\n");
//		dbus_ae_open(g_proxy_idsp);
//		dbus_ae_open();

		//turn on the led
//		printf("start turn on led\n");
		system("echo 1 > /sys/class/leds/light/brightness");
		
		//wait led worked
		usleep(150 * 1000);
	}

	handle_alarm(event, pts, DSM_DEV_ID);
}

static void dfss_variant_set(GVariant *value)
{
	if(NULL == value)
	{
		return;
	}

	int event;
	DFSSResult stDFSSResult;

	g_variant_get(value, "(utuuuu)", &stDFSSResult.nAlarmType, &stDFSSResult.nTrigTime, &stDFSSResult.nDrvState, &stDFSSResult.nAlarmKeepingTime,
			&stDFSSResult.nCalibState, &stDFSSResult.nAlarmSpeed);

	if(stDFSSResult.nAlarmType == 0x63) {
		event = 0x0802;
	} else if (stDFSSResult.nAlarmType == 0x62){
		event = 0x0803;
	}else {
		event = stDFSSResult.nAlarmType | 0x1000;
	}

	if(stDFSSResult.nAlarmType != 0x1000 && stDFSSResult.nAlarmType != 0x0000){
		DBG_LEVEL_6("[alarmType[%04x] triggerTime[%lu] DriverState[%u] alarmKeepingtime[%u] CalibState[%u] Speed[%u]]",
			stDFSSResult.nAlarmType, stDFSSResult.nTrigTime, stDFSSResult.nDrvState,
			stDFSSResult.nAlarmKeepingTime, stDFSSResult.nCalibState,stDFSSResult.nAlarmSpeed);
		
		report_dfss_alarm(event, stDFSSResult.nTrigTime);
	}
}


static void hod_variant_set(GVariant *value)
{
	if(NULL == value)
	{
		return;
	}

	unsigned int alarm_type;
	unsigned int ts;
	unsigned int event;

	g_variant_get(value, "(uu)", &alarm_type, &ts);

	if(0x01 == alarm_type) {//left hand
		event = 0x1101;
	} else if (0x02 == alarm_type){//right hand
		event = 0x1102;
	}else if (0x03 == alarm_type){//hands
		event = 0x1103;
	}

	handle_alarm(event, ts, DSM_DEV_ID);
}

static void sd_status_change(GVariant *value)
{
	if(NULL == value)
	{
		return;
	}

	int status;
	time_t ts;

	g_variant_get(value, "(ut)", &status, &ts);

	set_sd_status(status);
}

static void acc_variant_set(GVariant *value)
{
	if(NULL == value)
	{
		return;
	}

	int acc_status;
	
	g_variant_get(value, "i", &acc_status);

	if (acc_status){
		
		g_mgr.gps_info.status &= ~(1 << 0);
	}else{
		g_mgr.gps_info.status |= (1 << 0);
	}

	struGPSData gpsData;
	
	if(g_mgr.test_mode)
	{
		create_test_GpsInfo(gpsData);
	}
	else
	{
		getGpsInfo(gpsData);
	}
	
	JT808_APP_ReportPosInfo(gpsData);
	DBG_LEVEL_6("[g_mgr.gps_info.status:%d]", g_mgr.gps_info.status);
}

static void on_properties_changed
(
	GDBusProxy *proxy, GVariant *changed_properties,
	const gchar* const *invalidated_properties,
	gpointer user_data
)
{
	if(g_variant_n_children(changed_properties) > 0)
	{
		GVariantIter *iter;
		const gchar *key;
		GVariant *value;

		g_variant_get(changed_properties, "a{sv}", &iter);
		while(g_variant_iter_loop(iter, "{&sv}", &key, &value))
		{
			if(g__key_dfss.compare(key) == 0)
			{
				dfss_variant_set(value);
			}
			else if(g__key_alarm_type.compare(key) == 0)
			{
				hod_variant_set(value);				
			}
			else if (g__key_sd_status.compare(key) == 0)
			{
				sd_status_change(value);
			}
			else if (g__key_acc.compare(key) == 0)
			{
				acc_variant_set(value);		
			}
		}
		g_variant_iter_free(iter);
	}
}

static void on_ifv_Event_catch(XCamIfv *object, guint ts, guint arg_WarningFlags, guint arg_speed, gpointer user_data)
{
	//IFVResult IFVResult;
	int alarm_type, alarm_sub_type;
	string alarm_type_str;
	int gsensor_alarm = *((int *)user_data);
	unsigned int ldw_w_flag[2];
	unsigned int fcw_w_type;
	unsigned int zebra_w_type = 0;
	int event = 0;

	/*modify by longtao*/
	fcw_w_type    = arg_WarningFlags&0x0000FFFF;
	ldw_w_flag[0] = (arg_WarningFlags>>24)&0xFF;
	ldw_w_flag[1] = (arg_WarningFlags>>16)&0xFF;

	if (1 == gsensor_alarm){
	    alarm_type_str = "urgent";
	    alarm_sub_type = arg_WarningFlags;
	    alarm_type     = URGENT_ALARM;
	}
	else {
	    if (fcw_w_type & 0x07) {
	        alarm_type_str = "fcw";//forward collision warning
	        alarm_sub_type = fcw_w_type&0x07;
	        alarm_type     = FCW_ALARM;

			if (0x02  == alarm_sub_type ){
				event = ADAS_FCW_ALARM;
			}else if ( 0x01 == alarm_sub_type){
				event  = ADAS_VDC_2_LARM;
			}
			/*else if (0x03 == alarm_sub_type ){
				event = ADAS_VDC_1_LARM;
			}*/
	    }

	    if (ldw_w_flag[0] & 0x03) {
	        alarm_type_str = "ldw";//lane offest warning
	        alarm_sub_type = LDW_LEFT_WARMING;
	        alarm_type     = LDW_ALARM;
			event          = ADAS_LDW_L_ALARM;
	    }

	    if (ldw_w_flag[1] & 0x03) {
	        alarm_type_str = "ldw";
	        alarm_sub_type = LDW_RIGHT_WARMING;
	        alarm_type     = LDW_ALARM;
			event          = ADAS_LDW_R_ALARM;
	    }

	    if (zebra_w_type) {
	        alarm_type_str = "zebra";
	        alarm_sub_type = zebra_w_type&0x01;
	        alarm_type     = ZEBRA_ALARM;
	    }

		DBG_LEVEL_6("sub type:%d", alarm_sub_type);

	}
	//printf("\e[32m func=%s : adas alarm_type=%d \e[0m\r\n", __func__, alarm_type);
	//printf("\e[32m func=%s : adas alarm_type=%x \e[0m\r\n", __func__, event);
	DBG_LEVEL_6("[alarm_type:%d event:%x]", alarm_type, event);
	
	if (event){	
		handle_alarm(event, ts, ADAS_DEV_ID);
	}

}

static void vehicle_get_properties(XCamVehiclestatus* object)
{
	GVariant *speed_prop = xcam_vehiclestatus_dup_speed(object);

	if(speed_prop)
	{
		int nSource;
		double dSpeed;
		g_variant_get(speed_prop, "(id)", &nSource, &dSpeed);
		g_variant_unref(speed_prop);
		handle_speed((int) dSpeed);
	}
}

static void on_vehicle_changed (
    GDBusProxy  *proxy,
    GVariant    *changed_properties,
    const gchar* const  *invalidated_properties,
    gpointer     user_data)
{
	vehicle_get_properties((XCamVehiclestatus*)proxy);
}


static XCamDfss* dfss_proxy_init(void)
{
	GError *error = NULL;
	string bus_name(DBUS_NAME_BASE);
	string bus_path(DBUS_PATH_BASE);

	bus_name += DBUS_NAME_DFSS;
	bus_path += DBUS_NAME_DFSS;
	XCamDfss* proxy = xcam_dfss_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, 
														G_DBUS_PROXY_FLAGS_NONE, 
														bus_name.c_str(), 
														bus_path.c_str(), 
														NULL, 
														&error);
	if(proxy)
	{
		g_signal_connect(proxy, SIGNAL_PROPERTIES_CHANGED, G_CALLBACK(on_properties_changed), NULL);
	}

	return proxy;
}

static XCamIfv *XCamIFV_dbs_start(void)
{
	static int ifv_alarm = 0;

	XCamIfv* proxy = xcam_ifv_proxy_new_for_bus_sync(
				G_BUS_TYPE_SYSTEM,
				G_DBUS_PROXY_FLAGS_NONE,
				"com.xcam.ifv",			 /* bus name */
				"/com/xcam/ifv", /* object */
				NULL,						   /* GCancellable* */
				NULL);

	if (proxy){
		g_signal_connect(proxy, "warning", G_CALLBACK(on_ifv_Event_catch), &ifv_alarm);
	}

	return proxy;

}

static XCamVehiclestatus* vehicle_proxy_init(void)
{
	GError *error = NULL;
	string bus_name(DBUS_NAME_BASE);
	string bus_path(DBUS_PATH_BASE);

	bus_name += DBUS_NAME_VEHICLE;
	bus_path += DBUS_NAME_VEHICLE;

	XCamVehiclestatus* proxy = xcam_vehiclestatus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, 
																			G_DBUS_PROXY_FLAGS_NONE, 
																			bus_name.c_str(),
																			bus_path.c_str(), 
																			NULL, 
																			&error);

	if(proxy)
	{
		vehicle_get_properties(proxy);
		g_signal_connect(proxy, SIGNAL_PROPERTIES_CHANGED, G_CALLBACK(on_vehicle_changed), NULL);
	}

	return proxy;
}

int gdbus_play_audio(const char *wav)
{
	if(!g_proxy_aplayd)
	{
		return -1;
	}

	DBG_LEVEL_6("gdbus_play_audio[%s]", wav);

	xcam_aplayd_call_play_sync(g_proxy_aplayd, wav, NULL, NULL);
	
	return 0;
}

static XCamAplayd* aplayd_proxy_init(void)
{
	string bus_name(DBUS_NAME_BASE);
	string bus_path(DBUS_PATH_BASE);

	bus_name += DBUS_NAME_APLAYD;
	bus_path += DBUS_NAME_APLAYD;

	XCamAplayd* proxy = xcam_aplayd_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, bus_name.c_str(), bus_path.c_str(), NULL,
	NULL);

	return proxy;
}

void gdbus_if_record_small_video(const char* file_name,time_t timestamp, int32_t pre_dur, int32_t pos_dur)
{

	char name[128] = {0};
	char tmp_fn[128];

  	sscanf(file_name, "%[^.]", tmp_fn);
	memcpy(name, tmp_fn, strlen(tmp_fn) - 4);
	sprintf(name + strlen(name), ".mp4");

	XCamBufrec* proxy = xcam_bufrec_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
                                                          G_DBUS_PROXY_FLAGS_NONE,
                                                          "com.xcam.bufrec",     /* bus name */
                                                          "/com/xcam/bufrec", 	 /* object */
                                                          NULL,                  /* GCancellable* */
                                                          NULL);
    if (proxy) {
        xcam_bufrec_call_capture_sync(proxy, name, timestamp, pre_dur, pos_dur, NULL, NULL);
    }
	
    g_object_unref(proxy);
}

void dfw_capture_image_alarm(int event, unsigned int ts, int index)
{
	if (0x11 == (( event >> 8 ) & 0xFF)){//hod
		if (g_proxy_hod){
			xcam_hod_call_capture_alarm_sync(g_proxy_hod, event&0xFF, 0, ts, index, NULL, NULL);
		}
	}else{
	    if (g__proxy_imgcap) {
	    	xcam_imgcap_call_capture_alarm_sync(g__proxy_imgcap, (event>>8)&0xFF, event&0xFF, ts, index, NULL, NULL);
	    }
	}
}

void dfw_capture_image(uint32_t *ts, uint32_t *speed)
{
	if (g__proxy_imgcap && NULL != ts && NULL != speed){
    	xcam_imgcap_call_capture_image_sync(g__proxy_imgcap, ts, speed, NULL, NULL);
    }
}

void ai_server_capture_img(uint32_t *ts, uint32_t *speed)
{
	if (g_proxy_ai_server && NULL != ts && NULL != speed){
    	xcam_ai_server_call_capture_image_sync(g_proxy_ai_server, ts, speed, NULL, NULL);
    }
}

static XCamImgcap* imgcap_proxy_init(void)
{
    GError *error = NULL;
    string bus_name(DBUS_NAME_BASE);
    string bus_path(DBUS_PATH_BASE);

    bus_name += DBUS_NAME_IMGCAP;
    bus_path += DBUS_NAME_IMGCAP;
    XCamImgcap* proxy = xcam_imgcap_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
																G_DBUS_PROXY_FLAGS_NONE,
																bus_name.c_str(),
																bus_path.c_str(),
																NULL,
																&error);
        
    return proxy;
}



static void gps_gprmc_get(GVariant *value)
{
	
	int bValid;
	unsigned long long p_timestamp	= 0;
	double p_latitude				= 0;
	double p_longitude				= 0;
	double p_magnetic				= 0;
	double p_speed					= 0;

	g_variant_get(value, "(btdddd)", &bValid, &p_timestamp, &p_latitude, 
					&p_longitude, &p_speed, &p_magnetic);
	
	//printf("func:%s line:%d bValid:%d p_timestamp:%llu p_latitude:%f p_longitude:%f p_magnetic:%f p_speed:%f\n", __func__, __LINE__, 
		//bValid, p_timestamp, p_latitude, p_longitude, p_magnetic, p_speed);
	
	g_mgr.gps_info.timestamp	= p_timestamp;
	if (g_mgr.gps_info.latitude	!= p_latitude){
		g_mgr.gps_info.latitude	= p_latitude;
	}
	
	if (g_mgr.gps_info.longitude != p_longitude	){
		g_mgr.gps_info.longitude = p_longitude;
	}
	
	if (g_mgr.gps_info.magnetic  != p_magnetic){
		g_mgr.gps_info.magnetic  = p_magnetic;
	}
	
	if (g_mgr.gps_info.speed    != p_speed	){
		g_mgr.gps_info.speed     = p_speed;
	}
}

static void gps_gpgga_get(GVariant *value)
{
	double altitude					= 0;
	unsigned int satelliteNum		= 0;
	
	g_variant_get(value, "(ud)",  &satelliteNum, &altitude);
	
	if (g_mgr.gps_info.altitude	!= altitude	){
		g_mgr.gps_info.altitude	= altitude;
	}

	if (g_mgr.gps_info.satelliteNum	!= satelliteNum	){
		g_mgr.gps_info.satelliteNum	= satelliteNum;
	}
}

static void gps_gpsex_get(GVariant *value)
{
	unsigned int gps_or_bd = 0;
	double antenna_status = 0;

	g_variant_get(value, "(uu)",  &gps_or_bd, &antenna_status);
	
	if (1 == gps_or_bd){
		if (g_mgr.gps_info.gpslocation != 1){
			g_mgr.gps_info.gpslocation	= 1;
		}
	}else if(2 == gps_or_bd){
		if (g_mgr.gps_info.BDlocation != 1){
			g_mgr.gps_info.BDlocation	= 1;
		}
	}
}


static void gps_get_properties(XCamGps* object)
{	
	GVariant *gprmc, *gpgga, *gpsex;
	
	gprmc = xcam_gps_dup_gprmc (object);
	if(gprmc) {
		gps_gprmc_get(gprmc);
		if (gprmc){
			g_variant_unref(gprmc);
		}
	}

	gpgga = xcam_gps_dup_gpgga (object);
	if (gpgga){
		gps_gpgga_get(gpgga);
		if (gpgga){
			g_variant_unref(gpgga);
		}
	}

	gpsex = xcam_gps_dup_gpsex (object);
	if (gpsex){
		gps_gpsex_get(gpsex);
		if (gpgga){
			g_variant_unref(gpsex);
		}
	}

}


static void on_gps_changed (
    GDBusProxy  *proxy,
    GVariant    *changed_properties,
    const gchar* const  *invalidated_properties,
    gpointer     user_data)
{

	gps_get_properties((XCamGps*)proxy);

}

XCamGps* gps_proxy_init(void)
{
	GError *error = NULL;
	string bus_name(DBUS_NAME_BASE);
	string bus_path(DBUS_PATH_BASE);

	bus_name += DBUS_NAME_GPS;
	bus_path += DBUS_NAME_GPS;
	XCamGps* proxy = xcam_gps_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
														G_DBUS_PROXY_FLAGS_NONE,
														bus_name.c_str(),
														bus_path.c_str(),
														NULL,
														&error);

	if (proxy)
	{
	/*
		GVariant *value;
		value = xcam_gps_get_gpgga(proxy);
		if (value){
			gps_gpgga_get(value);
		}

		value = xcam_gps_get_gpsex(proxy);
		if (value){
			gps_gpsex_get(value);
		}
		*/
        g_signal_connect(proxy,SIGNAL_PROPERTIES_CHANGED, G_CALLBACK(on_gps_changed), NULL);
    }

	return proxy;
}



static void sig_strength_set(XCamV4gbox* object)
{		
	unsigned int csq = 0;
	csq = xcam_v4gbox_get_csq(object);
	g_mgr.box_info.sig_strength = csq;
	DBG_LEVEL_6("csq:%d", csq);
}


static void sim_id_set(XCamV4gbox* object)
{	
	char *iccid = (char *)malloc(20);
	memset(iccid, 0, 20);
	if(NULL != iccid){
		memset(iccid, 0, 10);
		iccid = (char *)xcam_v4gbox_get_iccid (object);
		if (NULL != iccid){
			int i;
			for (i = 0; i < 20; i++){
				printf("%02x ", iccid[i]);
			}
			DBG_LEVEL_6("iccid:%s", iccid);

			memset(g_mgr.box_info.ICCID, 0, 10);
			for (i = 0; i < 10; i++){
				g_mgr.box_info.ICCID[i] = iccid[i*2] << 4 | iccid[i*2 + 1];
			}
		}
		if (iccid)free(iccid);		
	}

}


static void on_v4g_box_changed (GDBusProxy  *proxy,
    		GVariant    *changed_properties,
    		const gchar* const  *invalidated_properties,
    		gpointer     user_data)
{
	
	if (g_variant_n_children (changed_properties) > 0)
	{
		GVariantIter *iter;
		const gchar *key;
		GVariant *value;
		
		g_variant_get (changed_properties,"a{sv}",&iter);
		while (g_variant_iter_loop (iter, "{&sv}", &key, &value))
		{
			
			if(g__key_iccid.compare(key)==0)
			{
				sim_id_set((XCamV4gbox*)proxy);
			}else if (g__key_sig_strength.compare(key)==0)
			{
				sig_strength_set((XCamV4gbox*)proxy);
			}
		}
		g_variant_iter_free (iter);
	}
	
	
}

XCamV4gbox* v4gbox_proxy_init(void)
{
	GError *error = NULL;
	string bus_name(DBUS_NAME_BASE);
	string bus_path(DBUS_PATH_BASE);

	bus_name += DBUS_NAME_V4GBOX;
	bus_path += DBUS_NAME_V4GBOX;
	XCamV4gbox* proxy = xcam_v4gbox_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,G_DBUS_PROXY_FLAGS_NONE,bus_name.c_str(),bus_path.c_str(),NULL,&error);

	if (proxy)
	{
		g_mgr.box_info.ICCID[0]	= 0x01;
		g_mgr.box_info.ICCID[1]	= 0x02;
		g_mgr.box_info.ICCID[2]	= 0x03;
		g_mgr.box_info.ICCID[3]	= 0x04;
		g_mgr.box_info.ICCID[4]	= 0x05;
		g_mgr.box_info.ICCID[5]	= 0x06;
		g_mgr.box_info.ICCID[6]	= 0x07;
		g_mgr.box_info.ICCID[7]	= 0x08;
		g_mgr.box_info.ICCID[8]	= 0x09;
		g_mgr.box_info.ICCID[9]	= 0x11;
		
		sig_strength_set(proxy);
		sim_id_set(proxy);
	
        g_signal_connect(proxy,SIGNAL_PROPERTIES_CHANGED, G_CALLBACK(on_v4g_box_changed), NULL);
    }
	
	return proxy;
}



void xcam_jt808_param_get (
						    XCamJt808 *object,
						    GDBusMethodInvocation *invocation,
						    guint arg_type,
							gpointer user_data)
{
	DBG_LEVEL_6("--[arg_type:%d]--", arg_type);
	char msg[128] = {0};
	if (1 == arg_type){//get ip and port
		sprintf(msg, "%s:%d", g_mgr.config.session.server_url, g_mgr.config.session.port);
	}else if (2 == arg_type){//get phonenum
		memcpy(msg, g_mgr.terminal.phone_num, 12);
	}
	DBG_LEVEL_6("[msg:%s]", msg );
	
	xcam_jt808_complete_get(object, invocation, msg);
}

void xcam_jt808_param_set (
								XCamJt808 *object,
								GDBusMethodInvocation *invocation,
								guint arg_type,
								gchar *arg_msg,
								gpointer user_data)
{
	DBG_LEVEL_6("-[arg_type:%d arg_msg:%s]", arg_type, arg_msg);
	#if 0
	if (1 == arg_type){
		sscanf(arg_msg, "%s:%u", g_mgr.config.session.server_url, g_mgr.config.session.port);

		DBG_LEVEL_6("func:%s-------------[%d ***** %s]-------------- line:%d\n", __func__, g_mgr.config.session.port, 
				g_mgr.config.session.server_url, __LINE__);
	}else if (2 == arg_type){//set phonenum
		memcpy(g_mgr.terminal.phone_num, arg_msg,  12);
		DBG_LEVEL_6("func:%s-------------[%s]-------------- line:%d\n", __func__,g_mgr.terminal.phone_num, __LINE__);
	}
	
	#endif
	#if 1
	bool bRestartFlag = false;
	XMLParse objXMLParse;
	char *p = (char *)malloc(128);
	if (p) {
		if (1 == arg_type){//set ip and port
			//sscanf(buf. "%s:%d", ipdaar, port);
			if (arg_msg != NULL){
				while (NULL != (p = strchr(arg_msg, ':'))) {
					memset(&g_mgr.config.session.server_url, 0, sizeof(g_mgr.config.session.server_url));
					memcpy(g_mgr.config.session.server_url, arg_msg, strlen(arg_msg) - strlen(p));
					arg_msg = p + 1;
				}

				DBG_LEVEL_6("arg_msg:[%s], arg_msg len :[%lu]",arg_msg, strlen(arg_msg));
				g_mgr.config.session.port = atoi(arg_msg);
				DBG_LEVEL_6("--[port:%d server_url:%s]-", g_mgr.config.session.port, 
					g_mgr.config.session.server_url);
				bRestartFlag = true;
			}
		} else if (2 == arg_type) {//set phonenum
			g_mgr.terminal.nFlag = 1;
			int nPhoneLen = strlen(arg_msg);
			if (nPhoneLen < 12) {
				memset(g_mgr.terminal.phone_num, '0', 12);
				unsigned int nOffset = 12 - nPhoneLen;
				memcpy(g_mgr.terminal.phone_num + nOffset, arg_msg, nPhoneLen + 1);
				g_mgr.terminal.phone_num[nOffset + nPhoneLen] = {'\0'};
			} else {
				memset(g_mgr.terminal.phone_num, 0, sizeof(g_mgr.terminal.phone_num));
				memcpy(g_mgr.terminal.phone_num, arg_msg, nPhoneLen);
				g_mgr.terminal.phone_num[nPhoneLen] = {'\0'};
			}
			DBG_LEVEL_6("--[phone_num:%s]---", g_mgr.terminal.phone_num);
			bRestartFlag = true;
		}

		//save config
		objXMLParse.SetMgrXml(JTT808_CONFIG_PATH, g_mgr);

		free(p);
	}
	#endif 

	xcam_jt808_complete_set(object, invocation);

	if (bRestartFlag) {
		system("systemctl restart jt808");
	}
}


static void on_name_acquired (GDBusConnection* connection, const gchar* name, gpointer user_data)
{
	string bus_path(DBUS_PATH_BASE);

	bus_path += DBUS_NAME_JT808;
	g_print ("Acquired the name %s\n", name);

	XCamJt808* object = (XCamJt808*)user_data;

    g_signal_connect (object,
                      SIGNAL_JT808_SET,
                      G_CALLBACK (xcam_jt808_param_set),
                      NULL);

    g_signal_connect (object,
                      SIGNAL_JT808_GET,
                      G_CALLBACK (xcam_jt808_param_get),
                      NULL);

    if (!g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (object),
                connection,
                bus_path.c_str(),
                NULL))
    {
        /* handle error */
        g_print("export skeleton error.\n");
    }
}

static void on_name_lost (GDBusConnection* connection,const gchar* name,gpointer user_data)
{
	g_print ("Lost the name %s\n", name);
}

static void on_bus_acquired (GDBusConnection* connection,const gchar* name,gpointer user_data)
{
	g_print ("Acquired a message bus connection\n");
}


static XCamJt808* jt808_proxy_init(void)
{
	string bus_name(DBUS_NAME_BASE);

	bus_name += DBUS_NAME_JT808;

	XCamJt808* object  = xcam_jt808_skeleton_new ();
	
	g_bus_own_name(G_BUS_TYPE_SYSTEM,
	               bus_name.c_str(),
                   G_BUS_NAME_OWNER_FLAGS_NONE,
                   on_bus_acquired,
                   on_name_acquired,
                   on_name_lost,
                   object,
                   NULL);
	
	return object;
}





void face_pic_download(unsigned int result, char *name, char *id, uint32_t ts)
{
    if (g_proxy_ai_server) {
    	xcam_ai_server_call_download_result_sync(g_proxy_ai_server, result, name, id, ts, NULL, NULL);
    }
}

void face_info_req_data_to_sql(DriverInfoToAIServer *driverinfo)
{
	if (g_proxy_ai_server ){
    	xcam_ai_server_call_req_data_to_sql_sync(g_proxy_ai_server, driverinfo->driver_id, driverinfo->driver_name, 
													&driverinfo->pic_src, driverinfo->picNo, driverinfo->picSerial, 
													driverinfo->picMode, driverinfo->picSize, driverinfo->face_left_posX,
													driverinfo->face_left_posY, driverinfo->face_right_posX, driverinfo->face_right_posY,
													driverinfo->left_eye_centerX, driverinfo->left_eye_centerY, driverinfo->right_eye_centerX,
													driverinfo->right_eye_centerY, driverinfo->nose_centerX, driverinfo->nose_centerY,
													driverinfo->left_mouth_posX, driverinfo->left_mouth_posY, driverinfo->right_mouth_posX,
													driverinfo->right_mouth_posY,driverinfo->pic_path, driverinfo->ts,NULL, NULL);
    }
}

void face_info_query_from_sql(unsigned int *driver_id_num, char **IDList, uint32_t *ts)
{
	if (g_proxy_ai_server ){
		xcam_ai_server_call_req_data_of_sql_sync(g_proxy_ai_server, driver_id_num, IDList, 
													ts,NULL, NULL);
	}
}

void trigger_face_compare(char *alarm_type, unsigned int ts)
{
	if (g_proxy_ai_server ){
		xcam_ai_server_call_remote_trigger_sync(g_proxy_ai_server, alarm_type, 
													ts,NULL, NULL);
	}
}

void face_auth_result(char *driver_name, unsigned char name_len, unsigned int result)
{
	if (name_len < 1)return;
	if (g_proxy_ai_server ){
		GVariantBuilder builder;
		g_variant_builder_init(&builder, G_VARIANT_TYPE("a(y)"));
		for (int i = 0; i < name_len; ++i){
			DBG_LEVEL_6("i:%d data:%02x ", i , driver_name[i]);
			
			g_variant_builder_add (&builder, "(y)", driver_name[i]);
		}
		
		GVariant *driver_info = g_variant_new("a(y)", &builder);
		if (driver_info == NULL){
			return ;
		}
		xcam_ai_server_call_msg_set_sync(g_proxy_ai_server, result, name_len,
													driver_info,NULL, NULL);
	}
}

static void face_compare_result(GVariant *value)
{
	unsigned int result = 0;
	unsigned int cmp_type, similarity, similar_degree;
	char driver_id[64];
	unsigned int pic_format;
	char pic_path[256];
	uint32_t ts;
	FaceCmpResult cmp_result;
	int event = 0x1200;
	g_variant_get(value, "(uuuususu)", &result, &similarity, &similar_degree, &cmp_type, driver_id, &pic_format, pic_path, &ts);

	if (DVR_IC_TRIGGER == cmp_type || TIMER_TRIGGER == cmp_type || SPEED_TRIGGER == cmp_type
		 || DRIVER_TRIGGER == cmp_type || PLATFORM_TRIGGER == cmp_type) {
		cmp_result.result			= result;
		cmp_result.cmp_type			= cmp_type;
		cmp_result.similarity		= similarity;
		cmp_result.similar_degree	= similar_degree;
		cmp_result.ID_len			= strlen(driver_id);
		memcpy(cmp_result.ID, driver_id, cmp_result.ID_len);
		memcpy(cmp_result.pic_name, pic_path, 128);
		cmp_result.pts = ts;
		event |= cmp_type;

		handle_face_auth_result(&cmp_result, event);
	}
}


static void trigger_face_auth(GVariant *value)
{
	unsigned int alarm_type = 0;
	uint32_t ts;
	unsigned int event = 0;

	g_variant_get(value, "(uu)", &alarm_type, &ts);
	DBG_LEVEL_6("-alarm_type:%d", alarm_type);

	if ((AUTH_DRIVER_TRIGGER == alarm_type) || (AUTH_TIMER_TRIGGER == alarm_type) 
		|| (AUTH_SPEED_TRIGGER == alarm_type) || (AUTH_DVR_IC_TRIGGER == alarm_type) || (AUTH_PLATFORM_TRIGGER == alarm_type)){
		event = 0x1200 + (alarm_type & 0xFF);

		handle_alarm(event, ts, DSM_DEV_ID);
	}
}

static void on_ai_server_changed (GDBusProxy  *proxy,
    		GVariant    *changed_properties,
    		const gchar* const  *invalidated_properties,
    		gpointer     user_data)
{
	if (g_variant_n_children (changed_properties) > 0)
	{
		GVariantIter *iter;
		const gchar *key;
		GVariant *value;
		
		g_variant_get (changed_properties,"a{sv}",&iter);
		while (g_variant_iter_loop (iter, "{&sv}", &key, &value))
		{
			if (g__key_facecmp_result.compare(key)==0){
				face_compare_result(value);
				DBG_LEVEL_6(" ");
			} else if (g__key_trigger_faceauth.compare(key)==0){
				trigger_face_auth(value);
				DBG_LEVEL_6(" ");
			}

			
		}
		g_variant_iter_free (iter);
	}
	
}


static XCamAi_server* ai_server_proxy_init(void)
{
	GError *error = NULL;

	string bus_name(DBUS_NAME_BASE);
	string bus_path(DBUS_PATH_BASE);

	bus_name += DBUS_NAME_AI_SERVER;
	bus_path += DBUS_NAME_AI_SERVER;

	XCamAi_server* proxy = xcam_ai_server_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
								G_DBUS_PROXY_FLAGS_NONE,bus_name.c_str(),
								bus_path.c_str(),NULL,&error);
	
	if (proxy)
	{
        g_signal_connect(proxy,SIGNAL_PROPERTIES_CHANGED, G_CALLBACK(on_ai_server_changed), NULL);
    }
	
	return proxy;
}




void jt1078_req_stream(Jt1078ReqStrem *req_av)
{
	GError *error = NULL;
	g_proxy_jt1078 = xcam_jt1078_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
																G_DBUS_PROXY_FLAGS_NONE,
																"com.xcam.jt1078",
																"/com/xcam/jt1078",
																NULL,
																&error);

	DBG_LEVEL_6("[addr:%s tcp_port:%d chn:%d phone_num:%s] ", req_av->addr, req_av->tcp_port, 
		req_av->chn, req_av->phone_num);
	
	if (g_proxy_jt1078){
		xcam_jt1078_call_req_stream_sync(g_proxy_jt1078, req_av->addr_len, req_av->addr,
													req_av->tcp_port, req_av->udp_port, req_av->chn,
													req_av->data_type, req_av->stream_type, (gchar *)req_av->phone_num, 
													NULL, NULL);
	}
	DBG_LEVEL_6(" ");
	g_object_unref(g_proxy_jt1078);
	g_proxy_jt1078 = NULL;

	DBG_LEVEL_6(" ");

}

void jt1078_ctrl_stream(Jt1078CtrlStrem *ctrl_av)
{
	DBG_LEVEL_6("[chn:%d control_instrut:%d -close_av:%d- switch_stream:%d] ", 
					ctrl_av->chn,ctrl_av->control_instrut, ctrl_av->close_av,
					ctrl_av->switch_stream);
	
	GError *error = NULL;
	g_proxy_jt1078 = xcam_jt1078_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
																G_DBUS_PROXY_FLAGS_NONE,
																"com.xcam.jt1078",
																"/com/xcam/jt1078",
																NULL,
																&error);
	DBG_LEVEL_6(" ");
	if (g_proxy_jt1078){
		xcam_jt1078_call_ctrl_stream_sync(g_proxy_jt1078, ctrl_av->chn, ctrl_av->control_instrut,
										ctrl_av->close_av, ctrl_av->switch_stream, NULL, NULL);
	}

	g_object_unref(g_proxy_jt1078);
	g_proxy_jt1078 = NULL;

	DBG_LEVEL_6(" ");
}

void jt1078_req_playback(Jt1078ReqPlay *req_play)
{
	DBG_LEVEL_6("addr:%s tcp_port:%d -chn:%d- type:%d start_ts:%u end_ts:%u phone_num:%s ",
					req_play->addr,req_play->tcp_port, req_play->chn,
					req_play->type,req_play->start_ts, 
					req_play->end_ts, req_play->phone_num);
	
	GError *error = NULL;
	g_proxy_jt1078 = xcam_jt1078_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
																G_DBUS_PROXY_FLAGS_NONE,
																"com.xcam.jt1078",
																"/com/xcam/jt1078",
																NULL,
																&error);
	DBG_LEVEL_6(" ");
	if (g_proxy_jt1078){
		xcam_jt1078_call_req_playback_sync(g_proxy_jt1078, req_play->addr_len, req_play->addr,
													req_play->tcp_port, req_play->udp_port, req_play->chn,
													req_play->type, req_play->stream_type, req_play->store_type, 
													req_play->pb_method, req_play->multiple, req_play->start_ts, 
													req_play->end_ts, (gchar *)req_play->phone_num,
													NULL, NULL);
	}
	DBG_LEVEL_6(" ");
	g_object_unref(g_proxy_jt1078);
	g_proxy_jt1078 = NULL;

	DBG_LEVEL_6(" ");

}

void jt1078_ctrl_playback(Jt1078CtrlPlay *ctrl_play)
{
	DBG_LEVEL_6("chn:%d control_instrut:%d -close_av:%d- switch_stream:%d ",
					ctrl_play->chn,ctrl_play->control, ctrl_play->multiple,
					ctrl_play->position);
	
	GError *error = NULL;
	g_proxy_jt1078 = xcam_jt1078_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
																G_DBUS_PROXY_FLAGS_NONE,
																"com.xcam.jt1078",
																"/com/xcam/jt1078",
																NULL,
																&error);
	DBG_LEVEL_6(" ");
	if (g_proxy_jt1078){
		xcam_jt1078_call_ctrl_playback_sync(g_proxy_jt1078, ctrl_play->chn, ctrl_play->control,
										ctrl_play->multiple, ctrl_play->position, NULL, NULL);
	}
	DBG_LEVEL_6(" ");

	g_object_unref(g_proxy_jt1078);
	g_proxy_jt1078 = NULL;

	DBG_LEVEL_6(" ");
}
/*
static XCamJt1078* jt1078_proxy_init(void)
{
	
	DBG_LEVEL_6("[Func:%s] [line:%d]****[]\n", __func__, __LINE__);
    GError *error = NULL;
    string bus_name(DBUS_NAME_BASE);
    string bus_path(DBUS_PATH_BASE);
	
	DBG_LEVEL_6("[Func:%s] [line:%d]****[]\n", __func__, __LINE__);
    bus_name += DBUS_NAME_JT1078;
    bus_path += DBUS_NAME_JT1078;
    XCamJt1078* proxy = xcam_jt1078_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
																G_DBUS_PROXY_FLAGS_NONE,
																bus_name.c_str(),
																bus_path.c_str(),
																NULL,
																&error);

	DBG_LEVEL_6("[Func:%s] [line:%d]****[]\n", __func__, __LINE__);
																
    return proxy;
}
*/

 


static XCamHod* hod_proxy_init(void)
{
	GError *error = NULL;
	string bus_name(DBUS_NAME_BASE);
	string bus_path(DBUS_PATH_BASE);

	bus_name += DBUS_NAME_HOD;
	bus_path += DBUS_NAME_HOD;
	XCamHod* proxy = xcam_hod_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, 
														G_DBUS_PROXY_FLAGS_NONE, 
														bus_name.c_str(), 
														bus_path.c_str(), 
														NULL, 
														&error);
	if(proxy)
	{
		g_signal_connect(proxy, SIGNAL_PROPERTIES_CHANGED, G_CALLBACK(on_properties_changed), NULL);
	}

	return proxy;
}




static XCamDiskmgr* diskmgr_proxy_init(void)
{
	GError *error = NULL;
	string bus_name(DBUS_NAME_BASE);
	string bus_path(DBUS_PATH_BASE);

	bus_name += DBUS_NAME_diskmgr;
	bus_path += DBUS_NAME_diskmgr;
	XCamDiskmgr* proxy = xcam_diskmgr_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, 
														G_DBUS_PROXY_FLAGS_NONE, 
														bus_name.c_str(), 
														bus_path.c_str(), 
														NULL, 
														&error);
	if(proxy)
	{
		GVariant *value;
		value = xcam_diskmgr_get_sd_status(proxy);
		if (value){
			sd_status_change(value);
		}

		g_signal_connect(proxy, SIGNAL_PROPERTIES_CHANGED, G_CALLBACK(on_properties_changed), NULL);
	}

	return proxy;
}




static XCamVcom_st* vcomst_proxy_init(void)
{
	GError *error = NULL;
	string bus_name(DBUS_NAME_BASE);
	string bus_path(DBUS_PATH_BASE);

	bus_name += DBUS_NAME_VCOMST;
	bus_path += DBUS_NAME_VCOMST;
	XCamVcom_st* proxy = xcam_vcom_st_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, 
														G_DBUS_PROXY_FLAGS_NONE, 
														bus_name.c_str(), 
														bus_path.c_str(), 
														NULL, 
														&error);
	if(proxy)
	{
		gint acc_status;
		acc_status = xcam_vcom_st_get_acc(proxy);

		if (acc_status){
			g_mgr.gps_info.status &= (1 << 0);
		}else{
			g_mgr.gps_info.status |= (1 << 0);
		}

		DBG_LEVEL_6("[g_mgr.gps_info.status:%d]", g_mgr.gps_info.status);

		g_signal_connect(proxy, SIGNAL_PROPERTIES_CHANGED, G_CALLBACK(on_properties_changed), NULL);
	}

	return proxy;
}



void *dbus_client_start(void *param)
{
	g__main_loop = g_main_loop_new(NULL, FALSE);

	g_proxy_aplayd		= aplayd_proxy_init();
	g_proxy_gps			= gps_proxy_init();
	g_proxy_dfss		= dfss_proxy_init();
	g_proxy_vehicle		= vehicle_proxy_init();
	g__proxy_imgcap		= imgcap_proxy_init();
	IFVproxy			= XCamIFV_dbs_start();			//! *get adas alarm msg from dbus
	g_v4gbox_info		= v4gbox_proxy_init();
	g_proxy_jt808		= jt808_proxy_init();
	g_proxy_ai_server	= ai_server_proxy_init();
	g_proxy_hod			= hod_proxy_init();
	g_proxy_disk_status = diskmgr_proxy_init();
	//g_proxy_jt1078      = jt1078_proxy_init();
	g_proxt_vcomst      = vcomst_proxy_init();
	g_main_loop_run(g__main_loop);

	g_object_unref(g__proxy_imgcap);
	g_object_unref(g_proxy_gps);
	g_object_unref(g_proxy_dfss);
	g_object_unref(g_proxy_vehicle);
	g_object_unref(g_proxy_aplayd);
	g_object_unref(IFVproxy);
	g_object_unref(g_v4gbox_info);
	g_object_unref(g_proxy_jt808);
	g_object_unref(g_proxy_ai_server);
	//g_object_unref(g_proxy_jt1078);
	g_object_unref(g_proxy_disk_status);
	g_object_unref(g_proxt_vcomst);
	
	return NULL;
}

void dbus_client_stop(void)
{
	if(g__main_loop)
	{
		g_main_loop_quit(g__main_loop);
	}
}
#endif
