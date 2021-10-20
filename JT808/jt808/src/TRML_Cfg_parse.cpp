/*************************************************************
 * Copyright			: 
 * FileName				: XMLParse.cpp
 * Description			: XMLParse.cpp implement
 * Author		        : chas
 * Created on			: Jan 4, 2019
 *************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <string>

//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/xml_parser.hpp>
//#include <boost/foreach.hpp>
//#include <boost/lexical_cast.hpp>

#include "TRML_Cfg_parse.h"
#include "../inc/debug_log.h"

/*
double pt_dump(const boost::property_tree::ptree& pt,string& key, double defaut_value)
{
    double value = pt.get(key,defaut_value);

    //if (CDFWParam::Instance()->verbose)
    {
    	//cout<<key<<"-->"<<value<<endl;
    }
	
    return value;
}


int pt_dump(const boost::property_tree::ptree& pt,string& key, int defaut_value)
{
    int value = pt.get(key,defaut_value);

    //if (CDFWParam::Instance()->verbose)
    {
    	//cout<<key<<"-->"<<value<<endl;
    }
	
    return value;
}

string pt_str_dump(const boost::property_tree::ptree& pt,string& key, string defaut_value)
{
    string value = pt.get(key,defaut_value);

   // if (CDFWParam::Instance()->verbose)
    {
    	//cout<<key<<"-->"<<value<<endl;
    }
	
    return value;
}


int get_rdt_value(const boost::property_tree::ptree& pt,string key, const char* name,int defaut_value)
{
	key += ".";
	key += name;

	return pt_dump(pt,key,defaut_value);
}
double get_rdt_double_value(const boost::property_tree::ptree& pt,string key, const char* name,double defaut_value)
{
	key += ".";
	key += name;

	return pt_dump(pt,key,defaut_value);
}

string get_str_rdt_value(const boost::property_tree::ptree& pt,string key, const char* name,string defaut_value)
{
	key += ".";
	key += name;

	return pt_str_dump(pt,key,defaut_value);
}

void set_rdt_value(boost::property_tree::ptree& pt,string key, const char* name,int value)
{
	key += ".";
	key += name;

	pt.put(key,value);
}
void set_rdt_float_value(boost::property_tree::ptree& pt,string key, const char* name,float value)
{
	key += ".";
	key += name;

	pt.put(key,value);
}

void set_rdt_value(boost::property_tree::ptree& pt,string key, const char* name,string value)
{
	 key += ".";
	 key += name;

	 pt.put(key,value);
}
*/

TerminalXMLParse::TerminalXMLParse()
{
}

TerminalXMLParse::~TerminalXMLParse()
{
}


