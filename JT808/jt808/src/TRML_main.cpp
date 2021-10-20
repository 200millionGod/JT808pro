/*************************************************************
 * Copyright			: 
 * FileName				: main.cpp
 * Description			: main.cpp implement
 * Author		        : chas
 * Created on			: Jan 3, 2019
 *************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/socket.h>

#include "../inc/debug_log.h"
#include "../inc/TRML_video_ctrl.h"
#include "../inc/TRML_driver.h"
#include "../inc/TRML_alarm.h"
#include "../inc/TRML_capture.h"
#include "../inc/TRML_jt808.h"
#include "../inc/TRML_parse.h"
#include "../inc/TRML_util.h"
#include "../inc/TRML_Cfg_parse.h"
#include "../inc/TRML_TTS.h"
#include "../inc/TRML_third_party.h"
#include "version.h"

#include "../inc/TRML_tinyxml2.h"

#include "../inc/TRML_gps_info.h"

#define TERMINAL_PARAMS_PATH "./terminal_param.xml"
#define DFSS_PARAMS_PATH     "/usr/local/etc/dfsscfg-jsatl.xml"
#define IFV_PARAMS_PATH      "/usr/local/etc/ifv.xml"

JTT808_mgr g_mgr;

typedef struct
{
	const char *arg;
	const char *str;
} argv_hint_t;

static const char *G_argv_short_opts = "f:v:ts:";


static struct option G_argv_long_opts[] =
{
    { "config-file", required_argument, 0, 'f' },
	{ "save-log", required_argument, 0, 's' },
	{ "version", no_argument, 0, 'v' },
	{ "test-mode", no_argument,  0, 't'},
	{ 0, 0, 0, 0 }
};

int printInfo()
{
	printf("Version: \033[31;43mv%d.%d.%d\033[0m\n", MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION);
	printf("Build date and time:  \033[31;43m%s, %s\033[0m\n", __DATE__, __TIME__);
	return 0;
}

static int init_param(int argc, char **argv, JTT808_mgr *ptArgv)
{
	int ch;
	int option_index = 0;

	srand(time(NULL));
	pthread_mutex_init(&g_mgr.g_capture_mutex, NULL);
	pthread_mutex_init(&g_mgr.g_alarm_mutex, NULL);
	pthread_mutex_init(&g_mgr.g_facepic_mutex, NULL);
	pthread_mutex_init(&g_mgr.g_videofile_mutex, NULL);

	ptArgv->configure_file_path = (char *) CONFIG_FILE_PATH;
	g_mgr.log_file_path = NULL;

	while((ch = getopt_long(argc, argv, G_argv_short_opts, G_argv_long_opts, &option_index)) != -1)
	{
		switch(ch)
		{
		case 'f':
			ptArgv->configure_file_path = optarg;
			break;
		case 'v':
			printInfo();
			break;
		case 't':
			g_mgr.test_mode = 1;
			break;
		case 's':
			g_mgr.log_file_path = optarg;
			break;
		default:
			DBG_LEVEL_6("unknown option found: %c", ch);
			goto error;
		}
	}

	return 0;

	error: DBG_LEVEL_6("init param error");
	
	return -1;
}

int load_term_config(void)
{
	XMLParse xml_parse;
	if(!access(g_mgr.configure_file_path, F_OK))
	{
#if 1
		DBG_LEVEL_6("jtt808.xml exist...");
		if(!xml_parse.getInfo(g_mgr.configure_file_path, g_mgr.control_info, g_mgr.config, g_mgr.terminal, g_mgr.dev_param))
		{
			DBG_LEVEL_6("get XML file info error!");
			return -1;
		}
#endif
//		g_mgr.control_info.bEnable = 1;
//		xml_parse.SetMgrXml(g_mgr.configure_file_path, g_mgr);
	}
	else
	{
		DBG_LEVEL_6("configure file not exist,file Path[%s]", g_mgr.configure_file_path);
		return -1;
	}

	return 0;
}

int load_term_param(void)
{
	XMLParse xml_parse;
	if (access(TERMINAL_PARAMS_PATH, F_OK) == 0){
		DBG_LEVEL_6("terminal_param.xml exist, loadTerminalParam...");
		xml_parse.LoadTerminalParam(TERMINAL_PARAMS_PATH, g_mgr.terminal_cfg) ;
	}else{
		DBG_LEVEL_6("terminal_param.xml dose not exist, loadDefaultParam...");
		xml_parse.LoadDefaultParam(TERMINAL_PARAMS_PATH, &g_mgr.terminal_cfg) ;
	}
#if 0	
	TerminalXMLParse xml_parse;
	if (access(TERMINAL_PARAMS_PATH, F_OK) == 0){
		DBG_LEVEL_6("terminal_param.xml exist, loadTerminalParam...");
		xml_parse.LoadTerminalParam(TERMINAL_PARAMS_PATH, &g_mgr.terminal_cfg) ;
	}else{
		DBG_LEVEL_6("terminal_param.xml dose not exist, loadDefaultParam...");
		xml_parse.LoadDefaultParam(TERMINAL_PARAMS_PATH, &g_mgr.terminal_cfg) ;
	}
#endif	
	return 0;
}

int load_video_enable(void)
{
	DBG_LEVEL_6(" ");
	TerminalXMLParse xml_parse;
	if (access(DFSS_PARAMS_PATH, F_OK) == 0){
		DBG_LEVEL_6(" ");
		xml_parse.LoadDfssParam(DFSS_PARAMS_PATH, &g_mgr.config.dsm_config) ;
	}else{
		//xml_parse.LoadDefaultParam(TERMINAL_PARAMS_PATH, &g_mgr.terminal_cfg) ;
	}

	if (access(IFV_PARAMS_PATH, F_OK) == 0){
		DBG_LEVEL_6(" ");
		xml_parse.LoadIfvParam(IFV_PARAMS_PATH, &g_mgr.config.adas_config) ;
	}
	
	return 0;
}

static void set_term_cfg_callback(JT808_Config_S *cfg)
{
	
	TerminalXMLParse xml_parse;
	memcpy(&g_mgr.terminal_cfg, cfg, sizeof(JT808_Config_S));

	xml_parse.saveTerminalParam(TERMINAL_PARAMS_PATH, cfg) ;//terminal_cfg);

	char cmd[128] = {0};
	sprintf(cmd, "/bin/sync %s", TERMINAL_PARAMS_PATH);
	system(cmd);
}

static bool snapshot_callback(int dev, unsigned short cmd, unsigned short period, unsigned int *pts, char *filename)
{
//	return handle_snapshot(dev, 0x0801, pts, filename);
	return handle_snapshot_by_xj(dev, cmd, period, 0x0801, pts, filename);
}

static bool facesnap_callback(int dev, uint32_t *pts, char *filename)
{
	return handle_snapshot(dev, 0x1200, pts, filename);
}

static void face_picture_callback(FacePicPublish *facePicData)
{
	if (0 == facePicData->cmd_type ){
		//add
		download_driver_pic(facePicData);
	}else if(0x1 == facePicData->cmd_type ){
		//delete all 
	}else if (0x2 == facePicData->cmd_type ){
		//delete this item
	}else if(0x3 == facePicData->cmd_type ){
		//update
		download_driver_pic(facePicData);
	}
}

static void face_cmp_res_cb(FaceCmpPubResult *fcmp)
{
    if (0 == fcmp->result) {
	   DBG_LEVEL_6("face pic compare result: %d, %d", fcmp->result, fcmp->similarity);
	}

//	face_auth_result( fcmp->ID, fcmp->ID_len, fcmp->result);
}

static void get_gpsdata_callback(struGPSData *gpsData)
{
	if(gpsData != NULL){
		//struGPSData stGpsData;
		if(g_mgr.test_mode){
			create_test_GpsInfo(*gpsData);
		} else {
			getGpsInfo(*gpsData);
		}
		//memcpy(gpsData, &stGpsData, sizeof(struGPSData));
	}
}

#define version_path          "/usr/local/etc/version"
#define hw_version_filename   "/tmp/box_version.txt"
#if 0
static int get_vesion_from_file(char *sw_version, DWORD len)
{
	/*char buf[len] = {0};*/char buf[len];
	DWORD i;
    //no such file
    if(access(version_path, F_OK|R_OK)){
        return -1;
    }

    FILE* fp = fopen(version_path, "r");
	if (!fp)
	{
		printf("open file fail\n");
		return -1;
	}
	
    if(fgets(buf, len, fp)) {

		i = 0;
		printf("buf:%s\n", buf);
		while(  (buf[i] != '\n')   && (buf[i]  !=   '\0' )  )
		{
			sw_version[i] = buf[i];
			i++;
			if (i == len - 1)
			{
				sw_version[i] ='\0';
				break;
			}
			
		}
				
        if(sw_version[i] ==  '\n')
        {
			sw_version[i] = '\0';
		}
    }

	int j = 0, flag = 0;
	for (unsigned int i = 0; i < strlen(sw_version); i++)
	{
		if (sw_version[i] == ' ' && !flag){
			//sw_version[j++] = sw_version[i];
			flag = 1;
			continue;
		}

		if (flag){
			sw_version[j++] = sw_version[i];
		}
	}

	sw_version[j] = '\0';

	DBG_LEVEL_6(" ver:%s", sw_version);
	
    fclose(fp);
	
    return 0;
}
#endif

