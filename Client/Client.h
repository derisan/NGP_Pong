#pragma once

#include "Game.h"

class Scene;

class Client : public Game
{
public:
	Client();

	virtual bool Init() override;
	virtual void Shutdown() override;
	virtual void Run() override;

	void ChangeScene(Scene* scene);

	void RenderWaitingScreen();

	TCPSocketPtr& GetClientSocket() { return mClientSocket; }

	const string& GetStringInput() const { return mStringInput; }
	string& GetStringInput() { return mStringInput; }

	void RecvPacketFromServer(ServerToClient& outPacket);

private:
	void ProcessInput();
	void Update();
	void Render();

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	TCPSocketPtr mClientSocket;
	Scene* mActiveScene;

	uint32_t mTicksCount;

	string mStringInput;
};

