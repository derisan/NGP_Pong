#pragma once

constexpr uint8_t LEFT_PADDLE_ID = 0;
constexpr uint8_t RIGHT_PADDLE_ID = 1;
constexpr uint8_t BALL_ONE_ID = 2;
constexpr uint8_t BALL_TWO_ID = 3;

enum class PacketType
{
	Hello,      // ���� ���� �ʱ�ȭ ��Ŷ
	GameStart,  // ���� ���� ��ȣ ��Ŷ
	Update,     // ���� ���� ������Ʈ ��Ŷ
	GameOver    // ���� ���� ��ȣ ��Ŷ
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