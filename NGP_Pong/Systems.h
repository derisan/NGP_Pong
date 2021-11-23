#pragma once

class Systems
{
public:
	static void DrawRect(SDL_Renderer* renderer, float w, float h, const Vector2& position);
	static void UpdatePosition(float speed, const Vector2& direction, Vector2& outPosition, float deltaTime = 0.0166f);
	static bool Intersects(const SDL_Rect& a, const SDL_Rect& b);
};