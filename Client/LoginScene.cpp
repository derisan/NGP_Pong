#include "ClientPCH.h"
#include "LoginScene.h"

#include "Client.h"
#include "GameScene.h"

#include "TextureManager.h"

LoginScene* LoginScene::sInstance;

void LoginScene::StaticCreate(Client* client)
{
	static LoginScene instance(client);
	sInstance = &instance;
}

LoginScene* LoginScene::Get()
{
	return sInstance;
}

LoginScene::LoginScene(Client* client)
	: Scene(client)
	, mBackgroundTexture(nullptr)
{}

void LoginScene::Enter()
{
	mBackgroundTexture = TextureManager::GetTexture("Assets/LoginBackground.png");

	if (mBackgroundTexture == nullptr)
	{
		ASSERT(nullptr, "Cannot load texture");
	}
}

void LoginScene::Exit()
{
	mOwner->SetStringInput("");
}

void LoginScene::ProcessInput(const uint8_t* keystate)
{
	if (keystate[SDL_SCANCODE_RETURN])
	{
		bool isConnected = Connects();
		
		if (isConnected)
		{
			mOwner->ChangeScene(GameScene::Get());
		}
	}
}

void LoginScene::Update(float deltaTime)
{
	
}

void LoginScene::Render(SDL_Renderer* renderer)
{
	// Draw background image first
	{
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = mOwner->WINDOW_WIDTH;
		rect.h = mOwner->WINDOW_HEIGHT;

		SDL_RenderCopyEx(renderer, mBackgroundTexture, nullptr,
			&rect, 0, nullptr, SDL_FLIP_NONE);
	}

	const string& text = mOwner->GetStringInput();

	if (text.size() > 0)
	{
		int x = mOwner->WINDOW_WIDTH / 2;
		int y = mOwner->WINDOW_HEIGHT / 2;

		mOwner->DrawFont(text, x, y);
	}
}

bool LoginScene::Connects()
{
	TCPSocketPtr& sock = mOwner->GetClientSocket();

	SocketAddress serveraddr(mOwner->GetStringInput(), mOwner->SERVER_PORT);

	if (sock->Connect(serveraddr) != SOCKET_ERROR)
	{
		return true;
	}

	return false;
}