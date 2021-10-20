#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include "ParamInterface.h"
#include "GnssNetService.h"
#include "debug_log.h"

static bool s_app_run_flag = false;
static CParamInterface s_objParamInterface;
static CNetService s_objCNetService;
static unsigned int alarm_id = 0;
static char s_sz_devid[16] = {0};
static char *s_version = (char *)"jt808_inferface_v1.0.2";
//static int s_alarm = 0x00;
//static int s_status = 0x00;

enum {
    DSM_ALARM_NONE = 0x00,
    DSM_ALARM_TIRED = 0x01,
    DSM_ALARM_CALLING = 0x02,
    DSM_ALARM_SMOKING = 0x03,
    DSM_ALARM_DISTRACTION = 0x04, 
    DSM_ALARM_DRIVER_ABN = 0x05,
    DSM_ALARM_SNAPSHOT = 0x10,
    DSM_ALARM_DRIVER_CHANGED = 0x11,
};

void JT808_APP_Init()
{
	DBG_LEVEL_6(" Version:%s", s_version);
    if(s_app_run_flag){
        return;
    }

    s_objCNetService.InitService(&s_objParamInterface, &s_objCNetService);
    if(!s_objCNetService.StartService()){
        return;
    }

    s_app_run_flag = true;
}

void JT808_APP_DeInit()
{
    if(s_app_run_flag){
        s_objCNetService.StopService();
        s_app_run_flag = false;
    }
}

void JT808_APP_ReportPosInfo(struGPSData& gpsData)
{
    s_objCNetService.ReportPosInfo(&gpsData);
}

void JT808_APP_ReportSaveGpsInfo(unsigned char* buf,int len,int packNum)
{
    s_objCNetService.ReportSaveGpsInfo(buf,len,packNum);
}

