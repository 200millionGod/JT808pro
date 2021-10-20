#include "GnssNetSocket.h"
#include <stdio.h>
CSocket::CSocket()
{
	m_nSocket = -1;
	m_nSocketType = -1;
	m_nPort = 0;
	m_nTermiType = 0;
	memset(m_szIp, 0 , sizeof(m_szIp) );
}


CSocket::~CSocket()
{
	close(m_nSocket);
}


bool CSocket::Close()
{
	
	if ( m_nSocket != -1)
	{
		if ( -1 == close(m_nSocket) )
		{
			return false;
		}
		m_nSocket = -1;
		
	}

	return true;
}


bool CSocket::Create(int nSocketType)
{
	m_nSocketType = nSocketType;
	Close();
	switch (nSocketType)
	{
		case SOCK_DGRAM:   //UDP
			
			m_nSocket = socket(AF_INET, SOCK_DGRAM, 0);
			if (m_nSocket == -1)
			{
				return false;
			}	
			
			break;

		case SOCK_STREAM:   //TCP

			m_nSocket = socket(AF_INET, SOCK_STREAM, 0);
			if (m_nSocket == -1)
			{
				return false;
			}	
			break;

		default:
				return false;
			
		}
	
	return 	SetSocketStatus();
}


bool CSocket::BindLocal(unsigned int nSocketPort)
{	
	m_nPort  = nSocketPort;
	
	struct sockaddr_in local_addr = {0};
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(nSocketPort);
    local_addr.sin_addr.s_addr = INADDR_ANY;
	
    int nRet = bind(m_nSocket, (struct sockaddr*) &local_addr, sizeof(struct sockaddr_in));
    if ( - 1 == nRet)
    {
		Close();
        return false;
	} 

	return true;
}


bool CSocket::Listen(int nConnectBackLog)
{
	
  	int nRet = listen(m_nSocket, nConnectBackLog);
    if ( - 1 == nRet)
    {
        Close();
        return false;
    }

	return true;
}


