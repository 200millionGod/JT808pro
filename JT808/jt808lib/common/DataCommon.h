#ifndef _DATA_COMMON_H_
#define _DATA_COMMON_H_

#include "CommDefine.h"

//系统日期时间结构体定义
typedef struct{
	char		 year;		// 年份，其值等于实际年份减去2000
	char		 month;		//月份（从一月开始，1代表一月） - 取值区间为[1, 12] 
	char		 mday;		//一个月中的日期 - 取值区间为[1,31]
	char		 hour;		//时 - 取值区间为[0,23] 
	char		 minute;	//分 - 取值区间为[0,59]
	char		 second;	//秒 – 取值区间为[0,59]
	char		 week;		//星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推
	char		 reserve;
}SystemDateTime;

typedef struct DATA_SIM{
	BCD ICCID[10];
	BYTE SignalStrength;
	BYTE IMME;
	BYTE PhoneNumber[12];
}struSIMInfo;

typedef struct DATA_GPS 
{ 
	char				cGpsStatus; 			/*gps是否有效标识*/ 
	char				cSpeedUnit; 			/*速度单位*/ 
	unsigned short		usSpeed;				/*速度值*/ 
	#if 0
	char				cLatitudeDegree;		/*纬度值的度*/ 
	char				cLatitudeCent; 			/*纬度值的分*/ 
	char				cLongitudeDegree;		/*经度值的度*/ 
	char				cLongitudeCent;			/*经度值的分*/ 
	long				lLatitudeSec;			/*纬度值的秒*/ 
	long				lLongitudeSec;			/*经度值的秒*/ 
	#else
	double 				fLatitude;				/*纬度 以度为单位剩以10的6次方，精确到百万分之一度*/
	double				fLongitude;				/*经度 以度为单位剩以10的6次方，精确到百万分之一度*/
	#endif
	unsigned short		usGpsAngle;				/*gps夹角*/ 
	char				cDirectionLatitude;		/*纬度的方向*/ 
	char				cDirectionLongitude;	/*经度的方向*/ 
	unsigned char       cSatallite;            	/*0-GPS/BD,  1-GPS,  2-BD */
	char				reserved[3]; 
	WORD				Altitude;               /*above sea level*/
	BYTE				SignalIntensity;		/*Wireless Signal Intensity*/
	BYTE				GPSSatelliteNum;		/*Number of GPS positioning satellites*/
	BYTE				GpsLocation;			/*0:disable 1:enable*/
	BYTE				BeiDouPosition;			/*0:disable 1:enable*/
	unsigned int        status;                 /*acc状态，gps，南北纬东西经等。*/
}struGPSData;

typedef struct _stru_G_Sensor_
{
	short					x;
	short					y;
	short					z;
	short 					unit;			//> 0 加速度有效，实际的加速度计算方法是加速度读数除以此值。 
											//= 0 表示本加速度无效，即没有安装加速度传感器，或者接收的数据无效 
											//< 0 本结构无效，非法值

}struGSensor;

/* 终端属性，定义说明见部标协议 */
typedef struct 
{
	/*终端类型*/
	WORD	supportMask;
	/* 制造商ID */
	BYTE	manufacturerID[11];
	/* 终端型号 */
	BYTE	devType[30];
	/* 设备ID */
	BYTE	devID[30];
	/* SIM卡ID */
	BYTE	simICCID[10];
	/* 软件版本号 */
	BYTE	softVer[24];
	/* 硬件版本号 */
	char	hardVer[24];
	/* GNSS模块属性 */
	BYTE	gnss;
	/* 通讯模块属性 */
	BYTE	wireless;
	/*终端IMEI*/
	BYTE	devIMEI[15];
}Terminal_Attr;

/*驾驶员信息*/
typedef struct
{
	/* 驾驶员姓名 */
	char 		name[20];
	/* 驾驶员身份证编号 */
	char 		id[20];
	/* 从业资格证编码 */
	char 		qc[20];
	/* 发行机构名称 */
	char 		issuingAuthority[40];
	/* 证件有效期YYYYMMDD */
	char		date[4];
}Driver_Info;

#endif	/* _DATA_COMMON_H_ */
	
