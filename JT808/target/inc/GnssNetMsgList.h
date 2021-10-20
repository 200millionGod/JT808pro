/******************************************************************************
  Copyright (C), 2010-2020, Shenzhen Wekomp Technology Co., Ltd.
******************************************************************************
File Name       :MocNetMsgList.h
Version          : 
Author           : ouyangming
Created         : 2011/12/22
Last Modified  :
Description     : 消息链表
Function List   :
History           :  
******************************************************************************/
#ifndef __GNSS_MOC_NET_MSG_LIST_H__
#define __GNSS_MOC_NET_MSG_LIST_H__


#include <list>

#include "GnssNetPublic.h"
#include "GnssNetProtocol.h"
#include "debug_log.h"

template<class T>
class CNetMsgList
{
	/*类型定义*/
	typedef std::list<T*>    lstMsg;                                        //链表类型重定义
	typedef typename lstMsg::iterator lstMsgIt;             //链表成员容器类型重定义


public:

	/***********************************************************
	* 函数功能:  构造，析构函数
	* 输入参数: 
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者:
	************************************************************/
	CNetMsgList();

	virtual ~CNetMsgList();

	
	/***********************************************************
	* 函数功能:  初始化链表大小，每个节点申请内存大小
	* 输入参数:  
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool Init();

	/***********************************************************
	* 函数功能:  释放链表空间
	* 输入参数:  
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者:
	************************************************************/
	bool Free();

	
	/***********************************************************
	* 函数功能: 获取一个可用的内存空间
	* 输入参数:  
	* 输出参数: 
	* 返回参数: 返回一个消息内存单元
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	T* GetFreeMsg();

	
	/***********************************************************
	* 函数功能: 添加一个可用消息内存单元空间
	* 输入参数:  
	*  param  pMsg   消息单元
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者:
	************************************************************/
	void AddFreeMsg(T* pMsg);


	/***********************************************************
	* 函数功能:从消息队列 获取一个消息
	* 输入参数:  
	* 输出参数: 
	* 返回参数: 返回一个消息
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	T* GetUsedMsg();

	
	/***********************************************************
	* 函数功能: 添加一个消息到消息队列中去
	* 输入参数:  
	* param pMsg      消息
	* 输出参数: 
	* 返回参数: 
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	void AddUsedMsg(T* pMsg);
	

	/***********************************************************
	* 函数功能: 获取消息队列中消息个数
	* 输入参数:  
	* 输出参数: 
	* 返回参数: 消息个数
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	int CntOfUsed();


	/***********************************************************
	* 函数功能: 获取消息队列中可添加消息个数
	* 输入参数:  
	* 输出参数: 
	* 返回参数: 空闲内存单元数
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	int CntOfFree();

	/////////////////////下面接口与特定实例有关/////
	/***********************************************************
	* 函数功能:   从接收队列中拼接分包的消息
	* 输入参数: 
	*  param  wId                      消息命令
	*  param  wSerialNumber     消息流水号
	*  param  wPackTotal           封包总数
	*  param  wPackSN              包序列号
	*  param    pBuf                   重组消息
	* 输出参数: 
	* 返回参数:  0 :不存在,  >0:存在
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	int MakeupRcvMsg(WORD wId, WORD wSN, WORD wPackTotal, WORD wPackSN,  char* pBuf, int len);

	
	/***********************************************************
	* 函数功能: 获取消息队列中可添加消息个数
	* 输入参数:  
	* 输出参数: 
	* 返回参数: 空闲内存单元数
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool DelRcvMsgOfTimeout(WORD wId, WORD wSN, WORD wPackTotal, WORD wPackSN);
	

	/***********************************************************
	* 函数功能: 根据序列号删除消息
	* 输入参数:  
	* param  nSerialNum    序列号
	* 输出参数: 
	* 返回参数: 空闲内存单元数
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool DelBySerialNum(int nSerailNum);


	/***********************************************************
	* 函数功能: 根据命令id删除消息
	* 输入参数:  
	* param  nCmdId    命令id
	* 输出参数: 
	* 返回参数: 空闲内存单元数
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool DelByCmdID(int nCmdId);

	/***********************************************************
	* 函数功能: 释放所有的消息为空闲消息
	* 输入参数:  
	* 输出参数: 
	* 返回参数: 空闲内存单元数
	* 使用说明: 无
	* 参考函数:
	* 创建作者: 
	************************************************************/
	bool All2FreeMsg();
	
	
	private:
		CMutexLock			m_lock;
		lstMsg          m_lstAll;
		lstMsg          m_lstUsed;              //存放即将发送到网络的数据
		lstMsg          m_lstFree;              //存放过期或无效作废数据
		//m_lstFree的作用仅仅是提供内存单元(节点)给程序使用
	
};


