#include "ClientPCH.h"
#include "GameScene.h"

#include "Client.h"

GameScene* GameScene::sInstance;

GameScene::GameScene(Client* client)
	: Scene(client)
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
	// Rendering test code
	mOwner->CreatePaddle(0);
	////////////////////////
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
