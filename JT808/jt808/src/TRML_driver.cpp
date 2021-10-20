/*************************************************************
 * Copyright			: 
 * FileName				: process_alarm.cpp
 * Description			: process_alarm.cpp implement
 * Author		        : chas
 * Created on			: Jan 7, 2019
 *************************************************************/

#include "../inc/TRML_alarm.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <list>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "../inc/debug_log.h"
#include "../inc/TRML_capture.h"
#include "../inc/TRML_jt808.h"
#include "../inc/TRML_util.h"
#include "../inc/TRML_TTS.h"
#include "TRML_dbus.h"
#include "JT808App.h"

const static char* PICTURE_PATH = 	"/opt/download";

int face_pic_download_reply(FacePicDownloadReply *result)
{
	JT808_APP_FacePic_Download_res(result);

	return 0;
}

void face_info_query_callback(void *data)
{
	FaceIDInfo *info = (FaceIDInfo *)data;
	info->face_num = 2;
	info->face_info[0].Id_len = 5;
	memcpy(info->face_info[0].Id, (char *)"test1", 5);
	info->face_info[1].Id_len = 5;
	memcpy(info->face_info[1].Id, (char *)"test2", 5);

	return;
	unsigned int num = 0, i = 0;
	char *id_list[2048] = {0}, *p;

	uint32_t ts = time(NULL);
	
	//query
	face_info_query_from_sql(&num, id_list, &ts);
	info->face_num = num;

	p = strtok(*id_list, ":");
	while(p)
	{
		DBG_LEVEL_6("p:%s ", p);
		info->face_info[i].Id_len = strlen(p);
		memcpy(info->face_info[i].Id, p, info->face_info[i].Id_len);
		i++;

		p = strtok(NULL, ":");
		
	}

	DBG_LEVEL_6("i:%d num:%d", i, num);
}

void trigger_face_cmp_callback(void *data)
{
//	unsigned int ts = time(NULL);
//	trigger_face_compare((char *)"platform_trigger", ts);
}

static size_t write_pic_pack(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);

    return written;
}

int download_picture(struct FacePic_s *pic_info)
{

	CURL *hCurl;
	FILE *hd_tgt = NULL;
	CURLcode res = CURLE_OK;	
	
	char file_name[256] = {0};
	//sprintf(file_name, "%s/%s", PICTURE_PATH, pic_info->pic_name);
	sprintf (file_name, "%s/%s_%ld_%02d.jpg", PICTURE_PATH, pic_info->driver_id, pic_info->cur_time, pic_info->pic_serial);
	/* open the file */
	if ((hd_tgt = fopen(file_name, "wb")) == NULL) {
		DBG_LEVEL_6("can not create file %s", file_name);
		return -1;
	}

	/* get a curl handle */
	hCurl = curl_easy_init();
	if(hCurl) 
	{
		curl_easy_setopt(hCurl, CURLOPT_NOSIGNAL, 1);
		/* connection timeout is 150 / 10 = 15 seconds */
		res = curl_easy_setopt(hCurl, CURLOPT_CONNECTTIMEOUT, 15);
		if (res != CURLE_OK)  
		{  
			DBG_LEVEL_6("set CURLOPT_FORBID_REUSE erro");
			goto Curl_easy_cleanup;
			 
		} 

		DBG_LEVEL_6("url:%s",  pic_info->pic_url);

		/* set URL to get here */
		res = curl_easy_setopt(hCurl, CURLOPT_URL, pic_info->pic_url);
		if (res != CURLE_OK)  
		{  
			DBG_LEVEL_6("set CURLOPT_FORBID_REUSE erro");
			
			goto Curl_easy_cleanup;
			 
		}

		/* disable progress meter, set to 0L to enable and disable debug output */
		res = curl_easy_setopt(hCurl, CURLOPT_NOPROGRESS, 1L);
		if (res != CURLE_OK)  
		{  
			DBG_LEVEL_6("set CURLOPT_FORBID_REUSE erro");
			goto Curl_easy_cleanup;
			 
		}

		curl_easy_setopt(hCurl, CURLOPT_READFUNCTION, NULL);
		
		DBG_LEVEL_6(" ");
		
		/* send all data to this function  */
		res = curl_easy_setopt(hCurl, CURLOPT_WRITEFUNCTION, write_pic_pack);
		if (res != CURLE_OK)  
		{  
			DBG_LEVEL_6("set CURLOPT_FORBID_REUSE erro");
			goto Curl_easy_cleanup;
			 
		}

		DBG_LEVEL_6(" ");
		
		/* write the page body to this file handle. CURLOPT_FILE is also known as
		CURLOPT_WRITEDATA*/
		res = curl_easy_setopt(hCurl, CURLOPT_FILE, hd_tgt);
		if (res != CURLE_OK)  
		{  
			DBG_LEVEL_6("set CURLOPT_FORBID_REUSE erro");
			goto Curl_easy_cleanup;
			 
		}
		
		DBG_LEVEL_6(" ");
		
		/* get page timeout is 30 seconds*/
		res = curl_easy_setopt(hCurl, CURLOPT_TIMEOUT, 300);
		if (res != CURLE_OK)  
		{  
			DBG_LEVEL_6("set CURLOPT_FORBID_REUSE erro");
			goto Curl_easy_cleanup;
			 
		}
		DBG_LEVEL_6(" ");
		
		/* get it! */
		res = curl_easy_perform(hCurl);
		if (res != CURLE_OK)
		{
			DBG_LEVEL_6("[CURL get]curl_easy_perform() failed: %s %d", curl_easy_strerror(res), res);
		}

		/* cleanup curl stuff */
	Curl_easy_cleanup:
		curl_easy_cleanup(hCurl);
	}
	else 
	{
	  DBG_LEVEL_6("curl_easy_init() failed!");
	}
	
 	if (hd_tgt != NULL)
 		fclose(hd_tgt); /* close the local file */
	
	DBG_LEVEL_6(" ");

	curl_global_cleanup();

	return res;
}