template<class T>
CNetMsgList<T>::CNetMsgList()
{
	Init();
}


template<class T>
CNetMsgList<T>::~CNetMsgList()
{
	Free();
}

template<class T>
bool CNetMsgList<T>::Free()
{
	CAutoLock lock(&m_lock);
	lstMsgIt iterB = m_lstAll.begin();
	lstMsgIt iterE = m_lstAll.end();	
	
	while (iterB != iterE)
	{
		T* pMsg = (*iterB);
		SAFE_DELETE_ARRAY(pMsg->pData);
		SAFE_DELETE_OBJECT(pMsg);
		++iterB;
	}

	m_lstAll.clear();
	m_lstUsed.clear();
	m_lstFree.clear();
	return true;
}

template<class T>
bool CNetMsgList<T>::Init()
{
	Free();
	for (int i = 0; i < MSG_RCV_SEND_LIST_SIZE; ++i)
	{
		T* pMsg = new T;      //根据具体的数据类型分配内存

		if (NULL == pMsg)
		{
			DBG_LEVEL_6("memory malloc failed(1).");
			continue;
		}

		memset(pMsg, 0, sizeof(T));
		pMsg->pData = new char[MSG_BUF_SIZE];      //分配数据缓冲空间
		if (NULL == pMsg->pData)
		{
			DBG_LEVEL_6("memory malloc failed(2).");
			continue;
		}
		m_lstAll.push_back(pMsg);
		m_lstFree.push_back(pMsg);           //将分配的1200个内存空间地址存入m_lstFree链表节点中...
		                                                                 //注意这里并没有给m_lstUsed分配内存，它是个空链表
	}
	return false;
}

template<class T>
T* CNetMsgList<T>::GetFreeMsg()
{
	T* pMsg = NULL;
	CAutoLock lock(&m_lock);
	if (m_lstFree.size() > 0)
	{
		pMsg = *m_lstFree.begin();
		m_lstFree.pop_front();
	}
	else
	{
		T* p = new T;      //根据具体的数据类型分配内存
		if (p)
		{
			memset(p, 0, sizeof(T));
			p->pData = new char[MSG_BUF_SIZE];      //分配数据缓冲空间
			if (p->pData)
			{
				m_lstAll.push_back(p);
				pMsg = p;
			}
			else
			{
				delete p;
			}
		}
	}
	return pMsg;
}


template<class T>
void CNetMsgList<T>::AddFreeMsg(T* pMsg)
{
	CAutoLock lock(&m_lock);
	m_lstFree.push_back(pMsg);
}


template<class T>
T* CNetMsgList<T>::GetUsedMsg()
{
	CAutoLock lock(&m_lock);
	
	if (m_lstUsed.size() > 0)
	{
		T* pMsg = *m_lstUsed.begin();    //从m_lstUsed链表头取出一条消息的内存地址
		m_lstUsed.pop_front();                //同时删除头节点----->m_lstUsed减少一个节点空间

		return pMsg;
	}
	
	return NULL;
}

template<class T>
void CNetMsgList<T>::AddUsedMsg(T* pMsg)
{
	CAutoLock lock(&m_lock);
	m_lstUsed.push_back(pMsg);                     //从链表的尾部插入一条消息--->m_lstUsed增长一个节点空间
}

template<class T>
int CNetMsgList<T>::CntOfUsed()
{
	return m_lstUsed.size();                        //返回消息的大小
}

