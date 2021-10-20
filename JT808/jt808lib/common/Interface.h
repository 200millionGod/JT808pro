#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <stdio.h>
#include <stdarg.h>
#include <list>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "Defines.h"


class InterFace
{
public:
	InterFace(){}
	virtual ~InterFace(){}
	
	
	/********************************************************
	*函数功能:  网络模块日志输出
	*输入参数: 
	* @param           pLog       日志内容?
	* 输出参数: 
	* @param  
	* 返回参数: 
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual void PutOutLog(const char * fmt, ...)
	{
		va_list vl;
		va_start(vl, fmt);
		vprintf(fmt, vl);
#if 0
		printf(fmt);
#else
		printf("%s", fmt);
#endif
		va_end(vl);
	}
	
		
	/***********************************************************
	*函数功能:  设置终端参数
	*输入参数: 
	* @param           sysCfg  终端配置
	* 输出参数: 
	* 返回参数: 
	* 使用说明:返回0:成功，1:失败
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int SetSysCfg(JT808_Config_S *sysCfg){ return 1;}


	/***********************************************************
	*函数功能: 获取终端参数
	*输入参数: 
	* @param           sysCfg  终端配置
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 返回0:成功，1:失败
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int GetSysCfg(JT808_Config_S *sysCfg ){ return 1;}
	
	/***********************************************************
	* 函数功能:  获取鉴权码
	* 输入参数: 
	* 输出参数: 
	* @param  szACode    鉴权码
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int GetAuthorizeCode(char* szACode)
	{
		bool bInit = false;
		if ( !bInit )
			return false;
		if (szACode != NULL)
			strcpy(szACode, "andy123456");
		return true;
	}
		
	/***********************************************************
	*函数功能:  获取终端手机号
	*输入参数: 
	* @param      bcdPhoneNum 
	* 
	* 输出参数: 终端手机号
	* @param  
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int GetPhoneNumber(unsigned char* szPhoneNum){return 1;}
	virtual void SetPhoneNumber(const unsigned char* szPhoneNum){};

	/***********************************************************
	*函数功能:  获取驾驶员信息
	*输入参数: 
	* @param 
	* 输出参数: 
	* 返回参数: 0:成功，-1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int GetDriverInfo(IC_Read_Info* pICInfo,Driver_Info* pDriverInfo){return -1;}

	/***********************************************************
	*函数功能: 设置车辆信息
	*输入参数: 
	* @param    pVehicle 
	* 输出参数: 
	* 返回参数: 无
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual void SetVehicleInfo(const Vehicle_Info* pVehicle){};
	
	/***********************************************************
	*函数功能: 获取车辆信息
	*输入参数: 
	* 输出参数: 
		@param    pVehicle 
	* 返回参数: 0:成功，-1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int GetVehicleInfo(Vehicle_Info* pVehicle){return 1;}

	/***********************************************************
	*函数功能 :  获取网络通讯参数
	*输入参数 : 
	* @param			 
	* 输出参数: 
	* 返回参数:  重传次数
	* 使用说明:  无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int GetNetworkParam(Net_Communication_Attr* pParam){return 1;}
		
	
	/***********************************************************
	*函数功能 :  获取服务器地址
	*输入参数 : 
	* @param		int nType	 :0--中心服务器,1--认证服务器
	* 输出参数: 
	* 返回参数:  
	* 使用说明:  无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int GetServerAddr(int &nType,char* pAddr,int& port){return 1;}
	virtual	void SetServerAddr(int nType, const char* pAddr, int port){};
	/***********************************************************
	*函数功能:	获取终端属性
	*输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual void GetTerminalAttr(Terminal_Attr* pTermAttr){	 };
	virtual void SetTerminalAttr(const Terminal_Attr& stTermAttr){};
	
	virtual void GetGnssData(void* pData){}
	
	/***********************************************************
	* 函数功能: 车辆控制
	* 输入参数: 
	* @param   mark      0:车门解锁 1:车门加锁    
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual void VehiDoorCtrlSet(unsigned char mark){}

	
	/***********************************************************
	*函数功能 :  软件升级
	*输入参数 : 
	* 输出参数:   
	* 返回参数:  
	* 使用说明:  0: 成功， 1:失败
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int Upgrade(const char* pVersion,void* pData, int len){return 1;}

	/***********************************************************
	*函数功能 :  临时位置跟踪
	*输入参数 : 
	* 输出参数:   
	* 返回参数:  
	* 使用说明:  0: 成功， 1:失败
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int LocationTracking(int interval, int time){return 1;}

	/***********************************************************
	* 函数功能: 平台清除报警标志位接口
	* 输入参数: 
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int ConfirmAlarm(unsigned int alarmMask){return 0;}
	
	/***********************************************************
	* 函数功能:短消息处理
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 0:成功, 1:失败
	* 使用说明:
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int SMSProc(const char* pSMS,SMS_MASK_E mask)	{return 1;}

	/***********************************************************
	* 函数功能:事件菜单设置
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 0:成功, 1:失败
	* 使用说明:
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int SetEventMenu(InfoOp opr,Event_Menu_Item* pItem,int num){return 1;}

	/***********************************************************
	* 函数功能:提问
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 0:成功, 1:失败
	* 使用说明:
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int SetAskAnswer(const char* pQustion,SMS_MASK_E mask,Event_Menu_Item* pItem,int num){return 1;}
	
	/***********************************************************
	* 函数功能:信息点播菜单设置
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 0:成功, 1:失败
	* 使用说明:
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int SetMsgMenu(InfoOp opr,Msg_Menu_Item* pItem,int num){return 1;}

	
	/***********************************************************
	* 函数功能:信息服务
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 0:成功, 1:失败
	* 使用说明:
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int MessageService(int nType,char* pMessage){return 1;}
	
	/***********************************************************
	* 函数功能: 电话回拨
	* 输入参数: 
	* @param   nMark           标志0: 普通通话, 1:监听
	* @param   szPhoneNo    电话号码
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int  TelCallback(int nMark, char* pPhoneNo){ return 1;}
	
	/***********************************************************
	* 函数功能:电话本设置
	* 输入参数: 
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int  TelBookSet(InfoOp  op, Address_Book_Item* pAddrBook,int num){ return 1;}

	/***********************************************************
	* 函数功能: 设置圆形区域
	* 输入参数: 
	* @param   opr   区域设置类型0--更新,1--追加,2--修改
	* @param   rgn   参数值
	* @param   num  区域数
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int SetRingRgn(int opr,Ring_Region_Attr* rgn,int num) {return 1;}

	
	/***********************************************************
	* 函数功能: 设置矩形区域
	* 输入参数: 
	* @param   opr   区域设置类型0--更新,1--追加,2--修改
	* @param   rgn   参数值
	* @param   num  区域数
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int SetRectRgn(int opr,Rectangle_Region_Attr* rgn,int num) {return 1;}

	/***********************************************************
	* 函数功能: 设置圆形区域
	* 输入参数: 
	* @param   rgn	 参数值
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int SetPolyRgn(Poly_Region_Attr* rgn) {return 1;}


	/***********************************************************
	* 函数功能: 设置线路区域
	* 输入参数: 
	* @param   rgn	 参数值
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int SetRouteRgn(Route_Region_Attr* rgn) {return 1;}

		
	/***********************************************************
	* 函数功能: 删除区域
	* 输入参数: 
	* @param   rgnType   区域类型0--圆形,1--矩形,2--多边形,3--线路
	* @param   pRgnID   区域ID
	* @param   num  区域数
	* 输出参数: 
	* 返回参数:0:成功, 1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int DelRgn(int rgnType,DWORD* pRgnID,int num){return 1;}

	/***********************************************************
	* 函数功能: 行驶记录采集
	* 输入参数: 
	* @param cmd  下传命令字
	* @param data           设置参数
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int SetDrivingRecord(int cmd, char* data){return 0;}

	/***********************************************************
	* 函数功能: 数据下行透传
	* 输入参数: 
	* @param type    	0x00--GNSS模块详细定位数据
					0x01--
	* @param buf      	透传消息
	* @param size     	透传消息大小
	* 输出参数: 
	* 返回参数: 0:成功，1:失败
	* 使用说明:	无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int DownlinkTrans(TRANSPARENT_TYPE_E type,char* buf,int size){return 0;}
	
	/***********************************************************
	* 函数功能: 多媒体数据上传应答
	* 输入参数: 
	* 输出参数:
	* @param  id     		     多媒体id
	* @param retryPackCnt      重传包数
	* @param retryIdList          重传id列表
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int MultimediaDataReply(int id, unsigned char  retryPackCnt, short* retryIdList){return 0;}


	/***********************************************************
	* 函数功能: 存储多媒体检索
	* 输入参数: 
	* 输出参数:
	* @param   mdIdxCmd;     	      多媒体检索信息
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int MdSearchCmd(NetMdIdxCmd mdIdxCmd, vector <IdxCmdRes> &stResultList){return 0;}

	/***********************************************************
	* 函数功能: 单条存储多媒体检索
	* 输入参数: 
	* 输出参数:
	* @param   mdIdxCmd;     	      多媒体检索信息
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int SingleMdSearchCmd(SingleNetMdIdxCmd SingleMdIdxCmd, NetMdUploadCmd &cmd){return 0;}
	
	/***********************************************************
	* 函数功能: 存储多媒体上传
	* 输入参数: 
	* param  recordcmd   储存多媒体上传命令信息
	* 输出参数:
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int MdUploadCmd(NetMdUploadCmd& mdUploadCmd){return 0;}


	/***********************************************************
	* 函数功能: 启动录音
	* 输入参数: 
	* 输出参数:
	* param  recordcmd   录音命令信息
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int MdRecordVoiceCmd(NetRecordCmd& recordCmd){return 0;}

	

	/***********************************************************
	* 函数功能: 摄像头立即拍摄或摄像
	* 输入参数: 
	* 输出参数:
	* param  mdShotCmd   拍摄信息
	* 返回参数: 0:成功，1:失败
	* 使用说明:无
	* 参考函数:
	* 创建作者:
	************************************************************/
	virtual int MdShotCmd(NetMdShotCmd &mdShotCmd, vector<ShotResult> &resultList){return 0;}

