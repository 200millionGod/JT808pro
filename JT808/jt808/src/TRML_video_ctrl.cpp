#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <dirent.h>

#include "../inc/TRML_util.h"
#include "TRML_dbus.h"
#include "TRML_video_ctrl.h"
#include "../inc/debug_log.h"
#include "../inc/TRML_third_party.h"

#define SD_CARD_PATH      "/sdcard"
#define SUPPORT_MAX_CHN   2

#define BCD2BIN(v)	((((v)>>4)&0xf) * 10 + ((v)&0xf))
#define RECORD_LEN    3*60     //3 minutes

static unsigned int sd_status = SDCARD_STATUS_NO_EXIST;

time_t bcd2time(unsigned char bcd[6])
{
	struct tm stm;
	
	stm.tm_year = BCD2BIN(bcd[0]) + 2000 - 1900;
	stm.tm_mon  = BCD2BIN(bcd[1]) - 1;
	stm.tm_mday = BCD2BIN(bcd[2]);
	stm.tm_hour = BCD2BIN(bcd[3]);
	stm.tm_min  = BCD2BIN(bcd[4]);
	stm.tm_sec  = BCD2BIN(bcd[5]);
		
	return mktime(&stm);
}

void set_sd_status(unsigned int status)
{
	sd_status = status;
}

int switch_BCD_time(BCD *start_time, BCD *end_time, tm *start,tm *end)
{	
	start_time[0] = (((start->tm_year - 100) / 10) << 4) + (start->tm_year % 10);
	start_time[1] = (((start->tm_mon + 1) / 10) << 4) + ((start->tm_mon + 1) % 10);
	start_time[2] = ((start->tm_mday / 10) << 4) + (start->tm_mday % 10);
	start_time[3] = ((start->tm_hour / 10) << 4) + (start->tm_hour % 10);
	start_time[4] = ((start->tm_min / 10) << 4) + (start->tm_min % 10);
	start_time[5] = ((start->tm_sec / 10) << 4) + (start->tm_sec % 10);

	end_time[0] = (((end->tm_year - 100) / 10) << 4) + (end->tm_year % 10);
	end_time[1] = (((end->tm_mon + 1) / 10) << 4) + ((end->tm_mon + 1) % 10);
	end_time[2] = ((end->tm_mday / 10) << 4) + (end->tm_mday % 10);
	end_time[3] = ((end->tm_hour / 10) << 4) + (end->tm_hour % 10);
	end_time[4] = ((end->tm_min / 10) << 4) + (end->tm_min % 10);
	end_time[5] = ((end->tm_sec / 10) << 4) + (end->tm_sec % 10);

	return 0;
}

int filter_fn(const struct dirent *ent)
{
	if (ent->d_type != DT_REG){
		return 0;
	}

	if ( strstr(ent->d_name, ".mp4")){
		return 1;
	}
	
	return 0;
}

