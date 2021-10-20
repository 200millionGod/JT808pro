/******************************************************************************
  Copyright (C), 2010-2020, Shenzhen Wekomp Technology Co., Ltd.
******************************************************************************
File Name     :NetFifo.cpp
Version       : 
Author        : ouyangming
Created       : 2011/12/08
Last Modified :
Description   :循环buffer
Function List :
History       :

******************************************************************************/


#include "GnssNetFifo.h"
#include "GnssNetPublic.h"


CFifo::CFifo()
{
	m_pBuffer = NULL;
}


CFifo::~CFifo()
{	
	if (NULL != m_pBuffer)
	{
		delete []m_pBuffer;
		m_pBuffer = NULL;
	}
}


bool CFifo::Init(unsigned int size)
{
	m_pBuffer = new unsigned char[size];
	memset(m_pBuffer, 0, size);
	m_nSize = size;
	m_nIn = m_nOut = 0;
	return true;
}

unsigned int CFifo::Put(unsigned char * buffer, unsigned int len)
{
	unsigned int l;
	//m_nSize=32KB       m_nIn默认值为0.
	len = MIN(len, m_nSize - m_nIn + m_nOut);

	/* first put the data starting from m_nIn to m_pBuffer end */
	l =   MIN(len, m_nSize - (m_nIn%m_nSize));
	memcpy(m_pBuffer + (m_nIn%m_nSize), buffer, l);

	/* then put the reset (if any) at the beginning of the m_pBuffer*/
	memcpy(m_pBuffer, buffer + l, len - l);
	
	m_nIn += len;      //统计存入缓冲区的数据量      
	return len;
}

unsigned int CFifo::Get(unsigned char* buffer, unsigned int len)
{
	unsigned int l;
	
	len = MIN(len, m_nIn - m_nOut);

	/* first get the data from m_nOut until the end of m_pBuffer*/
	l   = MIN(len, m_nSize - (m_nOut%m_nSize));
	
	memcpy(buffer, m_pBuffer + (m_nOut%m_nSize), l);

	/*then get the reset (if any) from the beginning of the m_pBuffer*/
	memcpy(buffer +l, m_pBuffer,  len - l);
	
	m_nOut += len;
	if (m_nIn == m_nOut )
	{
		m_nIn = m_nOut = 0;
	}
	else if(m_nIn>=m_nSize && m_nOut>=m_nSize)
	{
		m_nIn -= m_nSize;
		m_nOut -= m_nSize;
	}
	
	return len;
}

unsigned int CFifo::Find(unsigned char* pStartMark, unsigned char* pEndMark, int& index)
{
	unsigned int nMarkLen1 = strlen((char*)pStartMark);
	unsigned int nMarkLen2 = strlen((char*)pEndMark);

	unsigned int nBeg = 0;
	unsigned int nEnd = 0;
	
	unsigned int i = 0;

	for ( i =  m_nOut; i < m_nIn; ++i )
	{
		if ( m_pBuffer[i%m_nSize] == *pStartMark)
		{
			if ( memcmp(m_pBuffer +  (i%m_nSize), pStartMark, nMarkLen1) == 0 )
			{
				nBeg  = i;
				index = i;
				break;
			}	
		}
	}
	if ( i == m_nIn)
	{
		index = -1;
		return 0;
	}	

	/* 避免重复查找*/
	++i;
	for ( ; i < m_nIn - nMarkLen2 + 1 ; ++i)
	{
		if ( m_pBuffer[i%m_nSize] == *pEndMark )
		{
			if ( memcmp(m_pBuffer +  (i%m_nSize), pEndMark, nMarkLen2) == 0 )
			{
				nEnd = i + nMarkLen2;
				break;
			}
		}
	}
	if ( i == m_nIn)
	{
		index = -1;
		return 0;
	}

	if ( nBeg != m_nOut)
	{
		m_nOut = nBeg;
	}
	return (nEnd - nBeg);
}


unsigned char CFifo::At(unsigned int idx)
{
	return *(m_pBuffer + ((idx+m_nOut)%m_nSize));
}