void JT808_APP_ReportAlarm(Alarm_S *pstAlarm)
{
    AlarmData_s alarm_data;
    memset(&alarm_data, 0, sizeof(AlarmData_s));
    vector<File_Info_s> fileList;
    fileList.clear();

    if(pstAlarm->nDevType == DEV_DSM){
        alarm_data.extend_type = pstAlarm->nDevType;
        alarm_data.dsm_alarm.type = pstAlarm->dsm_alarm.nType;
        memcpy(&alarm_data.gnss, &pstAlarm->gpsData, sizeof(struGPSData));
        alarm_data.dsm_alarm.id = alarm_id ++;
        
        alarm_data.dsm_alarm.flag = 0x00;
        alarm_data.dsm_alarm.level = pstAlarm->dsm_alarm.nLevel;
        alarm_data.dsm_alarm.tired_level = pstAlarm->dsm_alarm.nTiredLevel;
        alarm_data.dsm_alarm.speed = ((pstAlarm->gpsData.usSpeed)/(double)100);
        /*海拔高度 m*/
        alarm_data.dsm_alarm.altitude = 0;
        alarm_data.dsm_alarm.latitude = pstAlarm->gpsData.fLatitude;
        alarm_data.dsm_alarm.longitude = pstAlarm->gpsData.fLongitude;
        /*YY-MM-DD-hh-mm-ss GMT+8*/
        memcpy(&alarm_data.dsm_alarm.datetime[0], &pstAlarm->datetime[0], 6);
        /*车辆状态*/
        alarm_data.dsm_alarm.vechile_status = 0x0401;
        /*报警标识号*/
        memcpy(&alarm_data.dsm_alarm.alarm_flag.id[0], s_sz_devid, 7);
        /*YY-MM-DD-hh-mm-ss GMT+8*/
        memcpy(&alarm_data.dsm_alarm.alarm_flag.datetime[0], &pstAlarm->datetime[0], 6);
        /*同一时间点报警的序号，从0循环累加*/
        alarm_data.dsm_alarm.alarm_flag.sequence = pstAlarm->sequence;
        /*附件数量，标识该报警对应的附件数量*/
        alarm_data.dsm_alarm.alarm_flag.attachment_num = pstAlarm->fileList.size();

        if(alarm_data.dsm_alarm.alarm_flag.attachment_num){
            fileList.assign(pstAlarm->fileList.begin(), pstAlarm->fileList.end());
        }

		alarm_data.dsm_alarm.result			= pstAlarm->dsm_alarm.result;
		alarm_data.dsm_alarm.similarity		= pstAlarm->dsm_alarm.similarity;
		alarm_data.dsm_alarm.similar_degree	= pstAlarm->dsm_alarm.similar_degree;
		alarm_data.dsm_alarm.cmp_type		= pstAlarm->dsm_alarm.cmp_type;
		alarm_data.dsm_alarm.dirver_id_len	= pstAlarm->dsm_alarm.dirver_id_len;

		memcpy(&alarm_data.dsm_alarm.dirver_id[0], pstAlarm->dsm_alarm.dirver_id, pstAlarm->dsm_alarm.dirver_id_len);
    } else if(pstAlarm->nDevType == DEV_ADAS){
        alarm_data.extend_type = pstAlarm->nDevType;
        alarm_data.adas_alarm.type = pstAlarm->adas_alarm.nType;
        memcpy(&alarm_data.gnss, &pstAlarm->gpsData, sizeof(struGPSData));
        alarm_data.adas_alarm.id = alarm_id ++;
        alarm_data.adas_alarm.flag = 0x00;
        alarm_data.adas_alarm.level = pstAlarm->adas_alarm.nLevel;
        alarm_data.adas_alarm.speedFront = pstAlarm->adas_alarm.speedFront;
        alarm_data.adas_alarm.distanceFront = pstAlarm->adas_alarm.distanceFront;
        alarm_data.adas_alarm.deviationType = pstAlarm->adas_alarm.deviationType;
        alarm_data.adas_alarm.roadFlag = pstAlarm->adas_alarm.roadFlag;
        alarm_data.adas_alarm.roadFlagData = pstAlarm->adas_alarm.roadFlagData;

        alarm_data.adas_alarm.speed = ((pstAlarm->gpsData.usSpeed)/(double)100);
        /*海拔高度 m*/
        alarm_data.adas_alarm.altitude = 0;
        alarm_data.adas_alarm.latitude = pstAlarm->gpsData.fLatitude;
        alarm_data.adas_alarm.longitude = pstAlarm->gpsData.fLongitude;
        /*YY-MM-DD-hh-mm-ss GMT+8*/
        memcpy(&alarm_data.adas_alarm.datetime[0], &pstAlarm->datetime[0], 6);
        /*车辆状态*/
        alarm_data.adas_alarm.vechile_status = 0x0401;
        /*报警标识号*/
        memcpy(&alarm_data.adas_alarm.alarm_flag.id[0], s_sz_devid, 7);
        /*YY-MM-DD-hh-mm-ss GMT+8*/
        memcpy(&alarm_data.adas_alarm.alarm_flag.datetime[0], &pstAlarm->datetime[0], 6);
        /*同一时间点报警的序号，从0循环累加*/
        alarm_data.adas_alarm.alarm_flag.sequence =  pstAlarm->sequence;
        /*附件数量，标识该报警对应的附件数量*/
        alarm_data.adas_alarm.alarm_flag.attachment_num = pstAlarm->fileList.size();

        if(alarm_data.adas_alarm.alarm_flag.attachment_num){
            fileList.assign(pstAlarm->fileList.begin(), pstAlarm->fileList.end());
        }
    } else {
        return;
    }

    s_objCNetService.ReportAlarm(&alarm_data, fileList);
}

void JT808_APP_Report_OverSpeed_Alarm(bool bFlag, struGPSData& gpsData, Alarm_Speed_S& speed)
{
    AlarmData_s alarm_data;
    memset(&alarm_data, 0, sizeof(AlarmData_s));
    vector<File_Info_s> fileList;
    fileList.clear();

    memcpy(&alarm_data.gnss, &gpsData, sizeof(struGPSData));
    memcpy(&alarm_data.speed, &speed, sizeof(Alarm_Speed_S));
    if(bFlag){
        alarm_data.alarm |= 0x0002;
    } else {
        alarm_data.alarm &= (~0x0002);
    }

    s_objCNetService.ReportAlarm(&alarm_data, fileList);
}

void JT808_APP_SetServerAddr(int nType, const char* pAddr, int port)
{
    s_objParamInterface.SetServerAddr(nType, pAddr, port);
}

void JT808_APP_SetTerminalAttr(const Terminal_Attr& stTermAttr)
{
    memcpy(&s_sz_devid[0], stTermAttr.devID, 16);
    s_objParamInterface.SetTerminalAttr(stTermAttr);
}

