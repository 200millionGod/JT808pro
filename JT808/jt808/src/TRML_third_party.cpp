/*************************************************************
 * Copyright			: 
 * FileName				: TRML_third_party.cpp
 * Description			: TRML_third_party.cpp implement
 * Author		        : 
 * Created on			: 20211008
 *************************************************************/
#include <stdio.h>
/*ril_info inclue start*/
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <termios.h> 
#include <errno.h>
#include <string.h>
#include <syslog.h>
#include <android/log.h>
#include <vector>

//#define  LOG_TAG  "RIL_info"
/*ril_info inclue end*/

#include "../inc/debug_log.h"
#include "../inc/TRML_third_party.h"
#include "../inc/gps/gps_ext.h"
#include "../inc/gps/gps_info.h"
#include "../inc/rilinfo/ril_info.h"
/*catch mp4 or jpg include start*/
#include "../inc/nl/libnl.h"
#include "../inc/cJSON.h"
/*catch mp4 or jpg include end*/
/*catch h264 stream include start*/
#include "../inc/TRML_dbus.h"
#include "../inc/videoStream/VideoClient.h"
/*catch h264 stream include end*/

/*ril_info start*/
//#define  LOG_TAG  "RIL_info"

//#ifndef ALOGD
//#define ALOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
//#endif

int getRilInfo(JTT808_mgr& g_mgr)
{
//	int enable = 0;
	RilInfo ril;
	memset(&ril, 0, sizeof(ril));
	while(1)
	{
		int ret =xj_ril_read_info(&ril);
		if (ret >= 0)
		{
			DBG_LEVEL_6("conneced is %d", ril.connected);
		    DBG_LEVEL_6("simready is %d", ril.simready);
		    DBG_LEVEL_6("imei is %s", ril.imei);
		    DBG_LEVEL_6("imsi is %s", ril.imsi);
		    DBG_LEVEL_6("iccid is %s", ril.iccid);
		    DBG_LEVEL_6("networkmode is %d", ril.networkmode);
		    DBG_LEVEL_6("networksel is %d", ril.networksel);
		    DBG_LEVEL_6("rsrp is %d", ril.rsrp);
	    	DBG_LEVEL_6("data enable %d", xj_ril_get_data_enable());
			memcpy(g_mgr.terminal.devIMEI, ril.imei, 30);
			memcpy(g_mgr.terminal.phone_num, ril.imei, 30);
			break;
		}else
		{
			DBG_LEVEL_6("ril not ready!!!");
			sleep(2);
		}
	}
	/*
	if (argc == 2) {
	    enable = atoi(argv[1]);
	    xj_ril_set_data_enable(enable);
	}
	else if (argc == 3) {
	    int cmd = atoi(argv[1]);
	    if (cmd == 0) {
	        int type = atoi(argv[2]);
	        xj_ril_set_network_prefer(type);
	    }
	    else if (cmd == 1) {
	        int type = atoi(argv[2]);
	        printf("change sim %d\n", type);
	        xj_ril_change_sim(type);
	    } 
	}
	*/
	return 0;
}
/*ril_info end*/
/*gps start*/
static unsigned int get_count(unsigned int mask)
{
    unsigned int count = 0;
    while (mask > 0) {
        count += mask & 1;
        mask = mask >> 1;
    }
    return count;
}

