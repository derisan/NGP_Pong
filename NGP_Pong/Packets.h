#pragma once

constexpr uint8_t LEFT_PADDLE_ID = 0;
constexpr uint8_t RIGHT_PADDLE_ID = 1;
constexpr uint8_t BALL_ONE_ID = 2;
constexpr uint8_t BALL_TWO_ID = 3;

enum class PacketType
{
	Hello,      // 게임 월드 초기화 패킷
	GameStart,  // 게임 시작 신호 패킷
	Update,     // 게임 월드 업데이트 패킷
	GameOver    // 게임 오버 신호 패킷
};

struct ServerToClient 
{
	PacketType PType;
	int ClientNum;

	uint8_t LeftPaddleID;
	Vector2 LeftPaddlePosition;

	uint8_t RightPaddleID;
	Vector2 RightPaddlePosition;

	uint8_t BallOneID;
	Vector2 BallOnePosition;
};