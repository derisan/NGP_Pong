#include "ServerPCH.h"
#include "Server.h"

Server::Server()
	: Game()
{
}

bool Server::Init()
{
	//윈속 초기화
	SocketUtil::StaticInit();

	// 플레이어 접속 대기
	WaitAllPlayers();

	return true;
	
}

void Server::Shutdown()
{
	// 윈속 제거 
	SocketUtil::StaticShutdown();
}

void Server::Run()
{
	// 게임시작~
}

void Server::WaitAllPlayers()
{
	// 소켓 생성
	TCPSocketPtr listenSock = SocketUtil::CreateTCPSocket();
	SocketAddress serveraddr(SERVER_IP, SERVER_PORT);

	//소켓 Bind
	if (listenSock->Bind(serveraddr) == SOCKET_ERROR)
	{
		// 바인드 오류 출력
		exit(EXIT_FAILURE);
	}

	// 소켓 Listen상태 전환
	if (listenSock->Listen() == SOCKET_ERROR)
	{
		// 리슨 오류 출력
		exit(EXIT_FAILURE);
	}

	SocketAddress clientaddr;
	int clientNum = 0;

	//MAXMINUM_PLAYER_NUM=3인이 될때까지 플레이어 accept
	while (clientNum < MAXIMUM_PLAYER_NUM)
	{
		TCPSocketPtr clientSocket = listenSock->Accept(clientaddr);
		
		// 클라이언트별 스레드 생성.
		mClientThreads.emplace_back(&Server::ClientThreadFunc, this, clientSocket, clientNum);

		clientNum++;

		mClientSockets.push_back(clientSocket);
	}
}

void Server::ClientThreadFunc(const TCPSocketPtr& clientSock, int clientNum)
{
	while (1)
	{	
		// 클라이언트 패킷 수신
	}
}

void Server::CreateGameWorld()
{
}

void Server::SendPacketToClient(PacketType pType, const TCPSocketPtr& clientSocket)
{
}
