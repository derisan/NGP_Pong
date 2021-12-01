#pragma once

class Entity;

constexpr int SERVER_PORT = 9000;
static const char* SERVER_IP = "127.0.0.1";

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;
constexpr float PADDLE_WIDTH = 15.0f;
constexpr float PADDLE_HEIGHT = 100.0f;
constexpr float SMALL_PADDLE_HEIGHT = 50.0f;
constexpr float PADDLE_SPEED = 200.0f;
constexpr float BALL_WIDTH = 15.0f;
constexpr float BALL_SPEED = 150.0f;
constexpr float MAX_BALL_SPEED = 200.0f;

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

	Entity* GetEntity(uint8_t id);

	entt::registry mRegistry;

protected:
	bool mIsRunning;

	unordered_map<uint8_t, Entity*> mEntities;
};
