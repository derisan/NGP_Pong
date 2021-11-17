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
		ProcessHelloPacket(packet);
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
	// TODO :: send packet that contains player's input state to server
}

void GameScene::Update(float deltaTime)
{
	// TODO :: recv packet from server and update all entities' position
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