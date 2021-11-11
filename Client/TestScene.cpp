#include "ClientPCH.h"
#include "TestScene.h"

TestScene* TestScene::sInstance;

void TestScene::StaticCreate(Client* client)
{
	static TestScene instance(client);
	sInstance = &instance;
}

TestScene* TestScene::Get()
{
	return sInstance;
}

void TestScene::Enter()
{
	LOG("TestScene::Enter()");
}

void TestScene::Exit()
{
	LOG("TestScene::Exit()");
}

void TestScene::ProcessInput(const uint8_t* keystate)
{
	
}

void TestScene::Update(float deltaTime)
{
	LOG("TestScene::Update()");
}

void TestScene::Render(SDL_Renderer* mRenderer)
{

}
