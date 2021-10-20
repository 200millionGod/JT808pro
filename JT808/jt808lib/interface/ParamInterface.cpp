#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <dirent.h>

#include <sstream>
#include "debug_log.h"
#include "ParamInterface.h"

#define TERMINAL_PARAMS_PATH "./terminal_param.xml"
#define SD_CARD_PATH		 "/data"

static char bcdtoascii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static char asciitobcd1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
static char asciitobcd2[6]  = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};


static int ASCtoBCD(char *bcd, char *asc, uint32 len)
{
	char c = 0;
    char index = 0;
    uint8 i = 0;  
    
    len >>= 1;    

    for(; i < len; i++) {
        //first BCD
        if(*asc >= 'A' && *asc <= 'F') {
            index = *asc - 'A'; 
            c  = asciitobcd2[index] << 4;
        } else if(*asc >= '0' && *asc <= '9') {
            index = *asc - '0';
            c  = asciitobcd1[index] << 4;
        }
        asc++;

        //second BCD
        if(*asc >= 'A' && *asc <= 'F') {
            index = *asc - 'A'; 
            c  |= asciitobcd2[index];
        } else if(*asc >= '0' && *asc <= '9') {
            index = *asc - '0';
            c  |= asciitobcd1[index];
        }
        asc++;

        *bcd++ = c;
    }

    return 0;
}


static LPCSTR BCD2LPCSTR(BCD p)
{
	string str = "";
	str += p;
 	return str.c_str();
 
}
#if 0
static const char* BCDArray2PCSTR(BCD p)
{
	string str = "";
	int len = strlen((char*)p);
	for(int i = 0; i < len; i ++)
	{
		str = str + BCD2LPCSTR(p[i]);
	}
	return str.c_str();
}
#endif
static LPCSTR WORD2LPCSTR(WORD p)
{
	stringstream KeyFrameInter;
	KeyFrameInter << p;
	string str = KeyFrameInter.str();
 	return str.c_str();
}

static LPCSTR DWORD2LPCSTR(DWORD p)
{
	stringstream KeyFrameInter;
	KeyFrameInter << p;
	string str = KeyFrameInter.str();
 	return str.c_str();
}

