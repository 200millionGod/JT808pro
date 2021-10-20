#include "GnssNetMsgSendRcv.h"
#include "debug_log.h"

CNetMsgSendRcv::CNetMsgSendRcv()
{
	m_bAuthorized = false;		//默认鉴权未成功
	m_pRcvMsg = NULL;
	m_nPort = 0;
	m_nConnectTimeOut = 0;
	pthread_mutex_init(&m_stBufResetMutex, NULL);
}

CNetMsgSendRcv::~CNetMsgSendRcv()
{
	SAFE_DELETE_ARRAY(m_pRcvMsg->pData);
	SAFE_DELETE_OBJECT(m_pRcvMsg);

	pthread_mutex_destroy(&m_stBufResetMutex);
}

bool CNetMsgSendRcv::Init(int socket_type)        //由主函数间接调用
{
	if(m_pRcvMsg == NULL){
		m_pRcvMsg = new NetRcvMsg;
		m_pRcvMsg->pData = new char[PACK_BUFFER_SIZE];   //为数据缓冲区分配RAM
	}

	/*初始化消息接收的循环buffer*/
	m_RcvBuf.Init(FIFO_BUFFER_SIZE);

    //创建套接字
	m_Socket.Init(socket_type);
	
	sleep(1);
	return true;
}

bool CNetMsgSendRcv::StartService()                          //由main函数调用启动线程
{
	/*启动线程*/
	m_sendThread.StartThread(SendThread, this);				//启动UDP发送线程
	DBG_LEVEL_6("SendProc() thread is startup!.....");
    usleep(20000);
         
	m_rcvThread.StartThread(RcvThread, this);               //启动UDP接收线程                    
	DBG_LEVEL_6("RcvProc() thread is startup!....");
    usleep(20000);
        
	m_transThread.StartThread(TransThread, this);           //接收数据解析线程，解析接收到的一帧数据放入链表
	DBG_LEVEL_6("TransProc() thread is startup!....");
	usleep(20000);
	return true;
}

bool CNetMsgSendRcv::StopService()
{

	/*停止线程*/
	m_sendThread.StopThread();
	DBG_LEVEL_6("stop send msg thread");

	m_rcvThread.StopThread();
	DBG_LEVEL_6("stop rcv msg thread ");

	m_transThread.StopThread();
	DBG_LEVEL_6("stop transmit msg thread ");

	m_Socket.Close();
	DBG_LEVEL_6("stop  close socket!!");

	m_lstSend.All2FreeMsg();	
	DBG_LEVEL_6("stop m_lstSend.All2FreeMsg(); ");
	m_lstRcv.All2FreeMsg();	
	DBG_LEVEL_6("stop m_lstRcv.All2FreeMsg(); ");
	
	m_RcvBuf.Reset();
	DBG_LEVEL_6("stop m_RcvBuf.Reset(); ");

	return true;
}


void* CNetMsgSendRcv::SendThread(void* pData)         //数据发送线程
{
	return ((CNetMsgSendRcv*)pData)->SendProc();
}

//放入待发送的协议数据到发送队列中由CNetMsgSendRcv::SendProc() 发送到网络
int CNetMsgSendRcv::SendMsg(NetSendMsg* pMsg)
{
	int nRet = 0;
	if (NULL == pMsg || pMsg->wTotalLen > MSG_BUF_SIZE) 										 //数据超出缓冲区长度
	{
		DBG_LEVEL_6(" msg len has err.");
		nRet = 2;
	}
	else
	{
		NetSendMsg* pEmptyMsg =  m_lstSend.GetFreeMsg();
		if(pEmptyMsg==NULL)
		{
			DBG_LEVEL_6("GetFreeMsg failure!");
			nRet = 1;
		} else {
			pEmptyMsg->wId				=		pMsg->wId;
			pEmptyMsg->wTotalLen		= 		pMsg->wTotalLen ;			
			pEmptyMsg->wSerialNumber	=		pMsg->wSerialNumber;		 
			pEmptyMsg->wPacketTotal		=		pMsg->wPacketTotal;		 
			pEmptyMsg->wPacketSn		=		pMsg->wPacketSn;			  
			pEmptyMsg->wReSendTimes		=		pMsg->wReSendTimes;		 
			pEmptyMsg->tTimeout			=		pMsg->tTimeout;				 
			pEmptyMsg->tLastVisit		=		pMsg->tLastVisit;
			memcpy(pEmptyMsg->pData, pMsg->pData, pMsg->wTotalLen);
			m_lstSend.AddUsedMsg(pEmptyMsg);
		}
	}

	return nRet;
}

