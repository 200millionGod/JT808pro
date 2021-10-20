#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "MutexLock.h"
#include "GnssNetLog.h"


CNetLog g_log;

CNetLog::CNetLog()
{
	m_nFileOffset = 0;
}

int CNetLog::Putout(const char* fmt, ...)
{
	CAutoLock lock( &m_lock );

	char log[LOG_FILE_BUFF_LEN];
	memset(log, 0 , sizeof(log));
#if 0
	//在所写 的日志前加入时间
	time_t t;
	t = time(NULL);
	int offset = strlen(ctime(&t)) - 1;

	char*  gap = ",  ";
	memcpy(log, ctime(&t), offset);
	memcpy(log + offset, gap, strlen(gap));

	offset += strlen(gap);
	
	va_list v1;
	va_start(v1, fmt);
	vsprintf(log + offset, fmt, v1);
	va_end(v1);

#endif	
	
	int fid = open(LOG_FILE_NAME, O_WRONLY | O_APPEND );
	
	if(-1 == fid)
	{
		if(ENOENT == errno)
		{
			fid = open(LOG_FILE_NAME, O_WRONLY | O_CREAT | O_EXCL , 0644);
			if(-1 == fid)
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	
	m_nFileOffset = lseek(fid, 0, SEEK_END);
	if ( m_nFileOffset > LOG_FILE_MAX_SIZE) 
	{
		char cmd[128] = {0};
		sprintf(cmd, "mv %s %s", LOG_FILE_NAME, LOG_FILE_BACKUP_NAME);
		system(cmd);
		close(fid);
		fid = open(LOG_FILE_NAME,  O_WRONLY | O_TRUNC);
		
	}
	//printf("%s\n",log);
	write(fid, log, strlen(log));
	close(fid);
	return 0;
}