int download_driver_pic(FacePicPublish *facePicData)
{
	int i; int j;
	struct FacePic_s pic_info;

	for( i = 0; i < facePicData->list_num; i++)
	{
		time_t ts = time(NULL);
		for (j = 0; j < facePicData->face_info[i].pic_num; j++)
		{
			memset(&pic_info, 0, sizeof(struct FacePic_s));
			pic_info.MsgSerialNum	= facePicData->wSerialNumber;

			memcpy(pic_info.driver_id, facePicData->face_info[i].face_id, facePicData->face_info[i].face_id_len);
			DBG_LEVEL_6("dev id:[%s]", pic_info.driver_id);
			
			GBK2UTF8(facePicData->face_info[i].name, pic_info.driver_name, sizeof(pic_info.driver_name));
			DBG_LEVEL_6("dev name:[%s]", pic_info.driver_name);
			sprintf(pic_info.pic_url, "%s", facePicData->face_info[i].pic_info[j].pic_url);
			DBG_LEVEL_6("dev url:[%s]", pic_info.pic_url);
			
			pic_info.opsMode					=	facePicData->cmd_type;
			pic_info.pic_src					=	facePicData->face_info[i].pic_src;
			pic_info.pic_size					=	facePicData->face_info[i].pic_info[j].pic_size;
			pic_info.pic_type					=	facePicData->face_info[i].pic_info[j].pic_type;
			pic_info.up_left_coordi[0]			=	facePicData->face_info[i].pic_info[j].up_left_coordi[0];
			pic_info.up_left_coordi[1]			=	facePicData->face_info[i].pic_info[j].up_left_coordi[1];
			pic_info.down_right_coordi[0]		=	facePicData->face_info[i].pic_info[j].down_right_coordi[0];
			pic_info.down_right_coordi[1]		=	facePicData->face_info[i].pic_info[j].down_right_coordi[1];
			pic_info.left_eye_center_coordi[0]	=	facePicData->face_info[i].pic_info[j].left_eye_center_coordi[0];
			pic_info.left_eye_center_coordi[1]	=	facePicData->face_info[i].pic_info[j].left_eye_center_coordi[1];
			pic_info.right_eye_center_coordi[0]	=	facePicData->face_info[i].pic_info[j].right_eye_center_coordi[0];
			pic_info.right_eye_center_coordi[1]	=	facePicData->face_info[i].pic_info[j].right_eye_center_coordi[1];
			pic_info.nose_coordi[0]				=	facePicData->face_info[i].pic_info[j].nose_coordi[0];
			pic_info.nose_coordi[1]				=	facePicData->face_info[i].pic_info[j].nose_coordi[1];
			pic_info.left_mouth_coordi[0]		=	facePicData->face_info[i].pic_info[j].left_mouth_coordi[0];
			pic_info.left_mouth_coordi[1]		=	facePicData->face_info[i].pic_info[j].left_mouth_coordi[1];
			pic_info.right_mouth_coordi[0]		=	facePicData->face_info[i].pic_info[j].right_mouth_coordi[0];
			pic_info.right_mouth_coordi[1]		=	facePicData->face_info[i].pic_info[j].right_mouth_coordi[1];

			memcpy(pic_info.pic_name, facePicData->face_info[i].pic_info[j].pic_name, 
				facePicData->face_info[i].pic_info[j].pic_name_len);
			DBG_LEVEL_6(" url:%s ", pic_info.pic_url);

			pic_info.pic_serial = j + 1;
			pic_info.pic_total	= facePicData->face_info[i].pic_num;
			pic_info.cur_time	= ts;

			pthread_mutex_lock(&g_mgr.g_facepic_mutex);
			g_mgr.g_facepic_list.push_back(pic_info);
			pthread_mutex_unlock(&g_mgr.g_facepic_mutex);
		}
	}

	return 0;
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t retcode;
//    curl_off_t nread;

    /* in real-world cases, this would probably get this data differently
    as this fread() stuff is exactly what the library already would do
    by default internally */
    retcode = fread(ptr, size, nmemb, (FILE *)stream);

//    nread = (curl_off_t)retcode;

    //RETAILMSG(1, (TEXT("[CURL] Read %d bytes from file\n"), nread));

    return retcode;
}

