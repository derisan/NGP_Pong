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
	, mFont(nullptr)
	, mBackgroundTexture(nullptr)
{}

void LoginScene::Enter()
{
	mFont = TTF_OpenFont("Assets/lazy.ttf", 72);

	if (mFont == nullptr)
	{
		SDL_Log("Unable to load font: %s", SDL_GetError());
		ASSERT(nullptr, "Failed to load font.");
	}

	SDL_StartTextInput();

	mBackgroundTexture = TextureManager::GetTexture("Assets/LoginBackground.png");

	if (mBackgroundTexture == nullptr)
	{
		ASSERT(nullptr, "Cannot load texture");
	}
}

void LoginScene::Exit()
{
	TTF_CloseFont(mFont);
	mFont = nullptr;
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
		SDL_Surface* surf = TTF_RenderText_Solid(mFont, text.c_str(), SDL_Color{ 0, 0, 0 });
		
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);

		SDL_Rect rect;
		rect.x = (mOwner->WINDOW_WIDTH / 2) - (surf->w / 2);
		rect.y = mOwner->WINDOW_HEIGHT / 2;
		rect.w = surf->w;
		rect.h = surf->h;
		SDL_RenderCopy(renderer, texture, NULL, &rect);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surf);
	}
}

bool LoginScene::Connects()
{
	TCPSocketPtr& sock = mOwner->GetClientSocket();

	SocketAddress serveraddr(mOwner->SERVER_IP, mOwner->SERVER_PORT);

	if (sock->Connect(serveraddr) != SOCKET_ERROR)
	{
		return true;
	}

	return false;
}