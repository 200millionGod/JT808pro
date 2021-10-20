// ThreadFrame.h: interface for the CPthreadFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREAD_H__0835B788_8E57_4AAA_A46A_9B490B47E307__INCLUDED_)
#define AFX_THREAD_H__0835B788_8E57_4AAA_A46A_9B490B47E307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <pthread.h>
#include "RefCount.h"


class CPthreadFrame : public CRefCount
{
private:
	typedef void*  ( *PFNThreadCallBack)(void* pUserData);
	
public:
	CPthreadFrame(void);
	virtual ~CPthreadFrame(void);
	
public:
	bool  StartThread(PFNThreadCallBack pfnCallback = NULL, void* pUsrData = NULL);
	bool  StopThread(unsigned int dwTime = 0xFFFFFFFF);
	void SetExit();
	bool GetExit(unsigned int dwTime = 0);
	pthread_t  GetThreadID() {	return m_ThreadID; }
	bool	IsRunning()	{	return m_bRunning; }
	bool	IsThrdExit(unsigned int dwTime );
	
public:
	virtual void*	ThreadRun();
	
private:
	static void* ThreadFunc(void*  lpParm);
	void* ThreadProcess();
	
private:
	bool		m_bRunning;
	pthread_t 	m_ThreadID;
	PFNThreadCallBack 	m_pfnThreadCallBack;
	void* 	m_pThreadUserData;
	pthread_mutex_t 	m_mutex;
	pthread_cond_t 		m_cond;
	bool 	m_bExit;
};

#endif // !defined(AFX_FITHREAD_H__0835B788_8E57_4AAA_A46A_9B490B47E307__INCLUDED_)