int TerminalXMLParse::LoadDefaultParam(const char* config_path, JT808_Config_S* cfg)
{
	//NetCommunication
	cfg->commCfg.heartbeat			= 30;
	cfg->commCfg.tcp.timeout		= 10;
	cfg->commCfg.tcp.trytimes		= 3;
	cfg->commCfg.udp.timeout		= 30;
	cfg->commCfg.udp.trytimes		= 3;
	cfg->commCfg.sms.timeout		= 30;
	cfg->commCfg.sms.trytimes		= 0;

	memset(cfg->commCfg.authenticationkey, 0, 64);
	memset(cfg->commCfg.phoneNumber, 0, 6);
	memset(cfg->centerSrvCfg.main.apn, 0, 64);
	memset(cfg->centerSrvCfg.main.username, 0, 32);	
	memset(cfg->centerSrvCfg.main.psw, 0, 32);
	memset(cfg->centerSrvCfg.main.addr, 0, 64);
	memset(cfg->centerSrvCfg.backup.apn, 0, 64);
	memset(cfg->centerSrvCfg.backup.username, 0, 32);
	memset(cfg->centerSrvCfg.backup.psw, 0, 32);
	memset(cfg->centerSrvCfg.backup.addr, 0, 64);
	
	
	cfg->centerSrvCfg.tcpport		  = 30;
	cfg->centerSrvCfg.udpport		  = 15;
	
	//ICServer
	memset(cfg->icSrvCfg.main, 0, 64);
	memset(cfg->icSrvCfg.backup, 0, 64);
	
	cfg->icSrvCfg.tcpport = 0;
	cfg->icSrvCfg.udpport = 0;

	//GnssReport
	cfg->reportCfg.tactics			   = 0;
	cfg->reportCfg.scheme			   = 0;
	cfg->reportCfg.interval[0].time    = 0;
	cfg->reportCfg.interval[0].mileage = 0;
	cfg->reportCfg.interval[1].time    = 0;
	cfg->reportCfg.interval[1].mileage = 0;
	cfg->reportCfg.interval[2].time    = 0;
	cfg->reportCfg.interval[2].mileage = 0;
	cfg->reportCfg.interval[3].time    = 0;
	cfg->reportCfg.interval[3].mileage = 0;
	cfg->reportCfg.angle			   = 0;
	cfg->reportCfg.radius			   = 0;

	memset(cfg->phoneNumCfg.monitornum, 0, 16);
	memset(cfg->phoneNumCfg.supersmsnum, 0, 16);
	memset(cfg->phoneNumCfg.centernum, 0, 16);
	memset(cfg->phoneNumCfg.resetnum, 0, 16);
	memset(cfg->phoneNumCfg.rebootnum, 0, 16);
	memset(cfg->phoneNumCfg.centersmsnum, 0, 16);
	memset(cfg->phoneNumCfg.alarmsmsnum, 0, 16);
	
	cfg->phoneNumCfg.answerway		   = 0;
	cfg->phoneNumCfg.calltime		   = 0;
	cfg->phoneNumCfg.monthtime		   = 0;
	//alarmMaskCfg
	cfg->alarmMaskCfg.alarmmask 	   = 0;
	cfg->alarmMaskCfg.sms			   = 0;
	cfg->alarmMaskCfg.snap			   = 0;
	cfg->alarmMaskCfg.snapsave		   = 0;
	cfg->alarmMaskCfg.keymask		   = 0;
	
	//AlarmSpeed
	cfg->alarmSpeed.speed			  = 0;
	cfg->alarmSpeed.time			  = 0;
	cfg->alarmSpeed.warning 		  = 0;

	//FatigueAlarm
	cfg->alarmFatigue.drivingtime	  = 0;
	cfg->alarmFatigue.totaltime 	  = 0;
	cfg->alarmFatigue.resttime		  = 0;
	cfg->alarmFatigue.warning		  = 0;
	

	//ParkingAlarm
	cfg->alarmParking.time			  = 0;

	//CollisionAlarm
	cfg->alarmCollision.time		  = 0;
	cfg->alarmCollision.acceleration  = 0;

	
	//RolloverAlarm
	cfg->alarmRollover.angle		  = 0;

	//snapCtrl
	cfg->snapCtrl.time.mask			  = 0;
	cfg->snapCtrl.time.savemask		  = 0;
	cfg->snapCtrl.mileage.mask	      = 0;
	cfg->snapCtrl.mileage.savemask	  = 0;
	cfg->snapCtrl.timeunit			  = 0;
	cfg->snapCtrl.mileageunit		  = 0;
	cfg->snapCtrl.timeInterval		  = 0;	
	cfg->snapCtrl.mileageInterval	  = 0;
	
	//PicQuality
	cfg->picQuality.quality 		  = 0;

	//PicColor
	cfg->picColor.luminance 		  = 0;
	cfg->picColor.contrast			  = 0;
	cfg->picColor.saturation		  = 0;
	cfg->picColor.chroma			  = 0;

	//VehicleInfo
	cfg->VehInfoCfg.mileage 		  = 0;
	cfg->VehInfoCfg.provinceid		  = 0;
	cfg->VehInfoCfg.cityid			  = 0;	

	memset(cfg->VehInfoCfg.license, 0, 16);
	memset(cfg->VehInfoCfg.vin, 0, 20);
	memset(cfg->VehInfoCfg.vehicletype, 0, 16);
	cfg->VehInfoCfg.nVehicletype	  = 0;
	cfg->VehInfoCfg.platecolor		  = 0;
	cfg->VehInfoCfg.coefficient 	  = 0;

	//VehicleInfo
	cfg->gnssDetail.enable			  = 0;
	cfg->gnssDetail.baudtate		  = 0;
	cfg->gnssDetail.outputFreq		  = 0;	
	cfg->gnssDetail.uploadmode		  = 0;
	cfg->gnssDetail.sampleFreq		  = 0;
	cfg->gnssDetail.uploadset		  = 0;	

	//canbusUpload
	cfg->canbusUpload.sample[0] 	  = 0;
	cfg->canbusUpload.sample[1] 	  = 0;
	cfg->canbusUpload.upload[0] 	  = 0;	
	cfg->canbusUpload.upload[1] 	  = 0;

	memset(cfg->canbusUpload.aloneSample.set, 0, 8);

	//video parameter
	cfg->VideoCfg.ai_vi_attr.coding_mode           = 0;
	cfg->VideoCfg.ai_vi_attr.resolution            = 3;
	cfg->VideoCfg.ai_vi_attr.key_frame_inter       = 25;
	cfg->VideoCfg.ai_vi_attr.frame_rate            = 25;
	cfg->VideoCfg.ai_vi_attr.bit_rate              = 0;
	cfg->VideoCfg.ai_vi_attr.store_coding_mode     = 0;
	cfg->VideoCfg.ai_vi_attr.store_resolution      = 3;
	cfg->VideoCfg.ai_vi_attr.store_key_frame_inter = 25;
	cfg->VideoCfg.ai_vi_attr.store_frame_rate      = 25;
	cfg->VideoCfg.ai_vi_attr.store_bit_rate        = 0;
	cfg->VideoCfg.ai_vi_attr.osd_set               = 0;
	cfg->VideoCfg.ai_vi_attr.enable_audio          = 0;

	//audio video chn parameter
	cfg->VideoCfg.ai_vi_chn.chn_total_num          = 0;
	cfg->VideoCfg.ai_vi_chn.audio_num              = 0;
	cfg->VideoCfg.ai_vi_chn.video_num              = 0;

	//single chn parameter
	cfg->VideoCfg.single_chn_ctl.chn_num		   = 0;

	//special alarm
	cfg->VideoCfg.special_alarm.video_storage_thres = 20;
	cfg->VideoCfg.special_alarm.video_duration      = 5;
	cfg->VideoCfg.special_alarm.start_time          = 1;

	//video alarm parameter
	cfg->VideoCfg.video_alarm.num_of_carries        = 30;
	cfg->VideoCfg.video_alarm.fatigue_thres         = 30;

	//daily awake
	cfg->VideoCfg.awake_attr.awake_mode                         = 30;
	cfg->VideoCfg.awake_attr.awake_type                         = 30;
	cfg->VideoCfg.awake_attr.set_awake_time                     = 30;
	cfg->VideoCfg.awake_attr.awake_params.flag                  = 30;
	cfg->VideoCfg.awake_attr.awake_params.period1_start_time[0] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period1_start_time[1] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period1_close_time[0] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period1_close_time[1] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period2_start_time[0] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period2_start_time[1] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period2_close_time[0] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period2_close_time[1] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period3_start_time[0] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period3_start_time[1] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period3_close_time[0] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period3_close_time[1] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period4_start_time[0] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period4_start_time[1] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period4_close_time[0] = 0;
	cfg->VideoCfg.awake_attr.awake_params.period4_close_time[1] = 0;
	
	saveTerminalParam(config_path, cfg);
	
	return 0;
}

int TerminalXMLParse::LoadDfssParam(const char* config_path,DSM_Config* cfg)
{/*
	string key = "DFSSCfg";
	boost::property_tree::ptree pt;
	boost::property_tree::read_xml(config_path,pt,boost::property_tree::xml_parser::no_comments|boost::property_tree::xml_parser::trim_whitespace);

	cfg->video_enable.level1		= get_rdt_value(pt,key,"WarningParams.Level1.VideoEnable",0);
	cfg->video_enable.level2		= get_rdt_value(pt,key,"WarningParams.Level2.VideoEnable",0);
	cfg->video_enable.facemissing	= get_rdt_value(pt,key,"WarningParams.FaceMissing.VideoEnable",0);
	cfg->video_enable.eyesmissing	= get_rdt_value(pt,key,"WarningParams.EyesMissing.VideoEnable",0);
	cfg->video_enable.lookaround	= get_rdt_value(pt,key,"WarningParams.LookAround.VideoEnable",0);
	cfg->video_enable.lookup		= get_rdt_value(pt,key,"WarningParams.LookDown.VideoEnable",0);
	cfg->video_enable.lookdown		= get_rdt_value(pt,key,"WarningParams.LookUp.VideoEnable",0);
	cfg->video_enable.smoking		= get_rdt_value(pt,key,"WarningParams.Smoking.VideoEnable",0);
	cfg->video_enable.phone			= get_rdt_value(pt,key,"WarningParams.Phone.VideoEnable",0);
	cfg->video_enable.yawn			= get_rdt_value(pt,key,"WarningParams.Yawn.VideoEnable",0);
	cfg->video_enable.talking		= get_rdt_value(pt,key,"WarningParams.Talking.VideoEnable",0);
	cfg->video_enable.speeding		= get_rdt_value(pt,key,"WarningParams.Speeding.VideoEnable",0);
	cfg->video_enable.sunglass		= get_rdt_value(pt,key,"WarningParams.Sunglass.VideoEnable",0);
	cfg->video_enable.belt			= get_rdt_value(pt,key,"WarningParams.Belt.VideoEnable",0);
*/	
	return 0;
}

