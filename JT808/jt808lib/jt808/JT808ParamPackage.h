#ifndef _JT808_Param_PACKAGE_H_
#define _JT808_Param_PACKAGE_H_

#include "Defines.h"

class CJT808ParamBase
{
public:
	void Init(JT808_Config_S* pSysCfg);
	void Get(char*& pData,DWORD id);
	DWORD Set(char*& pData,DWORD id);	
private:
	DWORD Exchange(char*& pData,DWORD id);
	void ExchangeByte(char*& pData,BYTE& value);
	void ExchangeWord(char*& pData,WORD& value);
	void ExchangeDWord(char*& pData,DWORD& value);
	void ExchangeString(char*& pData,char* value,int max);
	void ExchangeBCD(char*& pData,BCD* value,int len);
	JT808_Config_S* pConfig;
	DWORD flag;
};

#endif	//_JT808_PROTOCOL_PACKAGE_H_

