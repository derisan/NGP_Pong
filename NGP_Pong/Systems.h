#pragma once

enum class CollisionSide
{
	Top,
	Bottom,
	Others
};

class Systems
{
public:
	static void DrawRectColor(SDL_Renderer* renderer, float w, float h, const Vector2& position);
	static void DrawRect(SDL_Renderer* renderer, float w, float h, const Vector2& position);
	static void UpdatePosition(float speed, const Vector2& direction, Vector2& outPosition, float deltaTime = 0.0166f);
	static void AdjustXPos(const Vector2& paddlePosition, const Vector2& ballDirection, Vector2& outBallPosition);
	static bool Intersects(const SDL_Rect& a, const SDL_Rect& b);
	static CollisionSide GetCollisionSide(const SDL_Rect& paddleRect, const SDL_Rect& ballRect);
};