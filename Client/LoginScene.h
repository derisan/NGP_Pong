#pragma once

#include "Scene.h"

class Client;

class LoginScene : public Scene
{
public:
	LoginScene(Client* client);

	static void StaticCreate(Client* client);
	static LoginScene* Get();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void ProcessInput(const uint8_t* keystate) override;
	virtual void Update(float deltaTime) override;
	virtual void Render(SDL_Renderer* renderer) override;
	
private:
	bool Connects();

private:
	static LoginScene* sInstance;

	SDL_Texture* mBackgroundTexture;
};