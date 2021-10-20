#ifndef _REF_COUNT_H_
#define _REF_COUNT_H_

//#include "MutexLock.h"

class CRefCount
{
public:
	CRefCount();
	~CRefCount();

public:
	void AddRefCount();
	void DelRefCount();
	void ReleaseCount();
	int GetRefCount();
	bool IsRefCountNull();
	
private:
	int m_nRefCount;
	//CMutexLock m_refLock;
};

#endif

