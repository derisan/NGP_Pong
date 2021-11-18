#pragma once

#include <thread>

#include "Game.h"

class Server : public Game
{
public:
    Server();

    virtual bool Init() override;      // �ʱ�ȭ �۾� �� Ŭ���̾�Ʈ ���� ���
    virtual void Shutdown() override;  // ���ҽ� ���� ����
    virtual void Run() override;       // ���� ó���� ����Ǵ� ��. 

private:
    void WaitAllPlayers(); // ��� �÷��̾� ���� ��� �Լ� 

    void ClientThreadFunc(const TCPSocketPtr& clientSock, int clientNum); // �� Ŭ���̾�Ʈ�κ��� ��Ŷ�� ������ mPacketsFromClient�� ����
  
    void CreateGameWorld();

    void SendPacketToClient(const ServerToClient& packet, const TCPSocketPtr& target = nullptr);

private:
    static const int MAXIMUM_PLAYER_NUM = 2;

    vector<TCPSocketPtr> mClientSockets;

    vector<std::thread> mClientThreads;

    HANDLE mIsAllHelloPacketSent;
};