int get_record_file(VideoList *list, unsigned int start_t, unsigned int end_t, unsigned int chn)
{
	DIR              *dir;
    struct dirent *entry;
	char             *name;
	struct stat statbuf;
	statbuf.st_size = 0;
	char path[128] = {0}, buf[128];
	unsigned int year_s, hour_s, num = 0;
	tm start, *end;
	time_t end_pts,start_pts;
	
	dir = opendir(SD_CARD_PATH);
	if (!dir)
	{
		return 0;
	}

	while ((entry = readdir(dir)))
    {
        name = entry->d_name;       

        if ((name && *name == '.' && (!name[1] || (name[1] == '.' && !name[2]))))
            continue;

        if (entry->d_type == 4)//文件类型
        {
            //*dir
           
        }else if (8 == entry->d_type)
        {
        	
        	sprintf(path, "%s/%s", SD_CARD_PATH, name);
			stat(path, &statbuf);	//返回文件信息的数组

			sscanf(name, "%u_%u_%s", &year_s, &hour_s, buf);

			start.tm_sec  = hour_s % 100;
			hour_s        = hour_s / 100;
			start.tm_min  = hour_s % 100;
			start.tm_hour = hour_s / 100;

			start.tm_mday = year_s % 100;
			year_s        = year_s / 100;
			start.tm_mon  = year_s % 100 -1;
			start.tm_year = year_s / 100 - 1900;

			start_pts     = mktime(&start);
			end_pts       = start_pts + RECORD_LEN;

			if ((UINT)start_pts >= start_t && (UINT)end_pts <= end_t){	
				
				if (0 == chn)//all channel
	        	{
					if ( strstr(name, "adas")){
						list[num].chn = 1;
					} else if ( strstr(name, "dsm")) {
						list[num].chn = 2;
					}else{
						continue;
					}
				} else if (1 == chn){//adas
					if ( strstr(name, "adas")){
						list[num].chn = 1;
					}else{
						continue;
					}
				} else if (2 == chn){//dsm
					if ( strstr(name, "dsm")){
						list[num].chn = 2;
					}else{
						continue;
					}
				} else {
					DBG_LEVEL_6("unknown channal:%d ", chn);
					continue;
				}

				//DBG_LEVEL_6("name:%s ", name);
				end = localtime(&end_pts);
				switch_BCD_time(list[num].start_time, list[num].end_time, &start, end);
				list[num].file_size = statbuf.st_size;
				strncpy(list[num].file_name, name, strlen(name));
				num++;
				//DBG_LEVEL_6("num:%d ", num);
			}
        }
        
    }
        
    closedir(dir);
	
	return num;
}

void video_get_parameter_callback(void *data)
{
	VideoGetParam *ai_vi_conf = (VideoGetParam *)data;
	ai_vi_conf->audio_coding        = 0;
	ai_vi_conf->audio_chn_num       = 0;
	ai_vi_conf->audio_frequency     = 0;
	ai_vi_conf->audio_sampling_bits = 0;
	ai_vi_conf->audio_frame_len     = 0;
	ai_vi_conf->support_audio       = 0;
	ai_vi_conf->video_coding        = 98;
	ai_vi_conf->max_audio_num       = 0;
	ai_vi_conf->max_video_num       = 2;
}

void video_req_stream_callback(void *data)
{
	DBG_LEVEL_6("[enter]");
	VideoServerParamSet *server = (VideoServerParamSet *)data;
	if (server->logical_chn > SUPPORT_MAX_CHN){//0:adas; 1:dsm
		return;
	}
	
	Jt1078ReqStrem req_av;
	memset(&req_av, 0, sizeof(Jt1078ReqStrem));
	Jt1078CtrlStrem ctrl_av;
	memset(&req_av, 0, sizeof(Jt1078CtrlStrem));
	
	req_av.addr_len          = server->server_addr_len;
	memcpy(req_av.addr, server->server_addr, req_av.addr_len);
	req_av.tcp_port          = server->tcp_port;
	req_av.udp_port          = server->udp_port;
	req_av.chn               = server->logical_chn - 1;//通道是从1开始，1通道对应0通道
	req_av.data_type         = server->data_type;
	req_av.stream_type       = server->stream_type;
	memset(req_av.phone_num, 0, 20);
	memcpy(req_av.phone_num, g_mgr.terminal.phone_num, 15);
	int reqOrCtrl = 1;	//reqOrCtrl:1:直播请求 2.直播控制
	mutiStreamCatch(req_av, ctrl_av, reqOrCtrl);
	//send to dbus
//	jt1078_req_stream(&req_av);
}

void video_stream_ctrl_callback(void *data)
{
	DBG_LEVEL_6("[enter]");
	VideoTransCtrl *trans_ctrl = (VideoTransCtrl *)data;

	Jt1078ReqStrem req_av;
	memset(&req_av, 0, sizeof(Jt1078ReqStrem));
	Jt1078CtrlStrem ctrl_av;
	memset(&ctrl_av, 0, sizeof(Jt1078CtrlStrem));

	ctrl_av.chn               = trans_ctrl->logic_chn -1;
	ctrl_av.control_instrut   = trans_ctrl->control_instrut;
	ctrl_av.close_av          = trans_ctrl->close_av;	
	ctrl_av.switch_stream     = trans_ctrl->switch_stream;

	int reqOrCtrl = 2;	//reqOrCtrl:1:直播请求 2.直播控制
	mutiStreamCatch(req_av, ctrl_av, reqOrCtrl);
	//send to dbus
//	jt1078_ctrl_stream(&ctrl_av);
}

