/*************************************************************
 * Copyright			: 
 * FileName				: TRML_driver.h
 * Description			: Face recognition, face picture download etc.
 * Author		        :
 * Created on			: may 7, 2019
 *************************************************************/

#ifndef INC_TRML_DRIVER_H_
#define INC_TRML_DRIVER_H_

#include "../inc/TRML_util.h"

int UpOrDownloadPthread_start(void);
int download_driver_pic(FacePicPublish *facePicData);
int handle_face_compare_result(FaceCmpResult *result);
void face_info_query_callback(void *data);
void trigger_face_cmp_callback(void *data);


#endif /* INC_TRML_DRIVER_H_ */
