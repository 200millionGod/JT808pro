#include "RefCount.h"

CRefCount::CRefCount()
{
	m_nRefCount = 0;
}

CRefCount::~CRefCount()
{

}

void CRefCount::AddRefCount()
{
	//m_refLock.Lock();
	//m_nRefCount += 1;
	//m_refLock.Unlock();
	__sync_fetch_and_add(&m_nRefCount, 1);
}

void CRefCount::DelRefCount()
{
	//m_refLock.Lock();
	//m_nRefCount -= 1;
	//m_refLock.Unlock();
	__sync_fetch_and_sub(&m_nRefCount, 1);
}

void CRefCount::ReleaseCount()
{
	//m_refLock.Lock();
	//m_nRefCount = 0;
	//m_refLock.Unlock();
	__sync_lock_release(&m_nRefCount);
}

int CRefCount::GetRefCount()
{
	//int ret;
	
	//m_refLock.Lock();
	//ret = m_nRefCount;
	//m_refLock.Unlock();
	
	//return ret;
	return __sync_fetch_and_add(&m_nRefCount, 0);
}

bool CRefCount::IsRefCountNull()
{
	return 0 == GetRefCount();
}