int getGpsInfo(struGPSData& gpsData)
{
	static int flag = 0;
	if(!flag)
	{
		if(xj_gps_init() == 0)
		{
			printf("goto1\n");
			flag = 1;
		}else
		{
			printf("xj gps init failed...\n");
			flag = 0;
			return flag;
		}
	}
	int ret = -1;

	GpsLocation2 loc;
    GpsSvStatus svs;
    printf("try read gps info\n");
    printf("ver:%s\n", xj_gps_get_version());

	ret = xj_gps_read_svstatus(&svs);
	if(ret == -1)
	{
		printf("goto2\n");
		return ret;
	}

	DWORD count = get_count(svs.used_in_fix_mask[0]) +	\
	get_count(svs.used_in_fix_mask[1]) +						\
	get_count(svs.used_in_fix_mask[2]) +						\
	get_count(svs.used_in_fix_mask[3]) +						\
	get_count(svs.used_in_fix_mask[4]) +						\
	get_count(svs.used_in_fix_mask[5]) +						\
	get_count(svs.used_in_fix_mask[6]) +						\
	get_count(svs.used_in_fix_mask[7]);

	memset(&gpsData, 0, sizeof(gpsData));
	if(count >= 3)
	{
		printf("goto3\n");
		xj_gps_read_location(&loc);
		
//		time_t ts = time(NULL);
		gpsData.cGpsStatus = 'A';
		gpsData.usSpeed = loc.speed; /*速度值*/
#if 0
		gpsData.cLatitudeDegree = g_mgr.gps_info.latitude_degree; /*纬度值的度*/
		gpsData.cLatitudeCent = g_mgr.gps_info.latitude_cent; /*纬度值的分*/
		gpsData.cLongitudeDegree = g_mgr.gps_info.longitude_degree; /*经度值的度*/
		gpsData.cLongitudeCent = g_mgr.gps_info.longitude_cent; /*经度值的分*/
		gpsData.lLatitudeSec = g_mgr.gps_info.latitude_sec; /*纬度值的秒*/
		gpsData.lLongitudeSec = g_mgr.gps_info.longitude_sec; /*经度值的秒*/
#else

		gpsData.fLatitude		= loc.latitude;
		gpsData.fLongitude		= loc.longitude;
		gpsData.usGpsAngle		= loc.bearing;//240; /*gps夹角*/
		gpsData.GPSSatelliteNum = count;
		gpsData.Altitude		= loc.altitude;
		for(DWORD index = 0; index < count; index ++)
		{
			if(svs.sv_list[index].prn > 200)
			{
				gpsData.BeiDouPosition	= 1;
			}
			if(svs.sv_list[index].prn < 100)
			{
				gpsData.GpsLocation		= 1;
			}
			gpsData.SignalIntensity	+= svs.sv_list[index].snr;
		}
		gpsData.SignalIntensity = gpsData.SignalIntensity / count;
//		gpsData.status			= ;

		if (gpsData.fLatitude)
		{
			gpsData.status |= (1 << 1);//定位
			gpsData.status &= ~(1 << 2);//北纬
			gpsData.status &= ~(1 << 3);//东经
		}
	}else
	{
		DBG_LEVEL_6("gps signal weak...get gps info failed...");
	}
#endif
	printf("goto4\n");
	return count;
}
/*gps end*/

/*snap shot start*/
char cmd[NL_MAX_CMD_LEN];
char extra[NL_MAX_EXTRA_LEN];
NLMSG nlmsg;

int recvFlag = 0;

void* nlRecvThread(void *param)
{
	memset(&nlmsg, 0, sizeof(NLMSG));
	nlmsg.cmd = cmd;
	nlmsg.cmd_len = NL_MAX_CMD_LEN;
	nlmsg.extra = extra;
	nlmsg.extra_len = NL_MAX_EXTRA_LEN;
	while(1)
	{
		memset(nlmsg.cmd, 0 , NL_MAX_CMD_LEN);
		memset(nlmsg.extra, 0 , NL_MAX_EXTRA_LEN);
		int ret = recvNLMsg(&nlmsg);
		if (ret > 0)
//			printf("recv %d,%d get msg \"%s\"\n", src, srcgroup, nlmsg.cmd);
			printf("|%s|\n", nlmsg.extra);
			memcpy(extra, nlmsg.extra, ret);
			recvFlag = 1;
	}
}

