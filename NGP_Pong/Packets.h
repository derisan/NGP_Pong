#pragma once

enum EntityID : uint8_t
{
	LEFT_PADDLE_ID = 0,
	RIGHT_PADDLE_ID,
	L2_PADDLE_ID,
	R2_PADDLE_ID,
	BALL_ONE_ID,
	BALL_TWO_ID
};

enum class PacketType
{
	Hello,      // ���� ���� �ʱ�ȭ ��Ŷ
	GameStart,  // ���� ���� ��ȣ ��Ŷ
	Update,     // ���� ���� ������Ʈ ��Ŷ
	GameOver    // ���� ���� ��ȣ ��Ŷ
};

enum class WhoLose
{
	Left,
	Right,
	None
};

struct ServerToClient 
{
	PacketType PType;
	int ClientNum;
	WhoLose Who;

	uint8_t LeftPaddleID;
	Vector2 LeftPaddlePosition;

	uint8_t RightPaddleID;
	Vector2 RightPaddlePosition;

	uint8_t L2PaddleID;
	Vector2 L2PaddlePosition;

	uint8_t R2PaddleID;
	Vector2 R2PaddlePosition;

	uint8_t BallOneID;
	Vector2 BallOnePosition;

	uint8_t BallTwoID;
	Vector2 BallTwoPosition;
};

struct ClientToServer
{
	int ClientNum;
	float YDirection;
};