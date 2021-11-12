#pragma once

enum class PacketType
{
	Hello,      // 게임 월드 초기화 패킷
	GameStart,  // 게임 시작 신호 패킷
	Update,     // 게임 월드 업데이트 패킷
	GameOver    // 게임 오버 신호 패킷
};
