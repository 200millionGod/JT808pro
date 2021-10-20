/*************************************************************
 * Copyright			: 
 * FileName				: TRML_gps_info.cpp
 * Description			: TRML_gps_info.cpp implement
 * Author		        : 
 * Created on			: 20210918
 *************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>

#include <sys/stat.h>
#include <sys/types.h> 

#include "../inc/TRML_gps_info.h"
#include "../inc/TRML_tinyxml2.h"
#include "../inc/TRML_util.h"
#include "../inc/TRML_third_party.h"
//#include "JT808App.h"



using namespace tinyxml2;

static int currCount = 0;	/* 写数据的个数 */
static int startIndex = 0;	/* 数据开始 */
static int endIndex = 0;	/* 数据结束 */

time_t lastSendTime;		/* 最后一次发送数据的时间 */

/*
	获取存储GPS信息的开始结束位置
*/
int getSaveGpsInfoPosition(const char* xmlPath,SaveGpsInfo* gps_info)
{
	XMLDocument doc;

    if(doc.LoadFile(xmlPath)!=0)
    {
        //cout<<"load xml file failed"<<endl;
		printf("Query load xml file failed");
        return 0;
    }
    XMLElement* root = doc.RootElement();
	XMLElement* LengthInfoNode = root->FirstChildElement("LengthInfo");
	const XMLAttribute* startAttr = LengthInfoNode->FirstAttribute();
	printf("LengthInfoNode start  is %s<<  %d<< surfaceAttr->Value()\n",startAttr->Name(),atoi(startAttr->Value()));
	gps_info->start = atoi(startAttr->Value());
	startAttr = startAttr->Next();
	printf("LengthInfoNode end  is %s<<  %d<< surfaceAttr->Value()\n",startAttr->Name(),atoi(startAttr->Value()));
	gps_info->end = atoi(startAttr->Value());
	
	return 1;
}

//function:GPS根据索引号获取用户节点
//param:root:xml文件根节点；index：索引号
//return：用户节点
XMLElement* queryNodeByIndex(XMLElement* root,int index)
{
	XMLElement* LengthInfoElement=root->FirstChildElement("LengthInfo");
	XMLElement* BodyStructElement = LengthInfoElement->FirstChildElement("BodyStruct");
	XMLElement* InfoElement = BodyStructElement->FirstChildElement("info");

	while(InfoElement != NULL)
	{
		const XMLAttribute* surfaceAttr = InfoElement->FirstAttribute();
		if(atoi(surfaceAttr->Value()) == index)
		{
			//printf("find surfaceAttr->Name()  is %s<<  %d<< surfaceAttr->Value()\n",surfaceAttr->Name(),atoi(surfaceAttr->Value()));
			break;
		}
		InfoElement = InfoElement->NextSiblingElement( "info" );
		#if 0
		printf("query1 surfaceAttr->Name()  is %s<<  %d<< surfaceAttr->Value()\n",surfaceAttr->Name(),atoi(surfaceAttr->Value()));
		surfaceAttr = surfaceAttr->Next();
		printf("query2 surfaceAttr->Name()  is %s<<  %d<< surfaceAttr->Value()\n",surfaceAttr->Name(),atoi(surfaceAttr->Value()));
		surfaceAttr = surfaceAttr->Next();
		printf("query3 surfaceAttr->Name()  is %s<<  %s<< surfaceAttr->Value()\n",surfaceAttr->Name(),surfaceAttr->Value());
		#endif
	}
	//printf("Read over\n");
	return InfoElement;
}

int queryGpsNodeData(const char* xmlPath,int index)
{
	XMLDocument doc;

	if(doc.LoadFile(xmlPath)!=0)
	{
		//cout<<"load xml file failed"<<endl;
		printf("Query load xml file failed");
		return 0;
	}
	XMLElement* root=doc.RootElement();
	XMLElement* DataNode = queryNodeByIndex(root,4);
	// 找到节点 取出对应的数据
	const XMLAttribute* surfaceAttr = DataNode->FirstAttribute();
	printf("index surfaceAttr->Name()  is %s<<  %d<< surfaceAttr->Value()\n",surfaceAttr->Name(),atoi(surfaceAttr->Value()));
	surfaceAttr = surfaceAttr->Next();
	printf("length surfaceAttr->Name()  is %s<<  %d<< surfaceAttr->Value()\n",surfaceAttr->Name(),atoi(surfaceAttr->Value()));
	surfaceAttr = surfaceAttr->Next();
	printf("body surfaceAttr->Name()  is %s<<  %s<< surfaceAttr->Value()\n",surfaceAttr->Name(),surfaceAttr->Value());

	return 1;
}

