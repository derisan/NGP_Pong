#include "ClientPCH.h"
#include "GameScene.h"

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
	LOG("Hello, GameScene!");
}

void GameScene::Exit()
{
	
}

void GameScene::ProcessInput(const uint8_t* keystate)
{

}

void GameScene::Update(float deltaTime)
{

}

void GameScene::Render(SDL_Renderer* mRenderer)
{

}
