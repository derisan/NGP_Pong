#include "ClientPCH.h"
#include "GameScene.h"

#include "Client.h"

GameScene* GameScene::sInstance;

GameScene::GameScene(Client* client)
	: Scene(client)
	, mClientNum(-1)
{

}

void GameScene::StaticCreate(Client* client)
{
	static GameScene instance(client);
	sInstance = &instance;
}

GameScene* GameScene::Get()
{
	return sInstance;
}

void GameScene::Enter()
{
	ServerToClient packet;

	int retval = mOwner->RecvPacketFromServer(packet);

	// Hello Packet
	if (retval == NO_ERROR)
	{
		ProcessPacket(packet);
	}

	mOwner->RenderWaitingScreen();

	retval = mOwner->RecvPacketFromServer(packet);

	// GameStart Packet	
	if (retval == NO_ERROR)
	{
		ProcessPacket(packet);
	}

	retval = mOwner->GetClientSocket()->SetNonBlockingMode(true);

	if (retval == SOCKET_ERROR)
	{
		ASSERT(nullptr, "Cannot convert to nonblocking socket.");
	}

	// Hack for resolving deadlock
	{
		ClientToServer packet;

		packet.ClientNum = mClientNum;
		packet.YDirection = 0.0f;

		mOwner->SendPacketToServer(packet);
	}
}

void GameScene::Exit()
{
	
}

void GameScene::ProcessInput(const uint8_t* keystate)
{
	float yDirection = 0.0f;

	if (keystate[SDL_SCANCODE_W])
	{
		yDirection -= 1.0f;
	}

	if (keystate[SDL_SCANCODE_S])
	{
		yDirection += 1.0f;
	}

	ClientToServer packet;

	packet.ClientNum = mClientNum;
	packet.YDirection = yDirection;

	mOwner->SendPacketToServer(packet);
}

void GameScene::Update(float deltaTime)
{
	ServerToClient packet;

	int retval = mOwner->RecvPacketFromServer(packet);

	if (retval == WOULD_BLOCK)
	{
		return;
	}
	else if (retval == NO_ERROR)
	{
		ProcessPacket(packet);
	}
}

void GameScene::Render(SDL_Renderer* renderer)
{
	auto view = (mOwner->mRegistry).view<RectComponent, TransformComponent>();
	for (auto entity : view)
	{
		auto [rect, transform] = view.get<RectComponent, TransformComponent>(entity);

		Systems::DrawRect(renderer, rect.Width, rect.Height, transform.Position);
	}
}

void GameScene::ProcessPacket(const ServerToClient& packet)
{
	switch (packet.PType)
	{
	case PacketType::Hello:
		ProcessHelloPacket(packet);
		break;

	case PacketType::GameStart:
		ProcessGameStartPacket(packet);
		break;

	case PacketType::Update:
		ProcessUpdatePacket(packet);
		break;

	case PacketType::GameOver:
		break;

	default:
		LOG("Unknown packet type.");
		break;
	}
}

void GameScene::ProcessHelloPacket(const ServerToClient& packet)
{
	mClientNum = packet.ClientNum;

	LOG("My client num: {0}", mClientNum);

	// Create left paddle
	{
		auto leftPaddle = mOwner->CreatePaddle(packet.LeftPaddleID);
		auto& transform = leftPaddle->GetComponent<TransformComponent>();
		transform.Position = packet.LeftPaddlePosition;
	}

	// Create right paddle
	{
		auto rightPaddle = mOwner->CreatePaddle(packet.RightPaddleID);
		auto& transform = rightPaddle->GetComponent<TransformComponent>();
		transform.Position = packet.RightPaddlePosition;
	}

	// Create ball one
	{
		auto ballOne = mOwner->CreateBall(packet.BallOneID);
		auto& transform = ballOne->GetComponent<TransformComponent>();
		transform.Position = packet.BallOnePosition;
	}
}

void GameScene::ProcessGameStartPacket(const ServerToClient& packet)
{
	
}

void GameScene::ProcessUpdatePacket(const ServerToClient& packet)
{
	{
		auto leftPaddle = mOwner->GetEntity(packet.LeftPaddleID);
		auto& transform = leftPaddle->GetComponent<TransformComponent>();
		transform.Position = packet.LeftPaddlePosition;
	}

	{
		auto rightPaddle = mOwner->GetEntity(packet.RightPaddleID);
		auto& transform = rightPaddle->GetComponent<TransformComponent>();
		transform.Position = packet.RightPaddlePosition;
	}

	{
		auto ballOne = mOwner->GetEntity(packet.BallOneID);
		auto& transform = ballOne->GetComponent<TransformComponent>();
		transform.Position = packet.BallOnePosition;
	}
}
