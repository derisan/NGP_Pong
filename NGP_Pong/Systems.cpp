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
	outPosition.x = speed * direction.x * deltaTime;
	outPosition.y = speed * direction.y * deltaTime;
}
