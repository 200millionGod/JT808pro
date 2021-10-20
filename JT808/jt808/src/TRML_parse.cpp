/*************************************************************
 * Copyright			: 
 * FileName				: XMLParse.cpp
 * Description			: XMLParse.cpp implement
 * Author		        : chas
 * Created on			: Jan 4, 2019
 *************************************************************/
#include "../inc/TRML_parse.h"

#include <iostream>
#include <string>
#include "../inc/TRML_jt808.h"
#include "../inc/debug_log.h"



XMLParse::XMLParse()
{
}

XMLParse::~XMLParse()
{
}

void XMLParse::parse_basic_config(tinyxml2::XMLElement *pBasicConfigNode, Basic_Config& config)
{
	tinyxml2::XMLElement *pElement = pBasicConfigNode->FirstChildElement(XML_BASIC_SPEED_THRES)->ToElement();
	if(pElement){
		config.speed_thres = atoi(pElement->GetText());
		DBG_LEVEL_6("speed_thres[%u]", config.speed_thres);
	}

	pElement = pBasicConfigNode->FirstChildElement(XML_BASIC_ALARM_VOLUME)->ToElement();
	if(pElement){
		config.alarm_volume = atoi(pElement->GetText());	
		DBG_LEVEL_6("alarm_volume[%u]", config.alarm_volume);
	}

	pElement = pBasicConfigNode->FirstChildElement(XML_BASIC_ALARM_VOLUME)->ToElement();
	if(pElement){
		config.alarm_volume = atoi(pElement->GetText());
		DBG_LEVEL_6("alarm_volume[%u]", config.alarm_volume);
	}

	pElement = pBasicConfigNode->FirstChildElement(XML_BASIC_PHOTO_RES)->ToElement();
	if(pElement){
		config.photo_res = atoi(pElement->GetText());
		DBG_LEVEL_6("photo_res[%u]", config.photo_res);
	}

	pElement = pBasicConfigNode->FirstChildElement(XML_BASIC_VIDEO_RES)->ToElement();
	if(pElement){
		config.video_res = atoi(pElement->GetText());	
		DBG_LEVEL_6("video_res[%u]", config.video_res);
	}

	/*-------------------------------auto snap--------------------------------*/
	tinyxml2::XMLElement *pSnapNode = pBasicConfigNode->FirstChildElement(XML_BASIC_AUTO_SNAP)->ToElement();
	if(pSnapNode){
		param_auto_snap_config(pSnapNode, config.autosnap);
	}
}

void XMLParse::parse_interval_config(tinyxml2::XMLElement *pIntervalConfigNode, Interval_Attr& interval)
{
	tinyxml2::XMLElement *pElement = pIntervalConfigNode->FirstChildElement(XML_DEFAULT_TIME)->ToElement();
	if(pElement){
		interval.time = atoi(pElement->GetText());	
		DBG_LEVEL_6("time[%u]", interval.time);
	}
	pElement = pIntervalConfigNode->FirstChildElement(XML_DEFAULT_MILEAGE)->ToElement();
	if(pElement){
		interval.mileage = atoi(pElement->GetText());	
		DBG_LEVEL_6("mileage[%u]", interval.mileage);
	}
}

void XMLParse::parse_snapctl_time_config(tinyxml2::XMLElement *pSnapctlConfigNode, Snap_Mask_Attr& time)
{
	tinyxml2::XMLElement *pElement = pSnapctlConfigNode->FirstChildElement(XML_TIME_MASK)->ToElement();
	if(pElement){
		time.mask = atoi(pElement->GetText());	
		DBG_LEVEL_6("time.mask[%u]", time.mask);
	}
	pElement = pSnapctlConfigNode->FirstChildElement(XML_TIME_SAVE_MASK)->ToElement();
	if(pElement){
		time.savemask = atoi(pElement->GetText());	
		DBG_LEVEL_6("time.savemask[%u]", time.savemask);
	}
}

void XMLParse::parse_snapctl_mileage_config(tinyxml2::XMLElement *pSnapctlConfigNode, Snap_Mask_Attr& mileage)
{
	tinyxml2::XMLElement *pElement = pSnapctlConfigNode->FirstChildElement(XML_MILEAGE_MASK)->ToElement();
	if(pElement){
		mileage.mask = atoi(pElement->GetText());	
		DBG_LEVEL_6("mileage.mask[%u]", mileage.mask);
	}
	pElement = pSnapctlConfigNode->FirstChildElement(XML_MILEAGE_SAVE_MASK)->ToElement();
	if(pElement){
		mileage.savemask = atoi(pElement->GetText());	
		DBG_LEVEL_6("mileage.savemask[%u]", mileage.savemask);
	}
}

void XMLParse::parse_videoattr_ai_vi_config(tinyxml2::XMLElement *pVideoAttrConfigNode, JT1078_Audio_video_conf_set& ai_vi_conf)
{
	tinyxml2::XMLElement *pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_CODING_MODE)->ToElement();
	if(pElement){
		ai_vi_conf.coding_mode = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.coding_mode[%02x]", ai_vi_conf.coding_mode);
	}
	pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_RESOLUTION)->ToElement();
	if(pElement){
		ai_vi_conf.resolution = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.resolution[%02x]", ai_vi_conf.resolution);
	}
	pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_KEY_FRAME_INTER)->ToElement();
	if(pElement){
		ai_vi_conf.key_frame_inter = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.key_frame_inter[%u]", ai_vi_conf.key_frame_inter);
	}
	pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_FRAME_RATE)->ToElement();
	if(pElement){
		ai_vi_conf.frame_rate = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.frame_rate[%02x]", ai_vi_conf.frame_rate);
	}
	pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_BIT_RATE)->ToElement();
	if(pElement){
		ai_vi_conf.bit_rate = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.bit_rate[%u]", ai_vi_conf.bit_rate);
	}
	pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_STORE_CODING_MODE)->ToElement();
	if(pElement){
		ai_vi_conf.store_coding_mode = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.store_coding_mode[%02x]", ai_vi_conf.store_coding_mode);
	}
	pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_STORE_RESOLUTION)->ToElement();
	if(pElement){
		ai_vi_conf.store_resolution = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.store_resolution[%02x]", ai_vi_conf.store_resolution);
	}
	pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_STORE_KEY_FRAME_INTER)->ToElement();
	if(pElement){
		ai_vi_conf.store_key_frame_inter = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.store_key_frame_inter[%u]", ai_vi_conf.store_key_frame_inter);
	}
	pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_STORE_FRAME_RATE)->ToElement();
	if(pElement){
		ai_vi_conf.store_frame_rate = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.store_frame_rate[%02x]", ai_vi_conf.store_frame_rate);
	}
	pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_STORE_BIT_RATE)->ToElement();
	if(pElement){
		ai_vi_conf.store_bit_rate = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.coding_mode[%u]", ai_vi_conf.store_bit_rate);
	}
	pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_OSD_SET)->ToElement();
	if(pElement){
		ai_vi_conf.osd_set = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.osd_set[%u]", ai_vi_conf.osd_set);
	}
	pElement = pVideoAttrConfigNode->FirstChildElement(XML_AI_VI_CONF_ENABLE_AUDIO)->ToElement();
	if(pElement){
		ai_vi_conf.enable_audio = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_conf.enable_audio[%02x]", ai_vi_conf.enable_audio);
	}
}

void XMLParse::parse_videoattr_ai_vi_chn_config(tinyxml2::XMLElement *pAiViChnConfigNode, JT1078_Audio_video_chn_set& ai_vi_chn)
{
	tinyxml2::XMLElement *pElement = pAiViChnConfigNode->FirstChildElement(XML_AI_VI_CHN_CHN_TOTAL_NUM)->ToElement();
	if(pElement){
		ai_vi_chn.chn_total_num = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_chn.chn_total_num[%02x]", ai_vi_chn.chn_total_num);
	}
	pElement = pAiViChnConfigNode->FirstChildElement(XML_AI_VI_CHN_AUDIO_NUM)->ToElement();
	if(pElement){
		ai_vi_chn.audio_num = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_chn.audio_num[%02x]", ai_vi_chn.audio_num);
	}
	pElement = pAiViChnConfigNode->FirstChildElement(XML_AI_VI_CHN_VIDEO_NUM)->ToElement();
	if(pElement){
		ai_vi_chn.video_num = atoi(pElement->GetText());
		DBG_LEVEL_6("ai_vi_chn.video_num[%u]", ai_vi_chn.video_num);
	}
}

void XMLParse::parse_videoattr_single_chn_config(tinyxml2::XMLElement *pSingleChnConfigNode, JT1078_single_chn_control& single_chn_ctl)
{
	tinyxml2::XMLElement *pElement = pSingleChnConfigNode->FirstChildElement(XML_SINGLE_CHN_CHN_NUM)->ToElement();
	if(pElement){
		single_chn_ctl.chn_num = atoi(pElement->GetText());
		DBG_LEVEL_6("single_chn_ctl.chn_num[%02x]", single_chn_ctl.chn_num);
	}
}

void XMLParse::parse_videoattr_special_alarm_config(tinyxml2::XMLElement *pSpecialAlarmConfigNode, JT1078_special_alarm_param_set& special_alarm)
{
	tinyxml2::XMLElement *pElement = pSpecialAlarmConfigNode->FirstChildElement(XML_SPECIAL_ALARM_VIDEO_STORAGE_THRES)->ToElement();
	if(pElement){
		special_alarm.video_storage_thres = atoi(pElement->GetText());
		DBG_LEVEL_6("special_alarm.video_storage_thres[%02x]", special_alarm.video_storage_thres);
	}
	pElement = pSpecialAlarmConfigNode->FirstChildElement(XML_SPECIAL_ALARM_VIDEO_DURATION)->ToElement();
	if(pElement){
		special_alarm.video_duration = atoi(pElement->GetText());
		DBG_LEVEL_6("special_alarm.video_duration[%02x]", special_alarm.video_duration);
	}
	pElement = pSpecialAlarmConfigNode->FirstChildElement(XML_SPECIAL_ALARM_START_TIME)->ToElement();
	if(pElement){
		special_alarm.start_time = atoi(pElement->GetText());
		DBG_LEVEL_6("special_alarm.start_time[%02x]", special_alarm.start_time);
	}
}

void XMLParse::parse_videoattr_video_alarm_config(tinyxml2::XMLElement *pVideoAlarmConfigNode, JT1078_alarm_video_param& video_alarm)
{
	tinyxml2::XMLElement *pElement = pVideoAlarmConfigNode->FirstChildElement(XML_VIDEO_ALALRM_NUM_OF_CARRIES)->ToElement();
	if(pElement){
		video_alarm.num_of_carries = atoi(pElement->GetText());
		DBG_LEVEL_6("video_alarm.num_of_carries[%02x]", video_alarm.num_of_carries);
	}
	pElement = pVideoAlarmConfigNode->FirstChildElement(XML_VIDEO_ALALRM_FATIGUE_THRES)->ToElement();
	if(pElement){
		video_alarm.fatigue_thres = atoi(pElement->GetText());
		DBG_LEVEL_6("video_alarm.fatigue_thres[%02x]", video_alarm.fatigue_thres);
	}
}

