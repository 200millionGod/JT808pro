/*-------------------------------------------------------------------------
 环形缓冲池
-------------------------------------------------------------------------*/
#include "cyclebuf.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#define   MIN(x, y)                 ((x) < (y) ? (x):(y))

CCycleBuf::CCycleBuf():m_pBuffer(NULL),m_nSize(0),m_nIn(0),m_nOut(0)
{
	
}

CCycleBuf::~CCycleBuf()
{
	if (m_pBuffer != NULL)
	{
		delete[] m_pBuffer;
	}
}

bool CCycleBuf::Init(unsigned int size)
{
	bool ret = false;
	if(size>0&&size<0x80000000)
	{
		m_nSize = 8;
		for(int i=0;m_nSize<size;i++)
		{
			m_nSize = m_nSize<<1;
		}
		if(m_pBuffer)
		{
			delete[] m_pBuffer;
		}
		m_pBuffer = new char[m_nSize];
		Reset();
		ret = (m_pBuffer!=NULL);
	}
	
	return ret;
}

int CCycleBuf::Put(const char * buffer,int len)
{
	unsigned int m,n;
	
	m = MIN((unsigned int)len, m_nSize - m_nIn + m_nOut);
	n 	= MIN(m, m_nSize - (m_nIn&(m_nSize-1)));
	memcpy(m_pBuffer + (m_nIn%m_nSize), buffer, n);
	memcpy(m_pBuffer, buffer + n, m - n);
	m_nIn += m;
	
	return (int)m;
}

int CCycleBuf::Get(char* buffer, int len)
{
	unsigned int m,n;
	
	m = MIN((unsigned int)len, m_nIn - m_nOut);
	n   = MIN(m, m_nSize - (m_nOut & (m_nSize - 1)));
	memcpy(buffer, m_pBuffer + (m_nOut & (m_nSize - 1)), n);
	memcpy(buffer +n, m_pBuffer,  m - n);
	m_nOut += m;
	if (m_nIn == m_nOut )
	{
		m_nIn = m_nOut = 0;
	}
	
	return (int)m;
}

int CCycleBuf::Get(int len,char*& pPart1, int& part1Len,char*& pPart2, int& part2Len)
{
	unsigned int m;
	
	m = MIN((unsigned int)len, m_nIn - m_nOut);
	part1Len  = MIN(m, m_nSize - (m_nOut & (m_nSize - 1)));
	pPart1 = m_pBuffer + (m_nOut & (m_nSize - 1));
	pPart2 = m_pBuffer;
	part2Len = m - part1Len;
	m_nOut += m;
	if (m_nIn == m_nOut )
	{
		m_nIn = m_nOut = 0;
	}
	
	return (int)m;
}

int CCycleBuf::Find(const char *pStartSign,const char *pEndSign,int nStartLen,int nEndLen)
{
	int ret=0;
	unsigned int si = m_nOut;
	unsigned int ei = m_nIn;
	
	si = Search(si,pStartSign,(unsigned int)nStartLen);
	if(si!=m_nIn)
	{
		ei = Search(si+nStartLen,pEndSign,(unsigned int)nEndLen);
	}
	if(ei!=m_nIn)
	{
		m_nOut = si;
		ret = (int)(ei - si);
	}
	return ret;
}

unsigned int CCycleBuf::Search(unsigned int index,const char* pSign,unsigned int nSignLen)
{
	unsigned int i;
	for (; index < m_nIn; index++ )
	{
		if ( m_pBuffer[index & (m_nSize  -1 )] == *pSign)
		{
			for (i = 1; (i < nSignLen)&&(index+i<m_nIn); i++)
			{
				if(m_pBuffer[(index+i) & (m_nSize  -1 )] != *(pSign+i))
				{
					break;
				}
			}
			if(i==nSignLen)
			{
				break;
			}
		}
	}
	return index;
}