int upload_file(VideoUpLoad *file, DWORD dwTimeout)
{
	CURL *curl;
	CURLcode res;
	FILE *hd_src;
	struct stat file_info;
	curl_off_t fsize;
	//  char usrpasswd[50] = {0};
	char remote_url[512] = { 0 };
	char userpwd[128]     = {0};

	//#define LOCAL_FILE      "/tmp/capture/d1_dfw_01_19700101_085859.jpg"
	//#define REMOTE_URL    "tftp://192.168.2.10:69/"
	//#define REMOTE_URL      "tftp://192.168.8.156:69/"
	//struct curl_slist *headerlist = NULL;
	//static const char buf_1 [] = "RNFR " UPLOAD_FILE_AS;
	//static const char buf_2 [] = "RNTO " RENAME_FILE_TO;

	char pLocalPath[512] = {0};
	sprintf(pLocalPath, "%s/%s", file->local_path, file->file_name);


	/* get the file size of the local file */
	if(stat(pLocalPath, &file_info)) {
		DBG_LEVEL_6("Couldn't open '%s': %s", pLocalPath, strerror(errno));
		return 1;
	}
	fsize = (curl_off_t)file_info.st_size;

	DBG_LEVEL_6("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.", fsize);

	/* get a FILE * of the same file */
	hd_src = fopen(pLocalPath, "rb");
	if (hd_src == NULL) {
		DBG_LEVEL_6("open %s erro!", pLocalPath);
		return -1;
	}

	/* In windows, this will init the winsock stuff */
	//  curl_global_init(CURL_GLOBAL_ALL);	//! pthread not safe function

	/* get a curl handle */
	curl = curl_easy_init();
	if(curl) {
	/* build a list of commands to pass to libcurl */
	//    headerlist = curl_slist_append(headerlist, buf_1);
	//    headerlist = curl_slist_append(headerlist, buf_2);

	/* we want to use our own read function */
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

	/* enable uploading */
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

	//	sprintf(remote_url, "%s%s", REMOTE_URL, pRemotePath);
	//sprintf(remote_url, "tftp://%s:69/%s", g_mgr.config_params[0].session.server_url, pRemotePath);

	sprintf(remote_url, "ftp://%s:%d%s/%s", file->addr, file->ftp_port, file->target_path, file->file_name);

	DBG_LEVEL_6("REMOTE_URL AND FILENAME[%s]",remote_url);

	if (file->user_name[0])
	{
		sprintf(userpwd, "%s:%s", file->user_name, file->user_pwd);
		DBG_LEVEL_6("REMOTE_URL AND userpwd[%s]",userpwd);
		curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
		/*if (res != CURLE_OK)  
		{  
			printf("set CURLOPT_FORBID_REUSE erro\n");
			goto Curl_easy_cleanup;
		}*/
	}

	/* specify target */
	curl_easy_setopt(curl, CURLOPT_URL, remote_url);

	//    sprintf(usrpasswd, "%s:%s", "chas", "123654");
	//	res = curl_easy_setopt(curl, CURLOPT_USERPWD, usrpasswd);

	curl_easy_setopt(curl, CURLOPT_TIMEOUT, dwTimeout);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 15);

	/* pass in that last of FTP commands to run after the transfer */
	//    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

	/* now specify which file to upload */
	curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

	/* Set the size of the file to upload (optional).  If you give a *_LARGE
	   option you MUST make sure that the type of the passed-in argument is a
	   curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
	   make sure that to pass in a type 'long' argument. */
	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
	                 (curl_off_t)fsize);

	curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);

	/* Now run off and do what you've been told! */
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK) {
	  fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}

	/* clean up the FTP commands list */
	//    curl_slist_free_all(headerlist);

	/* always cleanup */
	curl_easy_cleanup(curl);
	}
	else {
	  DBG_LEVEL_6("curl_easy_init() failed!");
	}
	if (hd_src != NULL) fclose(hd_src); /* close the local file */

	curl_global_cleanup();

	return res;
}