void XMLParse::parse_videoattr_daily_awake_config(tinyxml2::XMLElement *pDailyAwakeConfigNode, JT1078_awake_param& awake_attr)
{
	tinyxml2::XMLElement *pElement = pDailyAwakeConfigNode->FirstChildElement(XML_DAILY_AWAKE_AWAKE_MODE)->ToElement();
	if(pElement){
		awake_attr.awake_mode = atoi(pElement->GetText());
		DBG_LEVEL_6("awake_attr.awake_mode[%02x]", awake_attr.awake_mode);
	}
	pElement = pDailyAwakeConfigNode->FirstChildElement(XML_DAILY_AWAKE_AWAKE_TYPE)->ToElement();
	if(pElement){
		awake_attr.awake_type = atoi(pElement->GetText());
		DBG_LEVEL_6("awake_attr.awake_type[%02x]", awake_attr.awake_type);
	}
	pElement = pDailyAwakeConfigNode->FirstChildElement(XML_DAILY_AWAKE_SET_AWAKE_TIME)->ToElement();
	if(pElement){
		awake_attr.set_awake_time = atoi(pElement->GetText());
		DBG_LEVEL_6("awake_attr.set_awake_time[%02x]", awake_attr.set_awake_time);
	}
	pElement = pDailyAwakeConfigNode->FirstChildElement(XML_DAILY_AWAKE_FLAG)->ToElement();
	if(pElement){
		awake_attr.awake_params.flag = atoi(pElement->GetText());
		DBG_LEVEL_6("awake_attr.awake_params.flag[%02x]", awake_attr.awake_params.flag);
	}
#if 0
	tinyxml2::XMLElement *period1StartTimeNode = pDailyAwakeConfigNode->FirstChildElement(XML_SUB_PERIOD1_START_TIME)->ToElement();
	if(period1StartTimeNode){
		param_period1_startTime_config(period1StartTimeNode, awake_attr.awake_params);
	}
	tinyxml2::XMLElement *period1CloseTimeNode = pDailyAwakeConfigNode->FirstChildElement(XML_SUB_PERIOD1_CLOSE_TIME)->ToElement();
	if(period1CloseTimeNode){
		param_period1_closeTime_config(period1CloseTimeNode, awake_attr.awake_params);
	}
	tinyxml2::XMLElement *period2StartTimeNode = pDailyAwakeConfigNode->FirstChildElement(XML_SUB_PERIOD2_START_TIME)->ToElement();
	if(period2StartTimeNode){
		param_period2_startTime_config(period2StartTimeNode, awake_attr.awake_params);
	}
	tinyxml2::XMLElement *period2CloseTimeNode = pDailyAwakeConfigNode->FirstChildElement(XML_SUB_PERIOD2_CLOSE_TIME)->ToElement();
	if(period2CloseTimeNode){
		param_period2_closeTime_config(period2CloseTimeNode, awake_attr.awake_params);
	}
	tinyxml2::XMLElement *period3StartTimeNode = pDailyAwakeConfigNode->FirstChildElement(XML_SUB_PERIOD3_START_TIME)->ToElement();
	if(period3StartTimeNode){
		param_period3_startTime_config(period3StartTimeNode, awake_attr.awake_params);
	}
	tinyxml2::XMLElement *period3CloseTimeNode = pDailyAwakeConfigNode->FirstChildElement(XML_SUB_PERIOD3_CLOSE_TIME)->ToElement();
	if(period3CloseTimeNode){
		param_period3_closeTime_config(period3CloseTimeNode, awake_attr.awake_params);
	}
	tinyxml2::XMLElement *period4StartTimeNode = pDailyAwakeConfigNode->FirstChildElement(XML_SUB_PERIOD4_START_TIME)->ToElement();
	if(period4StartTimeNode){
		param_period4_startTime_config(period4StartTimeNode, awake_attr.awake_params);
	}
	tinyxml2::XMLElement *period4CloseTimeNode = pDailyAwakeConfigNode->FirstChildElement(XML_SUB_PERIOD4_CLOSE_TIME)->ToElement();
	if(period4CloseTimeNode){
		param_period4_closeTime_config(period4CloseTimeNode, awake_attr.awake_params);
	}
#endif
}

void XMLParse::param_period1_startTime_config(tinyxml2::XMLElement *period1StartTimeConfigNode, JT1078_daily_awake_param& awake_params)
{}

void XMLParse::param_period1_closeTime_config(tinyxml2::XMLElement *period1CloseTimeConfigNode, JT1078_daily_awake_param& awake_params)
{}

void XMLParse::param_period2_startTime_config(tinyxml2::XMLElement *period2StartTimeConfigNode, JT1078_daily_awake_param& awake_params)
{}

void XMLParse::param_period2_closeTime_config(tinyxml2::XMLElement *period2CloseTimeConfigNode, JT1078_daily_awake_param& awake_params)
{}

void XMLParse::param_period3_startTime_config(tinyxml2::XMLElement *period3StartTimeConfigNode, JT1078_daily_awake_param& awake_params)
{}

void XMLParse::param_period3_closeTime_config(tinyxml2::XMLElement *period3CloseTimeConfigNode, JT1078_daily_awake_param& awake_params)
{}

void XMLParse::param_period4_startTime_config(tinyxml2::XMLElement *period4StartTimeConfigNode, JT1078_daily_awake_param& awake_params)
{}

void XMLParse::param_period4_closeTime_config(tinyxml2::XMLElement *period4CloseTimeConfigNode, JT1078_daily_awake_param& awake_params)
{}

void XMLParse::param_auto_snap_config(tinyxml2::XMLElement *pAutoSnapNode, AutoSnap& auto_snap)
{
	DBG_LEVEL_6("Auto Snap Config:");
	tinyxml2::XMLElement *pElement = pAutoSnapNode->FirstChildElement(XML_BASIC_TACTICS)->ToElement();
	if(pElement){
		auto_snap.tactics = atoi(pElement->GetText());
		DBG_LEVEL_6("tactics[%u]", auto_snap.tactics);
	}

    pElement = pAutoSnapNode->FirstChildElement(XML_BASIC_INTERVAL_TIME)->ToElement();
	if(pElement){
		auto_snap.interval_time = atoi(pElement->GetText());
		DBG_LEVEL_6("interval_time[%u]", auto_snap.interval_time);
	}

	pElement = pAutoSnapNode->FirstChildElement(XML_BASIC_INTERVAL_DIS)->ToElement();
	if(pElement){
		auto_snap.interval_distance = atoi(pElement->GetText());
		DBG_LEVEL_6("interval_distance[%u]", auto_snap.interval_distance);
	}

	pElement = pAutoSnapNode->FirstChildElement(XML_BASIC_SNAP_NUM)->ToElement();
	if(pElement){
		auto_snap.snap_num = atoi(pElement->GetText());
		DBG_LEVEL_6("snap_num[%u]", auto_snap.snap_num);
	}

	pElement = pAutoSnapNode->FirstChildElement(XML_BASIC_SNAP_INTERVAL)->ToElement();
	if(pElement){
		auto_snap.snap_interval = atoi(pElement->GetText());
		DBG_LEVEL_6("snap_interval[%u]", auto_snap.snap_interval);
	}
}

void XMLParse::parse_alarm_config(tinyxml2::XMLElement *pAlarmNode, AlarmConfig& config)
{
	tinyxml2::XMLElement * pElement = NULL;
	pElement = pAlarmNode->FirstChildElement(XML_ALARM_LEVEL_ONE)->ToElement();
	if(pElement){
		if(atoi(pElement->GetText()))
		{
			config.level1 = 1;
		}else
		{
			config.level1 = 0;
		}
	}
	pElement = pAlarmNode->FirstChildElement(XML_ALARM_LEVEL_TWO)->ToElement();
	if(pElement){
		if(atoi(pElement->GetText()))
		{
			config.level2 = 1;
		}else
		{
			config.level2 = 0;
		}
	}
	
	pElement = pAlarmNode->FirstChildElement(XML_ALARM_SPEED_THRES)->ToElement();
	if(pElement){
		config.speed_thres = atoi(pElement->GetText());
	}

	pElement = pAlarmNode->FirstChildElement(XML_ALARM_REC_TIME)->ToElement();
	if(pElement){
		config.rec_time = atoi(pElement->GetText());
	}

	pElement = pAlarmNode->FirstChildElement(XML_ALARM_SNAP_NUM)->ToElement();
	if(pElement){
		config.snap_num = atoi(pElement->GetText());
	}

	pElement = pAlarmNode->FirstChildElement(XML_ALARM_SNAP_INTERVAL)->ToElement();
	if(pElement){
		config.snap_interval = atoi(pElement->GetText());
	}
	
}

void XMLParse::parse_dsm_alarm_config(tinyxml2::XMLElement *pDSMAlarmNode, DSM_Config& config)
{
	tinyxml2::XMLElement *pTmpNode = pDSMAlarmNode->FirstChildElement(XML_ALARM_FATIGUE)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.fatigue);
		DBG_LEVEL_6("fatigue Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
					config.fatigue.level1, config.fatigue.level2, config.fatigue.speed_thres, config.fatigue.rec_time, 
					config.fatigue.snap_num, config.fatigue.snap_interval);
	}

	pTmpNode = pDSMAlarmNode->FirstChildElement(XML_ALARM_TELEPHONE)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.telephone);
		DBG_LEVEL_6("telephone Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
					config.telephone.level1, config.telephone.level2, config.telephone.speed_thres, 
					config.telephone.rec_time, config.telephone.snap_num, config.telephone.snap_interval);
	}

	pTmpNode = pDSMAlarmNode->FirstChildElement(XML_ALARM_SMOKING)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.smoking);
		DBG_LEVEL_6("smoking Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
					config.smoking.level1, config.smoking.level2, config.smoking.speed_thres, 
					config.smoking.rec_time, config.smoking.snap_num, config.smoking.snap_interval);
	}

	pTmpNode = pDSMAlarmNode->FirstChildElement(XML_ALARM_DISTRACTION)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.distraction);
		DBG_LEVEL_6("distraction Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
					config.distraction.level1, config.distraction.level2, config.distraction.speed_thres, 
					config.distraction.rec_time, config.distraction.snap_num, config.distraction.snap_interval);
	}

	pTmpNode = pDSMAlarmNode->FirstChildElement(XML_ALARM_ABNORMAL)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.abnormal);
		DBG_LEVEL_6("abnormal Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
					config.abnormal.level1, config.abnormal.level2, config.abnormal.speed_thres, 
					config.abnormal.rec_time, config.abnormal.snap_num, config.abnormal.snap_interval);
	}

