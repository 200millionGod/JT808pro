/*-------------------------------------------------------------------------
 调试日志模块实现
-------------------------------------------------------------------------*/
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>
#include "debuglog.h"
#include <stdio.h>
#include <stdarg.h>

const int CDebugLog::nLogDebugFileSize = (10 * 1024 * 1024);
const int CDebugLog::nLogContentMax = 1024;
const int LOG_SUCCESS = 0;
const int LOG_FAILED = -1;

const char * CDebugLog::strDbgFileName = "mdvrdebug.log";

CDebugLog::CDebugLog()
{
	m_data.Init(0x20000);
}

CDebugLog::~CDebugLog()
{
	
}

int CDebugLog::WriteFile(const char * const filename, const char * const str,const int len)
{
	int fid = open(filename, O_RDWR | O_CREAT | O_APPEND, 0666);
	if(-1 == fid)
		return LOG_FAILED;

	SetLock(fid, 2);
	if(nLogDebugFileSize <= lseek(fid, 0, SEEK_END))
	{
		SetLock(fid, 0);
		close(fid);
		
		char oldfile[128];
		sprintf(oldfile, "%s.bak", filename);
		rename(filename, oldfile);
		
		fid = open(filename, O_RDWR | O_CREAT | O_APPEND, 0666);
		if(-1 == fid)
			return LOG_FAILED;
		
		SetLock(fid, 2);
	}
	
	FILE * fp = fdopen(fid, "a+");
	if(NULL != fp)
	{
		fwrite(str,len,1,fp);
		SetLock(fid, 0);
		fclose(fp);
	}
	else
	{
		SetLock(fid, 0);
		close(fid);
	}	
	
	return LOG_SUCCESS;
}

//fcntl:F_RDLCK-共享锁，F_WRLCK-互斥锁，F_UNLCK-解锁
//flock:LOCK_SH-共享锁，LOCK_EX-互斥锁，LOCK_UN-解锁
//locktype:1-共享锁，2-互斥锁，0-解锁
int CDebugLog::SetLock(const int & fd, const unsigned char & locktype)
{
	const static int locks[3] = {LOCK_UN, LOCK_SH | LOCK_NB, LOCK_EX | LOCK_NB};
	if(locktype > 3)
		return LOG_FAILED;

	int count = 0;
	while(-1 == flock(fd, locks[locktype]))
	{
		if((errno != EAGAIN && errno !=  EACCES) || (++count >= 10))
		{
			perror("set log file lock failed:");
			return LOG_FAILED;
		}
		sleep(1);
	}
	return LOG_SUCCESS;
}

void *CDebugLog::ThreadRun()
{
	char buf[512];
	int ret = 0;
	
	while(!GetExit(50))
	{
		//写文件记录日志
		m_mutex.Lock();
		ret = m_path.size();
		if(ret > 0)
		{
			ret = m_data.Get(buf,sizeof(buf));
			if(ret > 0)
			{
				for(itWLogs it = m_path.begin(); it != m_path.end(); it++)
				{
					WriteFile(it->c_str(),buf,ret);
				}
			}
		}
		m_mutex.Unlock();
		
		if(ret <= 0)
		{
			sleep(1);
		}
	}
	
	return NULL;
}

int CDebugLog::WriteLog(const char * fmt,...)
{
	int ret = 0;
	char str[nLogContentMax];

	time_t sec = time(0);
	tm * dt = localtime(&sec);

	ret = sprintf(str, "[%d/%d/%d %d:%d:%d]", 
			dt->tm_year % 100, dt->tm_mon + 1, dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);
	
	va_list vl;
	va_start(vl, fmt);
	ret += vsnprintf(&str[ret], nLogContentMax - ret - 1, fmt, vl);
	va_end(vl);

	str[ret] = '\n';
	str[ret+1] = '\0';
	
	return m_data.Put(str,ret+1);
}

void CDebugLog::RegisterLogPath(char * path)
{
	std::string str = path;
	if('/' != *str.rbegin())
		str += '/';

	CAutoLock lock(&m_mutex);
	m_path.insert(str + strDbgFileName);
}

void CDebugLog::UnRegisterLogPath(char *path)
{
	char buf[512];
	int ret = 0;
	std::string str = path;
	if('/' != *str.rbegin())
		str += '/';

	CAutoLock Lock(&m_mutex);
	
	while((ret = m_data.Get(buf,sizeof(buf))))
	{
		for(itWLogs it = m_path.begin(); it != m_path.end(); it++)
		{
			WriteFile(it->c_str(),buf,ret);
		}
	}
	
	m_path.erase(str + strDbgFileName);
}

