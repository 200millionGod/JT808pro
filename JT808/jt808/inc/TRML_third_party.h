/*************************************************************
 * Copyright			: 
 * FileName				: TRML_third_party.h
 * Description			: TRML_third_party.h
 * Author		        : 
 * Created on			: 20210918
 *************************************************************/
//#include "gps/gps_ext.h"
//#include "gps/gps_info.h"
#include "TRML_jt808.h"

//xjGPS
int getGpsInfo(struGPSData& gpsData);
//xjril
int getRilInfo(JTT808_mgr& g_mgr);
//xjsnapshot
bool handle_snapshot_by_xj(int dev, unsigned short cmd, unsigned short period, unsigned int event, unsigned int *pts, char *filename);
//xjVideo

