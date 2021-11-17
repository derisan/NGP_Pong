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

	mOwner->RecvPacketFromServer(packet);

	if (packet.PType == PacketType::Hello)
	{
		ProcessPacket(packet);
	}
	else
	{
		ASSERT(nullptr, "Client should recv hello packet first.");
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
}

void GameScene::Update(float deltaTime)
{
	ServerToClient packet;

	//mOwner->RecvPacketFromServer(packet);

	if (packet.PType == PacketType::Update)
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

void GameScene::ProcessUpdatePacket(const ServerToClient& packet)
{

}
