#include "PongPCH.h"
#include "Game.h"

#include "Tags.h"
#include "Components.h"
#include "Entity.h"

Game::Game()
	: mIsRunning(true)
{

}

Entity* Game::CreateEntity(uint8_t id)
{
	Entity* e = new Entity{ mRegistry.create(), this };
	e->AddComponent<TransformComponent>();
	e->AddComponent<IdComponent>(id);

	return e;
}

Entity* Game::CreatePaddle(uint8_t id)
{
	auto e = CreateEntity(id);
	e->AddComponent<RectComponent>(PADDLE_WIDTH, PADDLE_HEIGHT);
	e->AddComponent<MovementComponent>(PADDLE_SPEED);
	e->AddTag<Paddle>();

	return e;
}

Entity* Game::CreateBall(uint8_t id)
{
	auto e = CreateEntity(id);
	e->AddComponent<RectComponent>(BALL_WIDTH, BALL_WIDTH);
	e->AddComponent<MovementComponent>(BALL_SPEED);
	e->AddTag<Ball>();

	return e;
}
