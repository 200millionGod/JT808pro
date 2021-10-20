/*************************************************************
 * Copyright			: 
 * FileName				: util.h
 * Description			: util.h implement
 * Author		        : chas
 * Created on			: Jan 9, 2019
 *************************************************************/

#ifndef INC_TRML_UTIL_H_
#define INC_TRML_UTIL_H_
#include <pthread.h>
#include "JT808App.h"
#include "TRML_jt808.h"
#include "./curl/curl.h"
#include "./iconv/iconv.h"

#define DEFAULT_DEVID_PATH	"./devid.txt"
//#define PHONENUM_PREFIX		"01397"
#define DEVTYPE_PREFIX		"0000"
#define DEFAULT_DEVTYPE		"E-322"
#define WAV_PATH			"/usr/local/share/sounds/"
#define WAV_SPEEDING       	"overspeed.wav"
#define WAV_SPEED_ATTEN		"attentionspeed.wav"

int setPthreadStackSize(pthread_attr_t *attr, int stacksize);
//int getGpsInfo(struGPSData& gpsData);
int get_devid(char *devid, int len);
int get_terminalInfo(char *devid, Terminal *term, int len, const char *pre_sim, int pre_sim_len);
int convert_GpsInfo(double value, char *degree, char *cent, long *sec);
void create_test_GpsInfo(struGPSData& gpsData);
int set_logfile(void);

#endif /* INC_TRML_UTIL_H_ */
