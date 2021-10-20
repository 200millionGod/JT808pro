/*************************************************************
 * Copyright			: 
 * FileName				: handler_capture.cpp
 * Description			: handler_capture.cpp implement
 * Author		        : chas
 * Created on			: Jan 8, 2019
 *************************************************************/
#include "../inc/TRML_capture.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <list>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "../inc/TRML_alarm.h"
#include "../inc/TRML_jt808.h"
#include "../inc/debug_log.h"

static void set_capture_attr(pthread_t tid)
{
	struct sched_param param;

	memset(&param, 0, sizeof(param));
	param.sched_priority = 90;
	pthread_setschedparam(tid, SCHED_RR, &param);
}

long long int get_cur_pts(void)
{
	static long long int s_last = 0;
	static long long int s_cur_pts = 0;

	long long int pts;
	timeval tv;

	gettimeofday(&tv, NULL);
	pts = tv.tv_sec * 1000;
	pts += tv.tv_usec / 1000;

	if(pts > s_last)
	{
		s_cur_pts += pts - s_last;
	}
	s_last = pts;

	return s_cur_pts;
}

long long int get_image_pts(Capture_S &stCapture)
{
	return stCapture.pts + (stCapture.interval * (stCapture.index + 1));
}

static void *capture_pthread(void *param)
{
	unsigned int ts;
	long long int cur_pts;
	long long int img_pts;
	Capture_S stCapture;
	bool capFlag = false;

	while(g_mgr.g_capture_status)
	{
		pthread_mutex_lock(&g_mgr.g_capture_mutex);

		if(g_mgr.g_capture_list.size() > 0)
		{
			auto iter = g_mgr.g_capture_list.begin();
			stCapture = *iter;
			img_pts = get_image_pts(stCapture);
			cur_pts = get_cur_pts();

			if(cur_pts >= img_pts)
			{
				capFlag = true;
				g_mgr.g_capture_list.erase(iter);
			}
		}

		pthread_mutex_unlock(&g_mgr.g_capture_mutex);

		if(capFlag)
		{
			ts = stCapture.event_pts + (stCapture.index * stCapture.interval) / 1000;

//			dfw_capture_image_alarm(stCapture.event, ts, stCapture.index);

			DBG_LEVEL_6("event[%x] index[%d] number[%d] ts[%d]", stCapture.event, stCapture.index, stCapture.num, ts);

			capFlag = false;
		}

		usleep(20 * 1000);
	}

	return NULL;
}

int captureImagePthread_start(void)
{
	int ret = 0;
	pthread_t capture_tid;
	g_mgr.g_capture_status = 1;

	ret = pthread_create(&capture_tid, 0, capture_pthread, NULL);
	set_capture_attr(capture_tid);

	return ret;
}