void video_que_resource_callback(void *data, void *up_list)
{
	DBG_LEVEL_6("[enter]");
	VideoResource      *src_info = (VideoResource *)data;
	VideoUploadResInfo *upload_list = (VideoUploadResInfo *)up_list;
	unsigned int       start_time, end_time, year_s, hour_s;
	long int           file_start_t = 0, file_end_t = 0;
	int                num = 0, i, list_num = 0;
	char               path[512] = {0}, buf[256];
	tm                 start, *end;
	struct dirent      **namelist = NULL;
	struct stat        statbuf;
	statbuf.st_size    = 0;

	//get source info
	DBG_LEVEL_6("sd status:%d ", sd_status);
	if ( (access( SD_CARD_PATH, F_OK ) == 0) && (SDCARD_STATUS_NORMAL == sd_status))
	{
		start_time            = bcd2time(src_info->start_time);
		end_time              = bcd2time(src_info->end_time);
		num = scandir(SD_CARD_PATH, &namelist, filter_fn, alphasort);
		
		upload_list->serial   = src_info->serial;
		if (num){
			upload_list->souce_info = new VideoResource[num];
			if (upload_list->souce_info == NULL){
				return ;
			}
		}
		
		for (i = 0; i < num; i++)
		{
			sprintf(path, "%s/%s", SD_CARD_PATH, namelist[i]->d_name);
			stat(path, &statbuf);
			
			sscanf(namelist[i]->d_name, "%u_%u_%s", &year_s, &hour_s, buf);
		
			start.tm_sec  = hour_s % 100;
			hour_s        = hour_s / 100;
			start.tm_min  = hour_s % 100;
			start.tm_hour = hour_s / 100;
			start.tm_mday = year_s % 100;
			year_s        = year_s / 100;
			start.tm_mon  = year_s % 100 -1;
			start.tm_year = year_s / 100 - 1900;
		
			file_start_t = mktime(&start);
			file_end_t   = file_start_t + RECORD_LEN;
			
			if (file_start_t >= start_time && file_end_t <= end_time)
			{	
				
				if (0 == src_info->logic_chn){//all channel
					if ( strstr(namelist[i]->d_name, "adas")){
						upload_list->souce_info[list_num].logic_chn = 2;
					} else if ( strstr(namelist[i]->d_name, "dsm")) {
						upload_list->souce_info[list_num].logic_chn = 1;
					}else{
						continue;
					}
				} else if (1 == src_info->logic_chn){//dsm
					if ( strstr(namelist[i]->d_name, "dsm")){
						upload_list->souce_info[list_num].logic_chn = 1;
					}else{
						continue;
					}
				} else if (2 == src_info->logic_chn){//adas
					if ( strstr(namelist[i]->d_name, "adas")){
						upload_list->souce_info[list_num].logic_chn = 2;
					}else{
						continue;
					}
				} 

				end = localtime(&file_end_t);
				switch_BCD_time(upload_list->souce_info[list_num].start_time, 
					upload_list->souce_info[list_num].end_time,&start, end);

				upload_list->souce_info[list_num].file_size   = statbuf.st_size;
				upload_list->souce_info[list_num].alarm_flag1 = src_info->alarm_flag1;
				upload_list->souce_info[list_num].alarm_flag2 = src_info->alarm_flag2;
				upload_list->souce_info[list_num].type		  = 2;
				upload_list->souce_info[list_num].stream_type = src_info->stream_type;
				upload_list->souce_info[list_num].store_type  = src_info->store_type;
				list_num++;

			}

			free(namelist[i]);
			namelist[i] = NULL;
		
		}

		upload_list->list_num = list_num;
		DBG_LEVEL_6("[num:%d]", upload_list->list_num);
		if (namelist){
			free(namelist);
		}
		
	} else {
		DBG_LEVEL_6("sd card invaild");
		upload_list->serial       = src_info->serial;
		upload_list->list_num     = 0;
	}

}