/*
	获取存储GPS具体的信息
*/
int getSaveGpsInfoData(const char* xmlPath,int index,SaveGpsData* gps_data)
{
	XMLDocument doc;

    if(doc.LoadFile(xmlPath)!=0)
    {
        //cout<<"load xml file failed"<<endl;
		printf("Query load xml file failed");
        return 0;
    }
	
	XMLElement* root=doc.RootElement();
	XMLElement* DataNode = queryNodeByIndex(root,index);
	// 找到节点 取出对应的数据
	const XMLAttribute* surfaceAttr = DataNode->FirstAttribute();
	//printf("index surfaceAttr->Name()  is %s<<  %d<< surfaceAttr->Value()\n",surfaceAttr->Name(),atoi(surfaceAttr->Value()));
	gps_data->index = atoi(surfaceAttr->Value());
	surfaceAttr = surfaceAttr->Next();
	//printf("length surfaceAttr->Name()  is %s<<  %d<< surfaceAttr->Value()\n",surfaceAttr->Name(),atoi(surfaceAttr->Value()));
	gps_data->length = atoi(surfaceAttr->Value());
	surfaceAttr = surfaceAttr->Next();
	//printf("body surfaceAttr->Name()  is %s<<  %s<< surfaceAttr->Value()\n",surfaceAttr->Name(),surfaceAttr->Value());
	strncpy(gps_data->buf, surfaceAttr->Value(), sizeof(gps_data->buf));

	return 1;
}

//function:insert XML node
//param:xmlPath:xml文件路径; count 索引,length 存数据的长度，pData 要存的数据
//return:0:成功; 非0:失败
int insertXMLNode(const char* xmlPath,int count,int length,void * pData)
{
    XMLDocument doc;
    int res = doc.LoadFile(xmlPath);
    if(res!=0)
    {
        //cout<<"load xml file failed"<<endl;
		printf("load xml file failed\n");        
		return res;
    }
    XMLElement* root = doc.RootElement();
	XMLElement* LengthInfoNode = root->FirstChildElement("LengthInfo");
	
	currCount = count+1;
	LengthInfoNode->SetAttribute( "End", currCount ); // 设置元素属性
	
	XMLElement* BodyStructNode = LengthInfoNode->FirstChildElement("BodyStruct");
	
	XMLElement* DataNode = doc.NewElement("info");
	DataNode->SetAttribute("Index",count);
	DataNode->SetAttribute("Length",length);
	DataNode->SetAttribute("GpsBody",(char *)pData);	//存储GPS数据体 HEX转String格式
    BodyStructNode->InsertEndChild(DataNode);

    return doc.SaveFile(xmlPath);
}


//function：create a xml file
//param：xmlPath:xml文件路径
//return:0,成功，非0，失败
int createXML(const char* xmlPath)
{
    const char* declaration ="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    XMLDocument doc;
    doc.Parse(declaration);//会覆盖xml所有内容

    //添加申明可以使用如下两行
    //XMLDeclaration* declaration=doc.NewDeclaration();
    //doc.InsertFirstChild(declaration);

    XMLElement* root=doc.NewElement("GPSINFO");
    doc.InsertEndChild(root);

    // 创建子元素<LengthInfo>
    XMLElement* LengthInfoElement = doc.NewElement( "LengthInfo" );
    LengthInfoElement->SetAttribute( "Start", "0" ); // 设置元素属性
    LengthInfoElement->SetAttribute( "End", "0" ); // 设置元素属性
    root->InsertEndChild( LengthInfoElement );

	XMLElement* BodyStructElement = doc.NewElement("BodyStruct");
    LengthInfoElement->InsertEndChild(BodyStructElement);
	
    return doc.SaveFile(xmlPath);
}

