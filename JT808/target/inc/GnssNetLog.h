#ifndef _GNSS_MOC_NET_LOG_H_
#define _GNSS_MOC_NET_LOG_H_

#include <stdarg.h>
#include "MutexLock.h"

using namespace std;



#define LOG_FILE_NAME        "/sata_1/netlog.txt"
#define LOG_FILE_BACKUP_NAME "/sata_1/netlog_backup.txt"


class CNetLog;
extern CNetLog g_log;

#define T_PUT2LOG(...) do { g_log.Putout(__VA_ARGS__); } while(0)

class CNetLog
{

enum{
	LOG_FILE_NAME_LEN = 64,	
	LOG_FILE_BUFF_LEN = 1024,
	LOG_FILE_MAX_SIZE = 1024 * 1024 * 50,
};

public:
	CNetLog();
	
	int Putout(const char* fmt, ...);
	
	
private:
	CMutexLock m_lock;
	int  m_nFileOffset;
};

#endif 