static int get_HW_version_from_file(char *hw_version, DWORD len)
{
	/*char buf[len] = {0};*/char buf[len];
	DWORD i = 0;
    //no such file
    if(access(hw_version_filename, F_OK|R_OK)){
        return -1;
    }

    FILE* fp = fopen(hw_version_filename, "r");
	if (!fp)
	{
		printf("open file fail\n");
		return -1;
	}
	
    if(fgets(buf, len, fp)) {
        while(  (buf[i] != '\n')   && (buf[i]  !=   '\0' )  )
		{
			hw_version[i] = buf[i];
			i++;
			if (i == len - 1)
			{
				hw_version[i] ='\0';
				break;
			}
			
		}
				
        if(hw_version[i] ==  '\n')
        {
			hw_version[i] = '\0';
		}
				
    }

	int j = 0, flag = 0;
	for (unsigned int i = 0; i < strlen(hw_version); i++)
	{
		if (hw_version[i] == ':' && !flag){
			flag = 1;
			continue;
		}

		if (flag){
			hw_version[j++] = hw_version[i];
		}
	}

	hw_version[j] = '\0';

	DBG_LEVEL_6(" ver:%s", hw_version);

    fclose(fp);
    return 0;
}

int terminalService_init(void)
{
	Terminal_Attr stTermAttr;
	memset(&stTermAttr, 0, sizeof(Terminal_Attr));
	Terminal term = { 0 };
	char devid[24] = { 0 };
	char version[24] = "3.7.15";

//	memset(&term, 0, sizeof(term));
	get_devid(devid, sizeof(devid));
	memset(devid, 0, sizeof(devid));
	memcpy(devid, g_mgr.terminal.devIMEI + 9, 6);
	DBG_LEVEL_6("devid[%s] len[%d]", devid, (int)strlen(devid));
	DBG_LEVEL_6("devIMEI[%s] len[%d]", g_mgr.terminal.devIMEI, (int)strlen(g_mgr.terminal.devIMEI));
	DBG_LEVEL_6("g_mgr.terminal.nFlag[%d]", g_mgr.terminal.nFlag);
	get_terminalInfo(devid, &term, (int)strlen(devid), g_mgr.dev_param.pre_sim, 5);
	if (1 == g_mgr.terminal.nFlag) {
		memset(term.phone_num, 0, sizeof(term.phone_num));
		memcpy(term.phone_num, g_mgr.terminal.phone_num, 16);
		DBG_LEVEL_6("term.phone_num[%s]", term.phone_num);
//		get_terminalInfo(devid, &term, (int)strlen(devid), g_mgr.dev_param.pre_sim, 5);
	}else{
		memset(g_mgr.terminal.phone_num, 0, sizeof(g_mgr.terminal.phone_num));
		memcpy(&g_mgr.terminal.phone_num[0], term.phone_num, 12);
	}
	
	char *url = g_mgr.config.session.server_url;
	int port = g_mgr.config.session.port;

	memcpy(stTermAttr.manufacturerID, "JWSRE", 5);
	/*
	 memcpy(stTermAttr.devID, g_mgr.terminal.devID, 7);
	 memcpy(stTermAttr.devType, g_mgr.terminal.devType, 20);
	 */
	memcpy(stTermAttr.devType, term.devType, sizeof(term.devType));
//	get_vesion_from_file(version, 24);
	memcpy(stTermAttr.softVer, version, strlen(version));
	DBG_LEVEL_6("softver:%s", stTermAttr.softVer);
	get_HW_version_from_file(stTermAttr.hardVer, 24);
	DBG_LEVEL_6("hardVer:%s", stTermAttr.hardVer);
	//memcpy(stTermAttr.hardVer, "V1.1.1", 6);
	
	memcpy(stTermAttr.simICCID, g_mgr.box_info.ICCID, 10);
	memset(term.devIMEI, 0, sizeof(term.devIMEI));
	memcpy(term.devIMEI, g_mgr.terminal.devIMEI, 30);

	stTermAttr.gnss			= 0x01;
	stTermAttr.wireless		= 0x80;
	stTermAttr.supportMask	= 0x08;
//	printf("g_mgr.test_mode=%d, g_mgr.terminal.devID = |%s|\n", g_mgr.test_mode, g_mgr.terminal.devID);
//	if(g_mgr.test_mode)
//	{
//		memcpy(stTermAttr.devID, g_mgr.terminal.devID, 16);
//		memcpy(term.phone_num, g_mgr.terminal.phone_num, 12);
//		memcpy(stTermAttr.devType, g_mgr.terminal.devType, 20);
//	}
	memcpy(stTermAttr.devID, term.devID, sizeof(term.devID));
	memcpy(stTermAttr.devIMEI, term.devIMEI, 15);
	DBG_LEVEL_6("g_mgr.terminal.devIMEI[%s] len[%d]", g_mgr.terminal.devIMEI, (int)strlen(g_mgr.terminal.devIMEI));
	DBG_LEVEL_6("term.devIMEI[%s] len[%d]", term.devIMEI, (int)strlen(term.devIMEI));
//	printf("bbbbbbbbbbbbbstTermAttr.devID=|%s|\n", stTermAttr.devID);
//	printf("stTermAttr.devIMEI=|%s|\n", stTermAttr.devIMEI);
//	printf("term.phone_num:");
//	for(int i = 0; i < 10; i++)
//	{
//		printf("%02x ", term.phone_num[i]);
//	}
//	printf("\n");
	g_mgr.n_current_speed = 0;
	JT808_APP_SetPhoneNumber((const unsigned char*) term.phone_num);
	JT808_APP_SetTerminalAttr(stTermAttr);
	JT808_APP_GetTerminalCfg(&g_mgr.terminal_cfg);
	JT808_APP_SetServerAddr(SOCK_STREAM, url, port);

	/*设置终端参数回调*/
	JT808_APP_SetCfg_Callback(set_term_cfg_callback);
	/*主动抓拍回调*/
	JT808_APP_SetSnapShot_Callback(snapshot_callback);
	/*人脸主动抓拍回调*/
	JT808_APP_FaceAuthSnap_Callback(facesnap_callback);
	/*获取gps数据回调*/
	JT808_APP_SetGPS_Callback(get_gpsdata_callback);
	/*下载人脸认证图片回调*/
	JT808_APP_FacePic_Callback(face_picture_callback);
	/*人脸认证结果处理回调*/
	JT808_APP_FacePicCmpPubRes_Callback(face_cmp_res_cb);
	/*查询人脸库信息回调*/
	JT808_APP_FaceInfo_Callback(face_info_query_callback);
	/*触发人脸认证回调*/
	JT808_APP_FaceCmp_Callback(trigger_face_cmp_callback);
	/*获取音视频参数回调*/
	JT808_App_videoCfgGet_CallBack(video_get_parameter_callback);
	/*请求实时音视频流回调*/
	JT808_App_videoReqStream_CallBack(video_req_stream_callback);
	/*实时音视频流控制回调*/
	JT808_App_videoStreamCtrl_CallBack(video_stream_ctrl_callback);
	/*音视频资源查询回调*/
	JT808_App_videoQueSource_CallBack(video_que_resource_callback);
	/*回放请求处理回调*/
	JT808_App_videoReqPlayback_CallBack(video_req_playback_callback);
	/*回放控制回调*/
	JT808_App_videoPlaybackCtrl_CallBack(video_ctrl_playback_callback);
	/*上传文件回调*/
	JT808_App_videoUpFile_CallBack(video_upfile_callback);
	/*文件上传控制回调*/
	JT808_App_videoCtrlUp_CallBack(video_ctrl_upfile_callback);
	/*文本信息下发处理回调*/
	JT808_App_TextInfo_CallBack(text_info_cb);

	
	JT808_APP_Init();

	int ret = SaveGpsInfo_Init();
	if(ret)
	{
		printf("load save gps info \n");
	}

	bool NetAlive = JT808_APP_GetStatus();
	printf("Net = %d\n",NetAlive);
	return 0;
}