static void InsertNetCommunicationXml(tinyxml2::XMLNode *root, const Net_Communication_Attr &commCfg)
{
	tinyxml2::XMLElement * pNetCommunicationElement = root->FirstChildElement(XML_SUBROOT_COMMCFG);
	if(pNetCommunicationElement == NULL)
	{
		printf("file %s, line %d:pNetCommunicationElementis is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pHeartBeatElement = pNetCommunicationElement->FirstChildElement(XML_COMMCFG_HEARTBEAT);
	pHeartBeatElement->SetText(commCfg.heartbeat);
	tinyxml2::XMLElement* pTcpTimeoutElement = pNetCommunicationElement->FirstChildElement(XML_COMMCFG_TCP_TIMEOUT);
	pTcpTimeoutElement->SetText(commCfg.tcp.timeout);
	tinyxml2::XMLElement* pTcpTrytimesElement = pNetCommunicationElement->FirstChildElement(XML_COMMCFG_TCP_TRYTIMES);
	pTcpTrytimesElement->SetText(commCfg.tcp.trytimes);
	tinyxml2::XMLElement* pUdpTimeoutElement = pNetCommunicationElement->FirstChildElement(XML_COMMCFG_UDP_TIMEOUT);
	pUdpTimeoutElement->SetText(commCfg.udp.timeout);
	tinyxml2::XMLElement* pUdpTrytimesElement = pNetCommunicationElement->FirstChildElement(XML_COMMCFG_UDP_TRYTIMES);
	pUdpTrytimesElement->SetText(commCfg.udp.trytimes);
	tinyxml2::XMLElement* pSmsTimeoutElement = pNetCommunicationElement->FirstChildElement(XML_COMMCFG_SMS_TIMEOUT);
	pSmsTimeoutElement->SetText(commCfg.sms.timeout);
	tinyxml2::XMLElement* pSmsTrytimesElement = pNetCommunicationElement->FirstChildElement(XML_COMMCFG_SMS_TRYTIMES);
	pSmsTrytimesElement->SetText(commCfg.sms.trytimes);
	tinyxml2::XMLElement* pAuthenTicationKeyElement = pNetCommunicationElement->FirstChildElement(XML_COMMCFG_AUTHEN_KEY);
	if(strlen(commCfg.authenticationkey))
	{
		pAuthenTicationKeyElement->SetText(commCfg.authenticationkey);
	}
#if 0
	int len = strlen((char*)commCfg.phoneNumber);
	if(len != 0)
	{
		(char*)commCfg.phoneNumber;
		string phoneNum = "";
		for(int i = 0; i < len; i ++)
		{
			phoneNum += BCD2LPCSTR(commCfg.phoneNumber[i]);
		}
		tinyxml2::XMLElement* pPhoneNumElement = pNetCommunicationElement->FirstChildElement(XML_COMMCFG_PHONE_NUM);
		pPhoneNumElement->SetText(phoneNum.c_str());
	}
#endif
	if(strlen((char*)commCfg.phoneNumber))
	{
		tinyxml2::XMLElement* pPhoneNumElement = pNetCommunicationElement->FirstChildElement(XML_COMMCFG_PHONE_NUM);
		pPhoneNumElement->SetText((char*)commCfg.phoneNumber);
	}
}

static void InsertIntervalXml(tinyxml2::XMLElement *root, const Gnss_Report_Attr &reportCfg)
{
	tinyxml2::XMLElement * pIntervalElement = root->FirstChildElement(XML_GNSSREPORT_INTERVAL);
	if(pIntervalElement == NULL)
	{
		printf("file %s, line %d:pIntervalElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	//default
	tinyxml2::XMLElement* pDefaultElement = pIntervalElement->FirstChildElement(XML_SUB_DEFAULT);
	if(pDefaultElement == NULL)
	{
		printf("file %s, line %d:pDefaultElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pDefaultTimeElement = pDefaultElement->FirstChildElement(XML_DEFAULT_TIME);
	pDefaultTimeElement->SetText(reportCfg.interval[0].time);
	tinyxml2::XMLElement* pDefaultMileageElement = pDefaultElement->FirstChildElement(XML_DEFAULT_MILEAGE);
	pDefaultMileageElement->SetText(reportCfg.interval[0].mileage);
	//alarm
	tinyxml2::XMLElement* pAlarmElement = pIntervalElement->FirstChildElement(XML_SUB_ALARM_T);
	if(pAlarmElement == NULL)
	{
		printf("file %s, line %d:pAlarmElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pAlarmTimeElement = pAlarmElement->FirstChildElement(XML_ALARM_T_TIME);
	pAlarmTimeElement->SetText(reportCfg.interval[1].time);
	tinyxml2::XMLElement* pAlarmMileageElement = pAlarmElement->FirstChildElement(XML_ALARM_T_MILEAGE);
	pAlarmMileageElement->SetText(reportCfg.interval[1].mileage);
	//dormant
	tinyxml2::XMLElement* pDormantElement = pIntervalElement->FirstChildElement(XML_SUB_DORMANT);
	if(pDormantElement == NULL)
	{
		printf("file %s, line %d:pDormantElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pDormantTimeElement = pAlarmElement->FirstChildElement(XML_DORMANT_TIME);
	pDormantTimeElement->SetText(reportCfg.interval[2].time);
	tinyxml2::XMLElement* pDormantMileageElement = pAlarmElement->FirstChildElement(XML_DORMANT_MILEAGE);
	pDormantMileageElement->SetText(reportCfg.interval[2].mileage);
	//driver_departure
	tinyxml2::XMLElement* pDriverDepartureElement = pIntervalElement->FirstChildElement(XML_SUB_DORMANT);
	if(pDriverDepartureElement == NULL)
	{
		printf("file %s, line %d:pDriverDepartureElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pDriverDepartureTimeElement = pAlarmElement->FirstChildElement(XML_DRIVER_DEPARTURE_TIME);
	pDriverDepartureTimeElement->SetText(reportCfg.interval[3].time);
	tinyxml2::XMLElement* pDriverDepartureMileageElement = pAlarmElement->FirstChildElement(XML_DRIVER_DEPARTURE_MILEAGE);
	pDriverDepartureMileageElement->SetText(reportCfg.interval[3].mileage);
}

static void InsertCenterServerXml(tinyxml2::XMLNode *root, const Center_Server_Attr &centerSrvCfg)
{
	tinyxml2::XMLElement * pCenterSrvElement = root->FirstChildElement(XML_SUBROOT_CENTERSRVCFG);
	if(pCenterSrvElement == NULL)
	{
		printf("file %s, line %d:pCenterSrvElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pMainApnElement = pCenterSrvElement->FirstChildElement(XML_CENTERSRVCFG_MAIN_APN);
	if(strlen(centerSrvCfg.main.apn))
	{
		pMainApnElement->SetText(centerSrvCfg.main.apn);
	}
	tinyxml2::XMLElement* pMainUsernameElement = pCenterSrvElement->FirstChildElement(XML_CENTERSRVCFG_MAIN_USERNAME);
	if(strlen(centerSrvCfg.main.username))
	{
		pMainUsernameElement->SetText(centerSrvCfg.main.username);
	}
	tinyxml2::XMLElement* pMainPswElement = pCenterSrvElement->FirstChildElement(XML_CENTERSRVCFG_MAIN_PSW);
	if(NULL != pMainPswElement)
	{
		if(strlen(centerSrvCfg.main.psw))
		{
			pMainPswElement->SetText(centerSrvCfg.main.psw);
		}
	}else
	{
		printf("pMainPswElement:main_psw does not exist...\n");
	}
	tinyxml2::XMLElement* pMainAddrElement = pCenterSrvElement->FirstChildElement(XML_CENTERSRVCFG_MAIN_ADDR);
	if(strlen(centerSrvCfg.main.addr))
	{
		pMainAddrElement->SetText(centerSrvCfg.main.addr);
	}
	tinyxml2::XMLElement* pBackApnElement = pCenterSrvElement->FirstChildElement(XML_CENTERSRVCFG_BACK_APN);
	if(strlen(centerSrvCfg.backup.apn))
	{
		pBackApnElement->SetText(centerSrvCfg.backup.apn);
	}
	tinyxml2::XMLElement* pBackUsernameElement = pCenterSrvElement->FirstChildElement(XML_CENTERSRVCFG_BACK_USERNAME);
	if(strlen(centerSrvCfg.backup.username))
	{
		pBackUsernameElement->SetText(centerSrvCfg.backup.username);
	}
	tinyxml2::XMLElement* pBackPswElement = pCenterSrvElement->FirstChildElement(XML_CENTERSRVCFG_BACK_PSW);
	if(strlen(centerSrvCfg.backup.psw))
	{
		pBackPswElement->SetText(centerSrvCfg.backup.psw);
	}
	tinyxml2::XMLElement* pBackAddrElement = pCenterSrvElement->FirstChildElement(XML_CENTERSRVCFG_BACK_ADDR);
	if(strlen(centerSrvCfg.backup.addr))
	{
		pBackAddrElement->SetText(centerSrvCfg.backup.addr);
	}
	tinyxml2::XMLElement* pTcpPortElement = pCenterSrvElement->FirstChildElement(XML_CENTERSRVCFG_TCP_PORT);
	pTcpPortElement->SetText(centerSrvCfg.tcpport);
	tinyxml2::XMLElement* pUdpPortElement = pCenterSrvElement->FirstChildElement(XML_CENTERSRVCFG_UDP_PORT);
	pUdpPortElement->SetText(centerSrvCfg.udpport);
}

static void InsertICServerXml(tinyxml2::XMLNode *root, const IC_Server_Attr &icSrvCfg)
{
	tinyxml2::XMLElement * pIcServerElement = root->FirstChildElement(XML_SUBROOT_ICSERVER);
	if(pIcServerElement == NULL)
	{
		printf("file %s, line %d:pIcServerElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pMainSerElement = pIcServerElement->FirstChildElement(XML_ICSERVER_MAIN_SERV);
	if(strlen(icSrvCfg.main))
	{
		pMainSerElement->SetText(icSrvCfg.main);
	}
	tinyxml2::XMLElement* pBackSerElement = pIcServerElement->FirstChildElement(XML_ICSERVER_BACK_SERV);
	if(strlen(icSrvCfg.main))
	{
		pBackSerElement->SetText(icSrvCfg.backup);
	}
	tinyxml2::XMLElement* pTcpPortElement = pIcServerElement->FirstChildElement(XML_ICSERVER_TCP_PORT);
	if(NULL != pTcpPortElement)
	{
		pTcpPortElement->SetText(icSrvCfg.tcpport);
	}else
	{
		printf("pMainPswElement:tcp_port does not exist...\n");
	}
	tinyxml2::XMLElement* pUdpPortElement = pIcServerElement->FirstChildElement(XML_ICSERVER_UDP_PORT);
	if(NULL != pUdpPortElement)
	{
		pUdpPortElement->SetText(icSrvCfg.udpport);
	}else
	{
		printf("pUdpPortElement:udp_port does not exist...\n");
	}
}

static void InsertGnssReportXml(tinyxml2::XMLNode *root, const Gnss_Report_Attr &reportCfg)
{
	tinyxml2::XMLElement * pGnssReportElement = root->FirstChildElement(XML_SUBROOT_GNSSREPORT);
	if(pGnssReportElement == NULL)
	{
		printf("file %s, line %d:pGnssReportElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pTacticsElement = pGnssReportElement->FirstChildElement(XML_GNSSREPORT_TACTICS);
	pTacticsElement->SetText(reportCfg.tactics);
	tinyxml2::XMLElement* pSchemeElement = pGnssReportElement->FirstChildElement(XML_GNSSREPORT_SCHEME);
	pSchemeElement->SetText(reportCfg.scheme);

	InsertIntervalXml(pGnssReportElement, reportCfg);

	tinyxml2::XMLElement* pAngleElement = pGnssReportElement->FirstChildElement(XML_GNSSREPORT_ANGLE);
	pAngleElement->SetText(reportCfg.angle);
	tinyxml2::XMLElement* pRadiusElement = pGnssReportElement->FirstChildElement(XML_GNSSREPORT_SCHEME);
	pRadiusElement->SetText(reportCfg.radius);
}

static void InsertPhoneNumberXml(tinyxml2::XMLNode *root, const Phone_Number_Attr &phoneNumCfg)
{
	tinyxml2::XMLElement * pPhoneNumberElement = root->FirstChildElement(XML_SUBROOT_PHONENUMBER);
	if(pPhoneNumberElement == NULL)
	{
		printf("file %s, line %d:pPhoneNumberElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pMonitorNumElement = pPhoneNumberElement->FirstChildElement(XML_PHONENUMBER_MONITOR_NUM);
	if(strlen(phoneNumCfg.monitornum))
	{
		pMonitorNumElement->SetText(phoneNumCfg.monitornum);
	}
	tinyxml2::XMLElement* pSuperSmsNumElement = pPhoneNumberElement->FirstChildElement(XML_PHONENUMBER_SUPER_SMS_NUM);
	if(strlen(phoneNumCfg.supersmsnum))
	{
		pSuperSmsNumElement->SetText(phoneNumCfg.supersmsnum);
	}
	tinyxml2::XMLElement* pCenterNumElement = pPhoneNumberElement->FirstChildElement(XML_PHONENUMBER_CENTER_NUM);
	if(strlen(phoneNumCfg.centernum))
	{
		pCenterNumElement->SetText(phoneNumCfg.centernum);
	}
	tinyxml2::XMLElement* pResetNumElement = pPhoneNumberElement->FirstChildElement(XML_PHONENUMBER_RESET_NUM);
	if(strlen(phoneNumCfg.resetnum))
	{
		pResetNumElement->SetText(phoneNumCfg.resetnum);
	}
	tinyxml2::XMLElement* pRebootNumElement = pPhoneNumberElement->FirstChildElement(XML_PHONENUMBER_REBOOT_NUM);
	if(strlen(phoneNumCfg.rebootnum))
	{
		pRebootNumElement->SetText(phoneNumCfg.rebootnum);
	}
	tinyxml2::XMLElement* pCenterSmsNumElement = pPhoneNumberElement->FirstChildElement(XML_PHONENUMBER_CENTER_SMS_NUM);
	if(strlen(phoneNumCfg.centersmsnum))
	{
		pCenterSmsNumElement->SetText(phoneNumCfg.centersmsnum);
	}
	tinyxml2::XMLElement* pAlarmSmsNumElement = pPhoneNumberElement->FirstChildElement(XML_PHONENUMBER_ALARM_SMS_NUM);
	if(strlen(phoneNumCfg.alarmsmsnum))
	{
		pAlarmSmsNumElement->SetText(phoneNumCfg.alarmsmsnum);
	}
	tinyxml2::XMLElement* pAnswerWayElement = pPhoneNumberElement->FirstChildElement(XML_PHONENUMBER_ANSWER_WAY);
	pAnswerWayElement->SetText(phoneNumCfg.answerway);
	tinyxml2::XMLElement* pCallTimeElement = pPhoneNumberElement->FirstChildElement(XML_PHONENUMBER_CALL_TIME);
	pCallTimeElement->SetText(phoneNumCfg.calltime);
	tinyxml2::XMLElement* pMonthTimeElement = pPhoneNumberElement->FirstChildElement(XML_PHONENUMBER_MONTH_TIME);
	pMonthTimeElement->SetText(phoneNumCfg.monthtime);
}

static void InsertAlarmMaskXml(tinyxml2::XMLNode *root, const Alarm_Mask_Attr &alarmMaskCfg)
{
	tinyxml2::XMLElement * pAlarmMaskElement = root->FirstChildElement(XML_SUBROOT_ALARMMASK);
	if(pAlarmMaskElement == NULL)
	{
		printf("file %s, line %d:pAlarmMaskElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pAlarmmaskElement = pAlarmMaskElement->FirstChildElement(XML_ALARMMASK_ALARM_MASK);
	pAlarmmaskElement->SetText(alarmMaskCfg.alarmmask);
	tinyxml2::XMLElement* pSmsElement = pAlarmMaskElement->FirstChildElement(XML_ALARMMASK_SMS);
	pSmsElement->SetText(alarmMaskCfg.sms);
	tinyxml2::XMLElement* pSnapElement = pAlarmMaskElement->FirstChildElement(XML_ALARMMASK_SNAP);
	pSnapElement->SetText(alarmMaskCfg.snap);
	tinyxml2::XMLElement* pSnapSaveElement = pAlarmMaskElement->FirstChildElement(XML_ALARMMASK_SNAP_SAVE);
	pSnapSaveElement->SetText(alarmMaskCfg.snapsave);
	tinyxml2::XMLElement* pKeyMaskElement = pAlarmMaskElement->FirstChildElement(XML_ALARMMASK_KEY_MASK);
	pKeyMaskElement->SetText(alarmMaskCfg.keymask);
}

static void InsertAlarmSpeedXml(tinyxml2::XMLNode *root, const Speed_Alarm_Attr &speedAlarmCfg)
{
	tinyxml2::XMLElement * pAlarmSpeedElement = root->FirstChildElement(XML_SUBROOT_ALARMSPEED);
	if(pAlarmSpeedElement == NULL)
	{
		printf("file %s, line %d:pAlarmSpeedElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pSpeedElement = pAlarmSpeedElement->FirstChildElement(XML_ALARMSPEED_SPEED);
	pSpeedElement->SetText(speedAlarmCfg.speed);
	tinyxml2::XMLElement* pTimeElement = pAlarmSpeedElement->FirstChildElement(XML_ALARMSPEED_TIME);
	pTimeElement->SetText(speedAlarmCfg.time);
	tinyxml2::XMLElement* pWarningElement = pAlarmSpeedElement->FirstChildElement(XML_ALARMSPEED_WARNING);
	pWarningElement->SetText(speedAlarmCfg.warning);
}

static void InsertAlarmFatigueXml(tinyxml2::XMLNode *root, const Fatigue_Alarm_Attr &fatigueAlarmCfg)
{
	tinyxml2::XMLElement * pFatigueAlarmElement = root->FirstChildElement(XML_SUBROOT_FATIGUEALARM);
	if(pFatigueAlarmElement == NULL)
	{
		printf("file %s, line %d:pFatigueAlarmElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pDrivingTimeElement = pFatigueAlarmElement->FirstChildElement(XML_FATIGUEALARM_DRIVING_TIME);
	pDrivingTimeElement->SetText(fatigueAlarmCfg.drivingtime);
	tinyxml2::XMLElement* pTotalTimeElement = pFatigueAlarmElement->FirstChildElement(XML_FATIGUEALARM_TOTAL_TIME);
	pTotalTimeElement->SetText(fatigueAlarmCfg.totaltime);
	tinyxml2::XMLElement* pRestTimeElement = pFatigueAlarmElement->FirstChildElement(XML_FATIGUEALARM_REST_TIME);
	pRestTimeElement->SetText(fatigueAlarmCfg.resttime);
	tinyxml2::XMLElement* pWarningElement = pFatigueAlarmElement->FirstChildElement(XML_FATIGUEALARM_WARNING);
	pWarningElement->SetText(fatigueAlarmCfg.warning);
}

static void InsertAlarmParkingXml(tinyxml2::XMLNode *root, const Parking_Alarm_Attr &parkingAlarmCfg)
{
	tinyxml2::XMLElement * pParkingAlarmElement = root->FirstChildElement(XML_SUBROOT_PARKINGALARM);
	if(pParkingAlarmElement == NULL)
	{
		printf("file %s, line %d:pParkingAlarmElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pTimeElement = pParkingAlarmElement->FirstChildElement(XML_PARKINGALARM_TIME);
	pTimeElement->SetText(parkingAlarmCfg.time);
}

static void InsertAlarmCollisionXml(tinyxml2::XMLNode *root, const Collision_Alarm_Attr &collisionAlarmCfg)
{
	tinyxml2::XMLElement * pCollisionAlarmElement = root->FirstChildElement(XML_SUBROOT_COLLISIONALARM);
	if(pCollisionAlarmElement == NULL)
	{
		printf("file %s, line %d:pCollisionAlarmElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pTimeElement = pCollisionAlarmElement->FirstChildElement(XML_COLLISIONALARM_TIME);
	pTimeElement->SetText(collisionAlarmCfg.time);
	tinyxml2::XMLElement* pAccelerationElement = pCollisionAlarmElement->FirstChildElement(XML_COLLISIONALARM_ACCELERATION);
	pAccelerationElement->SetText(collisionAlarmCfg.acceleration);
}

static void InsertAlarmRolloverXml(tinyxml2::XMLNode *root, const Rollover_Alarm_Attr &rolloverAlarmCfg)
{
	tinyxml2::XMLElement * pRolloverAlarmElement = root->FirstChildElement(XML_SUBROOT_ROLLOVERALARM);
	if(pRolloverAlarmElement == NULL)
	{
		printf("file %s, line %d:pRolloverAlarmElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pAngleElement = pRolloverAlarmElement->FirstChildElement(XML_ROLLOVERALARM_ANGLE);
	pAngleElement->SetText(rolloverAlarmCfg.angle);
}

static void InsertSnapCtrlXml(tinyxml2::XMLNode *root, const Snap_Ctrl_Attr &snapCtrlCfg)
{
	tinyxml2::XMLElement * pSnapCtrlElement = root->FirstChildElement(XML_SUBROOT_SNAPCTRL);
	if(pSnapCtrlElement == NULL)
	{
		printf("file %s, line %d:pSnapCtrlElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	//time start//
	tinyxml2::XMLElement* pTimeElement = pSnapCtrlElement->FirstChildElement(XML_SNAPCTRL_TIME);
	if(pTimeElement == NULL)
	{
		printf("file %s, line %d:pTimeElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pTimeMaskElement = pTimeElement->FirstChildElement(XML_TIME_MASK);
	pTimeMaskElement->SetText(snapCtrlCfg.time.mask);
	tinyxml2::XMLElement* pTimeSaveMaskElement = pTimeElement->FirstChildElement(XML_TIME_SAVE_MASK);
	pTimeSaveMaskElement->SetText(snapCtrlCfg.time.savemask);
	//time end//
	//mileage start//
	tinyxml2::XMLElement* pMileageElement = pSnapCtrlElement->FirstChildElement(XML_SNAPCTRL_MILEAGE);
	if(pMileageElement == NULL)
	{
		printf("file %s, line %d:pMileageElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pMileageMaskElement = pTimeElement->FirstChildElement(XML_MILEAGE_MASK);
	pMileageMaskElement->SetText(snapCtrlCfg.mileage.mask);
	tinyxml2::XMLElement* pMileageSaveMaskElement = pTimeElement->FirstChildElement(XML_MILEAGE_SAVE_MASK);
	pMileageSaveMaskElement->SetText(snapCtrlCfg.mileage.savemask);
	//mileage end//
	tinyxml2::XMLElement* pTimeUnitElement = pSnapCtrlElement->FirstChildElement(XML_SNAPCTRL_TIME_UNIT);
	pTimeUnitElement->SetText(snapCtrlCfg.timeunit);
	tinyxml2::XMLElement* pMikeageUnitElement = pSnapCtrlElement->FirstChildElement(XML_SNAPCTRL_MILEAGE_UNIT);
	pMikeageUnitElement->SetText(snapCtrlCfg.mileageunit);
	tinyxml2::XMLElement* pTimeIntervalElement = pSnapCtrlElement->FirstChildElement(XML_SNAPCTRL_TIME_INTERVAL);
	pTimeIntervalElement->SetText(snapCtrlCfg.timeInterval);
	tinyxml2::XMLElement* pMileageIntervalElement = pSnapCtrlElement->FirstChildElement(XML_SNAPCTRL_MILEAGE_INTERVAL);
	pMileageIntervalElement->SetText(snapCtrlCfg.mileageInterval);
}

static void InsertPicQualityXml(tinyxml2::XMLNode *root, const Pic_Quality_Attr &picQualityCfg)
{
	tinyxml2::XMLElement * pPicQualityElement = root->FirstChildElement(XML_SUBROOT_PICQUALITY);
	if(pPicQualityElement == NULL)
	{
		printf("file %s, line %d:pPicQualityElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pQualityElement = pPicQualityElement->FirstChildElement(XML_PICQUALITY_QUALITY);
	pQualityElement->SetText(picQualityCfg.quality);
}

static void InsertPicColorXml(tinyxml2::XMLNode *root, const Pic_Color_Attr &picColorCfg)
{
	tinyxml2::XMLElement * pPicColorElement = root->FirstChildElement(XML_SUBROOT_PICCOLOR);
	if(pPicColorElement == NULL)
	{
		printf("file %s, line %d:pPicColorElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pLuminanceElement = pPicColorElement->FirstChildElement(XML_PICCOLOR_LUMINANCE);
	pLuminanceElement->SetText(picColorCfg.luminance);
	tinyxml2::XMLElement* pContrastElement = pPicColorElement->FirstChildElement(XML_PICCOLOR_CONTRAST);
	pContrastElement->SetText(picColorCfg.contrast);
	tinyxml2::XMLElement* pSaturationElement = pPicColorElement->FirstChildElement(XML_PICCOLOR_SATURATION);
	pSaturationElement->SetText(picColorCfg.saturation);
	tinyxml2::XMLElement* pChromaElement = pPicColorElement->FirstChildElement(XML_PICCOLOR_CHROMA);
	pChromaElement->SetText(picColorCfg.chroma);
}

static void InsertVehicleInfoXml(tinyxml2::XMLNode *root, const Vehicle_Info &vehicleInfoCfg)
{
	tinyxml2::XMLElement * pVehicleInfoElement = root->FirstChildElement(XML_SUBROOT_VEHICLEINFO);
	if(pVehicleInfoElement == NULL)
	{
		printf("file %s, line %d:pVehicleInfoElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pProvinceIdElement = pVehicleInfoElement->FirstChildElement(XML_VEHICLEINFO_MILEAGE);
	pProvinceIdElement->SetText(vehicleInfoCfg.provinceid);
	tinyxml2::XMLElement* pMileageElement = pVehicleInfoElement->FirstChildElement(XML_VEHICLEINFO_PROVINCE_ID);
	pMileageElement->SetText(vehicleInfoCfg.mileage);
	tinyxml2::XMLElement* pLicenseElement = pVehicleInfoElement->FirstChildElement(XML_VEHICLEINFO_LICENSE);
	if(strlen(vehicleInfoCfg.license))
	{
		pLicenseElement->SetText(vehicleInfoCfg.license);
	}
	tinyxml2::XMLElement* pPlateColorElement = pVehicleInfoElement->FirstChildElement(XML_VEHICLEINFO_PLATE_COLOR);
	pPlateColorElement->SetText(vehicleInfoCfg.platecolor);
	tinyxml2::XMLElement* pCoefficientElement = pVehicleInfoElement->FirstChildElement(XML_VEHICLEINFO_COEFFICIENT);
	pCoefficientElement->SetText(vehicleInfoCfg.coefficient);
	tinyxml2::XMLElement* pVinElement = pVehicleInfoElement->FirstChildElement(XML_VEHICLEINFO_VIN);
	if(strlen(vehicleInfoCfg.vin))
	{
		pVinElement->SetText(vehicleInfoCfg.vin);
	}
	tinyxml2::XMLElement* pVehicletypeElement = pVehicleInfoElement->FirstChildElement(XML_VEHICLEINFO_VEHICLE_TYPE);
	if(strlen(vehicleInfoCfg.vehicletype))
	{
		pVehicletypeElement->SetText(vehicleInfoCfg.vehicletype);
	}
	tinyxml2::XMLElement* pNVehicletypeElement = pVehicleInfoElement->FirstChildElement(XML_VEHICLEINFO_NVEHICLE_TYPE);
	pNVehicletypeElement->SetText(vehicleInfoCfg.nVehicletype);
}

static void InsertGnssDetailXml(tinyxml2::XMLNode *root, const Gnss_Detail_Attr &gnssDetailCfg)
{
	tinyxml2::XMLElement * pGnssDetailElement = root->FirstChildElement(XML_SUBROOT_GNSSDETAIL);
	if(pGnssDetailElement == NULL)
	{
		printf("file %s, line %d:pGnssDetailElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pEnableElement = pGnssDetailElement->FirstChildElement(XML_GNSSDETAIL_ENABLE);
	pEnableElement->SetText(gnssDetailCfg.enable);
	tinyxml2::XMLElement* pBaudtateElement = pGnssDetailElement->FirstChildElement(XML_GNSSDETAIL_BAUDTATE);
	pBaudtateElement->SetText(gnssDetailCfg.baudtate);
	tinyxml2::XMLElement* pOutputFreqElement = pGnssDetailElement->FirstChildElement(XML_GNSSDETAIL_OUTPUT_FREQ);
	pOutputFreqElement->SetText(gnssDetailCfg.outputFreq);
	tinyxml2::XMLElement* pUploadmodeElement = pGnssDetailElement->FirstChildElement(XML_GNSSDETAIL_UPLOAD_MODE);
	pUploadmodeElement->SetText(gnssDetailCfg.uploadmode);
	tinyxml2::XMLElement* pSampleFreqElement = pGnssDetailElement->FirstChildElement(XML_GNSSDETAIL_SAMPLE_FREQ);
	pSampleFreqElement->SetText(gnssDetailCfg.sampleFreq);
	tinyxml2::XMLElement* pUploadSetElement = pGnssDetailElement->FirstChildElement(XML_GNSSDETAIL_UPLOAD_SET);
	pUploadSetElement->SetText(gnssDetailCfg.uploadset);
}

static void InsertCanUploadXml(tinyxml2::XMLNode *root, const CAN_Upload_Attr &CANUploadCfg)
{
	tinyxml2::XMLElement * pCanUploadElement = root->FirstChildElement(XML_SUBROOT_CANUPLOAD);
	if(pCanUploadElement == NULL)
	{
		printf("file %s, line %d:pCanUploadElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pSample1Element = pCanUploadElement->FirstChildElement(XML_CANUPLOAD_SAMPLE1);
	pSample1Element->SetText(CANUploadCfg.sample[0]);
	tinyxml2::XMLElement* pSample2Element = pCanUploadElement->FirstChildElement(XML_CANUPLOAD_SAMPLE2);
	pSample2Element->SetText(CANUploadCfg.sample[1]);
	tinyxml2::XMLElement* pUpload1Element = pCanUploadElement->FirstChildElement(XML_CANUPLOAD_UPLOAD1);
	pUpload1Element->SetText(CANUploadCfg.upload[0]);
	tinyxml2::XMLElement* pUpload2Element = pCanUploadElement->FirstChildElement(XML_CANUPLOAD_UPLOAD2);
	pUpload2Element->SetText(CANUploadCfg.upload[1]);
	//alone_sample start//
	tinyxml2::XMLElement* pAloneSampleElement = pCanUploadElement->FirstChildElement(XML_CANUPLOAD_ALONE_SAMPLE);
	if(pAloneSampleElement == NULL)
	{
		printf("file %s, line %d:pAloneSampleElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pSetElement = pAloneSampleElement->FirstChildElement(XML_ALONE_SAMPLE_SET);
	int len = strlen((char*)CANUploadCfg.aloneSample.set);
	if(len > 0)
	{
		string set = "";
		int len = strlen((char*)CANUploadCfg.aloneSample.set);
		for(int i = 0; i < len; i ++)
		{
			set += BCD2LPCSTR(CANUploadCfg.aloneSample.set[i]);
		}
		pSetElement->SetText(set.c_str());
	}
	//alone_sample end//
}


static void InsertAiViXml(tinyxml2::XMLElement *root, const JT1078_Audio_video_conf_set &aiviattr)
{
	tinyxml2::XMLElement* pAiViConfElement = root->FirstChildElement(XML_VIDEOATTR_AI_VI_CONF);
	if(pAiViConfElement == NULL)
	{
		printf("file %s, line %d:pAiViConfElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pCodingModeElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_CODING_MODE);
	pCodingModeElement->SetText(aiviattr.coding_mode);
	tinyxml2::XMLElement* pResolutionElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_RESOLUTION);
	pResolutionElement->SetText(aiviattr.resolution);
	tinyxml2::XMLElement* pKeyFrameInterElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_KEY_FRAME_INTER);
	pKeyFrameInterElement->SetText(aiviattr.key_frame_inter);
	tinyxml2::XMLElement* pFrameRateElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_FRAME_RATE);
	pFrameRateElement->SetText(aiviattr.frame_rate);
	tinyxml2::XMLElement* pBitRateElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_BIT_RATE);
	pBitRateElement->SetText(aiviattr.bit_rate);
	tinyxml2::XMLElement* pStoreCodingModeElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_STORE_CODING_MODE);
	pStoreCodingModeElement->SetText(aiviattr.store_coding_mode);
	tinyxml2::XMLElement* pStoreResolutinElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_STORE_RESOLUTION);
	pStoreResolutinElement->SetText(aiviattr.store_resolution);
	tinyxml2::XMLElement* pStoreKeyFrameInterElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_STORE_KEY_FRAME_INTER);
	pStoreKeyFrameInterElement->SetText(aiviattr.store_key_frame_inter);
	tinyxml2::XMLElement* pStoreFrameRateElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_STORE_FRAME_RATE);
	pStoreFrameRateElement->SetText(aiviattr.store_frame_rate);
	tinyxml2::XMLElement* pStoreBitRateElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_STORE_BIT_RATE);
	pStoreBitRateElement->SetText(aiviattr.store_bit_rate);
	tinyxml2::XMLElement* pOsdSetElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_OSD_SET);
	pOsdSetElement->SetText(aiviattr.osd_set);
	tinyxml2::XMLElement* pEnableAudioElement = pAiViConfElement->FirstChildElement(XML_AI_VI_CONF_ENABLE_AUDIO);
	pEnableAudioElement->SetText(aiviattr.enable_audio);
}

static void InsertAiViChnXml(tinyxml2::XMLElement *root, JT1078_Audio_video_chn_set &AiViChn)
{
	tinyxml2::XMLElement* pAiViChnElement = root->FirstChildElement(XML_VIDEOATTR_AI_VI_CHN);
	if(pAiViChnElement == NULL)
	{
		printf("file %s, line %d:pAiViChnElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pChnTotalNumElement = pAiViChnElement->FirstChildElement(XML_AI_VI_CHN_CHN_TOTAL_NUM);
	pChnTotalNumElement->SetText(AiViChn.chn_total_num);
	tinyxml2::XMLElement* pAudioNumElement = pAiViChnElement->FirstChildElement(XML_AI_VI_CHN_AUDIO_NUM);
	pAudioNumElement->SetText(AiViChn.audio_num);
	tinyxml2::XMLElement* pVideoNumElement = pAiViChnElement->FirstChildElement(XML_AI_VI_CHN_VIDEO_NUM);
	pVideoNumElement->SetText(AiViChn.video_num);
}

static void InsertSingleChnCtlXml(tinyxml2::XMLElement *root, const JT1078_single_chn_control &SingleChnCtl)
{
	tinyxml2::XMLElement* pSingleChnElement = root->FirstChildElement(XML_VIDEOATTR_SINGLE_CHN);
	if(pSingleChnElement == NULL)
	{
		printf("file %s, line %d:pSingleChnElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pSingleChnChnNumElement = pSingleChnElement->FirstChildElement(XML_SINGLE_CHN_CHN_NUM);
	if(NULL != pSingleChnChnNumElement)
	{
		pSingleChnChnNumElement->SetText(SingleChnCtl.chn_num);
	}else
	{
		printf("pSingleChnChnNumElement:Single_Chn does not exist...\n");
	}
}

static void InsertSpecialAlarmXml(tinyxml2::XMLElement *root, const JT1078_special_alarm_param_set &specialAlarm)
{
	tinyxml2::XMLElement* pSpecialAlarmElement = root->FirstChildElement(XML_VIDEOATTR_SPECIAL_ALARM);
	if(pSpecialAlarmElement == NULL)
	{
		printf("file %s, line %d:pSpecialAlarmElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pVideoStorageThresElement = pSpecialAlarmElement->FirstChildElement(XML_SPECIAL_ALARM_VIDEO_STORAGE_THRES);
	pVideoStorageThresElement->SetText(specialAlarm.video_storage_thres);
	tinyxml2::XMLElement* pVideoDurationElement = pSpecialAlarmElement->FirstChildElement(XML_SPECIAL_ALARM_VIDEO_DURATION);
	pVideoDurationElement->SetText(specialAlarm.video_duration);
	tinyxml2::XMLElement* pStartTimeElement = pSpecialAlarmElement->FirstChildElement(XML_SPECIAL_ALARM_START_TIME);
	pStartTimeElement->SetText(specialAlarm.start_time);
}

static void InsertVideoAlarmXml(tinyxml2::XMLElement *root, const JT1078_alarm_video_param &videoAlarm)
{
	tinyxml2::XMLElement* pVideoAlarmElement = root->FirstChildElement(XML_VIDEOATTR_VIDEO_ALARM);
	if(pVideoAlarmElement == NULL)
	{
		printf("file %s, line %d:pVideoAlarmElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pNumOfCarriesElement = pVideoAlarmElement->FirstChildElement(XML_VIDEO_ALALRM_NUM_OF_CARRIES);
	pNumOfCarriesElement->SetText(videoAlarm.num_of_carries);
	tinyxml2::XMLElement* pFatigueThresElement = pVideoAlarmElement->FirstChildElement(XML_VIDEO_ALALRM_FATIGUE_THRES);
	pFatigueThresElement->SetText(videoAlarm.fatigue_thres);
}

static void InsertDailyAwakeXml(tinyxml2::XMLElement *root, const JT1078_awake_param &awakeAttr)
{
	tinyxml2::XMLElement* pDailyAwakeElement = root->FirstChildElement(XML_VIDEOATTR_DAILY_AWAKE);
	if(pDailyAwakeElement == NULL)
	{
		printf("file %s, line %d:pDailyAwakeElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pAwakeModeElement = pDailyAwakeElement->FirstChildElement(XML_DAILY_AWAKE_AWAKE_MODE);
	pAwakeModeElement->SetText(awakeAttr.awake_mode);
	tinyxml2::XMLElement* pAwakeTypeElement = pDailyAwakeElement->FirstChildElement(XML_DAILY_AWAKE_AWAKE_TYPE);
	pAwakeTypeElement->SetText(awakeAttr.awake_type);
	tinyxml2::XMLElement* pSetAwakeTimeElement = pDailyAwakeElement->FirstChildElement(XML_DAILY_AWAKE_SET_AWAKE_TIME);
	pSetAwakeTimeElement->SetText(awakeAttr.set_awake_time);
	tinyxml2::XMLElement* pFlagElement = pDailyAwakeElement->FirstChildElement(XML_DAILY_AWAKE_FLAG);
	pFlagElement->SetText(awakeAttr.awake_params.flag);
	/*daily_awake_params*/
	//period1_start_time
	tinyxml2::XMLElement* pPeriod1StartTimeElement = pDailyAwakeElement->FirstChildElement(XML_SUB_PERIOD1_START_TIME);
	if(pPeriod1StartTimeElement == NULL)
	{
		printf("file %s, line %d:pPeriod1StartTimeElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pStartHour1Element = pPeriod1StartTimeElement->FirstChildElement(XML_PERIOD_TIME_HOUR);
	pStartHour1Element->SetText(awakeAttr.awake_params.period1_start_time[0]);
	tinyxml2::XMLElement* pStartMinutes1Element = pPeriod1StartTimeElement->FirstChildElement(XML_PERIOD_TIME_MINUTES);
	pStartMinutes1Element->SetText(awakeAttr.awake_params.period1_start_time[1]);
	//period1_close_time
	tinyxml2::XMLElement* pPeriod1CloseTimeElement = pDailyAwakeElement->FirstChildElement(XML_SUB_PERIOD1_CLOSE_TIME);
	if(pPeriod1CloseTimeElement == NULL)
	{
		printf("file %s, line %d:pPeriod1CloseTimeElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pCloseHour1Element = pPeriod1CloseTimeElement->FirstChildElement(XML_PERIOD_TIME_HOUR);
	pCloseHour1Element->SetText(awakeAttr.awake_params.period1_close_time[0]);
	tinyxml2::XMLElement* pCloseMinutes1Element = pPeriod1CloseTimeElement->FirstChildElement(XML_PERIOD_TIME_MINUTES);
	pCloseMinutes1Element->SetText(awakeAttr.awake_params.period1_close_time[1]);
	//period2_start_time
	tinyxml2::XMLElement* pPeriod2StartTimeElement = pDailyAwakeElement->FirstChildElement(XML_SUB_PERIOD2_START_TIME);
	if(pPeriod2StartTimeElement == NULL)
	{
		printf("file %s, line %d:pPeriod2StartTimeElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pStartHour2Element = pPeriod2StartTimeElement->FirstChildElement(XML_PERIOD_TIME_HOUR);
	pStartHour2Element->SetText(awakeAttr.awake_params.period2_start_time[0]);
	tinyxml2::XMLElement* pStartMinutes2Element = pPeriod2StartTimeElement->FirstChildElement(XML_PERIOD_TIME_MINUTES);
	pStartMinutes2Element->SetText(awakeAttr.awake_params.period2_start_time[1]);
	//period2_close_time
	tinyxml2::XMLElement* pPeriod2CloseTimeElement = pDailyAwakeElement->FirstChildElement(XML_SUB_PERIOD2_CLOSE_TIME);
	if(pPeriod2CloseTimeElement == NULL)
	{
		printf("file %s, line %d:pPeriod2CloseTimeElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pCloseHour2Element = pPeriod2CloseTimeElement->FirstChildElement(XML_PERIOD_TIME_HOUR);
	pCloseHour2Element->SetText(awakeAttr.awake_params.period2_close_time[0]);
	tinyxml2::XMLElement* pCloseMinutes2Element = pPeriod2CloseTimeElement->FirstChildElement(XML_PERIOD_TIME_MINUTES);
	pCloseMinutes2Element->SetText(awakeAttr.awake_params.period2_close_time[1]);
	//period3_start_time
	tinyxml2::XMLElement* pPeriod3StartTimeElement = pDailyAwakeElement->FirstChildElement(XML_SUB_PERIOD3_START_TIME);
	if(pPeriod3StartTimeElement == NULL)
	{
		printf("file %s, line %d:pPeriod3StartTimeElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pStartHour3Element = pPeriod3StartTimeElement->FirstChildElement(XML_PERIOD_TIME_HOUR);
	pStartHour3Element->SetText(awakeAttr.awake_params.period3_start_time[0]);
	tinyxml2::XMLElement* pStartMinutes3Element = pPeriod3StartTimeElement->FirstChildElement(XML_PERIOD_TIME_MINUTES);
	pStartMinutes3Element->SetText(awakeAttr.awake_params.period3_start_time[1]);
	//period3_close_time
	tinyxml2::XMLElement* pPeriod3CloseTimeElement = pDailyAwakeElement->FirstChildElement(XML_SUB_PERIOD3_CLOSE_TIME);
	if(pPeriod3CloseTimeElement == NULL)
	{
		printf("file %s, line %d:pPeriod3CloseTimeElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pCloseHour3Element = pPeriod3CloseTimeElement->FirstChildElement(XML_PERIOD_TIME_HOUR);
	pCloseHour3Element->SetText(awakeAttr.awake_params.period3_close_time[0]);
	tinyxml2::XMLElement* pCloseMinutes3Element = pPeriod3CloseTimeElement->FirstChildElement(XML_PERIOD_TIME_MINUTES);
	pCloseMinutes3Element->SetText(awakeAttr.awake_params.period3_close_time[1]);
	//period4_start_time
	tinyxml2::XMLElement* pPeriod4StartTimeElement = pDailyAwakeElement->FirstChildElement(XML_SUB_PERIOD4_START_TIME);
	if(pPeriod4StartTimeElement == NULL)
	{
		printf("file %s, line %d:pPeriod4StartTimeElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pStartHour4Element = pPeriod4StartTimeElement->FirstChildElement(XML_PERIOD_TIME_HOUR);
	pStartHour4Element->SetText(awakeAttr.awake_params.period4_start_time[0]);
	tinyxml2::XMLElement* pStartMinutes4Element = pPeriod4StartTimeElement->FirstChildElement(XML_PERIOD_TIME_MINUTES);
	pStartMinutes4Element->SetText(awakeAttr.awake_params.period4_start_time[1]);
	//period4_close_time
	tinyxml2::XMLElement* pPeriod4CloseTimeElement = pDailyAwakeElement->FirstChildElement(XML_SUB_PERIOD4_CLOSE_TIME);
	if(pPeriod4CloseTimeElement == NULL)
	{
		printf("file %s, line %d:pPeriod4CloseTimeElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	tinyxml2::XMLElement* pCloseHour4Element = pPeriod4CloseTimeElement->FirstChildElement(XML_PERIOD_TIME_HOUR);
	pCloseHour4Element->SetText(awakeAttr.awake_params.period4_start_time[0]);
	tinyxml2::XMLElement* pCloseMinutes4Element = pPeriod4CloseTimeElement->FirstChildElement(XML_PERIOD_TIME_MINUTES);
	pCloseMinutes4Element->SetText(awakeAttr.awake_params.period4_start_time[1]);
}

static int StopCameraShot()
{
	return 0;
}

static int StartCameraShot(NetMdShotCmd &mdShotCmd, vector<ShotResult> &resultList)
{
	return 0;
}

static int StartSnapShot(NetMdShotCmd &mdShotCmd, vector<ShotResult> &resultList)
{
	return 0;
}

CParamInterface::CParamInterface()
{
	//memset(&m_st_VehicleInfo, 0, sizeof(Vehicle_Info));
	//memcpy(&m_szPhoneNum[0], 0, 12);
	//memcpy(&m_szServerAddr[0], 0, 15);
	//memcpy(&m_stTermAttr, 0, sizeof(Terminal_Attr));
	terminal_cfg = new JT808_Config_S;
	m_nType = SOCK_STREAM;
	m_nPort = 8888;
}

CParamInterface::~CParamInterface()
{
	
}
DWORD CParamInterface::Get_terminal_param(JT808_Config_S *cfg)
{
	memcpy(terminal_cfg, cfg, sizeof(JT808_Config_S));
#if 0
	TerminalXMLParse xml_parse;
	if (access(TERMINAL_PARAMS_PATH, F_OK) == 0){
		xml_parse.LoadTerminalParam(TERMINAL_PARAMS_PATH, terminal_cfg) ;
	}else{
		xml_parse.LoadDefaultParam(TERMINAL_PARAMS_PATH, terminal_cfg) ;
	}
#endif
	return 0;
}

void CParamInterface::PutOutLog(const char * fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	vprintf(fmt, vl);
//	printf(fmt);
	va_end(vl);
}
	
int CParamInterface::SetSysCfg(JT808_Config_S *sysCfg)
{
	printf("into SetSysCfg\n");
	//继承InterFace类，虚成员函数重写，父类可调用子类成员函数，8103设置参数保存到配置文件
	memcpy(terminal_cfg, sysCfg, sizeof(JT808_Config_S));
	tinyxml2::XMLDocument doc;
	int res=doc.LoadFile("./terminal_param.xml");
    if(res!=0)
    {
        printf("load terminal_param xml failed...\n");
		return -1;
    }
	tinyxml2::XMLNode *root = doc.FirstChildElement(XML_ROOT);
	if(root == NULL)
	{
		printf("file %s, line %d:root is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
		return -1;
	}
	/*网络通信参数设置*/
	InsertNetCommunicationXml(root, terminal_cfg->commCfg);
	/*网络通信参数设置 end.*/
	/*中心服务器参数设置*/
	InsertCenterServerXml(root, terminal_cfg->centerSrvCfg);
	/*中心服务器参数设置 end.*/
	/*IC卡认证服务器参数设置*/
	InsertICServerXml(root, terminal_cfg->icSrvCfg);
	/*IC卡认证服务器参数设置 end.*/
	/*位置汇报参数设置*/
	InsertGnssReportXml(root, terminal_cfg->reportCfg);
	/*位置汇报参数设置 end.*/
	/*电话相关参数设置*/
	InsertPhoneNumberXml(root, terminal_cfg->phoneNumCfg);
	/*电话相关参数设置 end.*/
	/*报警掩码参数设置*/
	InsertAlarmMaskXml(root, terminal_cfg->alarmMaskCfg);
	/*报警掩码参数设置 end.*/
	/*报警条件参数设置*/
	InsertAlarmSpeedXml(root, terminal_cfg->alarmSpeed);
	InsertAlarmFatigueXml(root, terminal_cfg->alarmFatigue);
	InsertAlarmParkingXml(root, terminal_cfg->alarmParking);
	InsertAlarmCollisionXml(root, terminal_cfg->alarmCollision);
	InsertAlarmRolloverXml(root, terminal_cfg->alarmRollover);
	/*报警条件参数设置 end.*/
	/*图像抓拍控制参数设置*/
	InsertSnapCtrlXml(root, terminal_cfg->snapCtrl);
	/*图像抓拍控制参数设置 end.*/
	/*图像质量参数设置*/
	InsertPicQualityXml(root, terminal_cfg->picQuality);
	/*图像质量参数设置 end.*/
	/*图像颜色参数设置*/
	InsertPicColorXml(root, terminal_cfg->picColor);
	/*图像颜色参数设置 end.*/
	/*终端信息参数设置*/
	InsertVehicleInfoXml(root, terminal_cfg->VehInfoCfg);
	/*终端信息参数设置 end.*/
	/*GNSS详细信息参数设置*/
	InsertGnssDetailXml(root, terminal_cfg->gnssDetail);
	/*GNSS详细信息参数设置 end.*/
	/*CAN数据上传参数设置*/
	InsertCanUploadXml(root, terminal_cfg->canbusUpload);
	/*CAN数据上传参数设置 end.*/
	/*音视频参数设置*/
	tinyxml2::XMLElement * pVideoAttrElement = root->FirstChildElement(XML_SUBROOT_VIDEOATTR);
	if(pVideoAttrElement == NULL)
	{
		printf("file %s, line %d:pVideoAttrElement is NULL, error code :%d\n", __FILE__, __LINE__, tinyxml2::XML_ERROR_FILE_READ_ERROR);
	}
	InsertAiViXml(pVideoAttrElement, terminal_cfg->VideoCfg.ai_vi_attr);
	InsertAiViChnXml(pVideoAttrElement, terminal_cfg->VideoCfg.ai_vi_chn);
	InsertSingleChnCtlXml(pVideoAttrElement, terminal_cfg->VideoCfg.single_chn_ctl);
	InsertSpecialAlarmXml(pVideoAttrElement, terminal_cfg->VideoCfg.special_alarm);
	InsertVideoAlarmXml(pVideoAttrElement, terminal_cfg->VideoCfg.video_alarm);
	InsertDailyAwakeXml(pVideoAttrElement, terminal_cfg->VideoCfg.awake_attr);
	/*音视频参数设置 end.*/
	doc.SaveFile(TERMINAL_PARAMS_PATH);
#if 0
	XMLParse xml_parse;
	if (access(TERMINAL_PARAMS_PATH, F_OK) == 0){
		DBG_LEVEL_6("terminal_param.xml exist, saveTerminalParam...");
		xml_parse.SetTerXml(TERMINAL_PARAMS_PATH, terminal_cfg);
		return 0;
	}else{
		DBG_LEVEL_6("terminal_param.xml dose not exist...");
		return -1;
	}
#endif
#if 0
	DBG_LEVEL_6("ai_vi_attr\n");
	DBG_LEVEL_6("coding_mode:%d \n", terminal_cfg->VideoCfg.ai_vi_attr.coding_mode);
	DBG_LEVEL_6("resolution:%d \n", terminal_cfg->VideoCfg.ai_vi_attr.resolution);
	DBG_LEVEL_6("key_frame_inter:%d \n", terminal_cfg->VideoCfg.ai_vi_attr.key_frame_inter);
	DBG_LEVEL_6("frame_rate:%d \n", terminal_cfg->VideoCfg.ai_vi_attr.frame_rate);
	DBG_LEVEL_6("bit_rate:%d \n", terminal_cfg->VideoCfg.ai_vi_attr.bit_rate);
	DBG_LEVEL_6("store_coding_mode:%d \n", terminal_cfg->VideoCfg.ai_vi_attr.store_coding_mode);
	DBG_LEVEL_6("store_resolution:%d \n", terminal_cfg->VideoCfg.ai_vi_attr.store_resolution);

	DBG_LEVEL_6("comm cfg\n");
	DBG_LEVEL_6("heartbeat:%d \n", terminal_cfg->commCfg.heartbeat);
	DBG_LEVEL_6("tcp:%d \n", terminal_cfg->commCfg.tcp.timeout);
	DBG_LEVEL_6("tcp:%d \n", terminal_cfg->commCfg.tcp.trytimes);
	DBG_LEVEL_6("udp:%d \n", terminal_cfg->commCfg.udp.timeout);
	DBG_LEVEL_6("udp:%d \n", terminal_cfg->commCfg.udp.trytimes);
	DBG_LEVEL_6("sms:%d \n", terminal_cfg->commCfg.sms.timeout);
	DBG_LEVEL_6("sms:%d \n", terminal_cfg->commCfg.sms.trytimes);

	DBG_LEVEL_6("report cfg\n");
	DBG_LEVEL_6("tactics:%d \n", terminal_cfg->reportCfg.tactics);
	DBG_LEVEL_6("scheme:%d \n", terminal_cfg->reportCfg.scheme);
	
	DBG_LEVEL_6("0 time:%d \n", terminal_cfg->reportCfg.interval[0].time);
	DBG_LEVEL_6("0 mileage:%d \n", terminal_cfg->reportCfg.interval[0].mileage);

	DBG_LEVEL_6("1 time:%d \n", terminal_cfg->reportCfg.interval[1].time);
	DBG_LEVEL_6("1 mileage:%d \n", terminal_cfg->reportCfg.interval[1].mileage);

	DBG_LEVEL_6("2 time:%d \n", terminal_cfg->reportCfg.interval[2].time);
	DBG_LEVEL_6("2 mileage:%d \n", terminal_cfg->reportCfg.interval[2].mileage);

	DBG_LEVEL_6("3 time:%d \n", terminal_cfg->reportCfg.interval[3].time);
	DBG_LEVEL_6("3 mileage:%d \n", terminal_cfg->reportCfg.interval[3].mileage);

	DBG_LEVEL_6("angle:%d \n", terminal_cfg->reportCfg.angle);
	DBG_LEVEL_6("radius:%d \n", terminal_cfg->reportCfg.radius);

	DBG_LEVEL_6("------------alarm------------\n");
	DBG_LEVEL_6("speed:%d \n", terminal_cfg->alarmSpeed.speed);
	DBG_LEVEL_6("time:%d \n", terminal_cfg->alarmSpeed.time);
	DBG_LEVEL_6("warning:%d \n", terminal_cfg->alarmSpeed.warning);
	
	DBG_LEVEL_6("drivingtime:%d \n", terminal_cfg->alarmFatigue.drivingtime);
	DBG_LEVEL_6("totaltime:%d \n", terminal_cfg->alarmFatigue.totaltime);
	DBG_LEVEL_6("resttime:%d \n", terminal_cfg->alarmFatigue.resttime);
	DBG_LEVEL_6("warning:%d \n", terminal_cfg->alarmFatigue.warning);

	DBG_LEVEL_6("time:%d\n", terminal_cfg->alarmParking.time);

	DBG_LEVEL_6("time:%d\n", terminal_cfg->alarmCollision.time);
	DBG_LEVEL_6("acceleration:%d\n", terminal_cfg->alarmCollision.acceleration);
	DBG_LEVEL_6("angle:%d\n", terminal_cfg->alarmRollover.angle);
	DBG_LEVEL_6("------------alarm------------\n");

	DBG_LEVEL_6("------------snapctrl------------\n");
	DBG_LEVEL_6("mask:%d\n", terminal_cfg->snapCtrl.time.mask);
	DBG_LEVEL_6("savemask:%d\n", terminal_cfg->snapCtrl.time.savemask);
	DBG_LEVEL_6("timeunit:%d\n", terminal_cfg->snapCtrl.timeunit);
	DBG_LEVEL_6("timeInterval:%d\n", terminal_cfg->snapCtrl.timeInterval);
	DBG_LEVEL_6("mask:%d\n", terminal_cfg->snapCtrl.mileage.mask);
	DBG_LEVEL_6("savemask:%d\n", terminal_cfg->snapCtrl.mileage.savemask);
	DBG_LEVEL_6("timeunit:%d\n", terminal_cfg->snapCtrl.mileageunit);
	DBG_LEVEL_6("timeInterval:%d\n", terminal_cfg->snapCtrl.mileageInterval);
	DBG_LEVEL_6("------------snapctrl------------\n");

	DBG_LEVEL_6("------------vehicle info------------\n");
	DBG_LEVEL_6("mileage:%d\n", terminal_cfg->VehInfoCfg.mileage);
	DBG_LEVEL_6("provinceid:%d\n", terminal_cfg->VehInfoCfg.provinceid);
	DBG_LEVEL_6("cityid:%d\n", terminal_cfg->VehInfoCfg.cityid);
	DBG_LEVEL_6("license:%s\n", terminal_cfg->VehInfoCfg.license);
	DBG_LEVEL_6("platecolor:%d\n", terminal_cfg->VehInfoCfg.platecolor);
	DBG_LEVEL_6("------------vehicle info------------\n");
#endif

	return 0;
}

int CParamInterface::GetSysCfg(JT808_Config_S *sysCfg)
{
	memcpy(sysCfg, terminal_cfg, sizeof(JT808_Config_S));
#if 0

	DBG_LEVEL_6("ai_vi_attr\n");
	DBG_LEVEL_6("coding_mode:%d \n", sysCfg->VideoCfg.ai_vi_attr.coding_mode);
	DBG_LEVEL_6("resolution:%d \n", sysCfg->VideoCfg.ai_vi_attr.resolution);
	DBG_LEVEL_6("key_frame_inter:%d \n", sysCfg->VideoCfg.ai_vi_attr.key_frame_inter);
	DBG_LEVEL_6("frame_rate:%d \n", sysCfg->VideoCfg.ai_vi_attr.frame_rate);
	DBG_LEVEL_6("bit_rate:%d \n", sysCfg->VideoCfg.ai_vi_attr.bit_rate);
	DBG_LEVEL_6("store_coding_mode:%d \n", sysCfg->VideoCfg.ai_vi_attr.store_coding_mode);
	DBG_LEVEL_6("store_resolution:%d \n", sysCfg->VideoCfg.ai_vi_attr.store_resolution);

	DBG_LEVEL_6("------------commCfg------------\n");
	DBG_LEVEL_6("heartbeat:%d \n", sysCfg->commCfg.heartbeat);
	DBG_LEVEL_6("tcp:%d \n", sysCfg->commCfg.tcp.timeout);
	DBG_LEVEL_6("tcp:%d \n", sysCfg->commCfg.tcp.trytimes);
	DBG_LEVEL_6("udp:%d \n", sysCfg->commCfg.udp.timeout);
	DBG_LEVEL_6("udp:%d \n", sysCfg->commCfg.udp.trytimes);
	DBG_LEVEL_6("sms:%d \n", sysCfg->commCfg.sms.timeout);
	DBG_LEVEL_6("sms:%d \n", sysCfg->commCfg.sms.trytimes);
	DBG_LEVEL_6("------------commCfg------------\n");
	
	DBG_LEVEL_6("------------reportCfg------------\n");
	DBG_LEVEL_6("tactics:%d \n", sysCfg->reportCfg.tactics);
	DBG_LEVEL_6("scheme:%d \n", sysCfg->reportCfg.scheme);
	
	DBG_LEVEL_6("0 time:%d \n", sysCfg->reportCfg.interval[0].time);
	DBG_LEVEL_6("0 mileage:%d \n", sysCfg->reportCfg.interval[0].mileage);

	DBG_LEVEL_6("1 time:%d \n", sysCfg->reportCfg.interval[1].time);
	DBG_LEVEL_6("1 mileage:%d \n", sysCfg->reportCfg.interval[1].mileage);

	DBG_LEVEL_6("2 time:%d \n", sysCfg->reportCfg.interval[2].time);
	DBG_LEVEL_6("2 mileage:%d \n", sysCfg->reportCfg.interval[2].mileage);

	DBG_LEVEL_6("3 time:%d \n", sysCfg->reportCfg.interval[3].time);
	DBG_LEVEL_6("3 mileage:%d \n", sysCfg->reportCfg.interval[3].mileage);

	DBG_LEVEL_6("angle:%d \n", sysCfg->reportCfg.angle);
	DBG_LEVEL_6("radius:%d \n", sysCfg->reportCfg.radius);
	DBG_LEVEL_6("------------reportCfg------------\n");

	DBG_LEVEL_6("------------alarm------------\n");
	DBG_LEVEL_6("speed:%d\n", sysCfg->alarmSpeed.speed);
	DBG_LEVEL_6("time:%d\n", sysCfg->alarmSpeed.time);
	DBG_LEVEL_6("warning:%d\n", sysCfg->alarmSpeed.warning);
	
	DBG_LEVEL_6("drivingtime:%d\n", sysCfg->alarmFatigue.drivingtime);
	DBG_LEVEL_6("totaltime:%d\n", sysCfg->alarmFatigue.totaltime);
	DBG_LEVEL_6("resttime:%d\n", sysCfg->alarmFatigue.resttime);
	DBG_LEVEL_6("warning:%d\n", sysCfg->alarmFatigue.warning);

	DBG_LEVEL_6("time:%d\n", sysCfg->alarmParking.time);

	DBG_LEVEL_6("time:%d\n", sysCfg->alarmCollision.time);
	DBG_LEVEL_6("acceleration:%d\n", sysCfg->alarmCollision.acceleration);
	DBG_LEVEL_6("angle:%d\n", sysCfg->alarmRollover.angle);
	DBG_LEVEL_6("------------alarm------------\n");

	DBG_LEVEL_6("------------snapctrl------------\n");
	DBG_LEVEL_6("mask:%d\n", sysCfg->snapCtrl.time.mask);
	DBG_LEVEL_6("savemask:%d\n", sysCfg->snapCtrl.time.savemask);
	DBG_LEVEL_6("timeunit:%d\n", sysCfg->snapCtrl.timeunit);
	DBG_LEVEL_6("timeInterval:%d\n", sysCfg->snapCtrl.timeInterval);
	DBG_LEVEL_6("mask:%d\n", sysCfg->snapCtrl.mileage.mask);
	DBG_LEVEL_6("savemask:%d\n", sysCfg->snapCtrl.mileage.savemask);
	DBG_LEVEL_6("timeunit:%d\n", sysCfg->snapCtrl.mileageunit);
	DBG_LEVEL_6("timeInterval:%d\n", sysCfg->snapCtrl.mileageInterval);
	DBG_LEVEL_6("------------snapctrl------------\n");

	DBG_LEVEL_6("------------vehicle info------------\n");
	DBG_LEVEL_6("mileage:%d\n", sysCfg->VehInfoCfg.mileage);
	DBG_LEVEL_6("provinceid:%d\n", sysCfg->VehInfoCfg.provinceid);
	DBG_LEVEL_6("cityid:%d\n", sysCfg->VehInfoCfg.cityid);
	DBG_LEVEL_6("license:%s\n", sysCfg->VehInfoCfg.license);
	DBG_LEVEL_6("platecolor:%d\n", sysCfg->VehInfoCfg.platecolor);
	DBG_LEVEL_6("------------vehicle info------------\n");
#endif	
	return 0;
}

#define CONFIGFILE ".config"
int CParamInterface::GetAuthorizeCode(char* szACode)
{
#if 0
	bool bInit = false;
	if ( !bInit )
		return false;
	if (szACode != NULL)
		strcpy(szACode, "andy123456");
	return true;
#else
	memset(szACode, '\0', 64);
	if(NULL == szACode){
		return 0;
	}
	memset(szACode, 0, 64);
	FILE *fp = NULL;
	if(access(CONFIGFILE, F_OK | R_OK | W_OK) != 0){
		fp = fopen(CONFIGFILE, "a+");
		if(fp != NULL){
			fclose(fp);
		}

		return 0;
	}
	
 	fp = fopen(CONFIGFILE, "r");
	if(fp == NULL){
		return 0;
	}

	fgets(szACode, 64, fp);
	fclose(fp);

	return strlen(szACode);
#endif
}
		
int CParamInterface::GetPhoneNumber(unsigned char* szPhoneNum)
{
	memcpy(szPhoneNum, &m_szPhoneNum[0], 15);
	return 0;
}

void CParamInterface::SetPhoneNumber(const unsigned char* szPhoneNum)
{
	memcpy(&m_szPhoneNum[0], szPhoneNum, 15);
	printf("m_szPhoneNum======%s\n", m_szPhoneNum);
}

int CParamInterface::GetDriverInfo(IC_Read_Info* pICInfo,Driver_Info* pDriverInfo)
{
	return -1;
}

void CParamInterface::SetVehicleInfo(const Vehicle_Info* pVehicle)
{
	memcpy(&m_st_VehicleInfo, pVehicle, sizeof(Vehicle_Info));
}

int CParamInterface::GetVehicleInfo(Vehicle_Info* pVehicle)
{
	if(pVehicle != NULL)
	{
	#if 0
		memcpy(pVehicle, &m_st_VehicleInfo, sizeof(Vehicle_Info));
	#else
		pVehicle->mileage = 10000;
		pVehicle->provinceid = 0x44;
		pVehicle->cityid = 0x0300; //440300 ShenZhen
		pVehicle->platecolor = 1;
		//memcpy(pVehicle->license, "粤B12345", 9);
		memcpy(pVehicle->license, "辽ABD990", 9);
	#endif
		return 1;
	}
	return 0;
}

int CParamInterface::GetNetworkParam(Net_Communication_Attr* pParam)
{
	//pParam->heartbeat = 30;
	//pParam->tcp.trytimes = 2;
	//pParam->tcp.timeout = 10;

	pParam->heartbeat = terminal_cfg->commCfg.heartbeat;
	pParam->tcp.trytimes = terminal_cfg->commCfg.tcp.trytimes;
	pParam->tcp.timeout = terminal_cfg->commCfg.tcp.timeout;
	
	DBG_LEVEL_6("herat:%d try:%d timeout:%d", pParam->heartbeat, pParam->tcp.trytimes, pParam->tcp.timeout);
	
	return 0;
}

int CParamInterface::GetServerAddr(int &nType, char* pAddr,int& port)
{
	#if 0
	int nRet = 0;
	if(pAddr != NULL)
	{
		switch (nType)
		{
			case 0x0:
			{
				#if 0
				strncpy(pAddr, "47.99.196.215", strlen("47.96.196.215"));
				port = 7000;
				#else
				strncpy(pAddr, "59.172.105.82", strlen("59.172.105.82"));
				port = 6666;
				#endif
				break;
			}
			default:
			{
				DBG_LEVEL_6("%s: Invalid nType[%d]", __FUNCTION__, nType);
				nRet = 1;
				break;
			}
		}
	}
	#else

	memcpy(pAddr, &m_szServerAddr[0], 15);
	port = m_nPort;
	nType = m_nType;
	#endif

	return 0;
}

void CParamInterface::SetServerAddr(int nType, const char* pAddr, int port)
{
	if(pAddr != NULL){
		m_nType = nType;
		m_nPort = port;
		memcpy(&m_szServerAddr[0], pAddr, 15);
	}
}
	
void CParamInterface::GetTerminalAttr(Terminal_Attr* pTermAttr)
{
	if(pTermAttr != NULL){
		#if 0
		memcpy(pTermAttr->manufacturerID, "JW", 5);
		//memcpy(pTermAttr->devID, "8451000", 7);
		memcpy(pTermAttr->devID, "2855253", 7);
		memcpy(pTermAttr->devType, "12345678901234567890", 20);
		pTermAttr->supportMask = 0x08;
		pTermAttr->simICCID[0] = 0x01;
		pTermAttr->simICCID[1] = 0x02;
		pTermAttr->simICCID[2] = 0x03;
		pTermAttr->simICCID[3] = 0x04;
		pTermAttr->simICCID[4] = 0x05;
		pTermAttr->simICCID[5] = 0x06;
		pTermAttr->simICCID[6] = 0x07;
		pTermAttr->simICCID[7] = 0x08;
		pTermAttr->simICCID[8] = 0x09;
		pTermAttr->simICCID[9] = 0x11;
		memcpy(pTermAttr->softVer, "V1.2.1", 6);
		memcpy(pTermAttr->hardVer, "V1.1.1", 6);
		pTermAttr->gnss = 0x01;
		pTermAttr->wireless = 0x80;
		#else
		memcpy(pTermAttr, &m_stTermAttr, sizeof(Terminal_Attr));
		#endif
	}
}

void CParamInterface::SetTerminalAttr(const Terminal_Attr& stTermAttr)
{
	memcpy(&m_stTermAttr, &stTermAttr, sizeof(Terminal_Attr));
}

void CParamInterface::GetGnssData(void* pData)
{
	
}

void CParamInterface::VehiDoorCtrlSet(unsigned char mark)
{
	
}
	
int CParamInterface::Upgrade(const char* pVersion,void* pData, int len)
{
	return 1;
}

int CParamInterface::LocationTracking(int interval, int time)
{
	return 1;
}

int CParamInterface::ConfirmAlarm(unsigned int alarmMask)
{
	return 0;
}
	
int CParamInterface::SMSProc(const char* pSMS,SMS_MASK_E mask)
{
	DBG_LEVEL_6("\nMessage: %s\n", pSMS);
#if 0
	return 1;
#else
	return 0;
#endif
}

int CParamInterface::SetEventMenu(InfoOp opr,Event_Menu_Item* pItem,int num)
{
	return 1;
}

int CParamInterface::SetAskAnswer(const char* pQustion,SMS_MASK_E mask,Event_Menu_Item* pItem,int num)
{
	return 1;
}
	
int CParamInterface::SetMsgMenu(InfoOp opr,Msg_Menu_Item* pItem,int num)
{
	return 1;
}

	
int CParamInterface::MessageService(int nType,char* pMessage)
{
	return 1;
}

int CParamInterface::TelCallback(int nMark, char* pPhoneNo)
{
	return 1;
}

int CParamInterface::TelBookSet(InfoOp  op, Address_Book_Item* pAddrBook,int num)
{
	return 1;
}

int CParamInterface::SetRingRgn(int opr,Ring_Region_Attr* rgn,int num)
{
	return 1;
}

int CParamInterface::SetRectRgn(int opr,Rectangle_Region_Attr* rgn,int num)
{
	return 1;
}

int CParamInterface::SetPolyRgn(Poly_Region_Attr* rgn)
{
	return 1;
}

int CParamInterface::SetRouteRgn(Route_Region_Attr* rgn)
{
	return 1;
}

int CParamInterface::DelRgn(int rgnType,DWORD* pRgnID,int num)
{
	return 1;
}

int CParamInterface::SetDrivingRecord(int cmd, char* data)
{
	return 0;
}

int CParamInterface::DownlinkTrans(TRANSPARENT_TYPE_E type,char* buf,int size)
{
	return 0;
}

int CParamInterface::MultimediaDataReply(int id, unsigned char  retryPackCnt, short* retryIdList)
{
	return 0;
}

static DWORD ASCII2DWORD(char *src)
{
	if(!src)
		return 0;
	printf("In ascii2int, src|%s|\n", src);

	char enctypeKey[10] = "_enctype";
	char *tmp = strstr(src, enctypeKey);
	if(tmp)
	{
		int len = strlen(src) - strlen(tmp) - 7;//"mediaID"
		if(len == 4)
			return (*(src + 7 + len - 1) - 48) + (*(src + 7 + len - 2) - 48)*10 + (*(src + 7 + len - 3) - 48)*100 + (*(src + 7 + len - 4) - 48)*1000;// "mediaID"
		if(len == 3)
			return (*(src + 7 + len - 1) - 48) + (*(src + 7 + len - 2) - 48)*10 + (*(src + 7 + len - 3) - 48)*100;// "mediaID"
		if(len == 2)
			return (*(src + 7 + len - 1) - 48) + (*(src + 7 + len - 2) - 48)*10;// "mediaID"
		if(len == 1)
			return (*(src + 7 + len - 1) - 48);// "mediaID"
	}
}



int CParamInterface::MdSearchCmd(NetMdIdxCmd mdIdxCmd, vector <IdxCmdRes> &stResultList)
{
	printf("In MdSearchCmd, Index type: %02x\n", mdIdxCmd.type);
	printf("In MdSearchCmd, mdIdxCmd.chn: %02x\n", mdIdxCmd.chn);
	printf("In MdSearchCmd, mdIdxCmd.event: %02x\n", mdIdxCmd.event);
//	printf("In MdSearchCmd, mdIdxCmd.btime: %s\n", mdIdxCmd.btime);
//	printf("In MdSearchCmd, mdIdxCmd.etime: %s\n", mdIdxCmd.etime);
	stResultList.clear();

	DIR           *dir;
	struct dirent *entry;
	char          *name;
	char 		  *tmpKey;
	IdxCmdRes tmp;
	memset(&tmp, 0, sizeof(tmp));
	dir = opendir("./test");
	while((entry = readdir(dir)))
	{
		name = entry->d_name;
		if((name && *name == '.' && (!name[1] || (name[1] == '.' && !name[2]))))
			continue;
		printf("In MdSearchCmd, name|%s|\n", name);
		if(0 == mdIdxCmd.type)//文件类型
    	{
    		char chnKey[6] = "jpg";
			tmpKey = strstr(name, chnKey);
			if(!tmpKey)
			{
				continue;
			}
		}else if(1 == mdIdxCmd.type)//文件类型
		{
			char chnKey[6] = "mp3";
			tmpKey = strstr(name, chnKey);
			if(!tmpKey)
			{
				continue;
			}
		}else if(2 == mdIdxCmd.type)//文件类型
		{
			char chnKey[6] = "mp4";
			tmpKey = strstr(name, chnKey);
			if(!tmpKey)
			{
				continue;
			}
		}
		char chnKey[6] = {0};
		memset(chnKey, 0, sizeof(chnKey));
		sprintf(chnKey, "chn%u", mdIdxCmd.chn);
		char eventKey[8] = {0};
		memset(eventKey, 0, sizeof(eventKey));
		sprintf(eventKey, "event%u", mdIdxCmd.event);
		printf("In MdSearchCmd, mdIdxCmd.chn|%02x|\n", mdIdxCmd.chn);
		if(mdIdxCmd.chn == 0)	//所有查询满足所有通道的多媒体文件
		{
			tmpKey = strstr(name, eventKey);
			if(tmpKey)
			{
				tmpKey = NULL;
				char btimeKey[12] = "begintime";
				printf("btimeKey [%s]\n", btimeKey);
				tmpKey = strstr(name, btimeKey);
				if(tmpKey)
				{
					char btime[14] = {0};
					memcpy(btime, tmpKey + 9, 12);// "begintime" yymmddhhmmss
					ASCtoBCD(tmp.btime, btime, 12);
					tmpKey = NULL;
					
					char etimeKey[10] = "endtime";
					printf("etimeKey [%s]\n", etimeKey);
					tmpKey = strstr(name, etimeKey);
					if(tmpKey)
					{
						char etime[14] = {0};
						memcpy(etime, tmpKey + 7, 12);// "endtime" yymmddhhmmss
						ASCtoBCD(tmp.etime, etime, 12);
						tmpKey = NULL;
						
						char IDKey[10] = "mediaID";							
						tmpKey = strstr(name, IDKey);
						if(tmpKey)
						{
							tmp.mediaId = ASCII2DWORD(tmpKey);
							printf("In MdSearchCmd, tmp.mediaId|%d|\n", tmp.mediaId);
							tmp.type	= mdIdxCmd.type;
							tmp.chn		= mdIdxCmd.chn;
							printf("In MdSearchCmd, tmp.chn|%d|\n", tmp.chn);
							tmp.event	= mdIdxCmd.event;
							sprintf(tmp.fileName, "/system/bin/test/%s", name);
							printf("In MdSearchCmd, tmp.fileName|%s|\n", tmp.fileName);
							stResultList.push_back(tmp);
							printf("In MdSearchCmd, stResultList[0].chn|%d|\n", stResultList[0].chn);
							memset(&tmp, 0, sizeof(tmp));
						}
					}
				}
			}
		}else
		{
			if(strstr(name, eventKey) && strstr(name, chnKey))
			{
				char btimeKey[12] = "begintime";
				printf("btimeKey [%s]\n", btimeKey);
				tmpKey = strstr(name, btimeKey);
				if(tmpKey)
				{
					char btime[14] = {0};
					memcpy(btime, tmpKey + 9, 12);// "begintime" yymmddhhmmss
					ASCtoBCD(tmp.btime, btime, 12);
					tmpKey = NULL;
					
					char etimeKey[10] = "endtime";
					printf("etimeKey [%s]\n", etimeKey);
					tmpKey = strstr(name, etimeKey);
					if(tmpKey)
					{
						char etime[14] = {0};
						memcpy(etime, tmpKey + 7, 12);// "endtime" yymmddhhmmss
						ASCtoBCD(tmp.etime, etime, 12);
						tmpKey = NULL;

						char IDKey[10] = "mediaID";
						tmpKey = strstr(name, IDKey);
						if(tmpKey)
						{
							tmp.mediaId = ASCII2DWORD(tmpKey);
							tmp.type	= mdIdxCmd.type;
							tmp.chn		= mdIdxCmd.chn;
							tmp.event	= mdIdxCmd.event;
							sprintf(tmp.fileName, "/system/bin/test/%s", name);
							stResultList.push_back(tmp);
							memset(&tmp, 0, sizeof(tmp));
						}
					}
				}
			}
		}
	}
	
	return 0;
}

int CParamInterface::SingleMdSearchCmd(SingleNetMdIdxCmd SingleMdIdxCmd, NetMdUploadCmd &cmd)
{
	printf("In SingleMdSearchCmd, Index deleteFlag: %d\n", SingleMdIdxCmd.deleteFlag);
	printf("In SingleMdSearchCmd, Index mediaId: %d\n", SingleMdIdxCmd.mediaId);
	
	int ret = 1;
	DIR           *dir;
	struct dirent *entry;
	char          *name;
	dir = opendir("./test");
	while((entry = readdir(dir)))
	{
		name = entry->d_name;
		if((name && *name == '.' && (!name[1] || (name[1] == '.' && !name[2]))))
			continue;
		char IDKey[10] = {0};
		memset(IDKey, 0, sizeof(IDKey));
		sprintf(IDKey, "mediaID%d", SingleMdIdxCmd.mediaId);
		printf("name[%s]\n", name);
		printf("IDKey [%s]\n", IDKey);
		if(strstr(name, IDKey))
		{
			cmd.mId = SingleMdIdxCmd.mediaId;
		
			char *tmp = NULL;
			//mediaID0_enctype0_event0_chn0_begintime000000000000_endtime000000000000.jpg
			sprintf(cmd.filename, "/system/bin/test/%s", name);
			printf("find the file|%s| \n", cmd.filename);

			char enctypeKey[10] = "enctype";
			printf("enctypeKey [%s]\n", enctypeKey);
			tmp = strstr(name, enctypeKey);
			if(tmp)
			{
				cmd.enctype = *(tmp + 7);	// "enctype"
				tmp = NULL;
			}else
			{
				ret = 1;
				break;
			}
			char eventKey[10] = "event";
			printf("eventKey [%s]\n", eventKey);
			tmp = strstr(name, eventKey);
			if(tmp)
			{
				cmd.event = *(tmp + 5);	// "event"
				tmp = NULL;
			}else
			{
				ret = 1;
				break;
			}
			char chnKey[10] = "chn";
			printf("chnKey [%s]\n", chnKey);
			tmp = strstr(name, chnKey);
			if(tmp)
			{
				cmd.chn = *(tmp + 3);	// "chn"
				tmp = NULL;
			}else
			{
				ret = 1;
				break;
			}
			char btimeKey[12] = "begintime";
			printf("btimeKey [%s]\n", btimeKey);
			tmp = strstr(name, btimeKey);
			if(tmp)
			{
				char btime[14] = {0};
				memcpy(btime, tmp + 9, 12);// "begintime" yymmddhhmmss
				ASCtoBCD(cmd.btime, btime, 12);
				tmp = NULL;
			}else
			{
				ret = 1;
				break;
			}
			char etimeKey[10] = "endtime";
			printf("etimeKey [%s]\n", etimeKey);
			tmp = strstr(name, etimeKey);
			if(tmp)
			{
				char etime[14] = {0};
				memcpy(etime, tmp + 7, 12);// "endtime" yymmddhhmmss
				ASCtoBCD(cmd.etime, etime, 12);
				tmp = NULL;
			}else
			{
				ret = 1;
				break;
			}
			ret = 0;
			break;
		}
#if 0
			//上传找到的单个文件

			//判断是否删除单个文件
			if(SingleMdIdxCmd.deleteFlag)
			{
				//删除检索文件操作，这里根据具体名字要改
				char RMcmd[64] = {0};
				memset(RMcmd, 0, sizeof(RMcmd));
				sprintf(RMcmd, "rm ./test/%d\n", SingleMdIdxCmd.mediaId);
				printf("In SingleMdSearchCmd, RMcmd|%s|\n", RMcmd);
				system(RMcmd);
			}else
			{
				printf("CMD:8805 check file:%s exists, deleteFlag is %d\n", SingleMdIdxCmd.deleteFlag);
				ret = 0;
			}
#endif			
	}
	closedir(dir);
#if 0
	//检查文件是否删除
	DIR           *checkDir;
	struct dirent *checkEntry;
	char          *checkName;
	checkDir = opendir("./test");
	int fileExistFlag = -1;
	while((checkEntry = readdir(checkDir)))
	{
		checkName = checkEntry->d_name;
		if(checkName && *checkName == '.' && (!checkName[1] || (checkName[1] == '.' && !checkName[2])))
			continue;
		char checkKey[4] = {0};
		memset(checkKey, 0, sizeof(checkKey));
		sprintf(checkKey, "%d", SingleMdIdxCmd.mediaId);
		printf("name[%s]\n", name);
		printf("key [%s]\n", checkKey);
		if(strstr(checkName, checkKey))	//查询到文件
		{
			fileExistFlag = 1;
			break;
		}else							//查询不到文件
		{
			fileExistFlag = 0;
		}
	}
	closedir(checkDir);

	if(fileExistFlag)	//查询到文件
	{
		if(SingleMdIdxCmd.deleteFlag)//deleteFlag = 1，文件需要删除
		{
			ret = 1;	//文件需要删除，经过删除文件还存在，操作失败
			printf("find file, deleteFlag is %d, exec failed...\n", SingleMdIdxCmd.deleteFlag);
		}else
		{
			ret = 0;	//文件不需要删除，查询到文件，查询成功
			printf("find file, deleteFlag is %d, exec succeed...\n", SingleMdIdxCmd.deleteFlag);
		}
	}else							//查询不到文件
	{
		if(SingleMdIdxCmd.deleteFlag)//deleteFlag = 1，文件需要删除
		{
			ret = 0;	//文件需要删除，经过删除文件不存在，操作成功
			printf("can't find file, deleteFlag is %d, exec succeed...\n", SingleMdIdxCmd.deleteFlag);
		}else
		{
			ret = 1;	//文件不需要删除，查询不到文件，查询失败
			printf("can't find file, deleteFlag is %d, exec failed...\n", SingleMdIdxCmd.deleteFlag);
		}
	}
#endif
	return ret;
}


int CParamInterface::MdUploadCmd(NetMdUploadCmd& mdUploadCmd)
{
	return 0;
}

int CParamInterface::MdRecordVoiceCmd(NetRecordCmd& recordCmd)
{
	return 0;
}

#define MAX_RESOLUTION_INDEX (8)
static int g_resolution[MAX_RESOLUTION_INDEX][2] = {
	{320, 240}, {640, 480}, {800, 600}, 
	{1024, 768}, {176, 144}, {352, 288},
	{704, 288}, {704, 576}
};

typedef struct {
	int witdh; 
	int height;
}Resolution;

int CParamInterface::MdShotCmd(NetMdShotCmd &mdShotCmd, vector<ShotResult> &resultList)
{
	if(mdShotCmd.res > MAX_RESOLUTION_INDEX){
		return 1;
	}
	Resolution res = (*(Resolution *)&g_resolution[mdShotCmd.res - 1]);
	if(mdShotCmd.cmd)
	{
	/*setting resolution and trriger snapshot*/
	/*											
	 *
	 *
	 */
	/*snapshot testing*/
	}
	int ret = 1;
	
	switch(mdShotCmd.cmd)
	{
		case 0:				//停止拍摄
			ret = StopCameraShot();
			break;
		case 65535:			//录像
			ret = StartCameraShot(mdShotCmd, resultList);
			break;
		default://拍照
			ret = StartSnapShot(mdShotCmd, resultList);
			break;
	}

	/*setting resolution and trriger snapshot*/
	/*											
	 *
	 *
	 */
	/*snapshot testing*/
	mediaUpload = true;	

	return ret;
}

void CParamInterface::NetSetGnssCmsLinkState(bool state)
{
	
}

void CParamInterface::NetSetGnssCmssAuthorize(bool state)
{
	
}

void CParamInterface::SaveAuthCode(char* m_szAuthCode, int len)
{
	if(m_szAuthCode != NULL){
		FILE *fp = NULL;
		fp = fopen(CONFIGFILE, "w+");
		if(fp != NULL)
		{
			fwrite(m_szAuthCode, 1, len, fp);
			fclose(fp);
		}
	}
}

int CParamInterface::ReqIFrame(int chn,int streamType)
{
	return -1;
}

int CParamInterface::RegisterAVFrame(void* pUser, pFunPutFrameCallBack pFun,int chn,int streamType)
{
	return 1;
}

int CParamInterface::UnRegisterAVFrame(void* pUser,int chn,int streamType)
{
	return 1;
}