template<class T>
int CNetMsgList<T>::CntOfFree()
{
	return m_lstFree.size();
}

template<class T>
	//从接收队列中拼接分包的消息
int CNetMsgList<T>::MakeupRcvMsg(WORD wId, WORD wSN, WORD wPackTotal, WORD wPackSN, char* pBuf, int len)

{
	lstMsgIt iterB = m_lstUsed.begin();
	lstMsgIt iterE = m_lstUsed.end();	

	int nFindTotal = 0;
	int nRightScope = wSN + (wPackTotal - wPackSN);
	int nLeftScope  = wSN - (wPackSN - 1);

	T* pMsg;
	int nOffset = 0;
	while (iterB != iterE)
	{
		pMsg = (*iterB);

		if ( (pMsg->wId == wId)  && ( wSN >= nLeftScope) && (wSN <= nRightScope))
			++nFindTotal;
		
		++ iterB;
	}
	
	/*同一消息包收集齐全*/
	CAutoLock lock(&m_lock);
	if ( nFindTotal == wPackTotal)
	{
		for (iterB = m_lstUsed.begin(); iterB != iterE; )
		{
			pMsg = (*iterB);

			if ( (pMsg->wId == wId)  && ( wSN >= nLeftScope) && (wSN <= nRightScope))
			{
				memcpy(pBuf + nOffset, pMsg->pData, pMsg->wMsgLen);
				nOffset += pMsg->wMsgLen;

				iterB = m_lstUsed.erase(iterB);
				m_lstFree.push_back(pMsg);
			}
			else
				++ iterB;
		}
	}

	if (nFindTotal == wPackTotal)
		DBG_LEVEL_6("packet msg body len:%d.", nOffset);
	return (nFindTotal == wPackTotal ? 1 : 0);
}


template<class T>
bool CNetMsgList<T>::DelRcvMsgOfTimeout(WORD wId, WORD wSN, WORD wPackTotal, WORD wPackSN)
{
	CAutoLock lock(&m_lock);
	lstMsgIt iterE = m_lstUsed.end();	
	T* pMsg;
	int nRightScope = wSN + (wPackTotal - wPackSN);
	int nLeftScope  = wSN - (wPackSN - 1);
	
	for (lstMsgIt iterB = m_lstUsed.begin(); iterB != iterE; )
	{
		pMsg = (*iterB);

		if ( (pMsg->wId == wId)  && ( wSN >= nLeftScope) && (wSN <= nRightScope))
		{
			iterB = m_lstUsed.erase(iterB);
			m_lstFree.push_back(pMsg);
		}
		else
			++ iterB;
	}

	return true;
}


template<class T>
bool CNetMsgList<T>::DelBySerialNum(int nSerailNum)
{
	CAutoLock lock(&m_lock);
	T* pMsg = NULL;
	
	for (lstMsgIt iter = m_lstUsed.begin(); iter != m_lstUsed.end(); )
	{
		pMsg = (*iter);

		if ( (pMsg->wSerialNumber == nSerailNum) )
		{
			iter = m_lstUsed.erase(iter);
			m_lstFree.push_back(pMsg);
		}
		else
		{
			++iter;
		}
	}
	return true;
}

template<class T>
bool CNetMsgList<T>::DelByCmdID(int nCmdId)
{
	CAutoLock lock(&m_lock);
	T* pMsg = NULL;

	for (lstMsgIt iter = m_lstUsed.begin(); iter != m_lstUsed.end(); )
	{
		pMsg = (*iter);

		if ( (pMsg->wId == nCmdId) )
		{
			iter = m_lstUsed.erase(iter);
			m_lstFree.push_back(pMsg);
		}
		else
			++ iter;
	}

	return true;
}

template<class T>
bool CNetMsgList<T>::All2FreeMsg()
{	
	CAutoLock lock(&m_lock);
	T* pMsg = NULL;

	for (lstMsgIt iter = m_lstUsed.begin(); iter != m_lstUsed.end(); )
	{
		pMsg = (*iter);
		iter = m_lstUsed.erase(iter);
		m_lstFree.push_back(pMsg);
	}

	return true;
}

#endif

