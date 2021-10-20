// Thread.cpp: implementation of the CPthreadFrame class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#include "ThreadFrame.h"
#include "Public.h"


CPthreadFrame::CPthreadFrame(void)
{
	m_bRunning = false;
	m_bExit = false;
	m_ThreadID = 0;
	m_pfnThreadCallBack = NULL;
	m_pThreadUserData = NULL;
	pthread_mutex_init(&m_mutex, NULL);
	//pthread_cond_init(&m_cond, NULL);
}

CPthreadFrame::~CPthreadFrame(void)
{
	//pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
}

bool  CPthreadFrame::StartThread(PFNThreadCallBack pfnCallback /*= NULL*/, void* pUsrData /*= NULL*/)
{
	if (m_bRunning)                                                                                                   
	{
		return false;
	}

	m_pfnThreadCallBack = pfnCallback;
	m_pThreadUserData = pUsrData;

	int nError = pthread_create(&m_ThreadID, NULL, ThreadFunc, (void*)this);
	
	if (0 == nError)
	{
		m_bRunning = true;
		m_bExit = false;
		//AddRefCount();
		return true;
	}
	return false;
}

bool  CPthreadFrame::StopThread(unsigned int dwTime /*= INFINITE*/)
{
	if (m_bRunning){
		SetExit();
		usleep(500 * 1000);
		#if 0
		if (!IsThrdExit(dwTime)){
			pthread_cancel(m_ThreadID);
		}
		#else
		pthread_join(m_ThreadID, NULL);
		#endif

		m_bRunning = false;
		m_ThreadID = 0;
		m_pfnThreadCallBack = NULL;
		m_pThreadUserData = NULL;
		return true;
	}
	return false;
}

void CPthreadFrame::SetExit()
{
	m_bExit = true;
	#if 0
	pthread_mutex_lock(&m_mutex);
	ReleaseCount();
	pthread_cond_signal(&m_cond);
	pthread_mutex_unlock(&m_mutex);
	#endif
}

bool CPthreadFrame::GetExit(unsigned int dwTime /*= 0*/)
{
	bool bExit = false;
#if 0
	struct timespec abstime;
	struct timeval now;
	gettimeofday(&now, NULL); 
	abstime.tv_nsec = now.tv_usec * 1000 + (dwTime % 1000) * 1000000; 
	abstime.tv_sec = now.tv_sec + dwTime / 1000;
	if (abstime.tv_nsec >= 1000000000)
	{
   		abstime.tv_sec++;
    	abstime.tv_nsec %= 1000000000;
	}

	pthread_mutex_lock(&m_mutex);
	pthread_cond_timedwait(&m_cond, &m_mutex, &abstime);
	pthread_mutex_unlock(&m_mutex);
	bExit = IsRefCountNull();
#else
	usleep(dwTime * 1000);
	bExit = m_bExit;
#endif
	return bExit;
}

void* CPthreadFrame::ThreadFunc(void* lpParm)
{
	CPthreadFrame* pThis = (CPthreadFrame*)lpParm;
	return pThis->ThreadProcess();
}

void* CPthreadFrame::ThreadProcess()
{
	if (m_pfnThreadCallBack != NULL)
	{
		return m_pfnThreadCallBack(m_pThreadUserData);
	}
	else
	{
		return ThreadRun();
	}
}

void*  CPthreadFrame::ThreadRun()
{
	//	ASSERT(false);
	return ((void*)0);
}


bool CPthreadFrame::IsThrdExit(unsigned int dwTime)
{
	bool bExit = false;
	int nCount = 0;
	int nMaxCount = dwTime / 100;
	while (nCount ++ < nMaxCount)
	{
		int nRet = pthread_kill(m_ThreadID, 0);
		if (ESRCH == nRet)
		{
			bExit = true;
			break;
		}
		else
		{
			SetExit();
			usleep(80000);
		}
	}
	
	return bExit;		 
}

