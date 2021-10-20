#ifndef _GNSS_MOC_NET_SOCKET_H_
#define _GNSS_MOC_NET_SOCKET_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>

class CSocket
{

enum
{
	SOCK_SEL_TYPE_READ   	=	0,
	SOCK_SEL_TYPE_WRITE 	=	1,
	MAX_SOCKET_NUM 			=	64,
 	SOCK_RECV_BUF_LEN		=	500*1024,
 	SOCK_SEND_BUF_LEN		=	500*1024,
 	UDP_SOCK_BUF_LEN		=	1440,
 	MAX_CLIENT_NUM			=	32,
};

public:
	CSocket();
	virtual ~CSocket();

	/*
	*	create socket
	*/
	bool Create(int nSocketType = SOCK_STREAM);


	/*
	*    close socket
	*/
	virtual bool Close();

	/*
	*  bind to localhost
	*/
	bool BindLocal(unsigned int nSocketPort);

	/*
	*  listen
	*/
	bool Listen(int nConnectBackLog = MAX_CLIENT_NUM);

	/*
	*  connect to server
	*/
	bool Connect(unsigned int nIP, unsigned short usPort, int ulTimeout = 400);

	/*
	*  accept client
	*/
	bool Accept(int& nClientSock, sockaddr* pSockAddr = NULL, socklen_t* pSockAddrLen = NULL);

	/*
	*  sendto
	*/
	int SendTo(unsigned char*  szBuf, int nLen, struct sockaddr_in *pDestAddr, unsigned int ulTimeout = 400);

	/*
	*  sendto
	*/
	int SendTo(unsigned char*  pBuf, int nLen, const char* szIp, int nPort, unsigned int ulTimeout = 400);

	/*
	* client 
	*/
	int Send(unsigned char*  szBuf, int nLen, unsigned int ulTimeout = 400);

	
	/*
	* recvfrom 
	*/
	int RecvFrom(unsigned char*  szBuf, int nLen, struct sockaddr_in *pSrcAddr, unsigned int ulTimeout = 400);

	/*
	*    recv
	*/
	int Recv(unsigned char*  szBuf, int nLen, unsigned int ulTimeout = 400);

	/*
	* 设置超时
	*/
	int SelectSingleSocket(int nType, unsigned int ulTimeout);
	
private:
	/*
	*  设置socket 属性
	*/
	bool SetSocketStatus();

	/*
	*设置发送buff
	*/
	bool SetSockSendBufLen(int nBufferLen);

	/*
	*设置接收buff
	*/
	bool SetSockRecvBufLen(int nRecvLen);


protected:		
	int	m_nSocket;
	int m_nPort;
	char m_szIp[20];
	
	int m_nSocketType;
	int m_nTermiType;   //标志是否为tcp server  0:client, 1:server;
};

#endif

