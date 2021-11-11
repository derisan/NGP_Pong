#pragma once

class Entity;

class Game
{
	friend class Entity;

public:
	Game();
	virtual ~Game() = default;

	virtual bool Init() = 0;
	virtual void Shutdown() = 0;
	virtual void Run() = 0;

	Entity* CreateEntity(uint8_t id);
	Entity* CreatePaddle(uint8_t id);
	Entity* CreateBall(uint8_t id);

	const int SERVER_PORT = 9000;
	const char* SERVER_IP = "127.0.0.1";

	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	const float PADDLE_WIDTH = 15.0f;
	const float PADDLE_HEIGHT = 100.0f;
	const float PADDLE_SPEED = 200.0f;
	const float BALL_WIDTH = 15.0f;
	const float BALL_SPEED = 150.0f;

	entt::registry mRegistry;

protected:
	bool mIsRunning;

	unordered_map<uint8_t, Entity*> mEntities;
};