void video_req_playback_callback(void *data, void *up_info)
{
	DBG_LEVEL_6("[ enter]");
	VideoPlayback *play_back        = (VideoPlayback*)data;
	VideoUploadResInfo *upload_list = (VideoUploadResInfo *)up_info;

	if (!play_back || play_back->logic_chn > SUPPORT_MAX_CHN){//0:adas; 1:dsm
		DBG_LEVEL_6("[unknown channel] ");
		return;
	}

	Jt1078ReqPlay req_play;
	memset(&req_play, 0, sizeof(Jt1078ReqPlay));
	struct dirent     **namelist = NULL;
	unsigned int      start_time, end_time, year_s, hour_s;
	long int          file_start_t = 0, file_end_t = 0, num = 0, list_num = 0, i;
	char              path[512] = {0}, buf[256] = {0};
	tm                start, *end;
	struct stat       statbuf;
	statbuf.st_size   = 0;
	
	if ( (access( SD_CARD_PATH, F_OK ) == 0) && (SDCARD_STATUS_NORMAL == sd_status))
	{
		start_time            = bcd2time(play_back->start_time);
		end_time              = bcd2time(play_back->end_time);
		num = scandir(SD_CARD_PATH, &namelist, filter_fn, alphasort);
		
		upload_list->serial   = play_back->serial;
		if (num > 0)
		{
			upload_list->souce_info = new VideoResource[1];
			if (upload_list->souce_info == NULL){
				return ;
			}
		}
		
		for (i = 0; i < num; i++)
		{
			sprintf(path, "%s/%s", SD_CARD_PATH, namelist[i]->d_name);
			stat(path, &statbuf);
			
			sscanf(namelist[i]->d_name, "%u_%u_%s", &year_s, &hour_s, buf);
		
			start.tm_sec  = hour_s % 100;
			hour_s        = hour_s / 100;
			start.tm_min  = hour_s % 100;
			start.tm_hour = hour_s / 100;
			start.tm_mday = year_s % 100;
			year_s        = year_s / 100;
			start.tm_mon  = year_s % 100 -1;
			start.tm_year = year_s / 100 - 1900;
		
			file_start_t = mktime(&start);//video file start time
			file_end_t   = file_start_t + RECORD_LEN;//video file end time
			
			if (file_start_t >= start_time && file_end_t <= end_time)
			{	
				
				if (1 == play_back->logic_chn){//dsm
					if ( strstr(namelist[i]->d_name, "dsm")){
						upload_list->souce_info[list_num].logic_chn = 1;
					}else{
						continue;
					}
				} else if (2 == play_back->logic_chn){//adas
					if ( strstr(namelist[i]->d_name, "adas")){
						upload_list->souce_info[list_num].logic_chn = 2;
					}else{
						continue;
					}
				} 

				end = localtime(&file_end_t);
				switch_BCD_time(upload_list->souce_info[list_num].start_time, 
					upload_list->souce_info[list_num].end_time,&start, end);
				upload_list->souce_info[list_num].file_size      = statbuf.st_size;
				upload_list->souce_info[list_num].alarm_flag1    = 0;
				upload_list->souce_info[list_num].alarm_flag2    = 0;
				upload_list->souce_info[list_num].type		     = 2;
				upload_list->souce_info[list_num].stream_type    = play_back->stream_type;
				upload_list->souce_info[list_num].store_type     = play_back->store_type;
				list_num++;

			}

			free(namelist[i]);
			namelist[i] = NULL;
		
		}

		upload_list->list_num = list_num;
		DBG_LEVEL_6("[num:%d] ", upload_list->list_num);
		if (namelist){
			free(namelist);
		}

		req_play.addr_len          = play_back->addr_len;
		memcpy(req_play.addr, play_back->addr, req_play.addr_len);
		req_play.tcp_port          = play_back->tcp_port;
		req_play.udp_port          = play_back->udp_port;
		req_play.chn               = play_back->logic_chn -1;
		req_play.type              = play_back->type;
		req_play.stream_type       = play_back->stream_type;
		req_play.store_type        = play_back->store_type;
		req_play.pb_method         = play_back->pb_method;
		req_play.multiple          = play_back->multiple;
		req_play.start_ts          = start_time;
		req_play.end_ts            = end_time;
		memset(req_play.phone_num, 0, 20);
		memcpy(req_play.phone_num, g_mgr.terminal.phone_num, 12);
		//send dbus
//		jt1078_req_playback(&req_play);
		DBG_LEVEL_6(" ");
		
	}
	else {
		DBG_LEVEL_6(" sd card invaild");
		upload_list->serial       = play_back->serial;
		upload_list->list_num     = 0;
	}
	
}