int createFile(const char *folderpath, const char *filepath)
{
    if (access(folderpath, F_OK) != 0)
    {
        if (mkdir(folderpath, 0777) != 0)
        {
            printf("Create folder %s dir error...", folderpath);
            return -1;
        }
    }

    if (filepath == NULL)
    {
        return 0;
    }

	/*
    FILE *fd = fopen(filepath, "a");
    if (fd == NULL)
    {
        printf("Open file %s error...", filepath);
        return -2;
    }
    fclose(fd);
	*/
	if(access(filepath, F_OK) != 0)
	{
		createXML(filepath);
	}
		
    return 0;

}

//PosStautsInfo(char* pBuffer, void* pGnssData,void* pAlarmData, WORD devstatus)
int PackageByte(void* pBuffer,BYTE data)
{
	((BYTE*)pBuffer)[0] = data;
	return 1;
}
int PackageWord(void* pBuffer,WORD data)
{
	WORD m = htons(data);
	memcpy(pBuffer, &m, 2);
	return 2;
}
int PackageDword(void* pBuffer,DWORD data)
{
	DWORD n = htonl(data);
	memcpy(pBuffer, &n, 4);
	return 4;
}
int PackageBytes(void* pBuffer,BYTE* pData,BYTE len)
{
	memcpy(pBuffer,pData,len);
	return len;
}

static int packageSaveData(char* pBuffer,struGPSData* pGnss,WORD devstatus)
{
	WORD off;
	DWORD m,n;
	BYTE szData[8];
	//struGPSData* pGnss = (struGPSData*)pGnssData;
	//AlarmData_s* pAlarm = (AlarmData_s*)pAlarmData;

	if (pGnss->GpsLocation){
		devstatus |= (1 << 18);
	}

	if (pGnss->BeiDouPosition){
		devstatus |= (1 << 19);
	}

	devstatus |= pGnss->status;
	
	/* 报警标志 */
	PackageDword(pBuffer, 0);	//pAlarm->alarm
	/* 设备状态 */
	PackageDword(pBuffer+4, devstatus);
	if(pGnss->cGpsStatus=='A'){
		m = pGnss->fLatitude;
		n = pGnss->fLongitude;
	} else {
		m = 0;
		n= 0;
	}
	/* 纬度 */
	PackageDword(pBuffer+8,m);
	/* 经度 */
	PackageDword(pBuffer+12,n);
	/* 高程 */
	PackageWord(pBuffer+16,pGnss->Altitude);
	/* 速度 */
	PackageWord(pBuffer+18,pGnss->usSpeed/10);
	/* 方向 */
	PackageWord(pBuffer+20,pGnss->usGpsAngle/100);
	/* 时间 */
	time_t curtime =  time(NULL);
	tm *ptm = localtime(&curtime);
	szData[0] = (((ptm->tm_year-100)/10) << 4)+(ptm->tm_year%10);
	szData[1] = (((ptm->tm_mon+1)/10) << 4)+((ptm->tm_mon+1)%10);
	szData[2] = ((ptm->tm_mday/10) << 4)+(ptm->tm_mday%10);
	szData[3] = ((ptm->tm_hour/10) << 4)+(ptm->tm_hour%10);
	szData[4] = ((ptm->tm_min/10) << 4)+(ptm->tm_min%10);
	szData[5] = ((ptm->tm_sec/10) << 4)+(ptm->tm_sec%10);
	PackageBytes(pBuffer+22,szData,6);

	off = 28;

	return off;
}