bool handle_snapshot_by_xj(int dev, unsigned short cmd, unsigned short period, unsigned int event, unsigned int *pts, char *filename)
{
	openlog("nlrecv", LOG_CONS | LOG_PID, 0);
	int ret1 = initNL(NLU_MISC, NLG_TEST);
	DBG_LEVEL_6("initNL returns %d", ret1);
	bzero(extra, 20);

	pthread_t tid;
	int ret2 = pthread_create(&tid, NULL, nlRecvThread, NULL);
	DBG_LEVEL_6("recvThread pthread_create returns %d", ret2);
	sleep(3);
	if(ret1 < 0)
	{
		DBG_LEVEL_6("bind NETLINK failed.");
		return false;
	}
	DBG_LEVEL_6("bind NETLINK succeed.");
	char *tmp, *chnFlag, *copyFlag;
	char error[8] = "./error", oldPath[36] = {0}, newPath[36] = {0};
	int ret3 = 0;
	if(cmd != 0 && cmd != 0xFFFF)	//拍照，抓取图片
	{
		cJSON *root = cJSON_CreateObject();
	    cJSON_AddItemToObject(root, "cmd", cJSON_CreateString("takePicture"));
	    cJSON_AddItemToObject(root, "userData", cJSON_CreateString(""));//预留字段
	    cJSON_AddItemToObject(root, "dir", cJSON_CreateNumber(1));
	    cJSON_AddItemToObject(root, "uid", cJSON_CreateNumber(13));

		char *jsonStr = cJSON_Print(root);
	    ret3 = sendNLMsg(NLU_WEIXIN, NLG_NONE, "MSG_THIRD_TAKEPIC", jsonStr);
	    DBG_LEVEL_6("sendNLMsg returns %d.", ret3);
		cJSON_Delete(root);
	    free(jsonStr);
		while(1)
		{
			sleep(1);
			if(recvFlag)
				recvFlag = 0;
				break;
		}
		deinitNL();

		tmp = strstr(extra, "/data/Picture/");
		if(tmp)
		{
			memcpy(oldPath, tmp + 14, 29); //eg:2021-10-13_12-09-44_front.jpg
			chnFlag = strstr(oldPath, "front");
			if(!chnFlag)
			{
				chnFlag = strstr(oldPath, "back");
			}
			if(chnFlag)
			{
				sprintf(newPath, "/data/Picture/%s_%s.jpg", filename/*time*/, chnFlag);
				rename(oldPath, newPath);
			}else
			{
				memcpy(filename, error, strlen(error));
				DBG_LEVEL_6("muti filename:|%s| after cant't find chnFlag in catch jpg...", filename);
				return false;
			}
			//根据dev选择要赋予的文件名
			if(dev == 1)
			{
				copyFlag = strstr(newPath, "front");
				if(copyFlag)
				{
					memcpy(filename, newPath, strlen(newPath));
				}
			}else if(dev == 2)
			{
				copyFlag = strstr(newPath, "back");
				if(copyFlag)
				{
					memcpy(filename, newPath, strlen(newPath));
				}
			}
			if(!copyFlag)
			{
				memcpy(filename, error, strlen(error));
				DBG_LEVEL_6("muti filename:|%s| after cant't find copyFlag in catch jpg...", filename);
				return false;
			}
		}
	}else if(cmd == 0xFFFF)
	{
		cJSON *root = cJSON_CreateObject();
		cJSON_AddStringToObject(root, "cmd", "takeVideo");
		cJSON_AddStringToObject(root, "userData", "num:21,codeL333");
		cJSON_AddNumberToObject(root, "uid", NLU_MISC);//注册的uid
		cJSON_AddNumberToObject(root, "time", 10);	//短视频秒数
		char *retData = cJSON_Print(root);
		ret3 = sendNLMsg(NLU_WEIXIN, NLG_NONE, "MSG_THIRD_TAKEVID",retData);
		DBG_LEVEL_6("sendNLMsg returns %d.", ret3);
		cJSON_Delete(root);
		free(retData);
		while(1)
		{
			sleep(1);
			if(recvFlag)
				recvFlag = 0;
				break;
		}
		deinitNL();

		char *tmp = strstr(extra, "/data/Catch/");
		if(tmp)
		{
			memcpy(oldPath, tmp + 12, 29); //eg:2021-10-13_12-09-44_front.jpg
			chnFlag = strstr(oldPath, "front");
			if(!chnFlag)
			{
				chnFlag = strstr(oldPath, "back");
			}
			if(chnFlag)
			{
				sprintf(newPath, "/data/Catch/%s_%s.mp4", filename/*time*/, chnFlag);
				rename(oldPath, newPath);
			}else
			{
				memcpy(filename, error, strlen(error));
				DBG_LEVEL_6("muti filename:|%s| after cant't find chnFlag in catch mp4...", filename);
				return false;
			}
			//根据dev选择要赋予的文件名
			if(dev == 1)
			{
				copyFlag = strstr(newPath, "front");
				if(copyFlag)
				{
					memcpy(filename, newPath, strlen(newPath));
				}
			}else if(dev == 2)
			{
				copyFlag = strstr(newPath, "back");
				if(copyFlag)
				{
					memcpy(filename, newPath, strlen(newPath));
				}
			}
			if(!copyFlag)
			{
				memcpy(filename, error, strlen(error));
				DBG_LEVEL_6("muti filename:|%s| after cant't find copyFlag in catch mp4...", filename);
				return false;
			}
		}
	}
	closelog();

	return true;
}
/*snap shot end*/

