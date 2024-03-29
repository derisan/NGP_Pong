#include "PongPCH.h"
#include "TCPSocket.h"

#include "SocketUtil.h"

TCPSocket::~TCPSocket()
{
	closesocket(mSocket);
}

int TCPSocket::Bind(const SocketAddress& addr)
{
	int error = bind(mSocket, &addr.mSockAddr, addr.GetSize());

	if (error == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::Bind");
		return SOCKET_ERROR;
	}
	else
	{
		return NO_ERROR;
	}
}

int TCPSocket::Listen(int backlog /*= SOMAXCONN*/)
{
	int error = listen(mSocket, backlog);

	if (error == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::Listen");
		return SOCKET_ERROR;
	}
	else
	{
		return NO_ERROR;
	}
}

int TCPSocket::Connect(const SocketAddress& addr)
{
	int error = connect(mSocket, &addr.mSockAddr, addr.GetSize());

	if (error == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::Connect");
		return SOCKET_ERROR;
	}
	else
	{
		return NO_ERROR;
	}
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& outAddr)
{
	int addrlen = outAddr.GetSize();

	SOCKET newSock = accept(mSocket, &outAddr.mSockAddr, &addrlen);

	if (newSock == INVALID_SOCKET)
	{
		SocketUtil::ReportError("TCPSocket::Accept");
		return nullptr;
	}
	else
	{
		return TCPSocketPtr(new TCPSocket(newSock));
	}
}

int TCPSocket::Send(const void* data, int len, int flags /*= 0*/)
{
	int byteSent = send(mSocket, static_cast<const char*>(data), len, flags);

	if (byteSent == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::Send");
		return SOCKET_ERROR;
	}
	else
	{
		return byteSent;
	}
}

int TCPSocket::Recv(void* data, int len, int flags /*= 0*/)
{
	int byteRead = recv(mSocket, static_cast<char*>(data), len, flags);

	if (byteRead == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::Recv");
		return SOCKET_ERROR;
	}
	else
	{
		return byteRead;
	}
}

int TCPSocket::TurnOffNagleAlgorithm(bool value)
{
	u_long opt = value ? 1 : 0;

	int retval = setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&opt), sizeof(opt));

	if (retval == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::SetNagleAlgorithm");
		return SOCKET_ERROR;
	}
	else
	{
		return retval;
	}
}