#if 1
	pTmpNode = pDSMAlarmNode->FirstChildElement(XML_ALARM_ONEHAND_SW)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.onehand_sw);
		DBG_LEVEL_6("one hand Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
		config.abnormal.level1, config.abnormal.level2, config.abnormal.speed_thres, config.abnormal.rec_time, 
		config.abnormal.snap_num, config.abnormal.snap_interval);
	}

	pTmpNode = pDSMAlarmNode->FirstChildElement(XML_ALARM_HANDSOFF_SW)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.handsoff_sw);
		DBG_LEVEL_6("hands off Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
					config.abnormal.level1, config.abnormal.level2, config.abnormal.speed_thres, config.abnormal.rec_time, 
					config.abnormal.snap_num, config.abnormal.snap_interval);
	}

	pTmpNode = pDSMAlarmNode->FirstChildElement(XML_ALARM_SUNGCLASS)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.sungclass);
		DBG_LEVEL_6("sungclass Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
					config.abnormal.level1, config.abnormal.level2, config.abnormal.speed_thres, 
					config.abnormal.rec_time, config.abnormal.snap_num, config.abnormal.snap_interval);
	}

	pTmpNode = pDSMAlarmNode->FirstChildElement(XML_ALARM_BELT)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.belt);
		DBG_LEVEL_6("belt Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
					config.abnormal.level1, config.abnormal.level2, config.abnormal.speed_thres, config.abnormal.rec_time,
					config.abnormal.snap_num, config.abnormal.snap_interval);
	}
	
#endif

	tinyxml2::XMLElement *pElement = pDSMAlarmNode->FirstChildElement(XML_ALARM_IDENTIFY)->ToElement();
	if(pElement){
		config.driver_identify = atoi(pElement->GetText());
		DBG_LEVEL_6("driver_identify[%u]", config.driver_identify);
	}

	pElement = pDSMAlarmNode->FirstChildElement(XML_ALARM_SMOKE_INTERVAL)->ToElement();
	if(pElement){
		config.smoking_interval = atoi(pElement->GetText());
		DBG_LEVEL_6("smoking_interval[%u]", config.smoking_interval);
	}

	pElement = pDSMAlarmNode->FirstChildElement(XML_ALARM_PHONE_INTERVAL)->ToElement();
	if(pElement){
		config.phone_interval = atoi(pElement->GetText());
		DBG_LEVEL_6("phone_interval[%u]", config.phone_interval);
	}
}

void XMLParse::parse_adas_alarm_config(tinyxml2::XMLElement *pADASAlarmNode, ADAS_Config& config)
{
	tinyxml2::XMLElement *pTmpNode = pADASAlarmNode->FirstChildElement(XML_ALARM_LDW)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.ldw);
		DBG_LEVEL_6("ldw Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
					config.ldw.level1, config.ldw.level2, config.ldw.speed_thres, config.ldw.rec_time, 
					config.ldw.snap_num, config.ldw.snap_interval);
	}

	pTmpNode = pADASAlarmNode->FirstChildElement(XML_ALARM_FCW)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.fcw);
		DBG_LEVEL_6("fcw Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
					config.fcw.level1, config.fcw.level2, config.fcw.speed_thres, config.fcw.rec_time, 
					config.fcw.snap_num, config.fcw.snap_interval);
	}

	pTmpNode = pADASAlarmNode->FirstChildElement(XML_ALARM_HMW)->ToElement();
	if(pTmpNode){
		parse_alarm_config(pTmpNode, config.hmw);
		DBG_LEVEL_6("hmw Config: level1[%d]  level2[%d] speed_thres[%d] rec_time[%d] snap_num[%d] snap_intervalp[%d]", 
					config.hmw.level1, config.hmw.level2, config.hmw.speed_thres, config.hmw.rec_time, 
					config.hmw.snap_num, config.hmw.snap_interval);
	}
}

void XMLParse::parse_terminal_attr(tinyxml2::XMLElement *pTerminalNode, Terminal& terminal)
{
	tinyxml2::XMLElement *pElement = pTerminalNode->FirstChildElement(XML_TERMINAL_FLAG)->ToElement();
	if(pElement){
		terminal.nFlag = atoi(pElement->GetText());
		DBG_LEVEL_6("nFlag[%d]", terminal.nFlag);
	}

	pElement = pTerminalNode->FirstChildElement(XML_TERMINAL_PHONE)->ToElement();
	if(pElement){
		strncpy(terminal.phone_num, pElement->GetText(), sizeof(terminal.phone_num));
		DBG_LEVEL_6("phone_num[%s]", terminal.phone_num);
	}

	pElement = pTerminalNode->FirstChildElement(XML_TERMINAL_DEVID)->ToElement();
	if(pElement){
		strncpy(terminal.devID, pElement->GetText(), sizeof(terminal.devID));
		DBG_LEVEL_6("devID[%s]", terminal.devID);
	}

	pElement = pTerminalNode->FirstChildElement(XML_TERMINAL_DEVTYPE)->ToElement();
	if(pElement){
		strncpy(terminal.devType, pElement->GetText(), sizeof(terminal.devType));
		DBG_LEVEL_6("devType[%s]", terminal.devType);
	}
	pElement = pTerminalNode->FirstChildElement(XML_TERMINAL_DEVIMEI)->ToElement();
	if(pElement){
		strncpy(terminal.devIMEI, pElement->GetText(), sizeof(terminal.devIMEI));
		DBG_LEVEL_6("devIMEI[%s]", terminal.devIMEI);
	}
}

void XMLParse::parse_device_param(tinyxml2::XMLElement *pDevParamNode, Device_Param& dev_param)
{
	tinyxml2::XMLElement *pElement = pDevParamNode->FirstChildElement(XML_DEV_PARAM_PRE_SIM)->ToElement();
	if(pElement){
		strncpy(dev_param.pre_sim, pElement->GetText(), sizeof(dev_param.pre_sim));
		DBG_LEVEL_6("preSim[%s]", dev_param.pre_sim);
	}

	pElement = pDevParamNode->FirstChildElement(XML_DEV_PARAM_OVER_SPEED)->ToElement();
	if(pElement){
		dev_param.overspeed = atoi(pElement->GetText());
		DBG_LEVEL_6("overspeed[%d]", dev_param.overspeed);
	}

	pElement = pDevParamNode->FirstChildElement(XML_DEV_PARAM_VIDEOENABLE)->ToElement();
	if(pElement){
		dev_param.videoEnable = atoi(pElement->GetText());
		DBG_LEVEL_6("videoEnable[%d]", dev_param.videoEnable);
	}

	pElement = pDevParamNode->FirstChildElement(XML_DEV_PARAM_IMGENABLE)->ToElement();
	if(pElement){
		dev_param.imageEnable = atoi(pElement->GetText());
		DBG_LEVEL_6("imageEnable[%d]", dev_param.imageEnable);
	}
}

bool XMLParse::getInfo(const char *pXmlfile, Control_Info& stControlInfo, JT808_CONFIG& config, Terminal &stTerm, Device_Param& dev_param)
{
	bool ret = false;
	tinyxml2::XMLElement *pRoot = NULL;
	tinyxml2::XMLElement *pControlNode = NULL;
	tinyxml2::XMLElement *pTerminalNode = NULL;
	tinyxml2::XMLElement *pDevParamNode = NULL;
	tinyxml2::XMLElement *pPlatformNode = NULL;
	tinyxml2::XMLElement *pDsmNode = NULL;
	tinyxml2::XMLElement *pAdasNode = NULL;
	tinyxml2::XMLElement *pAlarmNode = NULL;
	tinyxml2::XMLElement *pElement = NULL;
	tinyxml2::XMLElement *pSessionNode = NULL;
	tinyxml2::XMLElement *pBasicNode = NULL;

	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument;
	if(pDoc != NULL) {
		ret = (pDoc->LoadFile(pXmlfile) == tinyxml2::XML_SUCCESS);
	}
 
	if(ret) {
		pRoot = pDoc->FirstChildElement(XML_ROOT);
		if(pRoot) {
			tinyxml2::XMLHandle rootHandle(pRoot);
			/*---------------------Control Attr------------------------------*/
			pControlNode = rootHandle.FirstChildElement(XML_SUBROOT_CONTROL).ToElement();
			if (pControlNode) {
				pElement = pControlNode->FirstChildElement(XML_ENABLE)->ToElement();
				if(pElement){
					stControlInfo.bEnable = atoi(pElement->GetText());
				}

				pElement = pControlNode->FirstChildElement(XML_GPS_REPORT_TIME)->ToElement();
				if(pElement){
					stControlInfo.nGPSReportTime = atoi(pElement->GetText());
				}
			}

			/*---------------------Terminal Attr------------------------------*/
			pTerminalNode = rootHandle.FirstChildElement(XML_SUBROOT_TERMINAL).ToElement();
			if(pTerminalNode){
				parse_terminal_attr(pTerminalNode, stTerm);
			}

			/*---------------------DeviceParam------------------------------*/
			pDevParamNode = rootHandle.FirstChildElement(XML_SUBROOT_DEV_PARAM).ToElement();
			if(pDevParamNode){
				parse_device_param(pDevParamNode, dev_param);
			}

			/*-----------------------Platform--------------------------*/
			pPlatformNode = rootHandle.FirstChildElement(XML_SUBROOT_PLATFORM).ToElement();
			if(pPlatformNode){
				/*---------------------Session--------------------------------*/
				pSessionNode = pPlatformNode->FirstChildElement(XML_PLATFORM_SESSION);
				if(pSessionNode){
					pElement = pSessionNode->FirstChildElement(XML_SESSION_SERVER_URL)->ToElement();
					if(pElement){
						strncpy(config.session.server_url, pElement->GetText(), sizeof(config.session.server_url));
						DBG_LEVEL_6("url[%s]", config.session.server_url);
					}

					pElement = pSessionNode->FirstChildElement(XML_SESSION_SERVER_PORT)->ToElement();
					if(pElement){
						config.session.port = atoi(pElement->GetText());
						DBG_LEVEL_6("url[%d]", config.session.port);
					}
				}

				/*--------------------------DSM-------------------------------------*/
				pDsmNode = pPlatformNode->FirstChildElement(XML_PLATFORM_DSM);
				if(pDsmNode){
					DBG_LEVEL_6("---------DSM Config-------");
					/*--------------------------Basic Config-------------------------------------*/
					pBasicNode = pDsmNode->FirstChildElement(XML_SUB_BASIC);
					if(pBasicNode){
						parse_basic_config(pBasicNode, config.dsm_config.basicConfig);
					}
					/*--------------------------Alarm Config-------------------------------------*/
					pAlarmNode = pDsmNode->FirstChildElement(XML_SUB_ALARM);
					if(pAlarmNode){
						parse_dsm_alarm_config(pAlarmNode, config.dsm_config);
					}
				}
				
				/*--------------------------ADAS-------------------------------------*/
				pAdasNode = pPlatformNode->FirstChildElement(XML_PLATFORM_ADAS);
				if(pAdasNode){
					DBG_LEVEL_6("--------ADAS Config-------");
					/*--------------------------Basic Config-------------------------------------*/
					pBasicNode = pAdasNode->FirstChildElement(XML_SUB_BASIC);
					if(pBasicNode){
						parse_basic_config(pBasicNode, config.adas_config.basicConfig);
					}
					/*--------------------------Alarm Config-------------------------------------*/
					pAlarmNode = pAdasNode->FirstChildElement(XML_SUB_ALARM);
					if(pAlarmNode){
						parse_adas_alarm_config(pAlarmNode, config.adas_config);
					}
				}
			}
			DBG_LEVEL_6("\n");
		}
    }

	if(pDoc){
		delete pDoc;
	}

    return ret;
}

