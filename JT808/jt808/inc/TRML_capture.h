/*************************************************************
 * Copyright			: 
 * FileName				: handle_capture.h
 * Description			: handle_capture.h implement
 * Author		        : chas
 * Created on			: Jan 8, 2019
 *************************************************************/

#ifndef INC_TRML_CAPTURE_H_
#define INC_TRML_CAPTURE_H_

int captureImagePthread_start(void);
void captureImagePthread_stop(void);
long long int get_cur_pts(void);


#endif /* INC_TRML_CAPTURE_H_ */
