#pragma once

class Client;

class Scene
{
public:
	Scene(Client* client)
		: mOwner(client) {}
	virtual ~Scene() = default;

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void ProcessInput(const uint8_t* keystate) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(SDL_Renderer* mRenderer) = 0;

private:
	Client* mOwner;
};