int XMLParse::LoadTerminalParam(const char* pXmlfile, JT808_Config_S& cfg)
{
	bool ret = false;
	tinyxml2::XMLElement *pRoot = NULL;
	tinyxml2::XMLElement *pNetCommunication = NULL;
	tinyxml2::XMLElement *CenterServer = NULL;
	tinyxml2::XMLElement *ICServer = NULL;
	tinyxml2::XMLElement *GnssReport = NULL;
	tinyxml2::XMLElement *PhoneNumber = NULL;
	tinyxml2::XMLElement *AlarmMask = NULL;
	tinyxml2::XMLElement *AlarmSpeed = NULL;
	tinyxml2::XMLElement *FatigueAlarm = NULL;
	tinyxml2::XMLElement *ParkingAlarm = NULL;
	tinyxml2::XMLElement *CollisionAlarm = NULL;
	tinyxml2::XMLElement *RolloverAlarm = NULL;
	tinyxml2::XMLElement *SnapCtrl = NULL;
	tinyxml2::XMLElement *PicQuality = NULL;
	tinyxml2::XMLElement *PicColor = NULL;
	tinyxml2::XMLElement *VehicleInfo = NULL;
	tinyxml2::XMLElement *GnssDetail = NULL;
	tinyxml2::XMLElement *CanUpload = NULL;
	tinyxml2::XMLElement *VideoAttr = NULL;
	
	tinyxml2::XMLElement *pIntervalNode = NULL;
	tinyxml2::XMLElement *pDefaultNode = NULL;
	tinyxml2::XMLElement *pAlarmTNode = NULL;
	tinyxml2::XMLElement *pDormantNode = NULL;
	tinyxml2::XMLElement *pDepartureNode = NULL;
	tinyxml2::XMLElement *pSnapctlTimeNode = NULL;
	tinyxml2::XMLElement *pSnapctlMileageNode = NULL;
	tinyxml2::XMLElement *pCanUploadConfigNode = NULL;
	tinyxml2::XMLElement *pAiViConfNode = NULL;
	tinyxml2::XMLElement *pAiViChnNode = NULL;
	tinyxml2::XMLElement *pSingleChnNode = NULL;
	tinyxml2::XMLElement *pSpecialAlarmNode = NULL;
	tinyxml2::XMLElement *pVideoAlarmNode = NULL;
	tinyxml2::XMLElement *pDailyAwakeNode = NULL;
	
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument;

	tinyxml2::XMLElement *pElement = NULL;
	if(pDoc != NULL) {
		ret = (pDoc->LoadFile(pXmlfile) == tinyxml2::XML_SUCCESS);
	}
 
	if(ret) {
		pRoot = pDoc->FirstChildElement(XML_ROOT);
		if(pRoot) {
			tinyxml2::XMLHandle rootHandle(pRoot);
			/*---------------------NetCommunication Attr------------------------------*/
			pNetCommunication = rootHandle.FirstChildElement(XML_SUBROOT_COMMCFG).ToElement();	
			if (pNetCommunication) {
				pElement = pNetCommunication->FirstChildElement(XML_COMMCFG_HEARTBEAT)->ToElement();
				if(pElement){
					cfg.commCfg.heartbeat = atoi(pElement->GetText());
					DBG_LEVEL_6("heartbeat[%d]", cfg.commCfg.heartbeat);
				}
				pElement = pNetCommunication->FirstChildElement(XML_COMMCFG_TCP_TIMEOUT)->ToElement();
				if(pElement){
					cfg.commCfg.tcp.timeout = atoi(pElement->GetText());
					DBG_LEVEL_6("tcp.timeout[%d]", cfg.commCfg.tcp.timeout);
				}
				pElement = pNetCommunication->FirstChildElement(XML_COMMCFG_TCP_TRYTIMES)->ToElement();
				if(pElement){
					cfg.commCfg.tcp.trytimes = atoi(pElement->GetText());
					DBG_LEVEL_6("tcp.trytimes[%d]", cfg.commCfg.tcp.trytimes);
				}
				pElement = pNetCommunication->FirstChildElement(XML_COMMCFG_UDP_TIMEOUT)->ToElement();
				if(pElement){
					cfg.commCfg.udp.timeout = atoi(pElement->GetText());
					DBG_LEVEL_6("udp.timeout[%d]", cfg.commCfg.udp.trytimes);
				}
				pElement = pNetCommunication->FirstChildElement(XML_COMMCFG_UDP_TRYTIMES)->ToElement();
				if(pElement){
					cfg.commCfg.udp.trytimes = atoi(pElement->GetText());
					DBG_LEVEL_6("udp.trytimes[%d]", cfg.commCfg.udp.trytimes);
				}
				pElement = pNetCommunication->FirstChildElement(XML_COMMCFG_SMS_TIMEOUT)->ToElement();
				if(pElement){
					cfg.commCfg.sms.timeout = atoi(pElement->GetText());
					DBG_LEVEL_6("sms.timeout[%d]", cfg.commCfg.sms.timeout);
				}
				pElement = pNetCommunication->FirstChildElement(XML_COMMCFG_SMS_TRYTIMES)->ToElement();
				if(pElement){
					cfg.commCfg.sms.trytimes = atoi(pElement->GetText());
					DBG_LEVEL_6("sms.trytimes[%d]", cfg.commCfg.sms.trytimes);
				}
				pElement = pNetCommunication->FirstChildElement(XML_COMMCFG_AUTHEN_KEY)->ToElement();
				if(pElement){
					strncpy(cfg.commCfg.authenticationkey, pElement->GetText(), sizeof(cfg.commCfg.authenticationkey));
					DBG_LEVEL_6("authenticationkey[%s]", cfg.commCfg.authenticationkey);
				}
				pElement = pNetCommunication->FirstChildElement(XML_COMMCFG_PHONE_NUM)->ToElement();
				if(pElement){
					//×Ö·û´®×ªBCD
					char tmp[13] = {0};	//len:6 * 2 + 1
					sprintf(tmp, "%010u", atoi(pElement->GetText()));
					for(int i = 0; i < 12; i += 2){
						cfg.commCfg.phoneNumber[i / 2] = (tmp[i] << 4) | (tmp[i + 1] & 0x0F);
					}
//					DBG_LEVEL_6("phoneNumber[%s]", cfg.commCfg.phoneNumber);
//					printf("phoneNumber=");
//					for(int i = 0; i < 6; i += 1){
//						printf("%02x", cfg.commCfg.authenticationkey[i]);
//					}printf("\n");
				}
			}
			/*---------------------CenterServer Attr------------------------------*/
			CenterServer = rootHandle.FirstChildElement(XML_SUBROOT_CENTERSRVCFG).ToElement();	
			if (CenterServer) {
				pElement = CenterServer->FirstChildElement(XML_CENTERSRVCFG_MAIN_APN)->ToElement();
				if(pElement){
					strncpy(cfg.centerSrvCfg.main.apn, pElement->GetText(), sizeof(cfg.centerSrvCfg.main.apn));
					DBG_LEVEL_6("main.apn[%s]", cfg.centerSrvCfg.main.apn);
				}
				pElement = CenterServer->FirstChildElement(XML_CENTERSRVCFG_MAIN_USERNAME)->ToElement();
				if(pElement){
					strncpy(cfg.centerSrvCfg.main.username, pElement->GetText(), sizeof(cfg.centerSrvCfg.main.username));
					DBG_LEVEL_6("main.username[%s]", cfg.centerSrvCfg.main.username);
				}
				pElement = CenterServer->FirstChildElement(XML_CENTERSRVCFG_MAIN_PSW)->ToElement();
				if(pElement){
					strncpy(cfg.centerSrvCfg.main.psw, pElement->GetText(), sizeof(cfg.centerSrvCfg.main.psw));
					DBG_LEVEL_6("main.psw[%s]", cfg.centerSrvCfg.main.psw);
				}
				pElement = CenterServer->FirstChildElement(XML_CENTERSRVCFG_MAIN_ADDR)->ToElement();
				if(pElement){
					strncpy(cfg.centerSrvCfg.main.addr, pElement->GetText(), sizeof(cfg.centerSrvCfg.main.addr));
					DBG_LEVEL_6("main.addr[%s]", cfg.centerSrvCfg.main.addr);
				}
				pElement = CenterServer->FirstChildElement(XML_CENTERSRVCFG_BACK_APN)->ToElement();
				if(pElement){
					strncpy(cfg.centerSrvCfg.backup.apn, pElement->GetText(), sizeof(cfg.centerSrvCfg.backup.apn));
					DBG_LEVEL_6("backup.apn[%s]", cfg.centerSrvCfg.backup.apn);
				}
				pElement = CenterServer->FirstChildElement(XML_CENTERSRVCFG_BACK_USERNAME)->ToElement();
				if(pElement){
					strncpy(cfg.centerSrvCfg.backup.username, pElement->GetText(), sizeof(cfg.centerSrvCfg.backup.username));
					DBG_LEVEL_6("backup.username[%s]", cfg.centerSrvCfg.backup.username);
				}
				pElement = CenterServer->FirstChildElement(XML_CENTERSRVCFG_BACK_PSW)->ToElement();
				if(pElement){
					strncpy(cfg.centerSrvCfg.backup.psw, pElement->GetText(), sizeof(cfg.centerSrvCfg.backup.psw));
					DBG_LEVEL_6("backup.psw[%s]", cfg.centerSrvCfg.backup.psw);
				}
				pElement = CenterServer->FirstChildElement(XML_CENTERSRVCFG_BACK_ADDR)->ToElement();
				if(pElement){
					strncpy(cfg.centerSrvCfg.backup.addr, pElement->GetText(), sizeof(cfg.centerSrvCfg.backup.addr));
					DBG_LEVEL_6("backup.addr[%s]", cfg.centerSrvCfg.backup.addr);
				}
				pElement = CenterServer->FirstChildElement(XML_CENTERSRVCFG_TCP_PORT)->ToElement();
				if(pElement){
					cfg.centerSrvCfg.tcpport = atoi(pElement->GetText());
					DBG_LEVEL_6("main.psw[%d]", cfg.centerSrvCfg.tcpport);
				}
				pElement = CenterServer->FirstChildElement(XML_CENTERSRVCFG_UDP_PORT)->ToElement();
				if(pElement){
					cfg.centerSrvCfg.udpport = atoi(pElement->GetText());
					DBG_LEVEL_6("main.udpport[%d]", cfg.centerSrvCfg.udpport);
				}
			}

			/*---------------------ICServer Attr------------------------------*/
			ICServer = rootHandle.FirstChildElement(XML_SUBROOT_ICSERVER).ToElement();	
			if (ICServer) {
				pElement = ICServer->FirstChildElement(XML_ICSERVER_MAIN_SERV)->ToElement();
				if(pElement){
					strncpy(cfg.icSrvCfg.main, pElement->GetText(), sizeof(cfg.icSrvCfg.main));
					DBG_LEVEL_6("icSrvCfg.main[%s]", cfg.icSrvCfg.main);
				}
				pElement = ICServer->FirstChildElement(XML_ICSERVER_BACK_SERV)->ToElement();
				if(pElement){
					strncpy(cfg.icSrvCfg.backup, pElement->GetText(), sizeof(cfg.icSrvCfg.backup));
					DBG_LEVEL_6("icSrvCfg.backup[%s]", cfg.icSrvCfg.backup);
				}
				pElement = ICServer->FirstChildElement(XML_ICSERVER_TCP_PORT)->ToElement();
				if(pElement){
					cfg.icSrvCfg.tcpport = atoi(pElement->GetText());
					DBG_LEVEL_6("icSrvCfg.tcpport[%d]", cfg.icSrvCfg.tcpport);
				}
				pElement = ICServer->FirstChildElement(XML_ICSERVER_UDP_PORT)->ToElement();
				if(pElement){
					cfg.icSrvCfg.udpport = atoi(pElement->GetText());
					DBG_LEVEL_6("icSrvCfg.udpport[%d]", cfg.icSrvCfg.udpport);
				}
			}
			/*---------------------GnssReport Attr------------------------------*/
			GnssReport = rootHandle.FirstChildElement(XML_SUBROOT_GNSSREPORT).ToElement();	
			if (GnssReport) {
				pElement = GnssReport->FirstChildElement(XML_GNSSREPORT_TACTICS)->ToElement();
				if(pElement){
					cfg.reportCfg.tactics = atoi(pElement->GetText());
					DBG_LEVEL_6("reportCfg.tactics[%d]", cfg.reportCfg.tactics);
				}
				pElement = GnssReport->FirstChildElement(XML_GNSSREPORT_SCHEME)->ToElement();
				if(pElement){
					cfg.reportCfg.scheme = atoi(pElement->GetText());
					DBG_LEVEL_6("reportCfg.scheme[%d]", cfg.reportCfg.scheme);
				}

				/*--------------------------INTERVAL-------------------------------------*/
				pIntervalNode = GnssReport->FirstChildElement(XML_GNSSREPORT_INTERVAL);
				if(pIntervalNode){
					DBG_LEVEL_6("---------Interval Config-------");
					/*--------------------------default Config-------------------------------------*/
					pDefaultNode = pIntervalNode->FirstChildElement(XML_SUB_DEFAULT);
					if(pDefaultNode){
						DBG_LEVEL_6("--------- Default-------");
						parse_interval_config(pDefaultNode, cfg.reportCfg.interval[0]);
					}
					/*--------------------------alarm Config-------------------------------------*/
					pAlarmTNode = pIntervalNode->FirstChildElement(XML_SUB_ALARM_T);
					if(pAlarmTNode){
						DBG_LEVEL_6("--------- AlarmT-------");
						parse_interval_config(pAlarmTNode, cfg.reportCfg.interval[1]);
					}
					/*--------------------------dormant Config-------------------------------------*/
					pDormantNode = pIntervalNode->FirstChildElement(XML_SUB_DORMANT);
					if(pDormantNode){
						DBG_LEVEL_6("--------- Dormant-------");
						parse_interval_config(pDormantNode, cfg.reportCfg.interval[2]);
					}
					/*--------------------------driver_departure Config-------------------------------------*/
					pDepartureNode = pIntervalNode->FirstChildElement(XML_SUB_DRIVER_DEPARTURE);
					if(pDepartureNode){
						DBG_LEVEL_6("--------- Departure-------");
						parse_interval_config(pDepartureNode, cfg.reportCfg.interval[3]);
					}
				}
				pElement = GnssReport->FirstChildElement(XML_GNSSREPORT_ANGLE)->ToElement();
				if(pElement){
					cfg.reportCfg.angle = atoi(pElement->GetText());
					DBG_LEVEL_6("reportCfg.angle[%d]", cfg.reportCfg.angle);
				}
				pElement = GnssReport->FirstChildElement(XML_GNSSREPORT_RADIUS)->ToElement();
				if(pElement){
					cfg.reportCfg.radius = atoi(pElement->GetText());
					DBG_LEVEL_6("reportCfg.radius[%d]", cfg.reportCfg.radius);
				}
			}
			/*---------------------PhoneNumber Attr------------------------------*/
			PhoneNumber = rootHandle.FirstChildElement(XML_SUBROOT_PHONENUMBER).ToElement();	
			if (PhoneNumber) {
				pElement = PhoneNumber->FirstChildElement(XML_PHONENUMBER_MONITOR_NUM)->ToElement();
				if(pElement){
					strncpy(cfg.phoneNumCfg.monitornum, pElement->GetText(), sizeof(cfg.phoneNumCfg.monitornum));
					DBG_LEVEL_6("phoneNumCfg.monitornum[%s]", cfg.phoneNumCfg.monitornum);
				}
				pElement = PhoneNumber->FirstChildElement(XML_PHONENUMBER_SUPER_SMS_NUM)->ToElement();
				if(pElement){
					strncpy(cfg.phoneNumCfg.supersmsnum, pElement->GetText(), sizeof(cfg.phoneNumCfg.supersmsnum));
					DBG_LEVEL_6("phoneNumCfg.supersmsnum[%s]", cfg.phoneNumCfg.supersmsnum);
				}
				pElement = PhoneNumber->FirstChildElement(XML_PHONENUMBER_CENTER_NUM)->ToElement();
				if(pElement){
					strncpy(cfg.phoneNumCfg.centernum, pElement->GetText(), sizeof(cfg.phoneNumCfg.centernum));
					DBG_LEVEL_6("phoneNumCfg.centernum[%s]", cfg.phoneNumCfg.centernum);
				}
				pElement = PhoneNumber->FirstChildElement(XML_PHONENUMBER_RESET_NUM)->ToElement();
				if(pElement){
					strncpy(cfg.phoneNumCfg.resetnum, pElement->GetText(), sizeof(cfg.phoneNumCfg.resetnum));
					DBG_LEVEL_6("phoneNumCfg.resetnum[%s]", cfg.phoneNumCfg.resetnum);
				}
				pElement = PhoneNumber->FirstChildElement(XML_PHONENUMBER_REBOOT_NUM)->ToElement();
				if(pElement){
					strncpy(cfg.phoneNumCfg.rebootnum, pElement->GetText(), sizeof(cfg.phoneNumCfg.rebootnum));
					DBG_LEVEL_6("phoneNumCfg.rebootnum[%s]", cfg.phoneNumCfg.rebootnum);
				}
				pElement = PhoneNumber->FirstChildElement(XML_PHONENUMBER_CENTER_SMS_NUM)->ToElement();
				if(pElement){
					strncpy(cfg.phoneNumCfg.centersmsnum, pElement->GetText(), sizeof(cfg.phoneNumCfg.centersmsnum));
					DBG_LEVEL_6("phoneNumCfg.centersmsnum[%s]", cfg.phoneNumCfg.centersmsnum);
				}
				pElement = PhoneNumber->FirstChildElement(XML_PHONENUMBER_ALARM_SMS_NUM)->ToElement();
				if(pElement){
					strncpy(cfg.phoneNumCfg.alarmsmsnum, pElement->GetText(), sizeof(cfg.phoneNumCfg.alarmsmsnum));
					DBG_LEVEL_6("phoneNumCfg.alarmsmsnum[%s]", cfg.phoneNumCfg.alarmsmsnum);
				}
				pElement = PhoneNumber->FirstChildElement(XML_PHONENUMBER_ANSWER_WAY)->ToElement();
				if(pElement){
					cfg.phoneNumCfg.answerway = atoi(pElement->GetText());
					DBG_LEVEL_6("phoneNumCfg.answerway[%d]", cfg.phoneNumCfg.answerway);
				}
				pElement = PhoneNumber->FirstChildElement(XML_PHONENUMBER_CALL_TIME)->ToElement();
				if(pElement){
					cfg.phoneNumCfg.calltime = atoi(pElement->GetText());
					DBG_LEVEL_6("phoneNumCfg.calltime[%d]", cfg.phoneNumCfg.calltime);
				}
				pElement = PhoneNumber->FirstChildElement(XML_PHONENUMBER_MONTH_TIME)->ToElement();
				if(pElement){
					cfg.phoneNumCfg.monthtime = atoi(pElement->GetText());
					DBG_LEVEL_6("phoneNumCfg.monthtime[%d]", cfg.phoneNumCfg.monthtime);
				}
			}
			/*---------------------AlarmMask Attr------------------------------*/
			AlarmMask = rootHandle.FirstChildElement(XML_SUBROOT_ALARMMASK).ToElement();	
			if (AlarmMask) {
				pElement = AlarmMask->FirstChildElement(XML_ALARMMASK_ALARM_MASK)->ToElement();
				if(pElement){
					cfg.alarmMaskCfg.alarmmask = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmMaskCfg.alarmmask[%d]", cfg.alarmMaskCfg.alarmmask);
				}
				pElement = AlarmMask->FirstChildElement(XML_ALARMMASK_SMS)->ToElement();
				if(pElement){
					cfg.alarmMaskCfg.sms = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmMaskCfg.sms[%d]", cfg.alarmMaskCfg.sms);
				}
				pElement = AlarmMask->FirstChildElement(XML_ALARMMASK_SNAP)->ToElement();
				if(pElement){
					cfg.alarmMaskCfg.snap = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmMaskCfg.snap[%d]", cfg.alarmMaskCfg.snap);
				}
				pElement = AlarmMask->FirstChildElement(XML_ALARMMASK_SNAP_SAVE)->ToElement();
				if(pElement){
					cfg.alarmMaskCfg.snapsave = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmMaskCfg.snapsave[%d]", cfg.alarmMaskCfg.snapsave);
				}
				pElement = AlarmMask->FirstChildElement(XML_ALARMMASK_KEY_MASK)->ToElement();
				if(pElement){
					cfg.alarmMaskCfg.keymask = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmMaskCfg.keymask[%d]", cfg.alarmMaskCfg.keymask);
				}
			}
			/*---------------------AlarmSpeed Attr------------------------------*/
			AlarmSpeed = rootHandle.FirstChildElement(XML_SUBROOT_ALARMSPEED).ToElement();	
			if (AlarmSpeed) {
				pElement = AlarmSpeed->FirstChildElement(XML_ALARMSPEED_SPEED)->ToElement();
				if(pElement){
					cfg.alarmSpeed.speed = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmSpeed.speed[%d]", cfg.alarmSpeed.speed);
				}
				pElement = AlarmSpeed->FirstChildElement(XML_ALARMSPEED_TIME)->ToElement();
				if(pElement){
					cfg.alarmSpeed.time = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmSpeed.time[%d]", cfg.alarmSpeed.time);
				}
				pElement = AlarmSpeed->FirstChildElement(XML_ALARMSPEED_WARNING)->ToElement();
				if(pElement){
					cfg.alarmSpeed.warning = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmSpeed.warning[%d]", cfg.alarmSpeed.warning);
				}
			}
			/*---------------------FatigueAlarm Attr------------------------------*/
			FatigueAlarm = rootHandle.FirstChildElement(XML_SUBROOT_FATIGUEALARM).ToElement();	
			if (FatigueAlarm) {
				pElement = FatigueAlarm->FirstChildElement(XML_FATIGUEALARM_DRIVING_TIME)->ToElement();
				if(pElement){
					cfg.alarmFatigue.drivingtime = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmFatigue.drivingtime[%d]", cfg.alarmFatigue.drivingtime);
				}
				pElement = FatigueAlarm->FirstChildElement(XML_FATIGUEALARM_TOTAL_TIME)->ToElement();
				if(pElement){
					cfg.alarmFatigue.totaltime = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmFatigue.totaltime[%d]", cfg.alarmFatigue.totaltime);
				}
				pElement = FatigueAlarm->FirstChildElement(XML_FATIGUEALARM_REST_TIME)->ToElement();
				if(pElement){
					cfg.alarmFatigue.resttime = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmFatigue.resttime[%d]", cfg.alarmFatigue.resttime);
				}
				pElement = FatigueAlarm->FirstChildElement(XML_FATIGUEALARM_WARNING)->ToElement();
				if(pElement){
					cfg.alarmFatigue.warning = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmFatigue.warning[%d]", cfg.alarmFatigue.warning);
				}
			}
			/*---------------------ParkingAlarm Attr------------------------------*/
			ParkingAlarm = rootHandle.FirstChildElement(XML_SUBROOT_PARKINGALARM).ToElement();	
			if (ParkingAlarm) {
				pElement = ParkingAlarm->FirstChildElement(XML_PARKINGALARM_TIME)->ToElement();
				if(pElement){
					cfg.alarmParking.time = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmParking.time[%d]", cfg.alarmParking.time);
				}
			}
			/*---------------------CollisionAlarm Attr------------------------------*/
			CollisionAlarm = rootHandle.FirstChildElement(XML_SUBROOT_COLLISIONALARM).ToElement();	
			if (CollisionAlarm) {
				pElement = CollisionAlarm->FirstChildElement(XML_COLLISIONALARM_TIME)->ToElement();
				if(pElement){
					cfg.alarmCollision.time = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmCollision.time[%d]", cfg.alarmCollision.time);
				}
				pElement = CollisionAlarm->FirstChildElement(XML_COLLISIONALARM_ACCELERATION)->ToElement();
				if(pElement){
					cfg.alarmCollision.acceleration = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmCollision.acceleration[%d]", cfg.alarmCollision.acceleration);
				}
			}
			/*---------------------RolloverAlarm Attr------------------------------*/
			RolloverAlarm = rootHandle.FirstChildElement(XML_SUBROOT_ROLLOVERALARM).ToElement();	
			if (RolloverAlarm) {
				pElement = RolloverAlarm->FirstChildElement(XML_ROLLOVERALARM_ANGLE)->ToElement();
				if(pElement){
					cfg.alarmRollover.angle = atoi(pElement->GetText());
					DBG_LEVEL_6("alarmRollover.angle[%d]", cfg.alarmRollover.angle);
				}
			}
			/*---------------------SnapCtrl Attr------------------------------*/
			SnapCtrl = rootHandle.FirstChildElement(XML_SUBROOT_SNAPCTRL).ToElement();	
			if (SnapCtrl) {
				pElement = SnapCtrl->FirstChildElement(XML_SNAPCTRL_TIME_UNIT)->ToElement();
				if(pElement){
					cfg.snapCtrl.timeunit = atoi(pElement->GetText());
					DBG_LEVEL_6("snapCtrl.timeunit[%u]", cfg.snapCtrl.timeunit);
				}
				pElement = SnapCtrl->FirstChildElement(XML_SNAPCTRL_MILEAGE_UNIT)->ToElement();
				if(pElement){
					cfg.snapCtrl.mileageunit = atoi(pElement->GetText());
					DBG_LEVEL_6("snapCtrl.mileageunit[%u]", cfg.snapCtrl.mileageunit);
				}
				pElement = SnapCtrl->FirstChildElement(XML_SNAPCTRL_TIME_INTERVAL)->ToElement();
				if(pElement){
					cfg.snapCtrl.timeInterval = atoi(pElement->GetText());
					DBG_LEVEL_6("snapCtrl.timeInterval[%d]", cfg.snapCtrl.timeInterval);
				}
				pElement = SnapCtrl->FirstChildElement(XML_SNAPCTRL_MILEAGE_INTERVAL)->ToElement();
				if(pElement){
					cfg.snapCtrl.mileageInterval = atoi(pElement->GetText());
					DBG_LEVEL_6("snapCtrl.mileageInterval[%d]", cfg.snapCtrl.mileageInterval);
				}
				DBG_LEVEL_6("---------Snapctl Config-------");
				pSnapctlTimeNode = SnapCtrl->FirstChildElement(XML_SNAPCTRL_TIME);
				if(pSnapctlTimeNode){
					DBG_LEVEL_6("--------- time-------");
					parse_snapctl_time_config(pSnapctlTimeNode, cfg.snapCtrl.time);
				}
				pSnapctlMileageNode = SnapCtrl->FirstChildElement(XML_SNAPCTRL_MILEAGE);
				if(pSnapctlMileageNode){
					DBG_LEVEL_6("--------- mileage-------");
					parse_snapctl_mileage_config(pSnapctlMileageNode, cfg.snapCtrl.mileage);
				}
			}
			/*---------------------PicQuality Attr------------------------------*/
			PicQuality = rootHandle.FirstChildElement(XML_SUBROOT_PICQUALITY).ToElement();	
			if (PicQuality) {
				pElement = PicQuality->FirstChildElement(XML_PICQUALITY_QUALITY)->ToElement();
				if(pElement){
					cfg.picQuality.quality = atoi(pElement->GetText());
					DBG_LEVEL_6("picQuality.quality[%d]", cfg.picQuality.quality);
				}
			}
			/*---------------------PicColor Attr------------------------------*/
			PicColor = rootHandle.FirstChildElement(XML_SUBROOT_PICCOLOR).ToElement();	
			if (PicQuality) {
				pElement = PicColor->FirstChildElement(XML_PICCOLOR_LUMINANCE)->ToElement();
				if(pElement){
					cfg.picColor.luminance = atoi(pElement->GetText());
					DBG_LEVEL_6("picColor.luminance[%d]", cfg.picColor.luminance);
				}
				pElement = PicColor->FirstChildElement(XML_PICCOLOR_CONTRAST)->ToElement();
				if(pElement){
					cfg.picColor.contrast = atoi(pElement->GetText());
					DBG_LEVEL_6("picColor.contrast[%d]", cfg.picColor.contrast);
				}
				pElement = PicColor->FirstChildElement(XML_PICCOLOR_SATURATION)->ToElement();
				if(pElement){
					cfg.picColor.saturation = atoi(pElement->GetText());
					DBG_LEVEL_6("picColor.saturation[%d]", cfg.picColor.saturation);
				}
				pElement = PicColor->FirstChildElement(XML_PICCOLOR_CHROMA)->ToElement();
				if(pElement){
					cfg.picColor.chroma = atoi(pElement->GetText());
					DBG_LEVEL_6("picColor.chroma[%d]", cfg.picColor.chroma);
				}
			}
			/*---------------------VehicleInfo Attr------------------------------*/
			VehicleInfo = rootHandle.FirstChildElement(XML_SUBROOT_VEHICLEINFO).ToElement();	
			if (VehicleInfo) {
				pElement = VehicleInfo->FirstChildElement(XML_VEHICLEINFO_MILEAGE)->ToElement();
				if(pElement){
					cfg.VehInfoCfg.mileage = atoi(pElement->GetText());
					DBG_LEVEL_6("VehInfoCfg.mileage[%d]", cfg.VehInfoCfg.mileage);
				}
				pElement = VehicleInfo->FirstChildElement(XML_VEHICLEINFO_PROVINCE_ID)->ToElement();
				if(pElement){
					cfg.VehInfoCfg.provinceid = atoi(pElement->GetText());
					DBG_LEVEL_6("VehInfoCfg.provinceid[%d]", cfg.VehInfoCfg.provinceid);
				}
				pElement = VehicleInfo->FirstChildElement(XML_VEHICLEINFO_CITY_ID)->ToElement();
				if(pElement){
					cfg.VehInfoCfg.cityid = atoi(pElement->GetText());
					DBG_LEVEL_6("VehInfoCfg.cityid[%d]", cfg.VehInfoCfg.cityid);
				}
				pElement = VehicleInfo->FirstChildElement(XML_VEHICLEINFO_LICENSE)->ToElement();
				if(pElement){
					strncpy(cfg.VehInfoCfg.license, pElement->GetText(), sizeof(cfg.VehInfoCfg.license));
					DBG_LEVEL_6("VehInfoCfg.license[%s]", cfg.VehInfoCfg.license);
				}
				pElement = VehicleInfo->FirstChildElement(XML_VEHICLEINFO_PLATE_COLOR)->ToElement();
				if(pElement){
					cfg.VehInfoCfg.platecolor = atoi(pElement->GetText());
					DBG_LEVEL_6("VehInfoCfg.platecolor[%u]", cfg.VehInfoCfg.platecolor);
				}
				pElement = VehicleInfo->FirstChildElement(XML_VEHICLEINFO_COEFFICIENT)->ToElement();
				if(pElement){
					cfg.VehInfoCfg.coefficient = atoi(pElement->GetText());
					DBG_LEVEL_6("VehInfoCfg.coefficient[%d]", cfg.VehInfoCfg.coefficient);
				}
				pElement = VehicleInfo->FirstChildElement(XML_VEHICLEINFO_VIN)->ToElement();
				if(pElement){
					strncpy(cfg.VehInfoCfg.vin, pElement->GetText(), sizeof(cfg.VehInfoCfg.vin));
					DBG_LEVEL_6("VehInfoCfg.vin[%s]", cfg.VehInfoCfg.vin);
				}
				pElement = VehicleInfo->FirstChildElement(XML_VEHICLEINFO_VEHICLE_TYPE)->ToElement();
				if(pElement){
					strncpy(cfg.VehInfoCfg.vehicletype, pElement->GetText(), sizeof(cfg.VehInfoCfg.vehicletype));
					DBG_LEVEL_6("VehInfoCfg.vehicletype[%s]", cfg.VehInfoCfg.vehicletype);
				}
				pElement = VehicleInfo->FirstChildElement(XML_VEHICLEINFO_NVEHICLE_TYPE)->ToElement();
				if(pElement){
					cfg.VehInfoCfg.nVehicletype = atoi(pElement->GetText());
					DBG_LEVEL_6("VehInfoCfg.nVehicletype[%d]", cfg.VehInfoCfg.nVehicletype);
				}
			}
			/*---------------------GnssDetail Attr------------------------------*/
			GnssDetail = rootHandle.FirstChildElement(XML_SUBROOT_GNSSDETAIL).ToElement();	
			if (GnssDetail) {
				pElement = GnssDetail->FirstChildElement(XML_GNSSDETAIL_ENABLE)->ToElement();
				if(pElement){
					cfg.gnssDetail.enable = atoi(pElement->GetText());
					DBG_LEVEL_6("gnssDetail.enable[%02x]", cfg.gnssDetail.enable);
				}
				pElement = GnssDetail->FirstChildElement(XML_GNSSDETAIL_BAUDTATE)->ToElement();
				if(pElement){
					cfg.gnssDetail.baudtate = atoi(pElement->GetText());
					DBG_LEVEL_6("gnssDetail.baudtate[%02x]", cfg.gnssDetail.baudtate);
				}
				pElement = GnssDetail->FirstChildElement(XML_GNSSDETAIL_OUTPUT_FREQ)->ToElement();
				if(pElement){
					cfg.gnssDetail.outputFreq = atoi(pElement->GetText());
					DBG_LEVEL_6("gnssDetail.outputFreq[%02x]", cfg.gnssDetail.outputFreq);
				}
				pElement = GnssDetail->FirstChildElement(XML_GNSSDETAIL_UPLOAD_MODE)->ToElement();
				if(pElement){
					cfg.gnssDetail.uploadmode = atoi(pElement->GetText());
					DBG_LEVEL_6("gnssDetail.uploadmode[%02x]", cfg.gnssDetail.uploadmode);
				}
				pElement = GnssDetail->FirstChildElement(XML_GNSSDETAIL_SAMPLE_FREQ)->ToElement();
				if(pElement){
					cfg.gnssDetail.sampleFreq = atoi(pElement->GetText());
					DBG_LEVEL_6("gnssDetail.sampleFreq[%d]", cfg.gnssDetail.sampleFreq);
				}
				pElement = GnssDetail->FirstChildElement(XML_GNSSDETAIL_UPLOAD_SET)->ToElement();
				if(pElement){
					cfg.gnssDetail.uploadset = atoi(pElement->GetText());
					DBG_LEVEL_6("gnssDetail.uploadset[%d]", cfg.gnssDetail.uploadset);
				}
			}
			/*---------------------CanUpload Attr------------------------------*/
			CanUpload = rootHandle.FirstChildElement(XML_SUBROOT_CANUPLOAD).ToElement();	
			if (CanUpload) {
				pElement = CanUpload->FirstChildElement(XML_CANUPLOAD_SAMPLE1)->ToElement();
				if(pElement){
					cfg.canbusUpload.sample[0] = atoi(pElement->GetText());
					DBG_LEVEL_6("canbusUpload.sample1[%d]", cfg.canbusUpload.sample[0]);
				}
				pElement = CanUpload->FirstChildElement(XML_CANUPLOAD_SAMPLE2)->ToElement();
				if(pElement){
					cfg.canbusUpload.sample[1] = atoi(pElement->GetText());
					DBG_LEVEL_6("canbusUpload.sample2[%d]", cfg.canbusUpload.sample[1]);
				}
				pElement = CanUpload->FirstChildElement(XML_CANUPLOAD_UPLOAD1)->ToElement();
				if(pElement){
					cfg.canbusUpload.upload[0] = atoi(pElement->GetText());
					DBG_LEVEL_6("canbusUpload.upload1[%d]", cfg.canbusUpload.upload[0]);
				}
				pElement = CanUpload->FirstChildElement(XML_CANUPLOAD_UPLOAD2)->ToElement();
				if(pElement){
					cfg.canbusUpload.upload[1] = atoi(pElement->GetText());
					DBG_LEVEL_6("canbusUpload.upload2[%d]", cfg.canbusUpload.upload[1]);
				}
				pCanUploadConfigNode = CanUpload->FirstChildElement(XML_CANUPLOAD_ALONE_SAMPLE)->ToElement();
				if(pCanUploadConfigNode){
					pElement = pCanUploadConfigNode->FirstChildElement(XML_ALONE_SAMPLE_SET)->ToElement();
					if(pElement){
						strncpy((char *)cfg.canbusUpload.aloneSample.set, pElement->GetText(), sizeof(cfg.canbusUpload.aloneSample.set));
						for(int index = 0; index < 8; index ++)
						{
							DBG_LEVEL_6("aloneSample.set[%02u]", cfg.canbusUpload.aloneSample.set[index]);
						}
					}
				}
			}
			/*---------------------VideoAttr Attr------------------------------*/
			VideoAttr = rootHandle.FirstChildElement(XML_SUBROOT_VIDEOATTR).ToElement();	
			DBG_LEVEL_6("---------VideoAttr Config-------");
			pAiViConfNode = VideoAttr->FirstChildElement(XML_VIDEOATTR_AI_VI_CONF);
			if(pAiViConfNode){
				DBG_LEVEL_6("--------- Ai_Vi_Conf-------");
				parse_videoattr_ai_vi_config(pAiViConfNode, cfg.VideoCfg.ai_vi_attr);
			}
			pAiViChnNode = VideoAttr->FirstChildElement(XML_VIDEOATTR_AI_VI_CHN);
			if(pAiViChnNode){
				DBG_LEVEL_6("--------- Ai_Vi_Chn-------");
				parse_videoattr_ai_vi_chn_config(pAiViChnNode, cfg.VideoCfg.ai_vi_chn);
			}
			pSingleChnNode = VideoAttr->FirstChildElement(XML_VIDEOATTR_SINGLE_CHN);
			if(pSingleChnNode){
				DBG_LEVEL_6("--------- Single_Chn-------");
				parse_videoattr_single_chn_config(pSingleChnNode, cfg.VideoCfg.single_chn_ctl);
			}
			pSpecialAlarmNode = VideoAttr->FirstChildElement(XML_VIDEOATTR_SPECIAL_ALARM);
			if(pSpecialAlarmNode){
				DBG_LEVEL_6("--------- Special_Alarm-------");
				parse_videoattr_special_alarm_config(pSpecialAlarmNode, cfg.VideoCfg.special_alarm);
			}
			pVideoAlarmNode = VideoAttr->FirstChildElement(XML_VIDEOATTR_VIDEO_ALARM);
			if(pVideoAlarmNode){
				DBG_LEVEL_6("--------- Video_Alarm-------");
				parse_videoattr_video_alarm_config(pVideoAlarmNode, cfg.VideoCfg.video_alarm);
			}
			pDailyAwakeNode = VideoAttr->FirstChildElement(XML_VIDEOATTR_DAILY_AWAKE);
			if(pDailyAwakeNode){
				DBG_LEVEL_6("--------- Daily_Awake-------");
				parse_videoattr_daily_awake_config(pDailyAwakeNode, cfg.VideoCfg.awake_attr);
			}
		}
	}
	if(pDoc){
		delete pDoc;
	}
	
	return 0;
}