	virtual void NetSetGnssCmsLinkState(bool state){}
	virtual	void NetSetGnssCmssAuthorize(bool state){}
	virtual void SaveAuthCode(char*m_szAuthCode,int len){}

	/*****************************************************************************
	 函 数 名  : InterFace.ReqIFrame
	 功能描述  : 请求生成I帧
	 输入参数  : int chn        通道号 
	             int streamType  码流类型:0--主码流,1--子码流
	 输出参数  : 无
	 返 回 值  : virtual
	 调用函数  : 
	 被调函数  : 
	
	*****************************************************************************/
	virtual int ReqIFrame(int chn,int streamType){return -1;}

	/*****************************************************************************
	 函 数 名  : RegisterAVFrame
	 功能描述  : 注册获取音视频帧钩子
	 输入参数  : void* pUser          用户参数      
	             pFunPutFrameCallBack pFun  钩子函数
	             int chn                    通道,从0计数
	             int streamType             码流类型:0--主码流,1--子码流,2--音频流
	 输出参数  : 无
	 返 回 值  : 0--成功,1--失败
	 调用函数  : 
	 被调函数  : 
	*****************************************************************************/
	virtual int RegisterAVFrame(void* pUser,pFunPutFrameCallBack pFun,int chn,int streamType){return 1;}

	/*****************************************************************************
	 函 数 名  : UnRegisterAVFrame
	 功能描述  : 反注册获取音视频帧钩子
	 输入参数  : void* pUser		  用户参数		
			pFunPutFrameCallBack pFun  钩子函数
			int chn					 通道,从0计数
			int streamType			 码流类型:0--主码流,1--子码流,2--音频流
	 
	 输出参数  : 无
	 返 回 值  : 0--成功,1--失败
	 调用函数  : 
	 被调函数  : 
	*****************************************************************************/
	virtual int UnRegisterAVFrame(void* pUser,int chn,int streamType){return 1;}
};

#endif