void video_ctrl_playback_callback(void *data)
{
	DBG_LEVEL_6(" enter");
	VideoPlaybackControl *pb_ctrl = (VideoPlaybackControl *)data;	
	Jt1078CtrlPlay ctrl_play;
	memset(&ctrl_play, 0, sizeof(Jt1078CtrlPlay));

	ctrl_play.chn       = pb_ctrl->logic_chn -1;
	ctrl_play.control   = pb_ctrl->control;
	ctrl_play.multiple  = pb_ctrl->multiple;	
	ctrl_play.position  = bcd2time(pb_ctrl->pb_position);
	//send to dbus
//	jt1078_ctrl_playback(&ctrl_play);
}

void video_upfile_callback(void *data)
{
	DBG_LEVEL_6("enter ");
	VideoUpFile *file = (VideoUpFile *)data;

	unsigned int start_time, end_time, num;
	VideoUpLoad file_info;
	VideoList list[1024] = {0};
	memset(list, 0, 1024*sizeof(VideoList));
	
	if ( (access( SD_CARD_PATH, F_OK ) == 0) && (SDCARD_STATUS_NORMAL == sd_status))
	{
		start_time = bcd2time(file->start_time);
		end_time  = bcd2time(file->end_time);
		num = get_record_file(list, start_time, end_time, file->logic_chn);
		//DBG_LEVEL_6(" ");
		for (unsigned int i = 0; i < num; i++ )
		{
			memset(&file_info, 0, sizeof(VideoUpLoad));
			file_info.msg_serial = file->serial;
			file_info.total = num;
			file_info.up_status = START;
			file_info.num = i + 1;
			memcpy(file_info.addr, file->addr, file->addr_len);
			file_info.ftp_port = file->ftp_port;
			memcpy(file_info.user_name, file->user_name, file->name_len);
			memcpy(file_info.user_pwd, file->user_pwd, file->pwd_len);
			memcpy(file_info.target_path, file->path, file->path_len);
			memcpy(file_info.file_name, list[i].file_name, sizeof(list[i].file_name));
			memcpy(file_info.local_path, SD_CARD_PATH, sizeof(SD_CARD_PATH));

			pthread_mutex_lock(&g_mgr.g_videofile_mutex);
			g_mgr.g_videofile_list.push_back(file_info);
			pthread_mutex_unlock(&g_mgr.g_videofile_mutex);
		}
	}

}

void video_report_upfile_result(void *data)
{
	DBG_LEVEL_6(" enter ");
	VideoUpFileResult *result = (VideoUpFileResult *)data;

	JT808_App_videoReportUpResult(result);

	//send dbus
}

void video_ctrl_upfile_callback(void *data)
{
	DBG_LEVEL_6("enter ");
	VideoUpFileCtrl *ctrl = (VideoUpFileCtrl *)data;
	VideoUpLoad          up_file = {0};
	
	pthread_mutex_lock(&g_mgr.g_videofile_mutex);
	if(g_mgr.g_videofile_list.size() > 0)
	{
		auto iter = g_mgr.g_videofile_list.begin();
		up_file = *iter;

		if (PAUSE == ctrl->ctrl){
			up_file.up_status = PAUSE;
		} else if (CONTINUE == ctrl->ctrl) {
			up_file.up_status = START;
		} else if (CANCEL == ctrl->ctrl){
			g_mgr.g_videofile_list.erase(iter);
		}else{
			DBG_LEVEL_6("unknown type ");
		}
	}
	
	pthread_mutex_unlock(&g_mgr.g_videofile_mutex);
	DBG_LEVEL_6("end ");
	//send dbus
}