static void *up_or_down_pthread(void *param)
{
	struct FacePic_s pic_info;
	
	FacePicDownloadReply download = {0};
	DriverInfoToAIServer driver_info = {0};
	VideoUpLoad          up_file = {0};
	VideoUpFileResult    up_result = {0};
	bool bUpdate = false, bUpfile = false;
	int curl_ret = 0;

	while(g_mgr.g_upordown_status)
	{
		//下载人脸图片
		pthread_mutex_lock(&g_mgr.g_facepic_mutex);
		if(g_mgr.g_facepic_list.size() > 0)
		{
			auto iter = g_mgr.g_facepic_list.begin();
			pic_info = *iter;
			g_mgr.g_facepic_list.erase(iter);
			bUpdate = true;
		}
		pthread_mutex_unlock(&g_mgr.g_facepic_mutex);

		if(bUpdate)
		{
			curl_ret = download_picture(&pic_info);
			if (CURLE_OK != curl_ret ){
				DBG_LEVEL_6("download faild! curl_ret:%d", curl_ret);
			}else{
				memset(&download, 0, sizeof(FacePicDownloadReply));
				download.serial			= pic_info.MsgSerialNum;
				download.download_num	= pic_info.pic_total;
				download.cur_download	= pic_info.pic_serial;
				download.face_id_len	= strlen(pic_info.driver_id);
				memcpy(download.face_id, pic_info.driver_id, download.face_id_len);
				download.result	= 0;
				face_pic_download_reply(&download);

				{
					//notify face authentication module
					memset(&driver_info, 0, sizeof(DriverInfoToAIServer));
					memcpy(driver_info.driver_id, pic_info.driver_id, download.face_id_len);
					memcpy(driver_info.driver_name, pic_info.driver_name, strlen(pic_info.driver_name));
					memcpy(driver_info.pic_path, (char *)"/opt/download", strlen("/opt/download"));
					driver_info.picNo 				= pic_info.pic_total;
					driver_info.pic_src				= pic_info.pic_src;
					driver_info.picSerial			= pic_info.pic_serial;
					driver_info.picMode				= pic_info.opsMode;
					driver_info.face_left_posX		= pic_info.up_left_coordi[0];
					driver_info.face_left_posY		= pic_info.up_left_coordi[1];
					driver_info.face_right_posX		= pic_info.down_right_coordi[0];
					driver_info.face_right_posY		= pic_info.down_right_coordi[1];
					driver_info.left_eye_centerX	= pic_info.left_eye_center_coordi[0];
					driver_info.left_eye_centerY	= pic_info.left_eye_center_coordi[1];
					driver_info.right_eye_centerX	= pic_info.right_eye_center_coordi[0];
					driver_info.right_eye_centerY	= pic_info.right_eye_center_coordi[1];
					driver_info.nose_centerX		= pic_info.nose_coordi[0];
					driver_info.nose_centerY		= pic_info.nose_coordi[1];
					driver_info.left_mouth_posX		= pic_info.left_mouth_coordi[0];
					driver_info.left_mouth_posY		= pic_info.left_mouth_coordi[1];
					driver_info.right_mouth_posX	= pic_info.right_mouth_coordi[0];
					driver_info.right_mouth_posY	= pic_info.right_mouth_coordi[1];
					driver_info.ts					= pic_info.cur_time;
					
//					face_info_req_data_to_sql(&driver_info);
				}
			}
			bUpdate = false;
		}

		//上传录像资源文件
		pthread_mutex_lock(&g_mgr.g_videofile_mutex);
		if(g_mgr.g_videofile_list.size() > 0)
		{
			auto iter = g_mgr.g_videofile_list.begin();
			up_file = *iter;
			if (up_file.up_status == START){
				g_mgr.g_videofile_list.erase(iter);
				bUpfile = true;
			}
		}
		pthread_mutex_unlock(&g_mgr.g_videofile_mutex);

		if(bUpfile)
		{
			curl_ret = upload_file(&up_file, 200);
			if (CURLE_OK != curl_ret ){
				DBG_LEVEL_6("upload_file! curl_ret:%d", curl_ret);
			}

			if (up_file.total == up_file.num)
			{
				up_result.serial = up_file.msg_serial;
				up_result.result = 0;
				JT808_App_videoReportUpResult(&up_result);
			}

			bUpfile = false;
		}
		
		usleep(20 * 1000);
	}

	return NULL;
}

int UpOrDownloadPthread_start(void)
{
	int ret = 0;
	pthread_t upload_tid;
	g_mgr.g_upordown_status = 1;
	ret = pthread_create(&upload_tid, 0, up_or_down_pthread, NULL);
	
	return ret;
}
