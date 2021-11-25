#pragma once

#include "Scene.h"

class GameScene : public Scene
{
public:
	GameScene(Client* client);

	static void StaticCreate(Client* client);
	static GameScene* Get();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void ProcessInput(const uint8_t* keystate) override;
	virtual void Update(float deltaTime) override;
	virtual void Render(SDL_Renderer* renderer) override;

private:
	void ProcessPacket(const ServerToClient& packet);
	void ProcessHelloPacket(const ServerToClient& packet);
	void ProcessGameStartPacket(const ServerToClient& packet);
	void ProcessUpdatePacket(const ServerToClient& packet);
	void ProcessGameOverPacket(const ServerToClient& packet);

private:
	static GameScene* sInstance;

	int mClientNum;

	bool mShouldSend;
};