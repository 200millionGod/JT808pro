/*************************************************************
 * Copyright			: 
 * FileName				: TRML_video_ctrl.h
 * Description			: 实时音视频控制
 * Author		        :
 * Created on			: june 7, 2019
 *************************************************************/

#ifndef INC_TRML_VIDEO_CTRL_H_
#define INC_TRML_VIDEO_CTRL_H_

#include "../inc/TRML_util.h"

enum SDCARD_STATUS{
    SDCARD_STATUS_NORMAL = 0,   //正常
    SDCARD_STATUS_NO_EXIST,     //SD不存在
    SDCARD_STATUS_RW_FAULT,     //SD读写异常
    SDCARD_STATUS_LACK_OF_SPACE, //空间不足
};

void set_sd_status(unsigned int status);

/*************************************************************************
*Function:video_get_parameter
*Description:获取音视频属性
*Params in:
*Params out:
*Result:
*author:
*date:20190620
**************************************************************************/
void video_get_parameter_callback(void *data);
void video_req_stream_callback(void *data);
void video_stream_ctrl_callback(void *data);
void video_que_resource_callback(void *data, void *up_info);
void video_req_playback_callback(void *data, void *up_info);
void video_ctrl_playback_callback(void *data);
void video_upfile_callback(void *data);
void video_ctrl_upfile_callback(void *data);


#endif /* INC_TRML_VIDEO_CTRL_H_ */

