#ifndef _JT808_Param_Parse_H_
#define _JT808_Param_Parse_H_


//#include "TM_tinyxml2.h"
#include "Defines.h"
#include "TRML_common.h"

class TerminalXMLParse
{

public:
	TerminalXMLParse();
	~TerminalXMLParse();
public:
	int LoadDfssParam(const char* config_path,DSM_Config* cfg);
	int LoadIfvParam(const char* config_path,ADAS_Config_S* cfg);
	int LoadTerminalParam(const char* config_path,JT808_Config_S* cfg);
	int LoadDefaultParam(const char* config_path, JT808_Config_S* cfg);
	int saveTerminalParam(const char *pXmlfile, JT808_Config_S *tParams);
};

#endif
