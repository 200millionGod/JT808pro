#include <stdio.h>
#include <stdarg.h> 
#include <time.h>
#include "Public.h"

unsigned int  GetTickCount()
{
	struct timespec ts;
	
   	clock_gettime(CLOCK_MONOTONIC,  &ts);        
	unsigned int dwNow = ts.tv_sec  * 1000 + ts.tv_nsec / (1000 * 1000);

	return dwNow;
}

bool IsTimeOut(unsigned int dwTime, unsigned int dwTimeout)
{
	bool bRtn = false;
	unsigned int dwNow = GetTickCount();
	//已经过去了多长时间(进程运行N天归0)
	if (dwNow >= dwTime)
	{
		if ((dwNow - dwTime) >= dwTimeout)
		{
			bRtn = true;
		}
	}
	else
	{
		unsigned int dwMax = 0xFFFFFFFF; 
		if (((dwMax - dwTime) + dwNow) >= dwTimeout)
		{
			bRtn = true;
		}
	}
	
	return bRtn;
}

