#pragma once

#include <thread>

#include "Game.h"

class Server : public Game
{
public:
    Server();

    virtual bool Init() override;      // 초기화 작업 및 클라이언트 접속 대기
    virtual void Shutdown() override;  // 리소스 정리 수행
    virtual void Run() override;       // 로직 처리가 실행되는 곳. 

private:
    void WaitAllPlayers(); // 모든 플레이어 접속 대기 함수 

    void ClientThreadFunc(const TCPSocketPtr& clientSock, int clientNum); // 각 클라이언트로부터 패킷을 수신해 mPacketsFromClient에 삽입
  
    void CreateGameWorld();

    void SendPacketToClient(const ServerToClient& packet, const TCPSocketPtr& target = nullptr);

private:
    static const int MAXIMUM_PLAYER_NUM = 2;

    vector<TCPSocketPtr> mClientSockets;

    vector<std::thread> mClientThreads;

    HANDLE mIsAllHelloPacketSent;
};

