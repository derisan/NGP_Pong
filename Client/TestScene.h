#pragma once

#include "Scene.h"

class Client;

class TestScene : public Scene
{
public:
	TestScene(Client* client)
		: Scene(client) {}

	static void StaticCreate(Client* client);
	static TestScene* Get();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void ProcessInput(const uint8_t* keystate) override;
	virtual void Update(float deltaTime) override;
	virtual void Render(SDL_Renderer* mRenderer) override;

private:
	static TestScene* sInstance;
};