void* CNetMsgSendRcv::SendProc()                                  //网络数据发送线程线程循环
{                                                                           
	NetSendMsg* pMsg = NULL;
	int nSendLen = 0;
	time_t nCur;
	bool bSend;
	
	struct timeval tvCur;
	struct timeval tvLast;
	gettimeofday(&tvCur, NULL);
	tvLast = tvCur;

	while (!m_sendThread.GetExit(50))
	{
//		DBG_LEVEL_6("In SendProc m_lstSend.size=%d", m_lstSend.CntOfUsed());
		pMsg = m_lstSend.GetUsedMsg();        
		bSend = true;                                              
		if( NULL != pMsg)
		{
			/*已经与服务器建立连接*/
			if (m_Socket.IsLinked())
			{
				if ( m_bAuthorized == false )
				{
					DBG_LEVEL_6("weijianquan...");
				}else
				{
					DBG_LEVEL_6("yijianquan...");
				}
				/* 平台鉴权未通过,不发送注册鉴权以外的消息*/
				if ( m_bAuthorized == false )
				{
					DBG_LEVEL_6("pMsg->wId = %d", pMsg->wId);
					if ((pMsg->wId != NET_CMD_REGISTER) && (pMsg->wId != NET_CMD_AUTHORIZE) && (pMsg->wId != NET_CMD_UNREGISTER))
					{	
						DBG_LEVEL_6("Device is not register!");
						m_lstSend.AddFreeMsg(pMsg);     
						continue;                   
					}
				}else
				{
					DBG_LEVEL_6("pMsg->wId = %d, jianquantongguo...", pMsg->wId);
				}

				/*消息重发处理*/
				if ( pMsg->wReSendTimes != m_dwReSendTimes)
				{
					nCur = time(NULL);
					/*未满足超时条件*/
//					printf("In SendProc nCur=%d, pMsg->tLastVisit=%d, pMsg->tTimeout=%d\n", nCur, pMsg->tLastVisit, pMsg->tTimeout);
					if ( nCur < pMsg->tLastVisit + pMsg->tTimeout ){
						bSend = false;
					}
				}

				if ( bSend) /*满足第一次发送或超时条件*/
				{
					int i;
					for (i = 0; i < pMsg->wTotalLen; i++)
					{
						printf("%02x ", pMsg->pData[i]);
					}
					printf("\n");
					nSendLen = m_Socket.Send((unsigned char*)pMsg->pData, pMsg->wTotalLen);
					while(nSendLen == -1){
						static int count = 0;
						DBG_LEVEL_6("Socket Send CMD1:0x%04x, SN:%d, Len:%d Ret:%d\n", 
										pMsg->wId, pMsg->wSerialNumber, pMsg->wTotalLen,nSendLen);
					
						if((++ count) > 2){
							count = 0;
							break;
						}
						usleep(200*1000);
						
						CloseConnect();
						usleep(200*1000);
						Link2Svr(m_szServerAddr, m_nPort, m_nConnectTimeOut);
						usleep(200*1000);
						nSendLen = m_Socket.Send((unsigned char*)pMsg->pData, pMsg->wTotalLen);
					}
#if 0 //just for test
					if(pMsg->wId == NET_CMD_MUTIMEDIA_DATA_UP)
					{
						static FILE *fp = NULL;
						static int runtime = 0;
						char tmp[126] = {0};
						char *pData = pMsg->pData;
						pData += 13;
						WORD packNum = *pData << 8;
						packNum = *(pData+1);
						pData += 2;
						WORD packID = *pData << 8;
						packID |= *(pData+1);

						if(packID == 1)
						{
							sprintf(tmp, "./test_back_%d.jpg", runtime);
							fp = fopen(tmp, "w+");
						}
						if(fp != NULL)
						{
							fwrite(pMsg->pData, 1, pMsg->wTotalLen, fp);
						}
						else
						{
							printf("xxxxxxxxinvalid file fp\n");
						}
						if(packID == packNum)
						{
							fclose(fp);
							runtime++;
						}
						
					}
#endif
					DBG_LEVEL_6("Socket Send CMD2:0x%04x, SN:%d, Len:%d Ret:%d", pMsg->wId, pMsg->wSerialNumber, pMsg->wTotalLen,nSendLen);
					if(!m_Socket.IsLinked())
					{
						DBG_LEVEL_6("In send proc, m_bNetAlive is false.");
					}else
					{
						DBG_LEVEL_6("In send proc, m_bNetAlive is true.");
					}
					if(pMsg->wId == 0x0104){
						DBG_LEVEL_6("=====>%.2x %.2x %.2x %.2x", pMsg->pData[0], pMsg->pData[1], pMsg->pData[2], pMsg->pData[3]);
					}

					if( pMsg->wReSendTimes > 1 ) {
						pMsg->tLastVisit  = time(NULL);
						/*下次超时时间根据公式(TN+1 = TN * (N + 1))*/
						pMsg->tTimeout =  (m_dwReSendTimes - pMsg->wReSendTimes  + 1) * pMsg->tTimeout;
						pMsg->wReSendTimes -= 1;
						m_lstSend.AddUsedMsg(pMsg);    //数据作为最新消息放到m_lstUsed链表中以备重新发送
						DBG_LEVEL_6("In send proc, goto1.");
					} else/*到达重发次数释放到空闲队列去*/	{
						DBG_LEVEL_6("In send proc, goto2.");
						m_lstSend.AddFreeMsg(pMsg);
					}
				} else {/*未满足超时条件*/
					DBG_LEVEL_6("In send proc, goto3.");
					m_lstSend.AddUsedMsg(pMsg);    //再放入链表，下次取出重新发送
				}
			} else {
				DBG_LEVEL_6("In send proc, goto4.");
				m_lstSend.AddFreeMsg(pMsg);     
			}
		}
	}	
	return NULL;
}

