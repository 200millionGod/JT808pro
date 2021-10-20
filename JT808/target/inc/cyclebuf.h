/*-------------------------------------------------------------------------
 环形缓冲池
-------------------------------------------------------------------------*/
#ifndef _CYCLE_BUF_H_
#define _CYCLE_BUF_H_
#include "MutexLock.h"

class CCycleBuf
{
public:
	CCycleBuf();
	~CCycleBuf();
	bool Init(unsigned int size);
	void Reset()
	{
		m_nIn = m_nOut = 0;
	}
	int Put(const char* buffer, int len);
	int Get(char* buffer, int len);
	int Get(int len,char*& pPart1, int& part1Len,char*& pPart2, int& part2Len);
	int Len()
	{
		return (int)(m_nIn - m_nOut);
	}
	int LeftSize()
	{
		return (int)(m_nSize-(m_nIn - m_nOut));
	}
	char At(int index)
	{
		return *(m_pBuffer + (((unsigned int)index+m_nOut)%m_nSize));
	}
	int Find(const char *pStartSign,const char *pEndSign,int nStartLen,int nEndLen);
	
private:
	unsigned int Search(unsigned int index,const char* pSign,unsigned int nSignLen);
	
	char*	m_pBuffer;
	unsigned int m_nSize;
	unsigned int m_nIn;
	unsigned int m_nOut;
};



#endif

