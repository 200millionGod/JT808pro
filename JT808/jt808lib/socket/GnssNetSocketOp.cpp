#include <netdb.h>
#include <stdio.h>
#include "GnssNetSocketOp.h"
#include <errno.h>
CSocketOp::CSocketOp()
{
	memset(&m_AddrSvr, 0, sizeof(struct sockaddr));
	m_nSocktype = -1;
	m_bNetAlive = false;
	m_nTimeOutCnt = 0;
	m_nLocalPort = 0;
}


CSocketOp::~CSocketOp()
{
	m_Socket.Close();
}

bool CSocketOp::Close()
{
	m_bNetAlive = false;
	return m_Socket.Close();
}

bool CSocketOp::Init(int nSockType, unsigned int nLocalPort, int ulTimeout)
{
	m_nSocktype = nSockType;
	m_ulTimeout = ulTimeout;
	m_nLocalPort = nLocalPort;
	
	return true;
}


/****************************************************************
count为偶数连接服务器A   ,为奇数连接服务器B  。
nTimeOut为连接服务器时指定超时时间。
//count为偶数连接第主服务器
连接成功返回1，否则返回0
****************************************************************/
bool CSocketOp::Link2Svr(const char* pAddr,int port,int nTimeOut)
{
	m_bNetAlive = false;
	if (m_Socket.Create(m_nSocktype) )      //创建socket套接字
	{
		unsigned long nIP;
		if(GetHostIP(pAddr,nIP)==0)
		{
			if (m_nLocalPort != 0)
			{
				m_Socket.BindLocal(m_nLocalPort);    //绑定端口号
			}
			if ( m_nSocktype == SOCK_STREAM )
			{
				if ( m_Socket.Connect( nIP, (unsigned short)port, nTimeOut) )
				{
					m_bNetAlive = true;
					printf("m_bNetAlive is true in tcp way...\n");
				}
			}	
			else if (m_nSocktype == SOCK_DGRAM )
			{
				m_AddrSvr.sin_family      = AF_INET;
				m_AddrSvr.sin_addr.s_addr = nIP;
				m_AddrSvr.sin_port        = htons(port);
				m_bNetAlive = true;
				printf("m_bNetAlive is true in udp way...\n");
			}	
		}
	}
	return m_bNetAlive;
}


int CSocketOp::Recv(unsigned char* szBuf, unsigned int nBufLen)
{
	int ret=-1;
	if (m_bNetAlive){
		if (m_nSocktype == SOCK_DGRAM){
			ret = m_Socket.RecvFrom(szBuf, nBufLen, NULL, m_ulTimeout);
		} else if ( m_nSocktype == SOCK_STREAM ){
			ret = m_Socket.Recv(szBuf, nBufLen, m_ulTimeout);
		}

		if(ret < 0) {
			m_bNetAlive = false;
			printf("recv ret < 0, set m_bNetAlive false!\n");
		}
	}

	return ret;
}

int CSocketOp::Send(unsigned char* szBuf, unsigned int nBufLen)
{
	if ( !m_bNetAlive){
		return 0;
	}
	
	int nSendLen = 0;
	int nOffset  = 0;
	int nLeftLen = nBufLen;
#if 0	
	int nSlice   = 1400;
#else
	int nSlice 	 = 1500 - 66;
#endif
	//Test(szBuf, nBufLen);
	if (m_nSocktype == SOCK_DGRAM)
	{
		return m_Socket.SendTo(szBuf, nBufLen, &m_AddrSvr, m_ulTimeout);
	}
	else if ( m_nSocktype == SOCK_STREAM )
	{
		while ( nLeftLen )
		{
#if 0
			if ( nLeftLen > 1400 )
			{
				nSendLen = m_Socket.Send(szBuf + nOffset, nSlice, m_ulTimeout);
				
			}
			else 
			{
				nSendLen = m_Socket.Send(szBuf + nOffset, nLeftLen, m_ulTimeout);
				
			}
#else
			if ( nLeftLen > nSlice )
			{
				nSendLen = m_Socket.Send(szBuf + nOffset, nSlice, m_ulTimeout);
			} else {
				nSendLen = m_Socket.Send(szBuf + nOffset, nLeftLen, m_ulTimeout);
			}
#endif

			if ( nSendLen > 0 )
			{
				nLeftLen -= nSendLen;
				nOffset  += nSendLen;
				m_bNetAlive = true;
				m_nTimeOutCnt = 0;
			}
			else if ( nSendLen < 0 )
			{
				printf("*****func:%s line:%d len:%d********, set m_bNetAlive false!, errno = %d\n", __func__, __LINE__, nSendLen, errno);
				m_bNetAlive = false;	//断线重连，通知应用层重新鉴权
				break;
			}
			else 
			{
				printf("*****func:%s line:%d len:%d********\n", __func__, __LINE__, nSendLen);
				if (++ m_nTimeOutCnt >= 3)
				{
					m_nTimeOutCnt = 0;
					m_bNetAlive = false;	//断线重连，通知应用层重新鉴权
					break;
				}	
			}
		}

	}
	else
	{
		return -1;
	}
	return nSendLen;
}

bool CSocketOp::IsLinked()
{
//	if(m_bNetAlive)
//	{
//		printf("m_bNetAlive in CSocketOp::IsLinked() is true...\n");
//	}else
//	{
//		printf("m_bNetAlive in CSocketOp::IsLinked() is false...\n");
//	}
	return m_bNetAlive;
}

int CSocketOp::GetHostIP(const char* pAddr, unsigned long& lAddr)
{
#if 1	
	/*confirm the pAddr means nameserver or ipaddress*/
	unsigned int i = 0;
	for(i = 0; i < strlen(pAddr); i++)
	{
		if(pAddr[i] != '.' && (pAddr[i] < '0' || pAddr[i] > '9'))
		{
			break;
		}
	}
	if(i == strlen(pAddr) - 1)
	{
		lAddr = inet_addr(pAddr);
		return 0;
	}
	else
	{
		struct hostent *hp = NULL;	 
		if ( (hp = gethostbyname(pAddr)) != NULL )	 
		{
			if (hp->h_addr_list[0])
			{
				lAddr = *((unsigned long *) hp->h_addr_list[0]);
				return 0;
			}
		}
	}
	return 1;
#else
	struct hostent *hp = NULL;   
	if ( (hp = gethostbyname(pAddr)) != NULL )   
	{
		if (hp->h_addr_list[0])
		{
			lAddr = *((unsigned long *) hp->h_addr_list[0]);
			return 0;
		}
	}
	return 1;
#endif
}