void* CNetMsgSendRcv::RcvThread(void* pData)
{
	return ((CNetMsgSendRcv*)pData)->RcvProc();
}

void* CNetMsgSendRcv::RcvProc()                     //网络数据接收线程
{
	unsigned char pBuf[MSG_BUF_SIZE] = {0};
	unsigned int nTime = 0;
	int nLen;

	while (!m_rcvThread.GetExit(nTime))
	{
		nLen = m_Socket.Recv((unsigned char*)pBuf, MSG_BUF_SIZE);     //接收UDP数据放入pBuf中
//		DBG_LEVEL_6("RcvProc is running...socket recv len:%d", nLen);
		if(nLen > 0){
			#if 1
			DBG_LEVEL_6("RcvProc------------:%d", nLen);
			for(int i = 0; i < nLen; ++ i){
				printf("%02X ", pBuf[i]);
			}
			printf("\n");
			#endif
			pthread_mutex_lock(&m_stBufResetMutex);
			if(m_RcvBuf.Put(pBuf, nLen) == 0){             //将数据从pBuf拷贝到m_pBuffer
				DBG_LEVEL_6("write to recieve buffer failed.");
			}
			pthread_mutex_unlock(&m_stBufResetMutex);
			nTime = 0;
		} else {
			nTime = 15;
		}
	}
	return NULL;
}

void* CNetMsgSendRcv::TransThread(void* pData)
{
	return ((CNetMsgSendRcv*)pData)->TransProc();
}

void* CNetMsgSendRcv::TransProc()               //将RcvProc() 接收的数据做解析并放入链表线程
{
	unsigned char pBuf[MSG_BUF_SIZE] = {0};
	NetRcvMsg* pRcvMsg;
	unsigned int nLen;
	int nIndex = 0;

	while ( !m_transThread.GetExit(15))
	{
		/*从循环m_pBuffer中获取消息*/              //从m_pBuffer获取数据的长度
		nLen = m_RcvBuf.Find((unsigned char*)NET_CMD_FLAG, (unsigned char*)NET_CMD_FLAG, nIndex);
		if(nLen > 0 && nLen < MSG_BUF_SIZE) {
			pRcvMsg = m_lstRcv.GetFreeMsg();
			m_RcvBuf.Get(pBuf, nLen);
			#if 1
			printf("In TransProc, before Revert------------:%d\n", nLen);
			for(int i = 0; i < nLen; ++ i){
				printf("%02X ", pBuf[i]);
			}
			printf("\n");
			#endif
			//从循环缓冲区m_pBuffer 拷贝数据到pBuf
			/*转义还原*/
			m_parse.RevertMsg(pBuf, nLen);
			printf("In TransProc, after Revert------------:%d\n", nLen);
			for(int i = 0; i < nLen; ++ i){
				printf("%02X ", pBuf[i]);
			}
			printf("\n");
			/*解析从循环buffer中获取的消息*/
			if(m_parse.Parse(pBuf, nLen, pRcvMsg) == 0)        //将p_Buf中的数据解析后复制到pRcvMsg消息体中
			{
			    /*添加到接收队列*/
				m_lstRcv.AddUsedMsg(pRcvMsg); 	
			} else {
				m_lstRcv.AddFreeMsg(pRcvMsg);			//回收内存
				pthread_mutex_lock(&m_stBufResetMutex);
				m_RcvBuf.Reset();
				pthread_mutex_unlock(&m_stBufResetMutex);
			}
		}
	}
	return NULL;
}


