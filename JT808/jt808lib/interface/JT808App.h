#ifndef __JT808_APP_H__
#define __JT808_APP_H__

#include "Defines.h"
//#ifdef __cplusplus
//extern "C" {
//#endif
void JT808_APP_Init();
void JT808_APP_DeInit();

void JT808_APP_SetCfg_Callback(SetTerminalParam_CallBack pTermCfg);
void JT808_APP_ReportPosInfo(struGPSData& gpsData);
void JT808_APP_ReportAlarm(Alarm_S *pstAlarm);
void JT808_APP_Report_OverSpeed_Alarm(bool bFlag, struGPSData& gpsData, Alarm_Speed_S& speed);
void JT808_APP_SetServerAddr(int nType, const char* pAddr, int port);
void JT808_APP_SetTerminalAttr(const Terminal_Attr& stTermAttr);
void JT808_APP_SetPhoneNumber(const unsigned char* bcdPhoneNum);
void JT808_APP_SetSnapShot_Callback(SnapShot_Callback pSnapshot);
void JT808_APP_FaceAuthSnap_Callback(FaceSnap_Callback pFaceSnap);
void JT808_APP_SetGPS_Callback(GetGPS_Callback pGetGPS);
void JT808_APP_GetTerminalCfg(JT808_Config_S *cfg);
void JT808_APP_FacePic_Callback(FacePic_Callback pFacePic);
void JT808_APP_FacePic_Download_res(FacePicDownloadReply *result);
void JT808_APP_FacePicCmpPubRes_Callback(FacePicCmpRes_Callback pFacePicCmpRes);
void JT808_APP_Report_FaceCompare_res(FaceCmpResult *faceCmpData);
void JT808_APP_FaceInfo_Callback(FaceInfo_Callback pFaceInfo);
void JT808_APP_FaceCmp_Callback(TriggerFaceCmp_Callback pFaceCmp);
void JT808_App_videoCfgGet_CallBack(VideoCfgGet_Callback pVideoCfg);
void JT808_App_videoReportCarries(VideoUploadCarries *pCarries);
void JT808_App_videoReqStream_CallBack(VideoReqStream_Callback pReqStream);
void JT808_App_videoStreamCtrl_CallBack(VideoTransCtrl_Callback pTransCtrl);
void JT808_App_videoQueSource_CallBack(VideoQueSource_Callback pSource);
void JT808_App_videoReqPlayback_CallBack(VideoReqPlayback_Callback pPlayback);
void JT808_App_videoPlaybackCtrl_CallBack(VideoPlaybackCtrl_Callback pPBCtrl);
void JT808_App_videoUpFile_CallBack(VideoUploadFile_Callback pUpFile);
void JT808_App_videoReportUpResult(VideoUpFileResult *pResult);
void JT808_App_videoCtrlUp_CallBack(VideoCtrlUpload_Callback pUpCtrl);
void JT808_App_TextInfo_CallBack(TextInfo_Callback pInfo);

bool JT808_APP_GetStatus(void);
bool JT808_APP_GetUpdataGpsStatus(void);
void JT808_APP_ReportSaveGpsInfo(unsigned char* buf,int len,int packNum);


//#ifdef __cplusplus
//}
//#endif

#endif //__JT808_APP_H__
