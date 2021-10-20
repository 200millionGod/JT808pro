#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include "JT808App.h"

static int s_running = 1;
static void __signal_handler(int s)
{
    s_running = 0;
}
static int countOn = 0;

static void get_BCD_time(BCD *bcd_time)
{
    time_t curtime = time(NULL);
    tm *ptm = localtime(&curtime);
    bcd_time[0] = (((ptm->tm_year-100)/10) << 4)+(ptm->tm_year%10);
    bcd_time[1] = (((ptm->tm_mon+1)/10) << 4)+((ptm->tm_mon+1)%10);
    bcd_time[2] = ((ptm->tm_mday/10) << 4)+(ptm->tm_mday%10);
    bcd_time[3] = ((ptm->tm_hour/10) << 4)+(ptm->tm_hour%10);
    bcd_time[4] = ((ptm->tm_min/10) << 4)+(ptm->tm_min%10);
    bcd_time[5] = ((ptm->tm_sec/10) << 4)+(ptm->tm_sec%10);
}

static char szPhone[12] = {0};
static Terminal_Attr stTermAttr;
static void init_param(void)
{

    {
        //memcpy(szPhone, "015012855253"/*"013600000056"*/, 12);
        //memcpy(szPhone, "013224455588"/*"013600000056"*/, 12);
        memcpy(szPhone, "013971078451", 12);
    }

    {
        memcpy(stTermAttr.manufacturerID, "JWSRE", 5);
        memcpy(stTermAttr.devID, "8451000", 7);
        //memcpy(stTermAttr.devID, /*"5000056"*/"2855253", 7);
        //memcpy(stTermAttr.devID, /*"5000056"*/"4455588", 7);
        memcpy(stTermAttr.devType, "12345678901234567890", 20);
        stTermAttr.supportMask = 0x08;
        stTermAttr.simICCID[0] = 0x01;
        stTermAttr.simICCID[1] = 0x02;
        stTermAttr.simICCID[2] = 0x03;
        stTermAttr.simICCID[3] = 0x04;
        stTermAttr.simICCID[4] = 0x05;
        stTermAttr.simICCID[5] = 0x06;
        stTermAttr.simICCID[6] = 0x07;
        stTermAttr.simICCID[7] = 0x08;
        stTermAttr.simICCID[8] = 0x09;
        stTermAttr.simICCID[9] = 0x11;
        memcpy(stTermAttr.softVer, "V1.2.1", 6);
        memcpy(stTermAttr.hardVer, "V1.1.1", 6);
        stTermAttr.gnss = 0x01;
        stTermAttr.wireless = 0x80;
    }
}

static void __create_gps(struGPSData& gpsData)
{
    gpsData.cGpsStatus = 'A';
    gpsData.usSpeed = 8000;                /*速度值*/ 
    gpsData.fLatitude = 22.571300 * 1000000;
    gpsData.fLongitude = 113.862312 * 1000000;
    gpsData.usGpsAngle = 240;             /*gps夹角*/ 
}

static void __create_dsm_alarm(Alarm_S& stAlarm)
{
    __create_gps(stAlarm.gpsData);
    stAlarm.nDevType = DEV_DSM;
    stAlarm.dsm_alarm.nType = 0x05;
    stAlarm.dsm_alarm.nLevel =  0x02;
    stAlarm.dsm_alarm.nTiredLevel = 0x05;
    char datetime[6] = {0};
    get_BCD_time((BCD *)&datetime[0]);
    memcpy(&stAlarm.datetime[0], &datetime[0], 6);
    File_Info_s stInfo;
#if 0
    memset(&stInfo, 0, sizeof(File_Info_s));
    stInfo.nIndex = 0x01;
    stInfo.nType = 0x00;
    memcpy(stInfo.szFileName, "/mnt/testjpg/dfw_01_20190108_173111.jpg", strlen("/mnt/testjpg/dfw_01_20190108_173111.jpg"));
    stAlarm.fileList.push_back(stInfo);
   
    memset(&stInfo, 0, sizeof(File_Info_s));
    stInfo.nIndex = 0x02;
    stInfo.nType = 0x00;
    memcpy(stInfo.szFileName, "/mnt/testjpg/dfw_01_20190108_173141.jpg", strlen("/mnt/testjpg/dfw_01_20190108_173141.jpg"));
    stAlarm.fileList.push_back(stInfo); 

    memset(&stInfo, 0, sizeof(File_Info_s));
    stInfo.nIndex = 0x03;
    stInfo.nType = 0x00;
    memcpy(stInfo.szFileName, "/mnt/testjpg/dfw_01_20190108_173211.jpg", strlen("/mnt/testjpg/dfw_01_20190108_173211.jpg"));
    stAlarm.fileList.push_back(stInfo);
#else
    memset(&stInfo, 0, sizeof(File_Info_s));
    stInfo.nIndex = 0x00;
    stInfo.nType = 0x02;
    memcpy(stInfo.szFileName, "/mnt/dfw_01_20190118_113836.mp4", strlen("/mnt/dfw_01_20190118_113836.mp4"));
    stAlarm.fileList.push_back(stInfo);
#endif
}