void JT808_APP_SetPhoneNumber(const unsigned char* bcdPhoneNum)
{
    s_objParamInterface.SetPhoneNumber(bcdPhoneNum);
}

void JT808_APP_SetSnapShot_Callback(SnapShot_Callback pSnapshot)
{
    s_objCNetService.SetSnapShot_Callback(pSnapshot);
}

void JT808_APP_FaceAuthSnap_Callback(FaceSnap_Callback pFaceSnap)
{
    s_objCNetService.SetFaceSnap_Callback(pFaceSnap);
}

void JT808_APP_SetGPS_Callback(GetGPS_Callback pGetGPS)
{
    s_objCNetService.SetGPSData_Callback(pGetGPS);
}

void JT808_APP_GetTerminalCfg(JT808_Config_S *cfg)
{
	s_objParamInterface.Get_terminal_param(cfg);
}

void JT808_APP_SetCfg_Callback(SetTerminalParam_CallBack pTermCfg)
{
    s_objCNetService.SetTermCfg_Callback(pTermCfg);
}

void JT808_APP_FacePic_Callback(FacePic_Callback pFacePic)
{
    s_objCNetService.SetFacePic_Callback(pFacePic);
}

void JT808_APP_FacePic_Download_res(FacePicDownloadReply *result)
{
    s_objCNetService.ReportFacePicDownload(result);
}

void JT808_APP_FacePicCmpPubRes_Callback(FacePicCmpRes_Callback pFaceCmpRes)
{
	s_objCNetService.SetFaceCmpRes_Callback(pFaceCmpRes);
}

void JT808_APP_Report_FaceCompare_res(FaceCmpResult *faceCmpData)
{
	s_objCNetService.ReportFaceCmpResult(faceCmpData);
}

void JT808_APP_FaceInfo_Callback(FaceInfo_Callback pFaceInfo)
{
	s_objCNetService.SetFaceInfo_Callback(pFaceInfo);
}

void JT808_APP_FaceCmp_Callback(TriggerFaceCmp_Callback pFaceCmp)
{
	s_objCNetService.SetFaceCmp_Callback(pFaceCmp);
}

void JT808_App_videoCfgGet_CallBack(VideoCfgGet_Callback pVideoCfg)
{
	s_objCNetService.GetVideoCfg_Callback(pVideoCfg);
}


void JT808_App_videoReportCarries(VideoUploadCarries *pCarries)
{
	s_objCNetService.ReportVideoCarries(pCarries);
}

void JT808_App_videoReqStream_CallBack(VideoReqStream_Callback pReqStream)
{
	s_objCNetService.ReqVideoStream_Callback(pReqStream);
}

void JT808_App_videoStreamCtrl_CallBack(VideoTransCtrl_Callback pTransCtrl)
{
	s_objCNetService.CtrlVideoTrans_Callback(pTransCtrl);
}

void JT808_App_videoQueSource_CallBack(VideoQueSource_Callback pSource)
{
	s_objCNetService.QueVideoSource_Callback(pSource);
}

void JT808_App_videoReqPlayback_CallBack(VideoReqPlayback_Callback pPlayback)
{
	s_objCNetService.ReqVideoPlayback_Callback(pPlayback);
}

void JT808_App_videoPlaybackCtrl_CallBack(VideoPlaybackCtrl_Callback pPBCtrl)
{
	s_objCNetService.CtrlVideoPlayback_Callback(pPBCtrl);
}

void JT808_App_videoUpFile_CallBack(VideoUploadFile_Callback pUpFile)
{
	s_objCNetService.ReqVideoUpFile_Callback(pUpFile);
}

void JT808_App_videoReportUpResult(VideoUpFileResult *pResult)
{
	s_objCNetService.ReportVideoUploadResult(pResult);
}

void JT808_App_videoCtrlUp_CallBack(VideoCtrlUpload_Callback pUpCtrl)
{
	s_objCNetService.CtrlVideoUpFile_Callback(pUpCtrl);
}

void JT808_App_TextInfo_CallBack(TextInfo_Callback pInfo)
{
	s_objCNetService.TextInformation_Callback(pInfo);
}

bool JT808_APP_GetStatus(void)
{
    return s_objCNetService.GetNetAliveStatus();
}

bool JT808_APP_GetUpdataGpsStatus(void)
{
    return s_objCNetService.GetSaveGpsDataUploadStatus();
}

