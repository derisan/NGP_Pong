#include "ClientPCH.h"
#include "LoginScene.h"

#include "Client.h"

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
	, mFont(nullptr) {}

void LoginScene::Enter()
{
	mFont = TTF_OpenFont("Assets/lazy.ttf", 72);

	if (mFont == nullptr)
	{
		SDL_Log("Unable to load font: %s", SDL_GetError());
		ASSERT(nullptr, "Failed to load font.");
	}

	SDL_StartTextInput();
}

void LoginScene::Exit()
{
	TTF_CloseFont(mFont);
	mFont = nullptr;
}

void LoginScene::ProcessInput(const uint8_t* keystate)
{
	
}

void LoginScene::Update(float deltaTime)
{
	
}

void LoginScene::Render(SDL_Renderer* mRenderer)
{
	const string& text = mOwner->GetStringInput();

	if (text.size() > 0)
	{
		SDL_Surface* surf = TTF_RenderText_Solid(mFont, text.c_str(), SDL_Color{ 255, 255, 0 });
		
		SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surf);

		SDL_Rect rect;
		rect.x = (mOwner->WINDOW_WIDTH / 2) - (surf->w / 2);
		rect.y = mOwner->WINDOW_HEIGHT / 2;
		rect.w = surf->w;
		rect.h = surf->h;
		SDL_RenderCopy(mRenderer, texture, NULL, &rect);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surf);
	}
}
