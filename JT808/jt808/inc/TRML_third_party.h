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
#include <vector>

//xjGPS
int getGpsInfo(struGPSData& gpsData);
//xjRil
int getRilInfo(JTT808_mgr& g_mgr);
//xjSnapshot
bool handle_snapshot_by_xj(int dev, unsigned short cmd, unsigned short period, unsigned int event, unsigned int *pts, char *filename);
//xjMutiStreamCatch
int mutiStreamCatch(Jt1078ReqStrem& req_av, Jt1078CtrlStrem& ctrl_av, int reqOrCtrl);

struct frame{
	unsigned int frameID;
	unsigned char *data;
	long long tick;
	int len;
};

extern vector <frame> FrontH264StreamList;
extern vector <frame> RearH264StreamList;
extern vector <frame> AudioStreamList;