/*muti stream catch start*/
#define START_CATCH_STREAM	1
#define STOP_CATCH_STREAM	2
#define FRONT				1
#define REAR				2

int reqOrCtrlFlag = STOP_CATCH_STREAM;
int frontProcExitFlag = 1;
int rearProcExitFlag = 1;

vector <frame> frontH264StreamList;
vector <frame> rearH264StreamList;
vector <frame> AudioStreamList;

static void result_callback1(const FramePacket* framePacket)
{
	static int frameIndex = 0;
	if(frontProcExitFlag)
	{
		frameIndex = 0;
	}
	if(framePacket)
	{
		DBG_LEVEL_6("result_callback1 get h264 framePacket, framePacket->len = %d, framePacket->check = %d.", framePacket->len, framePacket->check);
		unsigned char *frameBuf = new unsigned char[framePacket->len + 1];
		struct frame singleFrame;
		memset(&singleFrame, 0, sizeof(singleFrame));
		memcpy(frameBuf, framePacket->data, framePacket->len);
		singleFrame.len = framePacket->len;
		DBG_LEVEL_6("singleFrame.len=%d\n", singleFrame.len);
		singleFrame.data = new unsigned char[singleFrame.len + 1];
/*		just for test
		char tmp[6] = {
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06
		};
		memcpy(singleFrame.data, tmp, 6);
*/
		memcpy(singleFrame.data, framePacket->data, singleFrame.len);
//		for(int i = 0; i < 5; i ++)
//		{
//			printf("%02x ", singleFrame.data[i]);
//		}printf("\n");
		frontH264StreamList.push_back(singleFrame);
		memset(&singleFrame, 0, sizeof(singleFrame));
		free(singleFrame.data);
		free(frameBuf);
		DBG_LEVEL_6("In result_callback1 frameIndex = %d", frameIndex ++);
		for(int index = 0; index < framePacket->len; index ++)
		{
			printf("%02x ", framePacket->data[index]);
		}
		printf("-----------------------------------------------------------\n");
	}else
	{
		DBG_LEVEL_6("result_callback1 can't get h264 framePacket...");
	}
}

static void result_callback2(const FramePacket* framePacket)
{
	static int frameIndex = 0;
	if(rearProcExitFlag)
	{
		frameIndex = 0;
	}
	if(framePacket)
	{
		DBG_LEVEL_6("result_callback2 get h264 framePacket, framePacket->len = %d, framePacket->check = %d.", framePacket->len, framePacket->check);
		unsigned char *frameBuf = new unsigned char[framePacket->len + 1];
		struct frame singleFrame;
		memset(&singleFrame, 0, sizeof(singleFrame));
		memcpy(frameBuf, framePacket->data, framePacket->len);
		singleFrame.len = framePacket->len;
		DBG_LEVEL_6("singleFrame.len=%d\n", singleFrame.len);
		singleFrame.data = new unsigned char[singleFrame.len + 1];
/*		just for test
		char tmp[6] = {
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06
		};
		memcpy(singleFrame.data, tmp, 6);
*/
		memcpy(singleFrame.data, framePacket->data, singleFrame.len);
//		for(int i = 0; i < 5; i ++)
//		{
//			printf("%02x ", singleFrame.data[i]);
//		}printf("\n");
		rearH264StreamList.push_back(singleFrame);
		memset(&singleFrame, 0, sizeof(singleFrame));
		free(singleFrame.data);
		free(frameBuf);
		DBG_LEVEL_6("In result_callback2 frameIndex = %d", frameIndex ++);
		for(int index = 0; index < framePacket->len; index ++)
		{
			printf("%02x ", framePacket->data[index]);
		}
		printf("-----------------------------------------------------------\n");
	}else
	{
		DBG_LEVEL_6("result_callback2 can't get h264 framePacket...");
	}
}