bool CSocket::Connect(unsigned int nIP, unsigned short usPort, int nTimeout)
{
	if (-1 == m_nSocket)
	{	
		return false;
	}	
	
	int nRet = 0;
	struct sockaddr_in serv_addr = {0};
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(usPort);
   	serv_addr.sin_addr.s_addr = nIP;

	nRet = connect(m_nSocket, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	
	if (nRet == -1)
	{
		if(errno != EINPROGRESS)
		{
			Close();
			return false;
		}
		
		struct timeval tm;
		fd_set set;
		
		int error = 0;
		socklen_t len = sizeof(int);
		tm.tv_sec  =  nTimeout / 1000;
		tm.tv_usec = (nTimeout % 1000) *1000;
		FD_ZERO(&set);
		FD_SET(m_nSocket, &set);
			
		while(1)
		{
			switch( select(m_nSocket + 1, NULL, &set, NULL, &tm) )
			{
				
				case -1:
					if(EINTR == errno || EAGAIN == errno)
					{
						continue;
					}
					Close();
					return false;
					 
				case 0:

					Close();
					return false;
				
				default: 
					
					if (( 0 == getsockopt(m_nSocket, SOL_SOCKET, SO_ERROR, &error, &len) ))
					{
						if(  0 == error )
						{
							return true;
						}
					}
					 
					Close();
					return false;
			}
		}

		
	}
	return true;
}



bool CSocket::Accept(int& nClientSock,  sockaddr* pSockAddr, socklen_t* pSockAddrLen)
{
	if (-1 == m_nSocket)
	{	
		return false;
	}	
	
	nClientSock = accept(m_nSocket, pSockAddr, pSockAddrLen);
	if (-1 == nClientSock )
	{
		return false;
	}

	m_nTermiType = 1;
	return true;
}



int CSocket::SendTo(unsigned char*  szBuf, int nLen, const char* szIp, int nPort, unsigned int ulTimeout)
{
	if(m_nSocket < 0)
	{
		return -1;
	}
	
	struct sockaddr_in addrto = {0};
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = inet_addr(szIp);
	addrto.sin_port = htons(nPort);
	return SendTo( szBuf, nLen, &addrto, ulTimeout);
}



int CSocket::SendTo(unsigned char*  szBuf, int nLen, struct sockaddr_in *pDestAddr, unsigned int ulTimeout)
{
	
	if(m_nSocket < 0)
	{
		return -1;
	}
	
	if (SelectSingleSocket(SOCK_SEL_TYPE_WRITE, ulTimeout)>0)
	{
		int nRet = sendto(m_nSocket, szBuf, nLen, 0, (struct sockaddr*)pDestAddr, sizeof(struct sockaddr_in));
		if(nRet < 0)
		{
		}

		return nRet;
	}
	else
	{
		return 0;
	}
}


int CSocket::Send(unsigned char*  szBuf, int nLen, unsigned int ulTimeout)
{
	if(m_nSocket < 0)
	{
		return -1;
	}	
	
	int nRet =0;
	int nSendLen = nLen;
	if ( SelectSingleSocket(SOCK_SEL_TYPE_WRITE, ulTimeout)>0)
	{
		nRet = send(m_nSocket, szBuf, nLen, MSG_NOSIGNAL);
		nSendLen = nRet;

		if(nRet == -1)
		{
		}
	}
	else
	{
		if(errno == EBADF)
		{
			return -1;
		}
		else
		{
			int error =  - 1;
			int len = sizeof(int);
			getsockopt(m_nSocket, SOL_SOCKET, SO_ERROR, &error, (socklen_t*) &len);

			if (error != 0)
			{
				nSendLen = -1;
			}
			else
			{
				nSendLen = 0;
			}
		}
	}
	
	return nSendLen;
}


int CSocket::RecvFrom(unsigned char* szBuf, int nLen, struct sockaddr_in *pSrcAddr, unsigned int ulTimeout)
{
	if(m_nSocket< 0)
	{
		return -1;
	}	

	if( SelectSingleSocket(SOCK_SEL_TYPE_READ, ulTimeout) > 0 )
	{
		int nRecvLen = 0;
		
		nRecvLen = recvfrom(m_nSocket, szBuf, nLen, 0, 	NULL, NULL);//(struct sockaddr*)pSrcAddr, (socklen_t*)&nAddrLen);
		if(nRecvLen < 0)
		{
		}

		return nRecvLen;
	}
	else
	{
		return 0;
	}
} 


int CSocket::Recv(unsigned char*  szBuf, int nLen, unsigned int ulTimeout)
{
	if(m_nSocket< 0)
	{
		return -1;
	}	
	
	if(SelectSingleSocket(SOCK_SEL_TYPE_READ, ulTimeout) > 0)
	{
			
		int nRecvLen = 0;	
		do {
			nRecvLen = recv(m_nSocket, szBuf, nLen, 0);
//			printf("AAAAAAAAAAAAAAAAAAAAAAAerrno = %d\n", errno);
		}while(nRecvLen < 0 && EAGAIN == errno);

		if(nRecvLen < 0)
		{
		}
		return nRecvLen;
	}
	else
	{
		return 0;
	}
}


bool CSocket::SetSocketStatus()
{
	
	//取得文件描述符的状态
	int nFlags = fcntl(m_nSocket, F_GETFL, 0);
	
	if (-1 == nFlags) 
	{	
		return false;
	}
	#if 1
	//设置文件描述符为非阻塞
	if ( -1 == fcntl(m_nSocket, F_SETFL, nFlags | O_NONBLOCK) )	
	{
		return false;
	}

	#endif
	
	int bReUseAddr = 1;
	//设置套接字的属性使它能够在计算机重启的时候可以再次使用套接字的端口和IP 
	int error = setsockopt(m_nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*) &bReUseAddr, sizeof(bReUseAddr));
	if(error != 0 )
	{
		return false;
	}

	return true;
}


bool CSocket::SetSockSendBufLen(int nBufferLen)
{
	setsockopt(m_nSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nBufferLen, sizeof(int));
	return true;
}

	
bool CSocket::SetSockRecvBufLen(int nRecvLen)
{
	setsockopt(m_nSocket, SOL_SOCKET, SO_RCVBUF, (char*) &nRecvLen, sizeof(int));
	return true;
}


int CSocket::SelectSingleSocket(int nType, unsigned int ulTimeout)
{   
	if(m_nSocket >= 0)
	{
		struct timeval timeout;
		fd_set fd;
		fd_set ep;
		int rc = 0;
		FD_ZERO(&fd);
		FD_SET(m_nSocket, &fd);
		FD_ZERO(&ep);
		FD_SET(m_nSocket, &ep);
		timeout.tv_sec = ulTimeout / 1000;
		timeout.tv_usec = (ulTimeout % 1000) *1000;
		while(1)
		{
			if (SOCK_SEL_TYPE_READ == nType)
			{
				rc = select(m_nSocket + 1, &fd, NULL, NULL, &timeout);
			} 
			else
			{
				rc = select(m_nSocket + 1, NULL, &fd, NULL, &timeout);
			}
			if (rc > 0)
			{
				if (FD_ISSET(m_nSocket, &fd))
				{
					return rc;
				}
			}
			else if(-1 == rc)
			{
				if(EINTR == errno || EAGAIN == errno)
				{
					continue;
				}
			}
			break;
		}
	}

	return -1;
}

