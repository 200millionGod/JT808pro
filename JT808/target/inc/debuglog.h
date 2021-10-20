/*-------------------------------------------------------------------------
     调试日志实现
-------------------------------------------------------------------------*/
#ifndef _DEBUG_LOG_H_
#define _DEBUG_LOG_H_

#include "ThreadFrame.h"
#include "cyclebuf.h"
#include <string>
#include <set>

class CDebugLog:public CPthreadFrame
{
	private:
		CCycleBuf m_data;
		std::set<std::string> m_path;
		typedef std::set<std::string>::iterator itWLogs;
		
		const static int nLogDebugFileSize;
		const static int nLogContentMax;
		const static char *strDbgFileName;

		CMutexLock m_mutex;
		
		void *ThreadRun();
		int WriteFile(const char * const filename, const char * const str,const int count);
		int SetLock(const int & fd, const unsigned char & locktype);
		
	public:
		CDebugLog();
		~CDebugLog();

		int WriteLog(const char * fmt,...);
		void RegisterLogPath(char *path);
		void UnRegisterLogPath(char *path);
};
#endif