static int reportGpsInfo(void* user_data)
{
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

#if 0
	printf("JT808_APP_ReportPosInfo gpsData latitude:[%d][%d][%d]\n", gpsData.cLatitudeDegree, gpsData.cLatitudeCent, gpsData.lLatitudeSec);
	printf("JT808_APP_ReportPosInfo gpsData longitude:[%d][%d][%d]\n", gpsData.cLongitudeDegree, gpsData.cLongitudeCent, gpsData.lLongitudeSec);
#else
	DBG_LEVEL_6("latitude[%f] longitude[%f]", gpsData.fLatitude, gpsData.fLongitude);
#endif

	return 1;
}

/* 位置信息汇报线程 */
void* reportGpsInfoThread(void *param)
{
	while(1)
	{
		if(g_mgr.control_info.nGPSReportTime)
		{
			sleep(g_mgr.control_info.nGPSReportTime);
		}
		else
		{
			sleep(1);	/* 默认10S */
		}
	
		if(JT808_APP_GetStatus())	/* 如果后台正常连接 */
		{
			reportGpsInfo(NULL);
			#if 1
			if(access(SAVE_GPS_INFO_PATH, F_OK) == 0)
			{
				ReadGps();
			}
			#endif
			//saveGpsInfo();
			printf("reportGpsInfo,GPSReportTime = %d \n",g_mgr.control_info.nGPSReportTime);
		}
		else
		{
			//保存GPS数据之前判断一下GPS是否定位，定位才保存，待加上接口
			saveGpsInfo();
		}
		
	}
	return NULL;
}