int TerminalXMLParse::LoadIfvParam(const char* config_path, ADAS_Config_S* cfg)
{/*
	string key = "Roadefend";
	boost::property_tree::ptree pt;
	boost::property_tree::read_xml(config_path,pt,boost::property_tree::xml_parser::no_comments|boost::property_tree::xml_parser::trim_whitespace);

	unsigned int fcw = 0, ldw = 0;
	fcw		= get_rdt_value(pt,key,"ifv.video.mask.fcw",0);
	ldw		= get_rdt_value(pt,key,"ifv.video.mask.ldw",0);

	memset(&cfg->video_enable, 0, sizeof(IfvVideoEnable));
	
	cfg->video_enable.hwm_level2 = (fcw & (1 << 0)) ? 1:0;
	cfg->video_enable.TTC        = (fcw & (1 << 1)) ? 1:0;
	cfg->video_enable.hwm_level1 = (fcw & (1 << 2)) ? 1:0;
	cfg->video_enable.ufcw       = (fcw & (1 << 3)) ? 1:0;
	cfg->video_enable.ldw_left   = (ldw & (1 << 0)) ? 1:0;
	cfg->video_enable.ldw_right  = (ldw & (1 << 1)) ? 1:0;
	
	DBG_LEVEL_6("hwm_level2:%d TTC:%d hwm_level1:%d ufcw:%d ldw_left:%d ldw_right:%d", 
		cfg->video_enable.hwm_level2, cfg->video_enable.TTC, cfg->video_enable.hwm_level1, 
		cfg->video_enable.ufcw, cfg->video_enable.ldw_left, cfg->video_enable.ldw_right);
*/	
	return 0;
}


