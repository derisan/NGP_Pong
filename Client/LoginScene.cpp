#include "ClientPCH.h"
#include "LoginScene.h"

LoginScene* LoginScene::sInstance;

void LoginScene::StaticCreate(Client* client)
{
	static LoginScene instance(client);
	sInstance = &instance;
}

LoginScene* LoginScene::Get()
{
	return sInstance;
}

void LoginScene::Enter()
{
	
}

void LoginScene::Exit()
{
	
}

void LoginScene::ProcessInput(const uint8_t* keystate)
{
	
}

void LoginScene::Update(float deltaTime)
{
	
}

void LoginScene::Render(SDL_Renderer* mRenderer)
{

}
