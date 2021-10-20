/******************************************************************************
  Copyright (C), 2010-2020, Shenzhen Wekomp Technology Co., Ltd.
******************************************************************************
File Name     :NetFifo.h
Version       : 
Author        : ouyangming
Created       : 2011/12/08
Last Modified :
Description   :循环buffer
Function List :
History       :

******************************************************************************/


#ifndef  _GNSS_MOC_NET_FIFO_H_
#define  _GNSS_MOC_NET_FIFO_H_

#include "pthread.h"
#include <string.h>
#include <stdio.h>


#define MIN(x, y)   ({       \
	typeof( x ) _x = x;      \
	typeof( y ) _y = y;      \
	(void) (&_x == &_y);     \
	_x < _y ? _x : _y;})
	
#define MAX(x, y)   ({       \
	typeof( x ) _x = x;      \
	typeof( y ) _y = y;      \
	(void) (&_x == &_y);     \
	_x > _y ? _x : _y;})


class CFifo
{
public:
	CFifo();

	~CFifo();
	
	bool Init(unsigned int size);

	void Reset()
	{
		m_nIn = m_nOut = 0;
		memset(m_pBuffer, 0, m_nSize);
	}

	unsigned int Put(unsigned char* buffer, unsigned int len);

	unsigned int Get(unsigned char* buffer, unsigned int len);

	unsigned int Len()
	{
		unsigned int nRet = 0; 
		nRet = m_nIn - m_nOut;
		return nRet;
	}

	unsigned int Find(unsigned char* pStartMark, unsigned char* pEndMark, int& index);

private:
	
	unsigned char At(unsigned int index);
	
	
private:
	  unsigned char* 	m_pBuffer;     /* 数据缓冲区*/
	  unsigned int   	m_nSize;       /* 缓冲区大小 */
	  unsigned int  	m_nIn;         /* 已保存到缓冲区的数据量 */
	  unsigned int   	m_nOut;        /* 已从缓冲区取走的数据量 */
};


#endif