int TerminalXMLParse::LoadTerminalParam(const char* config_path,JT808_Config_S* cfg)
{/*
	string key = "root";
	boost::property_tree::ptree pt;
	boost::property_tree::read_xml(config_path,pt,boost::property_tree::xml_parser::no_comments|boost::property_tree::xml_parser::trim_whitespace);

	//NetCommunication
	cfg->commCfg.heartbeat			= get_rdt_value(pt,key,"NetCommunication.heart_beat",30);
	cfg->commCfg.tcp.timeout		= get_rdt_value(pt,key,"NetCommunication.tcp_timeout",10);
	cfg->commCfg.tcp.trytimes		= get_rdt_value(pt,key,"NetCommunication.tcp_trytimes",3);
	cfg->commCfg.udp.timeout		= get_rdt_value(pt,key,"NetCommunication.udp_timeout",30);
	cfg->commCfg.udp.trytimes		= get_rdt_value(pt,key,"NetCommunication.udp_trytimes",3);
	cfg->commCfg.sms.timeout		= get_rdt_value(pt,key,"NetCommunication.sms_timeout",30);
	cfg->commCfg.sms.trytimes		= get_rdt_value(pt,key,"NetCommunication.sms_trytimes",0);

	string authenticationkey = get_str_rdt_value(pt,key,"NetCommunication.authen_key","a");
	strncpy((char *)cfg->commCfg.authenticationkey, authenticationkey.c_str(), sizeof(cfg->commCfg.authenticationkey) - 1);

	string phone_num = get_str_rdt_value(pt,key,"NetCommunication.phone_num","a");
	strncpy((char *)cfg->commCfg.phoneNumber, phone_num.c_str(), sizeof(cfg->commCfg.phoneNumber) - 1);
	
	//CenterServer
	string apn = get_str_rdt_value(pt,key,"CenterServer.main_apn","a");
	strncpy(cfg->centerSrvCfg.main.apn, apn.c_str(), sizeof(cfg->centerSrvCfg.main.apn) - 1);
	string username = get_str_rdt_value(pt,key,"CenterServer.main_username","a");
	strncpy(cfg->centerSrvCfg.main.username, apn.c_str(), sizeof(cfg->centerSrvCfg.main.username) - 1);
	
	string psw = get_str_rdt_value(pt,key,"CenterServer.main_pwd","a");
	strncpy(cfg->centerSrvCfg.main.psw, psw.c_str(), sizeof(cfg->centerSrvCfg.main.psw) - 1);
	
	string addr = get_str_rdt_value(pt,key,"CenterServer.main_addr","a");
	strncpy(cfg->centerSrvCfg.main.addr, addr.c_str(), sizeof(cfg->centerSrvCfg.main.addr) - 1);
	string bk_apn = get_str_rdt_value(pt,key,"CenterServer.back_apn","a");
	strncpy(cfg->centerSrvCfg.backup.addr, bk_apn.c_str(), sizeof(cfg->centerSrvCfg.backup.addr) - 1);
	
	string bk_username = get_str_rdt_value(pt,key,"CenterServer.back_username","a");
	strncpy(cfg->centerSrvCfg.backup.username, bk_username.c_str(), sizeof(cfg->centerSrvCfg.backup.username) - 1);
	
	string bk_psw = get_str_rdt_value(pt,key,"CenterServer.back_pwd","a");
	strncpy(cfg->centerSrvCfg.backup.psw, addr.c_str(), sizeof(cfg->centerSrvCfg.backup.psw) - 1);
	string bk_addr = get_str_rdt_value(pt,key,"CenterServer.back_addr","a");
	strncpy(cfg->centerSrvCfg.backup.addr, bk_addr.c_str(), sizeof(cfg->centerSrvCfg.backup.addr) - 1);
	cfg->centerSrvCfg.tcpport		  = get_rdt_value(pt,key,"CenterServer.tcp_port",30);
	cfg->centerSrvCfg.udpport		  = get_rdt_value(pt,key,"CenterServer.udp_port",15);
	
	//ICServer
	string ic_main = get_str_rdt_value(pt,key,"ICServer.main_serv","a");
	strncpy(cfg->icSrvCfg.main, ic_main.c_str(), sizeof(cfg->icSrvCfg.main) - 1);
	string ic_bk_addr = get_str_rdt_value(pt,key,"ICServer.back_serv","a");
	strncpy(cfg->icSrvCfg.backup, ic_bk_addr.c_str(), sizeof(cfg->icSrvCfg.backup) - 1);
	cfg->icSrvCfg.tcpport = get_rdt_value(pt,key,"ICServer.tcp_port",1);
	cfg->icSrvCfg.udpport = get_rdt_value(pt,key,"ICServer.udp_port",0);

	//GnssReport
	cfg->reportCfg.tactics			   = get_rdt_value(pt,key,"GnssReport.tactics",0);
	cfg->reportCfg.scheme			   = get_rdt_value(pt,key,"GnssReport.scheme",1);
	cfg->reportCfg.interval[0].time    = get_rdt_value(pt,key,"GnssReport.interval.default.time",0);
	cfg->reportCfg.interval[0].mileage = get_rdt_value(pt,key,"GnssReport.interval.default.mileage",0);
	cfg->reportCfg.interval[1].time    = get_rdt_value(pt,key,"GnssReport.interval.alarm.time",0);
	cfg->reportCfg.interval[1].mileage = get_rdt_value(pt,key,"GnssReport.interval.alarm.mileage",0);
	cfg->reportCfg.interval[2].time    = get_rdt_value(pt,key,"GnssReport.interval.dormant.time",0);
	cfg->reportCfg.interval[2].mileage = get_rdt_value(pt,key,"GnssReport.interval.dormant.mileage",0);
	cfg->reportCfg.interval[3].time    = get_rdt_value(pt,key,"GnssReport.interval.driver_departure.time",0);
	cfg->reportCfg.interval[3].mileage = get_rdt_value(pt,key,"GnssReport.interval.driver_departure.mileage",0);
	cfg->reportCfg.angle			   = get_rdt_value(pt,key,"GnssReport.angle",0);
	cfg->reportCfg.radius			   = get_rdt_value(pt,key,"GnssReport.radius",0);
	
	string monitornum = get_str_rdt_value(pt,key,"PhoneNumber.monitor_num","a");
	strncpy(cfg->phoneNumCfg.monitornum, monitornum.c_str(), sizeof(cfg->phoneNumCfg.monitornum) - 1);
	
	string supersmsnum = get_str_rdt_value(pt,key,"PhoneNumber.super_sms_num","a");
	strncpy(cfg->phoneNumCfg.supersmsnum, supersmsnum.c_str(), sizeof(cfg->phoneNumCfg.supersmsnum) - 1);
	
	string centernum = get_str_rdt_value(pt,key,"PhoneNumber.center_num","a");
	strncpy(cfg->phoneNumCfg.centernum, centernum.c_str(), sizeof(cfg->phoneNumCfg.centernum) - 1);
	
	string resetnum = get_str_rdt_value(pt,key,"PhoneNumber.reset_num","a");
	strncpy(cfg->phoneNumCfg.resetnum, resetnum.c_str(), sizeof(cfg->phoneNumCfg.resetnum) - 1);
	
	string rebootnum = get_str_rdt_value(pt,key,"PhoneNumber.reboot_num","a");
	strncpy(cfg->phoneNumCfg.rebootnum, rebootnum.c_str(), sizeof(cfg->phoneNumCfg.rebootnum) - 1);
	
	string centersmsnum = get_str_rdt_value(pt,key,"PhoneNumber.center_sms_num","a");
	strncpy(cfg->phoneNumCfg.centersmsnum, centersmsnum.c_str(), sizeof(cfg->phoneNumCfg.centersmsnum) - 1);
	
	string alarmsmsnum = get_str_rdt_value(pt,key,"PhoneNumber.alarm_sms_num","a");
	strncpy(cfg->phoneNumCfg.alarmsmsnum, alarmsmsnum.c_str(), sizeof(cfg->phoneNumCfg.alarmsmsnum) - 1);
	
	cfg->phoneNumCfg.answerway		   = get_rdt_value(pt,key,"PhoneNumber.answer_way",0);
	cfg->phoneNumCfg.calltime		   = get_rdt_value(pt,key,"PhoneNumber.call_time",0);
	cfg->phoneNumCfg.monthtime		   = get_rdt_value(pt,key,"PhoneNumber.month_time",0);
	//alarmMaskCfg
	cfg->alarmMaskCfg.alarmmask 	   = get_rdt_value(pt,key,"AlarmMask.alarm_mask",0);
	cfg->alarmMaskCfg.sms			   = get_rdt_value(pt,key,"AlarmMask.sms",0);
	cfg->alarmMaskCfg.snap			   = get_rdt_value(pt,key,"AlarmMask.snap",0);
	cfg->alarmMaskCfg.snapsave		   = get_rdt_value(pt,key,"AlarmMask.snap_save",0);
	cfg->alarmMaskCfg.keymask		   = get_rdt_value(pt,key,"AlarmMask.key_mask",0);
	
	//AlarmSpeed
	cfg->alarmSpeed.speed			  = get_rdt_value(pt,key,"AlarmSpeed.speed",0);
	cfg->alarmSpeed.time			  = get_rdt_value(pt,key,"AlarmSpeed.time",0);
	cfg->alarmSpeed.warning 		  = get_rdt_value(pt,key,"AlarmSpeed.warning",0);

	//FatigueAlarm
	cfg->alarmFatigue.drivingtime	  = get_rdt_value(pt,key,"FatigueAlarm.driving_time",0);
	cfg->alarmFatigue.totaltime 	  = get_rdt_value(pt,key,"FatigueAlarm.total_time",0);
	cfg->alarmFatigue.resttime		  = get_rdt_value(pt,key,"FatigueAlarm.rest_time",0);
	cfg->alarmFatigue.warning		  = get_rdt_value(pt,key,"FatigueAlarm.warning",0);
	

	//ParkingAlarm
	cfg->alarmParking.time			  = get_rdt_value(pt,key,"ParkingAlarm.time",0);

	//CollisionAlarm
	cfg->alarmCollision.time		  = get_rdt_value(pt,key,"CollisionAlarm.time",0);
	cfg->alarmCollision.acceleration  = get_rdt_value(pt,key,"CollisionAlarm.acceleration",0);

	
	//RolloverAlarm
	cfg->alarmRollover.angle		  = get_rdt_value(pt,key,"RolloverAlarm.angle",0);

	//snapCtrl
	cfg->snapCtrl.time.mask			  = get_rdt_value(pt,key,"SnapCtrl.time.mask",0);
	cfg->snapCtrl.time.savemask		  = get_rdt_value(pt,key,"SnapCtrl.time.save_mask",0);
	cfg->snapCtrl.mileage.mask	      = get_rdt_value(pt,key,"SnapCtrl.mileage.mask",0);
	cfg->snapCtrl.mileage.savemask	  = get_rdt_value(pt,key,"SnapCtrl.mileage.save_mask",0);
	cfg->snapCtrl.timeunit			  = get_rdt_value(pt,key,"SnapCtrl.time_unit",0);
	cfg->snapCtrl.mileageunit		  = get_rdt_value(pt,key,"SnapCtrl.mileage_unit",0);
	cfg->snapCtrl.timeInterval		  = get_rdt_value(pt,key,"SnapCtrl.time_interval",0);	
	cfg->snapCtrl.mileageInterval	  = get_rdt_value(pt,key,"SnapCtrl.mileage_interval",0);
	
	//PicQuality
	cfg->picQuality.quality 		  = get_rdt_value(pt,key,"PicQuality.quality",0);

	//PicColor
	cfg->picColor.luminance 		  = get_rdt_value(pt,key,"PicColor.luminance",0);
	cfg->picColor.contrast			  = get_rdt_value(pt,key,"PicColor.contrast",0);
	cfg->picColor.saturation		  = get_rdt_value(pt,key,"PicColor.saturation",0);
	cfg->picColor.chroma			  = get_rdt_value(pt,key,"PicColor.chroma",0);

	//VehicleInfo
	cfg->VehInfoCfg.mileage 		  = get_rdt_value(pt,key,"VehicleInfo.mileage",0);
	cfg->VehInfoCfg.provinceid		  = get_rdt_value(pt,key,"VehicleInfo.province_id",0);
	cfg->VehInfoCfg.cityid			  = get_rdt_value(pt,key,"VehicleInfo.city_id",0);	

	string license                    = get_str_rdt_value(pt,key,"VehicleInfo.license","a");
	strncpy(cfg->VehInfoCfg.license, license.c_str(), sizeof(cfg->VehInfoCfg.license) - 1);
	
	cfg->VehInfoCfg.platecolor		  = get_rdt_value(pt,key,"VehicleInfo.plate_color",0);
	cfg->VehInfoCfg.coefficient 	  = get_rdt_value(pt,key,"VehicleInfo.coefficient",0);
	string vin                        = get_str_rdt_value(pt,key,"VehicleInfo.vin","a");
	strncpy(cfg->VehInfoCfg.vin, vin.c_str(), sizeof(cfg->VehInfoCfg.vin) - 1);

	string vehicletype                       = get_str_rdt_value(pt,key,"VehicleInfo.vehicle_type","a");
	strncpy(cfg->VehInfoCfg.vehicletype, vehicletype.c_str(), sizeof(cfg->VehInfoCfg.vehicletype) - 1);
	
	cfg->VehInfoCfg.nVehicletype	  = get_rdt_value(pt,key,"VehicleInfo.nvehicle_type",0);

	//VehicleInfo
	cfg->gnssDetail.enable			  = get_rdt_value(pt,key,"GnssDetail.enable",0);
	cfg->gnssDetail.baudtate		  = get_rdt_value(pt,key,"GnssDetail.baudtate",0);
	cfg->gnssDetail.outputFreq		  = get_rdt_value(pt,key,"GnssDetail.output_freq",0);	
	cfg->gnssDetail.uploadmode		  = get_rdt_value(pt,key,"GnssDetail.upload_mode",0);
	cfg->gnssDetail.sampleFreq		  = get_rdt_value(pt,key,"GnssDetail.sample_freq",0);
	cfg->gnssDetail.uploadset		  = get_rdt_value(pt,key,"GnssDetail.upload_set",0);	

	//canbusUpload
	cfg->canbusUpload.sample[0] 	  = get_rdt_value(pt,key,"CanUpload.sample1",0);
	cfg->canbusUpload.sample[1] 	  = get_rdt_value(pt,key,"CanUpload.sample2",0);
	cfg->canbusUpload.upload[0] 	  = get_rdt_value(pt,key,"CanUpload.upload1",0);	
	cfg->canbusUpload.upload[1] 	  = get_rdt_value(pt,key,"CanUpload.upload2",0);

	string set                        = get_str_rdt_value(pt,key,"CanUpload.alone_sample.set","a");
	strncpy((char *)cfg->canbusUpload.aloneSample.set, set.c_str(), sizeof(cfg->canbusUpload.aloneSample.set) - 1);

	//video parameter
	cfg->VideoCfg.ai_vi_attr.coding_mode           = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.coding_mode",0);
	cfg->VideoCfg.ai_vi_attr.resolution            = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.resolution",3);
	cfg->VideoCfg.ai_vi_attr.key_frame_inter       = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.key_frame_inter", 25);
	cfg->VideoCfg.ai_vi_attr.frame_rate            = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.frame_rate", 25);
	cfg->VideoCfg.ai_vi_attr.bit_rate              = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.bit_rate",0);
	cfg->VideoCfg.ai_vi_attr.store_coding_mode     = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.store_coding_mode",0);
	cfg->VideoCfg.ai_vi_attr.store_resolution      = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.store_resolution", 3);
	cfg->VideoCfg.ai_vi_attr.store_key_frame_inter = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.store_key_frame_inter", 25);
	cfg->VideoCfg.ai_vi_attr.store_frame_rate      = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.store_frame_rate", 25);
	cfg->VideoCfg.ai_vi_attr.store_bit_rate        = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.store_bit_rate",0);
	cfg->VideoCfg.ai_vi_attr.osd_set               = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.osd_set",0);
	cfg->VideoCfg.ai_vi_attr.enable_audio          = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.enable_audio",0);

	//audio video chn parameter
	cfg->VideoCfg.ai_vi_chn.chn_total_num          = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Chn.chn_total_num",0);
	cfg->VideoCfg.ai_vi_chn.audio_num              = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Chn.audio_num",0);
	cfg->VideoCfg.ai_vi_chn.video_num              = get_rdt_value(pt,key,"VideoAttr.Ai_Vi_Chn.audio_num",0);

	//single chn parameter
	cfg->VideoCfg.single_chn_ctl.chn_num		   = get_rdt_value(pt,key,"VideoAttr.Single_Chn.chn_num",0);

	//special alarm
	cfg->VideoCfg.special_alarm.video_storage_thres = get_rdt_value(pt,key,"VideoAttr.Special_Alarm.video_storage_thres",20);
	cfg->VideoCfg.special_alarm.video_duration      = get_rdt_value(pt,key,"VideoAttr.Special_Alarm.video_duration",5);
	cfg->VideoCfg.special_alarm.start_time          = get_rdt_value(pt,key,"VideoAttr.Special_Alarm.start_time",1);

	//video alarm parameter
	cfg->VideoCfg.video_alarm.num_of_carries        = get_rdt_value(pt,key,"VideoAttr.Video_Alarm.num_of_carries",30);
	cfg->VideoCfg.video_alarm.fatigue_thres         = get_rdt_value(pt,key,"VideoAttr.Video_Alarm.fatigue_thres",30);

	//daily awake
	cfg->VideoCfg.awake_attr.awake_mode                         = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.awake_mode",30);
	cfg->VideoCfg.awake_attr.awake_type                         = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.awake_type",30);
	cfg->VideoCfg.awake_attr.set_awake_time                     = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.set_awake_time",30);
	cfg->VideoCfg.awake_attr.awake_params.flag                  = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.flag",30);
	cfg->VideoCfg.awake_attr.awake_params.period1_start_time[0] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period1_start_time.hour",0);
	cfg->VideoCfg.awake_attr.awake_params.period1_start_time[1] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period1_start_time.minutes",0);
	cfg->VideoCfg.awake_attr.awake_params.period1_close_time[0] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period1_close_time.hour",0);
	cfg->VideoCfg.awake_attr.awake_params.period1_close_time[1] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period1_close_time.minutes",0);
	cfg->VideoCfg.awake_attr.awake_params.period2_start_time[0] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period2_start_time.hour",0);
	cfg->VideoCfg.awake_attr.awake_params.period2_start_time[1] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period2_start_time.minutes",0);
	cfg->VideoCfg.awake_attr.awake_params.period2_close_time[0] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period2_close_time.hour",0);
	cfg->VideoCfg.awake_attr.awake_params.period2_close_time[1] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period2_close_time.minutes",0);
	cfg->VideoCfg.awake_attr.awake_params.period3_start_time[0] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period3_start_time.hour",0);
	cfg->VideoCfg.awake_attr.awake_params.period3_start_time[1] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period3_start_time.minutes",0);
	cfg->VideoCfg.awake_attr.awake_params.period3_close_time[0] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period3_close_time.hour",0);
	cfg->VideoCfg.awake_attr.awake_params.period3_close_time[1] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period3_close_time.minutes",0);
	cfg->VideoCfg.awake_attr.awake_params.period4_start_time[0] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period4_start_time.hour",0);
	cfg->VideoCfg.awake_attr.awake_params.period4_start_time[1] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period4_start_time.minutes",0);
	cfg->VideoCfg.awake_attr.awake_params.period4_close_time[0] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period4_close_time.hour",0);
	cfg->VideoCfg.awake_attr.awake_params.period4_close_time[1] = get_rdt_value(pt,key,"VideoAttr.Daily_Awake.period4_close_time.minutes",0);
*/	
	return 0;
}

