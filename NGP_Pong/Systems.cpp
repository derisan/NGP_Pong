#include "PongPCH.h"
#include "Systems.h"

void Systems::DrawRect(SDL_Renderer* renderer, float w, float h, const Vector2& position)
{
	SDL_Rect r{
		static_cast<int>(position.x),
		static_cast<int>(position.y),
		static_cast<int>(w),
		static_cast<int>(h)
	};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &r);
}

void Systems::UpdatePosition(float speed, const Vector2& direction, Vector2& outPosition, float deltaTime /*= 0.0166f*/)
{
	outPosition.x += speed * direction.x * deltaTime;
	outPosition.y += speed * direction.y * deltaTime;
}

void Systems::IncrementPosition(const Vector2& direction, Vector2& outPosition, float value /*= 7.5f*/)
{
	outPosition.x += value * direction.x;
	outPosition.y += value * direction.y;
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
