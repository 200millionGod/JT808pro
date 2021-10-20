#include "MutexLock.h"
#include <stdio.h>
CMutexLock::CMutexLock()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr); 
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP); 
	pthread_mutex_init(&m_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

CMutexLock::~CMutexLock()
{
	pthread_mutex_destroy( &m_mutex );
}			

void	CMutexLock::Lock() 		
{	
	pthread_mutex_lock( &m_mutex );
}

void	CMutexLock::Unlock() 	
{	
	 pthread_mutex_unlock( &m_mutex );
}

CAutoLock::CAutoLock(CMutexLock* pMutex)
{
	m_pMutex = pMutex;
	if (m_pMutex != NULL)
	{
		m_pMutex->Lock();
	}
}

CAutoLock::~CAutoLock()
{
	Release();
}

CAutoLock::CAutoLock()
{
	m_pMutex = NULL;
}