/* 开启位置信息汇报 0x200 线程 */
void reportGpsInfoService(void)
{
//	g_timeout_add(g_mgr.control_info.nGPSReportTime * 1000, reportGpsInfo, NULL);

	pthread_t reportGps_tid;
	int ret;

	ret = pthread_create(&reportGps_tid,NULL,reportGpsInfoThread,NULL);
	if (ret == 0)
	{
		printf(" create  reportGpsInfoThread success \n");
	}
	else
	{
		printf(" create  reportGpsInfoThread fail %d \n", ret);
	}
}
#if 0
int gdbus_init(void *param)
{
	pthread_t tid1;
	pthread_create(&tid1, 0, dbus_client_start, param);
	return 0;
}
#endif

void checkNetStatus()
{
	while(1)
	{
		char buffer[1024] = {};
		FILE *read_fp = NULL;
	    int ret;
		char aliveFlag[36] = "3 packets transmitted, 3 received";
		memset(buffer, 0, 1024);
		DBG_LEVEL_6("start checking network status...");
		read_fp = popen("ping -c 3 -i 0.2 www.baidu.com | grep time", "r");
	    if(read_fp)
	    {
	        ret = fread(buffer, sizeof(char), 1024-1, read_fp);
			pclose(read_fp);
			if(ret)
			{
				if(strstr(buffer, aliveFlag))
				{
//					DBG_LEVEL_6("%s", strstr(buffer, aliveFlag));
					DBG_LEVEL_6("Net status is alive now, start services...\n");
					break;
				}else
				{
					DBG_LEVEL_6("Net status is not alive, please check network status...\n");
				}
			}
		}
		sleep(3);
	}
}