NetRcvMsg* CNetMsgSendRcv::GetMsg()
{
//	DBG_LEVEL_6("In NetServiceProcess GetMsg() m_lstRcv.CntOfUsed():%d!", m_lstRcv.CntOfUsed());
	NetRcvMsg* pMsg = m_lstRcv.GetUsedMsg();
	if ( NULL != pMsg )
	{
		/*若改消息分包，则需要检查各分包是否都收集齐全*/
		if ( pMsg->wPacketTotal > 1)
		{
			/*判断是否超时*/
			if(time(NULL) - pMsg->tLastVisit > m_dwReplyTimeout )
			{
				//删除超时的消息包
				m_lstRcv.DelRcvMsgOfTimeout(pMsg->wId, pMsg->wSerialNumber, pMsg->wPacketTotal, pMsg->wPacketSn);
				return NULL;
			}
			m_lstRcv.AddUsedMsg(pMsg);        //将取出的内存地址重新存入链表
			/* 判断是否获取一个消息所有分包*/
			if (m_lstRcv.MakeupRcvMsg(pMsg->wId, pMsg->wSerialNumber, pMsg->wPacketTotal, pMsg->wPacketSn, m_pRcvMsg->pData, m_pRcvMsg->wMsgLen) > 0 )	
			{
				memcpy(m_pRcvMsg, pMsg, sizeof(NetRcvMsg) - sizeof(char*) - sizeof(WORD));
				pMsg = m_pRcvMsg;
			}
			else
			{
				pMsg = NULL;
			}
		}
		else 
		{
			memcpy(m_pRcvMsg, pMsg, sizeof(NetRcvMsg) - sizeof(char*));
			memcpy(m_pRcvMsg->pData, pMsg->pData, pMsg->wMsgLen);
			m_lstRcv.AddFreeMsg(pMsg);

			pMsg = m_pRcvMsg;
		}		
	}		
		
	return pMsg;
}

bool CNetMsgSendRcv::DelMsgBySN(int nSerailNum)
{
	m_lstSend.DelBySerialNum(nSerailNum);
	return true;
}


void CNetMsgSendRcv::SetAuthorizeStatus(bool bMark)
{
	m_bAuthorized = bMark;		
	if(!m_bAuthorized)
	{

	}
}

void CNetMsgSendRcv::SetNetParam(DWORD timeout,DWORD sendTimes)
{
	m_dwReplyTimeout = timeout;
	m_dwReSendTimes  = sendTimes;
}

bool CNetMsgSendRcv::ServerIsLinked(void)
{
	return m_Socket.IsLinked();
}

void CNetMsgSendRcv::CloseConnect(void)
{
	m_Socket.Close();			//4关闭socket套接字连接	
}
/****************************************************************
count为偶数连接服务器A   ,为奇数连接服务器B  。
nTimeOut为连接服务器时指定超时时间。
连接成功返回1，否则返回0
****************************************************************/
bool CNetMsgSendRcv::Link2Svr(const char* pAddr,int port,int nTimeOut)
{
	memcpy(m_szServerAddr, pAddr, strlen(pAddr));
	m_nPort = port;
	m_nConnectTimeOut = nTimeOut;
	return m_Socket.Link2Svr(pAddr, port,nTimeOut);
}

bool CNetMsgSendRcv::DelMsgByCmdId(int nCmdId)
{
	m_lstSend.DelByCmdID(nCmdId);
	return true;
}


int CNetMsgSendRcv::SendData(const char *data, int size)
{	
	if(data == NULL || size <= 0){
		return 0;
	}
		
	return m_Socket.Send((unsigned char*)data, size);
}

