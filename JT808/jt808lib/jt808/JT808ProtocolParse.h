#ifndef _JT808_PROTOCOL_Parse_H_
#define _JT808_PROTOCOL_Parse_H_

#include "JT808ProtocolPackage.h"
#include "Interface.h"

class CJT808Parse
{
public:
	void Init(InterFace * pInterFace,CJT808Package* pPackage);
	int ReqPacket(NetRcvMsg* pMsg);
	int SetSysCfg(NetRcvMsg* pMsg);
	int SetSysCfg(NetRcvMsg* pMsg, JT808_Config_S*& sysCfg);
	int GetParam(NetRcvMsg* pMsg);
	int GetSelectParam(NetRcvMsg* pMsg);
	WORD GetSelectParam(NetRcvMsg* pMsg, DWORD* paramID, DWORD& num);
	int TerminalCtrl(NetRcvMsg* pMsg);
	int GetDevAttr(NetRcvMsg* pMsg);
	int Upgrade(NetRcvMsg* pMsg);
	int Upgrade(NetRcvMsg* pMsg, char *szVerNum, DWORD& nLen);
	int GetLocation(NetRcvMsg* pMsg);
	int LocationTracking(NetRcvMsg* pMsg);
	int LocationTracking(NetRcvMsg* pMsg, WORD& interval, DWORD& time);
	int ConfirmAlarm(NetRcvMsg* pMsg);
	int ConfirmAlarm(NetRcvMsg* pMsg, DWORD& alarmMask);
	int SMSProc(NetRcvMsg* pMsg);
	int SMSProc(NetRcvMsg* pMsg, char *strSms, SMS_MASK_E& flag);
	int EventSet(NetRcvMsg* pMsg);
	int EventSet(NetRcvMsg* pMsg, InfoOp& opr, vector<Event_Menu_Item>& vecMenuItem, BYTE& num);
	int QuestionDown(NetRcvMsg* pMsg);
	int QuestionDown(NetRcvMsg* pMsg, char *szQustion, SMS_MASK_E& flag, vector<Event_Menu_Item>& vecAnswerItem, int& i);
	int InfoDmdMenuSet(NetRcvMsg* pMsg);
	int InfoDmdMenuSet(NetRcvMsg* pMsg, InfoOp& opr, vector<Msg_Menu_Item>& vecMenuItem, BYTE& num);
	int InfoService(NetRcvMsg* pMsg);
	int InfoService(NetRcvMsg* pMsg, BYTE& msgType, char* pMessage);
	int TelCallback(NetRcvMsg* pMsg);
	int TelCallback(NetRcvMsg* pMsg, BYTE& bMark, char *szPhoneNo);
	int TelBookSet(NetRcvMsg* pMsg);
	int TelBookSet(NetRcvMsg* pMsg, InfoOp& opr, vector<Address_Book_Item>& vecAddrBook, BYTE& num);
	int VehicleCtrl(NetRcvMsg* pMsg);
	int VehicleCtrl(NetRcvMsg* pMsg, BYTE& bDoorLock);
	int SetRingRgn(NetRcvMsg* pMsg);
	int SetRingRgn(NetRcvMsg* pMsg, BYTE& opr, vector<Ring_Region_Attr>& vecRegionAttr, BYTE& regNum);
	int SetRectRgn(NetRcvMsg* pMsg);
	int SetRectRgn(NetRcvMsg* pMsg, BYTE& opr, vector<Rectangle_Region_Attr>& vecRegionAttr, BYTE& regNum);
	int SetPolyRgn(NetRcvMsg* pMsg);
	int SetPolyRgn(NetRcvMsg* pMsg, Poly_Region_Attr* pRegion);
	int SetRoute(NetRcvMsg* pMsg);
	int SetRoute(NetRcvMsg* pMsg, Route_Region_Attr* pRegion);
	int DelRingRgn(NetRcvMsg* pMsg);
	int DelRectRgn(NetRcvMsg* pMsg);
	int DelPolyRgn(NetRcvMsg* pMsg);
	int DelRoute(NetRcvMsg* pMsg);
	int DelRgn(NetRcvMsg* pMsg, DWORD* rgnID, DWORD& rgnNum);
	int ReqDriverInfo(NetRcvMsg* pMsg);
	int DownLinkTrans(NetRcvMsg* pMsg);
	int DownLinkTrans(NetRcvMsg* pMsg, char* strBuffer, TRANSPARENT_TYPE_E& type, int& nSize);
	int FileServerInfo(NetRcvMsg* pMsg, File_Server_Config& _config);
	int UploadFileFinish(NetRcvMsg* pMsg, File_Finish_Respone_s& stResponse);
	int FacePictureInfo(NetRcvMsg* pMsg,        FacePicPublish* picinfo);
	int FaceCmpResInfo(NetRcvMsg* pMsg,        FaceCmpPubResult* pFaceCmp);

	/*视频服务器信息*/
	int VideoServerInfo(NetRcvMsg* pMsg,  VideoServerParamSet *pServerInfo);
	/*视频传输控制*/
	int VideoTransControl(NetRcvMsg* pMsg,	VideoTransCtrl *pTransCtrl);
	/*视频传输状态通知*/
	int VideoTransStatus(NetRcvMsg* pMsg,	VideoTransSta *pTransSta);
	/*查询资源列表*/
	int VideoQueResource(NetRcvMsg* pMsg,	VideoResource *pResourceInfo);
	/*录像回放请求*/
	int VideoQuePlayback(NetRcvMsg* pMsg,	VideoPlayback *pPlayback);
	/*录像回放控制*/
	int VideoPlaybackCtrl(NetRcvMsg* pMsg, VideoPlaybackControl *pPb_ctrl);
	/*文件上传*/
	int VideoUploadFile(NetRcvMsg* pMsg,	VideoUpFile *pInfo);
	/*文件上传控制*/
	int VideoCtrlUpload(NetRcvMsg* pMsg,	VideoUpFileCtrl *pInfo);

	/*文本信息下发*/
	int TextInfoTrans(NetRcvMsg* pMsg, TextInfo *pInfo);
private:
	void CommReply(WORD wSerialNumber, WORD wId, BYTE bResult);
	int DelRgn(NetRcvMsg* pMsg,int rgnType);
	
	InterFace* m_pInterFace;
	CJT808Package* m_pPackage;
	WORD m_nSerialNumber;
};

#endif /* _JT808_PROTOCOL_Parse_H_ */
