#include "ServerPCH.h"
#include "Server.h"

Server::Server()
	: Game()
{
}

bool Server::Init()
{
	//���� �ʱ�ȭ
	SocketUtil::StaticInit();

	// �÷��̾� ���� ���
	WaitAllPlayers();

	return true;
	
}

void Server::Shutdown()
{
	// ���� ���� 
	SocketUtil::StaticShutdown();
}

void Server::Run()
{
	// ���ӽ���~
}

void Server::WaitAllPlayers()
{
	// ���� ����
	TCPSocketPtr listenSock = SocketUtil::CreateTCPSocket();
	SocketAddress serveraddr(SERVER_IP, SERVER_PORT);

	//���� Bind
	if (listenSock->Bind(serveraddr) == SOCKET_ERROR)
	{
		// ���ε� ���� ���
		exit(EXIT_FAILURE);
	}

	// ���� Listen���� ��ȯ
	if (listenSock->Listen() == SOCKET_ERROR)
	{
		// ���� ���� ���
		exit(EXIT_FAILURE);
	}

	SocketAddress clientaddr;
	int clientNum = 0;

	//MAXMINUM_PLAYER_NUM=3���� �ɶ����� �÷��̾� accept
	while (clientNum < MAXIMUM_PLAYER_NUM)
	{
		TCPSocketPtr clientSocket = listenSock->Accept(clientaddr);
		
		// Ŭ���̾�Ʈ�� ������ ����.
		mClientThreads.emplace_back(&Server::ClientThreadFunc, this, clientSocket, clientNum);

		clientNum++;

		mClientSockets.push_back(clientSocket);
	}
}

void Server::ClientThreadFunc(const TCPSocketPtr& clientSock, int clientNum)
{
	while (1)
	{	
		// Ŭ���̾�Ʈ ��Ŷ ����
	}
}

void Server::CreateGameWorld()
{
}

void Server::SendPacketToClient(PacketType pType, const TCPSocketPtr& clientSocket)
{
}