static void __create_adas_alarm(Alarm_S& stAlarm)
{
    __create_gps(stAlarm.gpsData);
    stAlarm.nDevType = DEV_ADAS;
    stAlarm.adas_alarm.nType = 0x01;
    stAlarm.adas_alarm.nLevel =  0x02;
    stAlarm.adas_alarm.speedFront = 80;
    stAlarm.adas_alarm.distanceFront = 2.5;
    stAlarm.adas_alarm.deviationType = 0x00;
    stAlarm.adas_alarm.roadFlag = 0x00;
    stAlarm.adas_alarm.roadFlagData = 0x00;

    char datetime[6] = {0};
    get_BCD_time((BCD *)&datetime[0]);
    memcpy(&stAlarm.datetime[0], &datetime[0], 6);
    printf("%02X-%02X-%02X %02X:%02X:%02X \n", datetime[0],datetime[1],datetime[2],datetime[3],datetime[4],datetime[5]);
    stAlarm.fileList.clear();
    File_Info_s stInfo;
#if 1
    memset(&stInfo, 0, sizeof(File_Info_s));
    stInfo.nIndex = 0x01;
    stInfo.nType = 0x00;
    memcpy(stInfo.szFileName, "/mnt/testjpg/dfw_01_20190108_173111.jpg", strlen("/mnt/testjpg/dfw_01_20190108_173111.jpg"));
    stAlarm.fileList.push_back(stInfo);
   
    memset(&stInfo, 0, sizeof(File_Info_s));
    stInfo.nIndex = 0x02;
    stInfo.nType = 0x00;
    memcpy(stInfo.szFileName, "/mnt/testjpg/dfw_01_20190108_173141.jpg", strlen("/mnt/testjpg/dfw_01_20190108_173141.jpg"));
    stAlarm.fileList.push_back(stInfo); 

    memset(&stInfo, 0, sizeof(File_Info_s));
    stInfo.nIndex = 0x03;
    stInfo.nType = 0x00;
    memcpy(stInfo.szFileName, "/mnt/testjpg/dfw_01_20190108_173211.jpg", strlen("/mnt/testjpg/dfw_01_20190108_173211.jpg"));
    stAlarm.fileList.push_back(stInfo);
#else
    memset(&stInfo, 0, sizeof(File_Info_s));
    stInfo.nIndex = 0x00;
    stInfo.nType = 0x02;
    memcpy(stInfo.szFileName, "/mnt/dfw_01_20190118_113836.mp4", strlen("/mnt/dfw_01_20190118_113836.mp4"));
    stAlarm.fileList.push_back(stInfo);
#endif
}

int main(void)
{
    signal(SIGINT,  __signal_handler);
    signal(SIGQUIT, __signal_handler);
    signal(SIGTERM, __signal_handler);
    init_param();
    JT808_APP_SetPhoneNumber((const unsigned char*)szPhone);
    #if 0
    //JT808_APP_SetServerAddr(SOCK_STREAM, "47.99.196.215", 7000);
    JT808_APP_SetServerAddr(SOCK_STREAM, "218.61.30.236", 7002);
    #else
    JT808_APP_SetServerAddr(SOCK_STREAM, "59.172.105.82", 6666);
    #endif
    JT808_APP_Init();
    while(s_running){
        {
            struGPSData gpsData;
            __create_gps(gpsData);
            JT808_APP_ReportPosInfo(gpsData);
        }
        sleep(5);
        if((++ countOn) == 2){
            Alarm_S stAlarm;
            __create_adas_alarm(stAlarm);
            JT808_APP_ReportAlarm(&stAlarm);
            #if 0
            struGPSData gpsData;
            __create_gps(gpsData);
            static bool over_speed = false;
            Alarm_Speed_S speed;
            speed.position = 0;
            if(!over_speed){
                over_speed = true;
            } else {
                over_speed = false;
            }
            JT808_APP_Report_OverSpeed_Alarm(over_speed, gpsData, speed);
            countOn = 0;
            #endif
            countOn = 0;
            sleep(10);
        }
    }

    JT808_APP_DeInit();

    return 0;
}
