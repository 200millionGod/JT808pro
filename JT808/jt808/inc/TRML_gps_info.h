/*************************************************************
 * Copyright			: 
 * FileName				: TRML_gps_info.h
 * Description			: TRML_gps_info.h
 * Author		        : 
 * Created on			: 20210918
 *************************************************************/
#ifndef INC_TRML_GPS_INFO_H_
#define INC_TRML_GPS_INFO_H_

#define SAVE_GPS_INFO_PATH     "/data/ecar/GpsInfo.xml"

/* save gps folder path */
#define ECAR_DATA_PATH         "/data/ecar"

#define GPS_ACQUISE_DURATION 	1000 		//1秒一个GPS

#define GPS_REPORT_DURATION 	5000 		//5秒钟一次上报

#define GPS_REPORT_MAX 			30 			//一次上报最大30个GPS(因为TCP消息长度已经接近1024，超过1024会导致分包)

#define GPS_BUFFER_MAX 			15 			//全局变量最大缓存15个GPS，每10秒钟写入一次离线GPS信息文件；

#define GPS_CACHE_MAX 			(3600) 		//GPS最大缓存3600个点

#define GPS_SEND_TIME_OUT		30			/*SendThread线程里面会重发3次 超时时间是每次10S   那么 这里暂定为30S后再发一次 

											那么，如果保存了GPS数据，却收不到平台应答，那么会30S发一次
											*/

typedef struct{
	int start;
	int end;
}SaveGpsInfo;								/* 保存信息的头和尾索引            */

typedef struct{
	int index;
	int length;
	char buf[100];
}SaveGpsData;								/* 保存GPS的具体内容 */

int SaveGpsInfo_Init(void);

int saveGpsInfo(void);
void readGpsInfo(void);
void ReadGps(void);



#endif

