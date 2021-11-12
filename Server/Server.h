#pragma once

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

    void ClientThreadFunc();  // �� Ŭ���̾�Ʈ�κ��� ��Ŷ�� ������ mPacketsFromClient�� ����



private:
    static const int MAXIMUM_PLAYER_NUM = 3;

    vector<TCPSocketPtr> mClientSockets;
};

