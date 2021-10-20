

/******************************************************************************
  File Name     : Public.cpp
  Version       : Initial Draft
  Author        : huangjun
  Created       : 2010-5
  Last Modified :
  Description   : 提供一些公用函数功能
  Function List :  
  History       :
  1.Date        : 2010-5
    Author      : huangjun
    Modification: Created file

******************************************************************************/

#include <string.h>
#include <stdarg.h>
#include <sys/mman.h> 
#include <pthread.h>

#include "GnssNetService.h"
#include "GnssNetPublic.h"


InterFace * g_pMocNetInterface = NULL;
			    


/***********************************************************
*函数功能 : 日志输出
*输入参数 :
* @param  leve  		告警类型
* @param  funName	所传入的函数名字
* @param  content 	日志内容
* 输出参数:   
* 返回参数:
* 使用说明:无
* 参考函数:
* 创建作者:
************************************************************/
void  PutToLog(const char* funName, const char* level, const char* content, ...)
{
	char log[512] = {0};
	int len = 0;
	len = snprintf(log, 256, "[PID:%d][FunName:%s()][INFO:%s][", 
		getpid(),
		funName, 
		level
		);
	va_list v1;
	va_start(v1, content);
	len += vsnprintf(log + len, 256 - len ,  content, v1);
	va_end(v1);
#if 0	
	snprintf(log + len, 256 - len, "]");
#else
	if(log[len] != '\n')
	{
		log[len + 1] = ']';
		log[len + 2] = '\n';
	}
	else
	{
		log[len] = ']';
		log[len + 1] = '\n';
	}
#endif
	if ( NULL != g_pMocNetInterface)
		g_pMocNetInterface->PutOutLog(log);
	
}

//在src中搜索和findMem相同内容的内存地址
unsigned char*  mem_search( unsigned char*  src, UINT len, const unsigned char*  findMem, UINT findLen)
{
	unsigned int idx ;
	unsigned int cnt = 0;
	
	if ( len <= 0 || findLen <= 0)
	{
		printf("func:%s len <= 0.\n", __func__);
		return NULL; //return src;
	}

	while ( cnt != (len -findLen + 1) )     //此循环先使Src逐个单元和findMem[0]首字符比较
	{
		idx = 0;
                 
		while(src[cnt + idx] == findMem[idx])     //有相同的字符，停止使用cnt，换为使用idx
		{								//没有相同的字符，不使用idx，换为使用cnt
		
			++idx;                //使指针同时指向src 和findMem的下一个字节
			if (idx == findLen)    //满足要比较长度的条件
			{
				return (src + cnt );   //返回和findMem有相同字符的src 内存地址
			}
		}
		++cnt;                  //指向src 的下一个字节
	}
	return NULL;
}


/***********************************************************
*函数功能 : 内存数据替换
*输入参数 :
* 输出参数:   
* 返回参数:
* 使用说明:无
* 参考函数:
* 创建作者:
************************************************************/
unsigned char*  mem_replace(unsigned char*  src, UINT& len, const unsigned char * oldMem, UINT oldLen, const unsigned char*  newMem, UINT newLen)
{
	if ( 0 ==len || 0 == oldLen )
	{
		printf("%s(), param len = 0 \n", __FUNCTION__);
		return NULL;
	}

	if ( (oldLen == newLen)  &&  (memcmp(oldMem, newMem, oldLen)== 0) )
		return src;


	BYTE dest[1024 * 2];
	memset(dest, 0, sizeof(dest));

	unsigned char* pOffset  = src+1;        //除去'~'字符，即: 数据起始的地方
	unsigned char* pBefore  = src;
	unsigned char* pCopy    = dest;
	unsigned int   nCopyLen = 0;            // the memmory before find oldmem

	int  		    nTimes   = 0;           // replaces times
	int            nSearchLen = len;        // pass mem_search  functions len
	unsigned  int            nTotalCopy = 0;
	

	while (pOffset < (src + len- oldLen + 1) )
	{
		/* the position that  find the string of odlmem*/
		pOffset = mem_search(pOffset, nSearchLen, oldMem, oldLen);

		//if (pOffset == NULL)
		if (pOffset == NULL || (int)(pOffset-src) == (int)(len-1))    //找不到旧的关键字，或者搜索的内容已经到了src缓冲区的数据末尾标识位前一个字节
		{
			break;                     //找不到指定的关键字停止搜索
		}

		nCopyLen = pOffset - pBefore;             //计算src缓冲区首地址到存放旧的关键字的地址之间的长度
		memcpy(pCopy, pBefore, nCopyLen);   //复制在旧的关键字(0x7d, 0x02)之前的所有数据到dest缓冲区
		memcpy(pCopy+ nCopyLen, newMem, newLen);    // 在旧的关键字0x7d开始的内存单元存入新的关键字0x7e 到dest缓冲区
		
		nSearchLen -=(nCopyLen + oldLen);      //得到旧的关键字(0x7d, 0x02) 0x02之后到(Src)缓冲区最后一个有效数据间的长度
		pOffset += oldLen;                                     //移动src缓冲的偏移地址到旧的关键字之后
		pBefore = pOffset;
		pCopy += (nCopyLen + newLen);         //移动dest缓冲区偏移量到存放新的关键字0x7e 之后
		nTotalCopy = nTotalCopy + nCopyLen + oldLen;  //统计已复制数据的长度
		++nTimes;                                                 //统计内存搜索次数
	}

	/* after replace the string*/
	/*if ( pCopy != dest)
		memcpy(src, dest, (pCopy - dest));
	*/

	if ( nTotalCopy !=  len )                                   //如果已复制的数据长度，不等于总的数据长度
		memcpy(pCopy, src + nTotalCopy, ( len -nTotalCopy));   //复制src缓冲剩余数据到dest缓冲区的新关键字0x7e 之后的下一字节
	
	len += nTimes * ( newLen - oldLen);
	memcpy(src, dest, len);          //复制dest缓冲区所有的数据到src缓冲区

	return src;
}

