#pragma once

#include "Game.h"

class Server : public Game
{
public:
    Server();

    virtual bool Init() override;      // 초기화 작업 및 클라이언트 접속 대기
    virtual void Shutdown() override;  // 리소스 정리 수행
    virtual void Run() override;       // 로직 처리가 실행되는 곳. 

private:
    void WaitAllPlayers();

private:
    static const int MAXIMUM_PLAYER_NUM = 3;

    vector<TCPSocketPtr> mClientSockets;
};