void audio_stream_callback(const AudioPacket* framePacket)
{
	static int frameIndex = 0;
	if(rearProcExitFlag)
	{
		frameIndex = 0;
	}
	if(framePacket)
	{
		DBG_LEVEL_6("audio_stream_callback get aac framePacket, framePacket->len = %d, framePacket->check = %d.", framePacket->len, framePacket->check);
		unsigned char *frameBuf = new unsigned char[framePacket->len + 1];
		struct frame singleFrame;
		memset(&singleFrame, 0, sizeof(singleFrame));
		memcpy(frameBuf, framePacket->data, framePacket->len);
		singleFrame.len = framePacket->len;
		DBG_LEVEL_6("singleFrame.len=%d\n", singleFrame.len);
		singleFrame.data = new unsigned char[singleFrame.len + 1];
/*		just for test
		char tmp[6] = {
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06
		};
		memcpy(singleFrame.data, tmp, 6);
*/
		memcpy(singleFrame.data, framePacket->data, singleFrame.len);
//		for(int i = 0; i < 5; i ++)
//		{
//			printf("%02x ", singleFrame.data[i]);
//		}printf("\n");
		rearH264StreamList.push_back(singleFrame);
		memset(&singleFrame, 0, sizeof(singleFrame));
		free(singleFrame.data);
		free(frameBuf);
		DBG_LEVEL_6("In audio_stream_callback frameIndex = %d", frameIndex ++);
		for(int index = 0; index < framePacket->len; index ++)
		{
			printf("%02x ", framePacket->data[index]);
		}
		printf("-----------------------------------------------------------\n");
	}else
	{
		DBG_LEVEL_6("result_callback2 can't get aac framePacket...");
	}

}

void * front_proc(void * arg)
{
    int time = 0;
	frontProcExitFlag = 0;
	videoUploadStart("127.0.0.1", 21010,1, result_callback1);
    while (time < 180)
	{
		if(reqOrCtrlFlag == STOP_CATCH_STREAM)
		{
			break;
		}
		time += 1;
		sleep(1);
		printf("Catching h.264 stream of front camera, time lasts %d seconds...\n", time);
    }
	videoUploadStop();
	frontProcExitFlag = 1;
	pthread_exit(0);

	return NULL;
}

void * rear_proc(void * arg)
{
    int time = 0;
	rearProcExitFlag = 0;
	videoUploadStartEx("127.0.0.1", 21010,1,1, result_callback2);
    while (time < 180)
	{
		if(reqOrCtrlFlag == STOP_CATCH_STREAM)
		{
			break;
		}
		time += 1;
		sleep(1);
		printf("Catching h.264 stream of rear camera, time lasts %d seconds...\n", time);
	}
	videoUploadStopEx(1);
	rearProcExitFlag = 1;
	pthread_exit(0);

	return NULL;
}

void * audio_proc(void * arg)
{
	int time = 0;

	audioUploadStart("127.0.0.1", 31010, audio_stream_callback);
	while(time < 180)
	{
		if(reqOrCtrlFlag == STOP_CATCH_STREAM)
		{
			break;
		}
		time += 1;
		sleep(1);
		printf("Catching AAC stream of rear camera, time lasts %d seconds...\n", time);
	}
	audioUploadStop();
	pthread_exit(0);

	return NULL;
}


int mutiStreamCatch(Jt1078ReqStrem& req_av, Jt1078CtrlStrem& ctrl_av, int reqOrCtrl)
{
	init(21020);
	if(reqOrCtrl == START_CATCH_STREAM)
	{
		reqOrCtrlFlag = reqOrCtrl;
		if(req_av.chn == FRONT)
		{
			pthread_t front_tid;
			if (0 != pthread_create(&front_tid, NULL, front_proc, NULL))
			{
				printf("Failed to create front_proc thread.errno:%u, reason:%s\n", errno, strerror(errno));
				return - 1;
			}else
			{
				printf("success to create front_proc thread\n");
			}
			pthread_join(front_tid, NULL);
		}else if(req_av.chn == REAR)
		{
			pthread_t rear_tid; 	
			if (0 != pthread_create(&rear_tid, NULL, rear_proc, NULL))
			{
				printf("Failed to create rear_proc thread.errno:%u, reason:%s\n", errno, strerror(errno));
				return - 1;
			}else
			{
				printf("success to create rear_proc thread\n"); 		
			}
			pthread_join(rear_tid, NULL);	
		}
		
		pthread_t audio_tid;
		if(0 != pthread_create(&audio_tid, NULL, audio_proc, NULL))
		{
			printf("Failed to create audio_proc thread.errno:%u, reason:%s\n", errno, strerror(errno));
			return - 1;
		}else
		{
			printf("success to create audio_proc thread\n");			
		}
		pthread_join(audio_tid, NULL);
	}else if(reqOrCtrl == STOP_CATCH_STREAM)
	{
		reqOrCtrlFlag = reqOrCtrl;	//设置标志位，停止抓流，退出线程
	}

	return 0;
}
/*muti stream catch end*/