int TerminalXMLParse::saveTerminalParam(const char *pXmlfile, JT808_Config_S *tParams)
{/*
	#if 1
	string key = "root";
    boost::property_tree::ptree pt;

	//NetCommunication
	set_rdt_value(pt,key,"NetCommunication.heart_beat",		tParams->commCfg.heartbeat);
	set_rdt_value(pt,key,"NetCommunication.tcp_timeout",	tParams->commCfg.tcp.timeout);
	set_rdt_value(pt,key,"NetCommunication.tcp_trytimes",	tParams->commCfg.tcp.trytimes);
	set_rdt_value(pt,key,"NetCommunication.udp_timeout",	tParams->commCfg.udp.timeout);
	set_rdt_value(pt,key,"NetCommunication.udp_trytimes",	tParams->commCfg.udp.trytimes);
	set_rdt_value(pt,key,"NetCommunication.sms_timeout",	tParams->commCfg.sms.timeout);
	set_rdt_value(pt,key,"NetCommunication.sms_trytimes",	tParams->commCfg.sms.trytimes);
	set_rdt_value(pt,key,"NetCommunication.authen_key",		tParams->commCfg.authenticationkey);
	string phone_num = (char *)tParams->commCfg.phoneNumber;
	set_rdt_value(pt,key,"NetCommunication.phone_num",		phone_num);
	
	//CenterServer
	set_rdt_value(pt,key,"CenterServer.main_apn",			tParams->centerSrvCfg.main.apn);
	set_rdt_value(pt,key,"CenterServer.main_username",		tParams->centerSrvCfg.main.username);
	set_rdt_value(pt,key,"CenterServer.main_pwd",			tParams->centerSrvCfg.main.psw);
	set_rdt_value(pt,key,"CenterServer.main_addr",			tParams->centerSrvCfg.main.addr);
	set_rdt_value(pt,key,"CenterServer.back_apn",			tParams->centerSrvCfg.backup.addr);
	set_rdt_value(pt,key,"CenterServer.back_username",		tParams->centerSrvCfg.backup.username);
	set_rdt_value(pt,key,"CenterServer.back_pwd",			tParams->centerSrvCfg.backup.psw);
	set_rdt_value(pt,key,"CenterServer.back_addr",			tParams->centerSrvCfg.backup.addr);
	set_rdt_value(pt,key,"CenterServer.tcp_port",			tParams->centerSrvCfg.tcpport);
	set_rdt_value(pt,key,"CenterServer.udp_port",			tParams->centerSrvCfg.udpport);
	
	//ICServer
	set_rdt_value(pt,key,"ICServer.main_serv",				tParams->icSrvCfg.main);
	set_rdt_value(pt,key,"ICServer.back_serv",				tParams->icSrvCfg.backup);
	set_rdt_value(pt,key,"ICServer.tcp_port",				tParams->icSrvCfg.tcpport);
	set_rdt_value(pt,key,"ICServer.udp_port",				tParams->icSrvCfg.udpport);

	//GnssReport
	set_rdt_value(pt,key,"GnssReport.tactics",							tParams->reportCfg.tactics);
	set_rdt_value(pt,key,"GnssReport.scheme",							tParams->reportCfg.scheme);
	set_rdt_value(pt,key,"GnssReport.interval.default.time",			tParams->reportCfg.interval[0].time);
	set_rdt_value(pt,key,"GnssReport.interval.default.mileage",			tParams->reportCfg.interval[0].mileage);
	set_rdt_value(pt,key,"GnssReport.interval.alarm.time",				tParams->reportCfg.interval[1].time);
	set_rdt_value(pt,key,"GnssReport.interval.alarm.mileage",			tParams->reportCfg.interval[1].mileage);
	set_rdt_value(pt,key,"GnssReport.interval.dormant.time",			tParams->reportCfg.interval[2].time);
	set_rdt_value(pt,key,"GnssReport.interval.dormant.mileage",			tParams->reportCfg.interval[2].mileage);
	set_rdt_value(pt,key,"GnssReport.interval.driver_departure.time",	tParams->reportCfg.interval[3].time);
	set_rdt_value(pt,key,"GnssReport.interval.driver_departure.mileage",tParams->reportCfg.interval[3].mileage);
	set_rdt_value(pt,key,"GnssReport.angle",							tParams->reportCfg.angle);
	set_rdt_value(pt,key,"GnssReport.radius",							tParams->reportCfg.radius);

	
	set_rdt_value(pt,key,"PhoneNumber.monitor_num",			tParams->phoneNumCfg.monitornum);
	set_rdt_value(pt,key,"PhoneNumber.super_sms_num",		tParams->phoneNumCfg.supersmsnum);
	set_rdt_value(pt,key,"PhoneNumber.center_num",			tParams->phoneNumCfg.centernum);
	set_rdt_value(pt,key,"PhoneNumber.reset_num",			tParams->phoneNumCfg.resetnum);
	set_rdt_value(pt,key,"PhoneNumber.reboot_num",			tParams->phoneNumCfg.rebootnum);
	set_rdt_value(pt,key,"PhoneNumber.center_sms_num",		tParams->phoneNumCfg.centersmsnum);
	set_rdt_value(pt,key,"PhoneNumber.alarm_sms_num",		tParams->phoneNumCfg.alarmsmsnum);
	set_rdt_value(pt,key,"PhoneNumber.answer_way",			tParams->phoneNumCfg.answerway);
	set_rdt_value(pt,key,"PhoneNumber.call_time",			tParams->phoneNumCfg.calltime);
	set_rdt_value(pt,key,"PhoneNumber.month_time",			tParams->phoneNumCfg.monthtime);
	//alarmMaskCfg
	set_rdt_value(pt,key,"AlarmMask.alarm_mask",			tParams->alarmMaskCfg.alarmmask);
	set_rdt_value(pt,key,"AlarmMask.sms",					tParams->alarmMaskCfg.sms);
	set_rdt_value(pt,key,"AlarmMask.snap",					tParams->alarmMaskCfg.snap);
	set_rdt_value(pt,key,"AlarmMask.snap_save",				tParams->alarmMaskCfg.snapsave);
	set_rdt_value(pt,key,"AlarmMask.key_mask",				tParams->alarmMaskCfg.keymask);
	
	//AlarmSpeed
	set_rdt_value(pt,key,"AlarmSpeed.speed",				tParams->alarmSpeed.speed);
	set_rdt_value(pt,key,"AlarmSpeed.time",					tParams->alarmSpeed.time);
	set_rdt_value(pt,key,"AlarmSpeed.warning",				tParams->alarmSpeed.warning);

	//FatigueAlarm
	set_rdt_value(pt,key,"FatigueAlarm.driving_time",		tParams->alarmFatigue.drivingtime);
	set_rdt_value(pt,key,"FatigueAlarm.total_time",			tParams->alarmFatigue.totaltime);
	set_rdt_value(pt,key,"FatigueAlarm.rest_time",			tParams->alarmFatigue.resttime);
	set_rdt_value(pt,key,"FatigueAlarm.warning",			tParams->alarmFatigue.warning);

	//ParkingAlarm
	set_rdt_value(pt,key,"ParkingAlarm.time",				tParams->alarmParking.time	);

	//CollisionAlarm
	set_rdt_value(pt,key,"CollisionAlarm.time",				tParams->alarmCollision.time);
	set_rdt_value(pt,key,"CollisionAlarm.acceleration",		tParams->alarmCollision.acceleration);
	set_rdt_value(pt,key,"RolloverAlarm.angle",				tParams->alarmRollover.angle);

	//snapCtrl
	set_rdt_value(pt,key,"SnapCtrl.time.mask",				tParams->snapCtrl.time.mask);
	set_rdt_value(pt,key,"SnapCtrl.time.save_mask",			tParams->snapCtrl.time.savemask);
	set_rdt_value(pt,key,"SnapCtrl.mileage.mask",			tParams->snapCtrl.mileage.mask);
	set_rdt_value(pt,key,"SnapCtrl.mileage.save_mask",		tParams->snapCtrl.mileage.savemask);
	set_rdt_value(pt,key,"SnapCtrl.time_unit",				tParams->snapCtrl.timeunit);
	set_rdt_value(pt,key,"SnapCtrl.mileage_unit",			tParams->snapCtrl.mileageunit);
	set_rdt_value(pt,key,"SnapCtrl.time_interval",			tParams->snapCtrl.timeInterval);	
	set_rdt_value(pt,key,"SnapCtrl.mileage_interval",		tParams->snapCtrl.mileageInterval);
	
	
	//PicQuality
	set_rdt_value(pt,key,"PicQuality.quality",				tParams->picQuality.quality);

	//PicColor
	set_rdt_value(pt,key,"PicColor.luminance",				tParams->picColor.luminance);
	set_rdt_value(pt,key,"PicColor.contrast",				tParams->picColor.contrast);
	set_rdt_value(pt,key,"PicColor.saturation",				tParams->picColor.saturation);
	set_rdt_value(pt,key,"PicColor.chroma",					tParams->picColor.chroma);

	//VehicleInfo
	set_rdt_value(pt,key,"VehicleInfo.mileage",				tParams->VehInfoCfg.mileage );
	set_rdt_value(pt,key,"VehicleInfo.province_id",			tParams->VehInfoCfg.provinceid);
	set_rdt_value(pt,key,"VehicleInfo.city_id",				tParams->VehInfoCfg.cityid);	
	set_rdt_value(pt,key,"VehicleInfo.license",				tParams->VehInfoCfg.license);	
	set_rdt_value(pt,key,"VehicleInfo.plate_color",			tParams->VehInfoCfg.platecolor);
	set_rdt_value(pt,key,"VehicleInfo.coefficient",			tParams->VehInfoCfg.coefficient);
	set_rdt_value(pt,key,"VehicleInfo.vin",					tParams->VehInfoCfg.vin);
	set_rdt_value(pt,key,"VehicleInfo.vehicle_type",		tParams->VehInfoCfg.vehicletype);
	set_rdt_value(pt,key,"VehicleInfo.nvehicle_type",		tParams->VehInfoCfg.nVehicletype);

	//VehicleInfo
	set_rdt_value(pt,key,"GnssDetail.enable",				tParams->gnssDetail.enable);
	set_rdt_value(pt,key,"GnssDetail.baudtate",				tParams->gnssDetail.baudtate);
	set_rdt_value(pt,key,"GnssDetail.output_freq",			tParams->gnssDetail.outputFreq);	
	set_rdt_value(pt,key,"GnssDetail.upload_mode",			tParams->gnssDetail.uploadmode);
	set_rdt_value(pt,key,"GnssDetail.sample_freq",			tParams->gnssDetail.sampleFreq);
	set_rdt_value(pt,key,"GnssDetail.upload_set",			tParams->gnssDetail.uploadset);	

	//canbusUpload
	set_rdt_value(pt,key,"CanUpload.sample1",				tParams->canbusUpload.sample[0]);
	set_rdt_value(pt,key,"CanUpload.sample2",				tParams->canbusUpload.sample[1]);
	set_rdt_value(pt,key,"CanUpload.upload1",				tParams->canbusUpload.upload[0]);	
	set_rdt_value(pt,key,"CanUpload.upload2",				tParams->canbusUpload.upload[1]);
	string set = (char *)tParams->canbusUpload.aloneSample.set;
	set_rdt_value(pt,key,"CanUpload.alone_sample.set",		set);	

	//video parameter
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.coding_mode", tParams->VideoCfg.ai_vi_attr.coding_mode);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.resolution", tParams->VideoCfg.ai_vi_attr.resolution);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.key_frame_inter", tParams->VideoCfg.ai_vi_attr.key_frame_inter);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.frame_rate", tParams->VideoCfg.ai_vi_attr.frame_rate);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.bit_rate", tParams->VideoCfg.ai_vi_attr.bit_rate);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.store_coding_mode", tParams->VideoCfg.ai_vi_attr.store_coding_mode);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.store_resolution", tParams->VideoCfg.ai_vi_attr.store_resolution);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.store_key_frame_inter", tParams->VideoCfg.ai_vi_attr.store_key_frame_inter);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.store_frame_rate", tParams->VideoCfg.ai_vi_attr.store_frame_rate);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.store_bit_rate", tParams->VideoCfg.ai_vi_attr.store_bit_rate);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.osd_set", tParams->VideoCfg.ai_vi_attr.osd_set);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Conf.enable_audio", tParams->VideoCfg.ai_vi_attr.enable_audio);

	//audio video chn parameter
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Chn.chn_total_num", tParams->VideoCfg.ai_vi_chn.chn_total_num);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Chn.audio_num", tParams->VideoCfg.ai_vi_chn.audio_num);
	set_rdt_value(pt,key,"VideoAttr.Ai_Vi_Chn.audio_num", tParams->VideoCfg.ai_vi_chn.video_num);

	//single chn parameter
	set_rdt_value(pt,key,"VideoAttr.Single_Chn.chn_num", tParams->VideoCfg.single_chn_ctl.chn_num);

	//special alarm
	set_rdt_value(pt,key,"VideoAttr.Special_Alarm.video_storage_thres", tParams->VideoCfg.special_alarm.video_storage_thres);
	set_rdt_value(pt,key,"VideoAttr.Special_Alarm.video_duration", tParams->VideoCfg.special_alarm.video_duration);
	set_rdt_value(pt,key,"VideoAttr.Special_Alarm.start_time", tParams->VideoCfg.special_alarm.start_time);

	//video alarm parameter
	set_rdt_value(pt,key,"VideoAttr.Video_Alarm.num_of_carries", tParams->VideoCfg.video_alarm.num_of_carries);
	set_rdt_value(pt,key,"VideoAttr.Video_Alarm.fatigue_thres", tParams->VideoCfg.video_alarm.fatigue_thres);

	//daily awake
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.awake_mode", tParams->VideoCfg.awake_attr.awake_mode);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.awake_type", tParams->VideoCfg.awake_attr.awake_type);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.set_awake_time", tParams->VideoCfg.awake_attr.set_awake_time);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.flag", tParams->VideoCfg.awake_attr.awake_params.flag);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period1_start_time.hour", tParams->VideoCfg.awake_attr.awake_params.period1_start_time[0]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period1_start_time.minutes", tParams->VideoCfg.awake_attr.awake_params.period1_start_time[1]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period1_close_time.hour", tParams->VideoCfg.awake_attr.awake_params.period1_close_time[0]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period1_close_time.minutes", tParams->VideoCfg.awake_attr.awake_params.period1_close_time[1]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period2_start_time.hour", tParams->VideoCfg.awake_attr.awake_params.period2_start_time[0]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period2_start_time.minutes", tParams->VideoCfg.awake_attr.awake_params.period2_start_time[1]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period2_close_time.hour", tParams->VideoCfg.awake_attr.awake_params.period2_close_time[0]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period2_close_time.minutes", tParams->VideoCfg.awake_attr.awake_params.period2_close_time[1]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period3_start_time.hour", tParams->VideoCfg.awake_attr.awake_params.period3_start_time[0]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period3_start_time.minutes", tParams->VideoCfg.awake_attr.awake_params.period3_start_time[1]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period3_close_time.hour", tParams->VideoCfg.awake_attr.awake_params.period3_close_time[0]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period3_close_time.minutes", tParams->VideoCfg.awake_attr.awake_params.period3_close_time[1]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period4_start_time.hour", tParams->VideoCfg.awake_attr.awake_params.period4_start_time[0]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period4_start_time.minutes", tParams->VideoCfg.awake_attr.awake_params.period4_start_time[1]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period4_close_time.hour", tParams->VideoCfg.awake_attr.awake_params.period4_close_time[0]);
	set_rdt_value(pt,key,"VideoAttr.Daily_Awake.period4_close_time.minutes", tParams->VideoCfg.awake_attr.awake_params.period4_close_time[1]);
		
	boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
	boost::property_tree::write_xml(pXmlfile, pt, std::locale(), settings);

	char cmd[128] = {0};
	sprintf(cmd, "/bin/sync %s", pXmlfile);
	system(cmd);
	#endif
*/	
    return 0;
}


