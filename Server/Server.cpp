#include "ServerPCH.h"
#include "Server.h"

Server::Server()
	: Game()
	, mIsAllHelloPacketSent(nullptr)
{
}

bool Server::Init()
{
	LOG("Server on duty.");

	mIsAllHelloPacketSent = CreateEvent(NULL, NULL, FALSE, NULL);

	InitializeCriticalSection(&mCS);

	//윈속 초기화
	SocketUtil::StaticInit();

	CreateGameWorld();

	// 플레이어 접속 대기
	WaitAllPlayers();

	return true;
}

void Server::Shutdown()
{
	CloseHandle(mIsAllHelloPacketSent);

	DeleteCriticalSection(&mCS);

	// 윈속 제거 
	SocketUtil::StaticShutdown();

	mIsRunning = false;

	for (auto& t : mClientThreads)
	{
		t.join();
	}
}

void Server::Run()
{
	// 게임시작~
	while (true)
	{
		EnterCriticalSection(&mCS);

		auto numPackets = mPackets.size();
		if (numPackets >= mClientSockets.size())
		{
			if (numPackets == 0)
			{
				LOG("All clients disconnected. Quit server.");
				LeaveCriticalSection(&mCS);
				break;
			}

			for (const auto& packet : mPackets)
			{
				UpdatePaddlesPosition(packet);
			}
			mPackets.clear();
			LeaveCriticalSection(&mCS);

			UpdateBallsPosition();
			CheckPaddleAndWall();
			CheckBallAndWall();

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
	// 소켓 생성
	TCPSocketPtr listenSock = SocketUtil::CreateTCPSocket();
	SocketAddress serveraddr(SERVER_PORT);

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
		clientSocket->TurnOffNagleAlgorithm(true);

		LOG("클라이언트 접속: {0}", clientNum);

		// 클라이언트별 스레드 생성.
		mClientThreads.emplace_back(&Server::ClientThreadFunc, this, clientSocket, clientNum);

		clientNum++;

		mClientSockets.push_back(clientSocket);
	}

	WaitForSingleObject(mIsAllHelloPacketSent, INFINITE);

	// 접속을 다받았으니 GameStart패킷타입을 보낸다.
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
		// 클라이언트 패킷 수신
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
			EnterCriticalSection(&mCS);
			mPackets.push_back(packet);
			LeaveCriticalSection(&mCS);
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

void Server::UpdatePaddlesPosition(const ClientToServer& packet)
{
	int id = -1;

	switch (packet.ClientNum)
	{
	case 0:
		id = LEFT_PADDLE_ID;
		break;

	case 1:
		id = RIGHT_PADDLE_ID;
		break;

	default:
		break;
	}

	auto paddle = GetEntity(id);

	if (paddle == nullptr)
	{
		LOG("Server::UpdatePaddlesPosition - client id({0})", packet.ClientNum);
		return;
	}

	auto& transform = paddle->GetComponent<TransformComponent>();
	auto& movement = paddle->GetComponent<MovementComponent>();

	movement.Direction.y = packet.YDirection;

	Systems::UpdatePosition(movement.Speed, movement.Direction, transform.Position);
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

void Server::CheckPaddleAndWall()
{
	auto paddles = mRegistry.view<Paddle>();

	for (auto entity : paddles)
	{
		Entity paddle = Entity(entity, this);

		auto& transform = paddle.GetComponent<TransformComponent>();
		auto& rect = paddle.GetComponent<RectComponent>();

		auto& pos = transform.Position;

		if (pos.y <= 0.0f)
		{
			pos.y = 0.0f;
		}

		if (pos.y + rect.Height >= WINDOW_HEIGHT)
		{
			pos.y = WINDOW_HEIGHT - rect.Height;
		}
	}
}

void Server::CheckBallAndWall()
{
	auto balls = mRegistry.view<Ball>();

	for (auto entity : balls)
	{
		Entity ball = Entity(entity, this);

		auto& transform = ball.GetComponent<TransformComponent>();
		auto& rect = ball.GetComponent<RectComponent>();
		auto& movement = ball.GetComponent<MovementComponent>();

		auto& pos = transform.Position;

		if (pos.y <= 0.0f || pos.y + rect.Height >= WINDOW_HEIGHT)
		{
			movement.Direction.y *= -1.0f;
		}

		if (pos.x <= 0.0f)
		{
			// left side player lose
		}
		else if (pos.x + rect.Width >= WINDOW_WIDTH)
		{
			// right side player lose
		}
	}
}