int XMLParse::LoadDefaultParam(const char* config_path, JT808_Config_S* cfg)
{
	//NetCommunication
	cfg->commCfg.heartbeat			= 60;
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
	
//	saveTerminalParam(config_path, cfg);
	
	return 0;
}

static void InsertAlarmXml(tinyxml2::XMLElement *node, const AlarmConfig& stAlarmConfig, const char *strAlarmType)
{
	tinyxml2::XMLElement * pAlarmType = node->FirstChildElement(strAlarmType);
	if(pAlarmType == NULL)
	{
		printf("file %s, line %d:p%s is NULL, error code :%d\n", __FILE__, __LINE__, strAlarmType, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	printf("seting strAlarmType para |%s|\n", strAlarmType);
	tinyxml2::XMLElement* pLevel1Element = pAlarmType->FirstChildElement(XML_ALARM_LEVEL_ONE);
	if(stAlarmConfig.level1)
	{
		pLevel1Element->SetText("1");
	}else
	{
		pLevel1Element->SetText("0");
	}
	tinyxml2::XMLElement* pLevel2Element = pAlarmType->FirstChildElement(XML_ALARM_LEVEL_TWO);
	if(stAlarmConfig.level2)
	{
		pLevel2Element->SetText("1");
	}else
	{
		pLevel2Element->SetText("0");
	}
	tinyxml2::XMLElement* pSpeedThresElement = pAlarmType->FirstChildElement(XML_ALARM_SPEED_THRES);
	pSpeedThresElement->SetText(stAlarmConfig.speed_thres);
	tinyxml2::XMLElement* pRecTimeElement = pAlarmType->FirstChildElement(XML_ALARM_REC_TIME);
	pRecTimeElement->SetText(stAlarmConfig.rec_time);
	tinyxml2::XMLElement* pSnapNumElement = pAlarmType->FirstChildElement(XML_ALARM_SNAP_NUM);
	pSnapNumElement->SetText(stAlarmConfig.snap_num);
	tinyxml2::XMLElement* pSnapIntrevalElement = pAlarmType->FirstChildElement(XML_ALARM_SNAP_INTERVAL);
	pSnapIntrevalElement->SetText(stAlarmConfig.snap_interval);
	tinyxml2::XMLElement* pUpVideoElement = pAlarmType->FirstChildElement(XML_ALARM_UP_VIDEO);
	pUpVideoElement->SetText(stAlarmConfig.up_video);

}

static void InsertControlXml(tinyxml2::XMLNode *node, const Control_Info &stControlInfo)
{
	tinyxml2::XMLElement * pControlElement = node->FirstChildElement(XML_SUBROOT_CONTROL);
	if(pControlElement == NULL)
	{
		printf("file %s, line %d:pControlElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pEnableElement = pControlElement->FirstChildElement(XML_ENABLE);
	if (stControlInfo.bEnable)
	{
		pEnableElement->SetText(1);
	}else
	{
		pEnableElement->SetText(0);
	}
	tinyxml2::XMLElement* pGpsReportTimeElement = pControlElement->FirstChildElement(XML_GPS_REPORT_TIME);
	pGpsReportTimeElement->SetText(stControlInfo.nGPSReportTime);
}

static void InsertTerminalXml(tinyxml2::XMLNode *node, const Terminal &stTerminal)
{
	tinyxml2::XMLElement * pTerminalElement = node->FirstChildElement(XML_SUBROOT_TERMINAL);
	if(pTerminalElement == NULL)
	{
		printf("file %s, line %d:pTerminalElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pFlagElement = pTerminalElement->FirstChildElement(XML_TERMINAL_FLAG);
	pFlagElement->SetText(stTerminal.nFlag);
	tinyxml2::XMLElement* pPhoneElement = pTerminalElement->FirstChildElement(XML_TERMINAL_PHONE);
	pPhoneElement->SetText(stTerminal.phone_num);
	tinyxml2::XMLElement* pDevIDElement = pTerminalElement->FirstChildElement(XML_TERMINAL_DEVID);
	pDevIDElement->SetText(stTerminal.devID);
	tinyxml2::XMLElement* pDevTypeElement = pTerminalElement->FirstChildElement(XML_TERMINAL_DEVTYPE);
	pDevTypeElement->SetText(stTerminal.devType);
}

static void InsertDevParamXml(tinyxml2::XMLNode *node, const Device_Param &stDevParam)
{
	tinyxml2::XMLElement * pDevParamElement = node->FirstChildElement(XML_SUBROOT_DEV_PARAM);
	if(pDevParamElement == NULL)
	{
		printf("file %s, line %d:pDevParamElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pPreSimElement = pDevParamElement->FirstChildElement(XML_DEV_PARAM_PRE_SIM);
	pPreSimElement->SetText(stDevParam.pre_sim);
	tinyxml2::XMLElement* pOverSpeedElement = pDevParamElement->FirstChildElement(XML_DEV_PARAM_OVER_SPEED);
	pOverSpeedElement->SetText(stDevParam.overspeed);
	tinyxml2::XMLElement* pVideoEnableElement = pDevParamElement->FirstChildElement(XML_DEV_PARAM_VIDEOENABLE);
	pVideoEnableElement->SetText(stDevParam.videoEnable);
	tinyxml2::XMLElement* pImgEnableElement = pDevParamElement->FirstChildElement(XML_DEV_PARAM_IMGENABLE);
	pImgEnableElement->SetText(stDevParam.imageEnable);
}

static void InsertSessionXml(tinyxml2::XMLElement *node, const Session &stSession)
{
	tinyxml2::XMLElement * pSessionElement = node->FirstChildElement(XML_PLATFORM_SESSION);
	if(pSessionElement == NULL)
	{
		printf("file %s, line %d:pSessionElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pUrlElement = pSessionElement->FirstChildElement(XML_SESSION_SERVER_URL);
	pUrlElement->SetText(stSession.server_url);
	tinyxml2::XMLElement* pPortElement = pSessionElement->FirstChildElement(XML_SESSION_SERVER_PORT);
	pPortElement->SetText(stSession.port);
}

static void InsertAutoSnapXml(tinyxml2::XMLElement* node,const AutoSnap &stAutoSnap)
{
	tinyxml2::XMLElement * pAutoSnapElement = node->FirstChildElement(XML_BASIC_AUTO_SNAP);
	if(pAutoSnapElement == NULL)
	{
		printf("file %s, line %d:pAutoSnapElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pTacticsElement = pAutoSnapElement->FirstChildElement(XML_BASIC_TACTICS);
	pTacticsElement->SetText(stAutoSnap.tactics);
	tinyxml2::XMLElement* pIntervalTimeElement = pAutoSnapElement->FirstChildElement(XML_BASIC_INTERVAL_TIME);
	pIntervalTimeElement->SetText(stAutoSnap.interval_time);
	tinyxml2::XMLElement* pIntervalDisElement = pAutoSnapElement->FirstChildElement(XML_BASIC_INTERVAL_DIS);
	pIntervalDisElement->SetText(stAutoSnap.tactics);
	tinyxml2::XMLElement* pSnapNumElement = pAutoSnapElement->FirstChildElement(XML_BASIC_SNAP_NUM);
	pSnapNumElement->SetText(stAutoSnap.snap_num);
	tinyxml2::XMLElement* pSnapIntervalElement = pAutoSnapElement->FirstChildElement(XML_BASIC_SNAP_INTERVAL);
	pSnapIntervalElement->SetText(stAutoSnap.snap_interval);
}

static void InsertBasicXml(tinyxml2::XMLElement *node, const Basic_Config &stBasicConfig)
{
	tinyxml2::XMLElement * pBasicElement = node->FirstChildElement(XML_SUB_BASIC);
	if(pBasicElement == NULL)
	{
		printf("file %s, line %d:pBasicElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pSpeedThresElement = pBasicElement->FirstChildElement(XML_BASIC_SPEED_THRES);
	pSpeedThresElement->SetText(stBasicConfig.speed_thres);
	tinyxml2::XMLElement* pAlarmVolumeElement = pBasicElement->FirstChildElement(XML_BASIC_ALARM_VOLUME);
	pAlarmVolumeElement->SetText(stBasicConfig.alarm_volume);
	tinyxml2::XMLElement* pPhotoResElement = pBasicElement->FirstChildElement(XML_BASIC_PHOTO_RES);
	pPhotoResElement->SetText(stBasicConfig.photo_res);
	tinyxml2::XMLElement* pVideoResElement = pBasicElement->FirstChildElement(XML_BASIC_VIDEO_RES);
	pVideoResElement->SetText(stBasicConfig.video_res);
	InsertAutoSnapXml(pBasicElement, stBasicConfig.autosnap);
}
#if 1
static void InsertDSMXml(tinyxml2::XMLElement *node, const DSM_Config &stDSMConfig)
{
	tinyxml2::XMLElement * pDSMElement = node->FirstChildElement(XML_PLATFORM_DSM);
	if(pDSMElement == NULL)
	{
		printf("file %s, line %d:pDSMElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	InsertBasicXml(pDSMElement, stDSMConfig.basicConfig);
	tinyxml2::XMLElement * pDSMAlarmElement = pDSMElement->FirstChildElement(XML_SUB_ALARM);
	if(pDSMAlarmElement == NULL)
	{
		printf("file %s, line %d:pDSMAlarmElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	InsertAlarmXml(pDSMAlarmElement, stDSMConfig.fatigue, XML_ALARM_FATIGUE);
	InsertAlarmXml(pDSMAlarmElement, stDSMConfig.telephone, XML_ALARM_TELEPHONE);
	InsertAlarmXml(pDSMAlarmElement, stDSMConfig.smoking, XML_ALARM_SMOKING);
	InsertAlarmXml(pDSMAlarmElement, stDSMConfig.distraction, XML_ALARM_DISTRACTION);
	InsertAlarmXml(pDSMAlarmElement, stDSMConfig.abnormal, XML_ALARM_ABNORMAL);
	InsertAlarmXml(pDSMAlarmElement, stDSMConfig.onehand_sw, XML_ALARM_ONEHAND_SW);
	InsertAlarmXml(pDSMAlarmElement, stDSMConfig.handsoff_sw, XML_ALARM_HANDSOFF_SW);
	InsertAlarmXml(pDSMAlarmElement, stDSMConfig.sungclass, XML_ALARM_SUNGCLASS);
	InsertAlarmXml(pDSMAlarmElement, stDSMConfig.belt, XML_ALARM_BELT);
	tinyxml2::XMLElement* pDriverIdentifyElement = pDSMAlarmElement->FirstChildElement(XML_ALARM_IDENTIFY);
	pDriverIdentifyElement->SetText(stDSMConfig.driver_identify);
	tinyxml2::XMLElement* pSmokeIntervalElement = pDSMAlarmElement->FirstChildElement(XML_ALARM_SMOKE_INTERVAL);
	pSmokeIntervalElement->SetText(stDSMConfig.smoking_interval);
	tinyxml2::XMLElement* pPhoneIntervalElement = pDSMAlarmElement->FirstChildElement(XML_ALARM_PHONE_INTERVAL);
	pPhoneIntervalElement->SetText(stDSMConfig.phone_interval);
}

static void InsertADASXml(tinyxml2::XMLElement *node, const ADAS_Config &stADASConfig)
{
	tinyxml2::XMLElement * pADASElement = node->FirstChildElement(XML_PLATFORM_ADAS);
	if(pADASElement == NULL)
	{
		printf("file %s, line %d:pADASElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	InsertBasicXml(pADASElement, stADASConfig.basicConfig);
	tinyxml2::XMLElement * pADASAlarmElement = pADASElement->FirstChildElement(XML_SUB_ALARM);
	if(pADASAlarmElement == NULL)
	{
		printf("file %s, line %d:pADASAlarmElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	InsertAlarmXml(pADASAlarmElement, stADASConfig.ldw, XML_ALARM_LDW);
	InsertAlarmXml(pADASAlarmElement, stADASConfig.fcw, XML_ALARM_FCW);
	InsertAlarmXml(pADASAlarmElement, stADASConfig.hmw, XML_ALARM_HMW);
}

static void InsertPlatformXml(tinyxml2::XMLNode *node, const JT808_CONFIG &stJTConfig)
{
	tinyxml2::XMLElement * pPlatformElement = node->FirstChildElement(XML_SUBROOT_PLATFORM);
	if(pPlatformElement == NULL)
	{
		printf("file %s, line %d:pPlatformElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	InsertSessionXml(pPlatformElement, stJTConfig.session);
	InsertDSMXml(pPlatformElement, stJTConfig.dsm_config);
	InsertADASXml(pPlatformElement, stJTConfig.adas_config);
}
#endif
void XMLParse::SetMgrXml(const char *pXmlfile, const JTT808_mgr &stJtt808_mgr)
{
	tinyxml2::XMLDocument doc;
    int res=doc.LoadFile(pXmlfile);
    if(res!=0)
    {
        cout<<"load jtt808.xml failed"<<endl;
    }
	tinyxml2::XMLNode *pRoot = doc.FirstChildElement(XML_ROOT);
	if(pRoot == NULL)
	{
		printf("file %s, line %d:pRoot is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}else
	{
		InsertControlXml(pRoot, stJtt808_mgr.control_info);
		InsertTerminalXml(pRoot, stJtt808_mgr.terminal);
		InsertDevParamXml(pRoot, stJtt808_mgr.dev_param);
		InsertPlatformXml(pRoot, stJtt808_mgr.config);
	}
	doc.SaveFile(pXmlfile);
}

void SetTerXml(const char *pXmlfile, const JT808_Config_S *cfg)
{
}

bool XMLParse::Load_tts_param(const char *pXmlfile, TTSInfo *cfg)
{
	bool ret = false;
	tinyxml2::XMLElement *pRoot = NULL;
	tinyxml2::XMLElement *pKeyNode = NULL;
	tinyxml2::XMLElement *pCfgNode = NULL;
	
	tinyxml2::XMLElement *pElement = NULL;
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument;
	if(pDoc != NULL) {
		ret = (pDoc->LoadFile(pXmlfile) == tinyxml2::XML_SUCCESS);
	}
 
	if(ret) {
		pRoot = pDoc->FirstChildElement(XML_TTS_ROOT);
		if(pRoot) {
			tinyxml2::XMLHandle rootHandle(pRoot);
			/*---------------------Control Attr------------------------------*/
			pKeyNode = rootHandle.FirstChildElement(XML_TTS_KEY).ToElement();
			if (pKeyNode) {
				pElement = pKeyNode->FirstChildElement(XML_TTS_APP_ID)->ToElement();
				if(pElement){
					
					strncpy(cfg->app_id, pElement->GetText(), sizeof(cfg->api_key));
					DBG_LEVEL_6("api app_id:%s", cfg->app_id);
				}
				
				pElement = pKeyNode->FirstChildElement(XML_TTS_API_KEY)->ToElement();
				if(pElement){
					
					strncpy(cfg->api_key, pElement->GetText(), sizeof(cfg->api_key));
					DBG_LEVEL_6("api key:%s", cfg->api_key);
				}

				pElement = pKeyNode->FirstChildElement(XML_TTS_SECRET_KEY)->ToElement();
				if(pElement){
					
					strncpy(cfg->secrect_key, pElement->GetText(), sizeof(cfg->api_key));
					DBG_LEVEL_6("secrect_key:%s", cfg->secrect_key);
				}

				
			}

			pCfgNode = rootHandle.FirstChildElement(XML_TTS_CFG).ToElement();
			if (pCfgNode){
				pElement = pCfgNode->FirstChildElement(XML_TTS_SPD)->ToElement();
				if(pElement){
					strncpy(cfg->spd, pElement->GetText(), sizeof(cfg->spd));
					DBG_LEVEL_6("spd:%s", cfg->spd);
				}

				pElement = pCfgNode->FirstChildElement(XML_TTS_PIT)->ToElement();
				if(pElement){
					strncpy(cfg->pit, pElement->GetText(), sizeof(cfg->pit));
					DBG_LEVEL_6("pit:%s", cfg->pit);
				}

				pElement = pCfgNode->FirstChildElement(XML_TTS_VOL)->ToElement();
				if(pElement){
					strncpy(cfg->vol, pElement->GetText(), sizeof(cfg->vol));
					DBG_LEVEL_6("vol:%s", cfg->vol);
				}

				pElement = pCfgNode->FirstChildElement(XML_TTS_PER)->ToElement();
				if(pElement){
					strncpy(cfg->per, pElement->GetText(), sizeof(cfg->per));
					DBG_LEVEL_6("per:%s", cfg->per);
				}
			}
			DBG_LEVEL_6("\n");
		}
    }

	if(pDoc){
		delete pDoc;
	}

    return ret;
}