/*
	十六进制字符串转换成十六进制数组
*/
int StringToHex(char *str, unsigned char *out, unsigned int *outlen)
{
    char *p = str;
    char high = 0, low = 0;
    int tmplen = strlen(p), cnt = 0;
    tmplen = strlen(p);
    while(cnt < (tmplen / 2))
    {
        high = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
        low = (*(++ p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *(p) - 48 - 7 : *(p) - 48;
        out[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));
        p ++;
        cnt ++;
    }
    if(tmplen % 2 != 0) out[cnt] = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
    
    if(outlen != NULL) *outlen = tmplen / 2 + tmplen % 2;
    return tmplen / 2 + tmplen % 2;
}

int SaveGpsInfo_Init(void)
{
	SaveGpsInfo gpsInfo;
	
	if(access(SAVE_GPS_INFO_PATH, F_OK) != 0)
	{
		return 0;
	}
	//读出已保存的信息里面 有多少个数据
	int ret = getSaveGpsInfoPosition(SAVE_GPS_INFO_PATH,&gpsInfo);
	if(ret)
	{
		printf("ret = %d,info is %d,%d\n",ret,gpsInfo.start,gpsInfo.end);
	}
	
	currCount = gpsInfo.end;	//保存的时候从已保存后面开始 eg：当前存储数据索引 0,1,2,3  那么gpsInfo.end = 4  
	startIndex = gpsInfo.start;
	endIndex = gpsInfo.end;		//一共保存了多少数据
	
	return 1;
}

/* 保存GPS信息 */
int saveGpsInfo(void)
{
	//如果不存在路径文件 就创建文件  有就打开文件
	struGPSData SaveGps;
		
	char strData[256] = {0};
	WORD len;

	char hexToStrData[100] = {0};

	if(currCount >= GPS_CACHE_MAX)
		return 0;
	//printf("currCount = %d,GPS_CACHE_MAX = %d\n",currCount,GPS_CACHE_MAX);
    if (createFile(ECAR_DATA_PATH, SAVE_GPS_INFO_PATH) != 0)
    {
        printf("saveGpsInfo %s createFile error", SAVE_GPS_INFO_PATH);
        return 0;
    }

	printf("g_mgr.test_mode = %d\n", g_mgr.test_mode);
	if(g_mgr.test_mode)	/*g_mgr.test_mode*/
	{
		create_test_GpsInfo(SaveGps);
	}
	else
	{
		getGpsInfo(SaveGps);
	}
	#if 0
	/* 如果没有定位就返回 */
	if(!SaveGps.fLatitude)
		return 0;
	#endif
	len = packageSaveData(strData,&SaveGps,0);

	#if 0
	int i;
	for(i = 0;i < len;i++)
	{
		printf("%02x",strData[i]);
	}
	printf("\n need save Gpsinfo\n");
	#endif
	//memset(hexToStrData,0,20);

	for(int i=0;i<len;i++)
	{
		sprintf(hexToStrData + strlen(hexToStrData),"%02X",strData[i]);
	}

	//写入数据
	printf("\n Gps Save Info :%s ,len = %d \n",hexToStrData,len);

	insertXMLNode(SAVE_GPS_INFO_PATH,currCount,len,hexToStrData);
	
	return 1;

	
}

/* 读GPS信息 */
void readGpsInfo(void)
{
	SaveGpsData gpsData;
	
	unsigned char out[50];
    
    unsigned int outlen = 0;

	getSaveGpsInfoData(SAVE_GPS_INFO_PATH,35,&gpsData);
	
	printf("Read gps data is length %d,data is %s\n",gpsData.length,gpsData.buf);

	int result = StringToHex(gpsData.buf,out,&outlen);
	if(result == gpsData.length)
	{
		printf("Conversion success! result = %d\n",result);
	}
	for(unsigned int cnt = 0; cnt < outlen; cnt ++)
    {
        printf("%02X ", out[cnt]);
    } 
	printf("\n");

	//JT808_APP_ReportSaveGpsInfo(out,gpsData.length);

}

//删除GPS具体数据BodyStruct节点	调试OK
//function:删除节点，xml框架还在
//param:xmlPath:xml文件路径；
//return：bool
bool deleteBodyStructNode(const char* xmlPath)
{
	XMLDocument doc;
	if(doc.LoadFile(xmlPath)!=0)
	{
		printf("load xml file failed\n");
		return false;
	}
	XMLElement* root=doc.RootElement();
	//doc.DeleteNode(root);//删除xml所有节点
	XMLElement* LengthInfoElement=root->FirstChildElement("LengthInfo");
	XMLElement* BodyStructElement = LengthInfoElement->FirstChildElement("BodyStruct");

	if(BodyStructElement != NULL)
	{
		LengthInfoElement->DeleteChild(BodyStructElement); //删除指定节点

		printf("delete gps end\n");
	}
	
	LengthInfoElement->SetAttribute( "start", 0 ); // 设置元素属性
	LengthInfoElement->SetAttribute( "End", 0 ); // 设置元素属性
	
	if(doc.SaveFile(xmlPath)==0)
		return true;

	return false;
	
}

/* 删除整个保存GPS数据的xml */
int FileRemove(const char* fname)
{
    return remove(fname);
}

void ReadGps(void)
{
	SaveGpsData gpsData;

	unsigned char GpsBodybuf[1024] = {0};
	
	unsigned char out[50];
    
    unsigned int outlen = 0;
	
	/*
		如果存储<30个 一包直接上传完
		如果存储>30个 一包只传30个
	*/
	unsigned int postion = 0;				//GPS_REPORT_MAX 这么多包组完包后的总长度

	static bool sendFlag = true;

	unsigned int packgetNum = 0;			// 一共有多少包 To JT808lib

	static unsigned int offset = 0;
	static unsigned int lastPackNum = 0;	//保存上一次存了多少包

	time_t nCur;
	
	if(sendFlag)
	{
		for(int i = startIndex+offset;i < endIndex;i++) 	
		{	
			getSaveGpsInfoData(SAVE_GPS_INFO_PATH,i,&gpsData);
			printf("Read gps data is index is %d, length is %d,data is %s\n",gpsData.index,gpsData.length,gpsData.buf);
	
			int result = StringToHex(gpsData.buf,out,&outlen);
			if(result == gpsData.length)
			{
				printf("Conversion success! result = %d\n",result);
			}
			for(unsigned int cnt = 0; cnt < outlen; cnt ++)
			{
				printf("%02X ", out[cnt]);
			} 
			printf("\n");
			
			PackageWord(&GpsBodybuf[postion],gpsData.length);		//组包有一个字节的长度
			memcpy(&GpsBodybuf[postion+2],out,gpsData.length);
			postion = postion+gpsData.length+2; 					//2代表数据体里面还有2个字节	 代表  位置汇报数据体的长度
			printf("bodyTotalLen = %d\n",postion);
			
			packgetNum++;
			if((i == endIndex-1)||(packgetNum == GPS_REPORT_MAX)) 	//到了最后不组包了， 假设0 1 2 3 	 总共4组数据 i = 3的时候跳出循环
				break;
			
		}
		#if 0
		for(unsigned int j = 0; j < postion; j ++)
		{
			printf("%02X ", GpsBodybuf[j]);
		}
		#endif
		printf(" \n packget over packgetNum = %d!\n",packgetNum);
		sendFlag = false;
		//offset += packgetNum;
		lastPackNum = packgetNum;
		JT808_APP_ReportSaveGpsInfo(GpsBodybuf,postion,packgetNum);
		
		lastSendTime = time(NULL);
		printf("get lastSendTime \n");
		
	}
	else	
	{
		if(JT808_APP_GetUpdataGpsStatus())
		{
			if(lastPackNum < GPS_REPORT_MAX)
			{
				//判断全部发送完毕
				//deleteBodyStructNode(SAVE_GPS_INFO_PATH);
				if(!FileRemove(SAVE_GPS_INFO_PATH))	//删除成功
				{
					printf("0x0704  send end,lastPackNum = %d\n",lastPackNum);
					offset = 0;
					sendFlag = true;
					currCount = 0;		
				}
			}
			else
			{
				sendFlag = true;
				offset += lastPackNum;
			}
		}
		else
		{
			/* 发送一帧后超时都没有收到平台回应  继续发 ，间隔时间 GPS_SEND_TIME_OUT*/
			nCur = time(NULL);
			if(nCur >= (lastSendTime + GPS_SEND_TIME_OUT))
			{
				sendFlag = true;
				printf("gps save data send timeout\n ");
			}
		}
	}
	
}

