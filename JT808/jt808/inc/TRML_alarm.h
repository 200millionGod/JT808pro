/*************************************************************
 * Copyright			: 
 * FileName				: process_alarm.h
 * Description			: process_alarm.h implement
 * Author		        : chas
 * Created on			: Jan 7, 2019
 *************************************************************/

#ifndef INC_TRML_ALARM_H_
#define INC_TRML_ALARM_H_
#include "../inc/TRML_util.h"
int handle_alarm_test(unsigned int event, unsigned int pts, unsigned char dev);

int handle_alarm(unsigned int event, unsigned int pts, unsigned char dev);
bool handle_snapshot(int dev, unsigned int event, unsigned int *pts, char *filename);
int uploadAlarmPthread_start(void);
int faceAuthenEvent_start(void);
int handle_speed(int speed);
void overSpeedService(void);
int handle_face_auth_result(FaceCmpResult *result, int event);

#endif /* INC_TRML_ALARM_H_ */
