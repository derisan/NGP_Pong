#pragma once

#include "TCPSocket.h"

class SocketUtil
{
public:
	static bool StaticInit();
	static void StaticShutdown();
	static void ReportError(const char* desc);
	static int GetLastError();
	static TCPSocketPtr CreateTCPSocket();
};