int main(int argc, char** argv)
{
	PRINT_VERSION(argc, argv);
	if(init_param(argc, argv, &g_mgr) != 0)
	{
		return -1;
	}	
	if(load_term_config() < 0){
		DBG_LEVEL_6("load configure failed");
		return -1;
	}
#if 1	
	if(load_term_param() < 0){
		DBG_LEVEL_6("load load_term_param failed");
		return -1;
	}

	if (load_tts_param() < 0){
		DBG_LEVEL_6("load load_term_param failed");
		return -1;
	}
	load_video_enable();
//	gdbus_init(NULL);
	sleep(1);
	DBG_LEVEL_6(" ");
	printf("g_mgr.terminal.phone_num:%s\n", g_mgr.terminal.phone_num);
	printf("g_mgr.terminal.phone_num:");
	for(int i = 0; i < 10; i++)
	{
		printf("%02x ", g_mgr.terminal.phone_num[i]);
	}
	printf("\n");
	if (g_mgr.control_info.bEnable) {
		set_logfile();
		reportGpsInfoService();
		overSpeedService();
		faceAuthenEvent_start();
		//阻塞检查网络状态，物理网络不通则不开启其他服务
		checkNetStatus();

		getRilInfo(g_mgr);
		terminalService_init();
		uploadAlarmPthread_start();
		captureImagePthread_start();
		UpOrDownloadPthread_start();
	}
	//dbus_client_start();
#endif

	static unsigned int last_time = time(NULL);
	unsigned int cur_time = 0;
	int flag = 1;
	while(1)
	{
#if 1
		cur_time = time(NULL);
		if (cur_time - last_time > 1*10 && flag){
//			printf("\n\n************----func:%s line:%d----\n\n\n", __func__, __LINE__);
			//handle_alarm(0x1201, cur_time, DSM_DEV_ID);
			//last_time = cur_time;
			flag = 0;

			//handle_face_compare_result(NULL);
		}
#endif
		sleep(10);
	}


	return 0;
}
