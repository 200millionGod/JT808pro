#ifndef _GNSS_MOC_NETWORK_NET_SOCKET_H_
#define _GNSS_MOC_NETWORK_NET_SOCKET_H_

#include "GnssNetSocket.h"


class CSocketOp
{
public:
	CSocketOp();
	~CSocketOp();
	
	bool Init(int nSockType, unsigned int nLocalPort = 0, int ulTimeout = 300);
	bool Link2Svr(const char* pAddr,int port,int nTimeOut = 1000);
	int Recv(unsigned char* szBuf, unsigned int nBufLen);
	int Send(unsigned char* szBuf, unsigned int nBufLen);
	bool IsLinked();
	bool Close();
	
private:
	int GetHostIP(const char* pAddr, unsigned long& lAddr);
	
	CSocket m_Socket;
	int     m_nSocktype;
	int     m_nLocalPort;

	struct sockaddr_in  m_AddrSvr;
	char   	m_nTimeOutCnt;
	int   m_ulTimeout;
	bool    m_bNetAlive;       //网络链路标志
	
private:
	void Test(unsigned char* buf, unsigned int  len);

};

#endif

