#include "ServerPCH.h"
#include "Server.h"

Server::Server()
	: Game()
	, mIsAllHelloPacketSent(nullptr)
{
}

bool Server::Init()
{
	mIsAllHelloPacketSent = CreateEvent(NULL, NULL, FALSE, NULL);

	InitializeCriticalSection(&mCS);

	//���� �ʱ�ȭ
	SocketUtil::StaticInit();
		
	CreateGameWorld();

	// �÷��̾� ���� ���
	WaitAllPlayers();

	return true;
}

void Server::Shutdown()
{
	CloseHandle(mIsAllHelloPacketSent);

	DeleteCriticalSection(&mCS);

	// ���� ���� 
	SocketUtil::StaticShutdown();

	mIsRunning = false;

	for (auto& t : mClientThreads)
	{
		t.join();
	}
}

void Server::Run()
{
	// ���ӽ���~
	while (true)
	{
		EnterCriticalSection(&mCS);
		if (!mPackets.empty())
		{
			ClientToServer ctsPacket = mPackets.front();
			mPackets.pop_front();

			LeaveCriticalSection(&mCS);
			
			UpdatePaddlesPosition();
			UpdateBallsPosition();

			ServerToClient stcPacket;

			stcPacket.PType = PacketType::Update;
			stcPacket.ClientNum = -1;
			stcPacket.LeftPaddleID = LEFT_PADDLE_ID;
			stcPacket.LeftPaddlePosition = GetEntity(LEFT_PADDLE_ID)->GetComponent<TransformComponent>().Position;
			stcPacket.RightPaddleID = RIGHT_PADDLE_ID;
			stcPacket.RightPaddlePosition = GetEntity(RIGHT_PADDLE_ID)->GetComponent<TransformComponent>().Position;
			stcPacket.BallOneID = BALL_ONE_ID;
			stcPacket.BallOnePosition = GetEntity(BALL_ONE_ID)->GetComponent<TransformComponent>().Position;

			SendPacketToClient(stcPacket);
		}
		else
		{
			LeaveCriticalSection(&mCS);
			continue;
		}
	}
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
		
		LOG("Ŭ���̾�Ʈ ����: {0}", clientNum);
		
		// Ŭ���̾�Ʈ�� ������ ����.
		mClientThreads.emplace_back(&Server::ClientThreadFunc, this, clientSocket, clientNum);

		clientNum++;

		mClientSockets.push_back(clientSocket);
	}

	WaitForSingleObject(mIsAllHelloPacketSent, INFINITE);

	// ������ �ٹ޾����� GameStart��ŶŸ���� ������.
	ServerToClient Packet;
	ZeroMemory(&Packet, sizeof(Packet));
	Packet.PType = PacketType::GameStart;

	SendPacketToClient(Packet);
}

void Server::ClientThreadFunc(const TCPSocketPtr& clientSock, int clientNum)
{
	// Send HelloPacket to client
	ServerToClient packet;

	packet.PType = PacketType::Hello;
	packet.ClientNum = clientNum;
	packet.LeftPaddleID = LEFT_PADDLE_ID;
	packet.LeftPaddlePosition = mEntities[LEFT_PADDLE_ID]->GetComponent<TransformComponent>().Position;
	packet.RightPaddleID = RIGHT_PADDLE_ID;
	packet.RightPaddlePosition = mEntities[RIGHT_PADDLE_ID]->GetComponent<TransformComponent>().Position;
	packet.BallOneID = BALL_ONE_ID;
	packet.BallOnePosition = mEntities[BALL_ONE_ID]->GetComponent<TransformComponent>().Position;

	SendPacketToClient(packet, clientSock);

	if (clientNum == (MAXIMUM_PLAYER_NUM - 1))
	{
		SetEvent(mIsAllHelloPacketSent);
	}

	while (1)
	{	
		// Ŭ���̾�Ʈ ��Ŷ ����
		bool shouldQuit = !mIsRunning;

		if (shouldQuit)
		{
			break;
		}

		ClientToServer packet;
		int retval = RecvPacketFromClient(packet, clientSock);

		if (retval == SOCKET_ERROR)
		{
			break;
		}
		else if (retval == 0)
		{
			break;
		}
		else
		{
			mPackets.push_back(packet);
		}
	}

	auto iter = std::find(mClientSockets.begin(), mClientSockets.end(), clientSock);
	if (iter != mClientSockets.end())
	{
		LOG("Client {0} disconnected.", clientNum);
		mClientSockets.erase(iter);
	}
	else
	{
		LOG("There is no client socket in vector.");
	}
}

void Server::CreateGameWorld()
{
	// Create left paddle
	{
		auto paddle = CreatePaddle(LEFT_PADDLE_ID);
		auto& transform = paddle->GetComponent<TransformComponent>();
		transform.Position = Vector2(0.0f, (WINDOW_HEIGHT / 2) - (PADDLE_HEIGHT / 2));
	}

	// Create right paddle
	{
		auto paddle = CreatePaddle(RIGHT_PADDLE_ID);
		auto& transform = paddle->GetComponent<TransformComponent>();
		transform.Position = Vector2((WINDOW_WIDTH - PADDLE_WIDTH), (WINDOW_HEIGHT / 2) - (PADDLE_HEIGHT / 2));
	}

	// Create Ball one
	{
		auto ball = CreateBall(BALL_ONE_ID);
		auto& transform = ball->GetComponent<TransformComponent>();
		auto& movement = ball->GetComponent<MovementComponent>();
		transform.Position = Vector2((WINDOW_WIDTH / 2) - (BALL_WIDTH / 2), (WINDOW_HEIGHT / 2) - (BALL_WIDTH / 2));  // Center of screen
		movement.Direction = Vector2(-1.0f, -1.0f); // Up-Left
	}
}

void Server::SendPacketToClient(const ServerToClient& packet, const TCPSocketPtr& target)
{
	if (target)
	{
		target->Send(&packet, sizeof(packet));
	}
	else
	{
		for (const auto& sock : mClientSockets)
		{
			sock->Send(&packet, sizeof(packet));
		}
	}
}

int Server::RecvPacketFromClient(ClientToServer& outPacket, const TCPSocketPtr& target)
{
	int retval = target->Recv(&outPacket, sizeof(outPacket), MSG_WAITALL);

	return retval;
}

void Server::UpdatePaddlesPosition()
{
	auto view = mRegistry.view<Paddle>();

	for (auto entity : view)
	{
		Entity e = { entity, this };

		auto& transform = e.GetComponent<TransformComponent>();
		auto& movement = e.GetComponent<MovementComponent>();

		Systems::UpdatePosition(movement.Speed, movement.Direction, transform.Position);
	}
}

void Server::UpdateBallsPosition()
{
	auto view = mRegistry.view<Ball>();

	for (auto entity : view)
	{
		Entity e = { entity, this };

		auto& transform = e.GetComponent<TransformComponent>();
		auto& movement = e.GetComponent<MovementComponent>();

		Systems::UpdatePosition(movement.Speed, movement.Direction, transform.Position);
	}
}