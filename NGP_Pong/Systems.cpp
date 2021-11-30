#include "PongPCH.h"
#include "Systems.h"

#include "Packets.h"
#include "Game.h"

void Systems::DrawRect(SDL_Renderer* renderer, float w, float h, const Vector2& position, const SDL_Color& color)
{
	SDL_Rect r{
		static_cast<int>(position.x),
		static_cast<int>(position.y),
		static_cast<int>(w),
		static_cast<int>(h)
	};

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &r);
}

void Systems::UpdatePosition(float speed, const Vector2& direction, Vector2& outPosition, float deltaTime /*= 0.0166f*/)
{
	outPosition.x += speed * direction.x * deltaTime;
	outPosition.y += speed * direction.y * deltaTime;
}

void Systems::AdjustXPos(const Vector2& paddlePosition, const Vector2& ballDirection, Vector2& outBallPosition)
{
	float offset = BALL_WIDTH + 2.0f;

	if (ballDirection.x < 0.0f)
	{
		outBallPosition.x = paddlePosition.x - offset;
	}
	else
	{
		outBallPosition.x = paddlePosition.x + offset;
	}
}

bool Systems::Intersects(const SDL_Rect& a, const SDL_Rect& b)
{
	int leftA = a.x;
	int rightA = a.x + a.w;
	int topA = a.y;
	int bottomA = a.y + a.h;

	int leftB = b.x;
	int rightB = b.x + b.w;
	int topB = b.y;
	int bottomB = b.y + b.h;

	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	return true;
}

CollisionSide Systems::GetCollisionSide(const SDL_Rect& paddleRect, const SDL_Rect& ballRect)
{
	SDL_Rect pRect = paddleRect;

	pRect.x += 7;
	pRect.w = 1;
	pRect.h = 1;

	bool isTopSideCollides = Intersects(pRect, ballRect);

	if (isTopSideCollides)
	{
		return CollisionSide::Top;
	}

	pRect.y += paddleRect.h - 1;

	bool isBottomSideCollides = Intersects(pRect, ballRect);
	
	if (isBottomSideCollides)
	{
		return CollisionSide::Bottom;
	}

	return CollisionSide::Others;
}
