#include "ClientPCH.h"
#include "Client.h"

#include "LoginScene.h"
#include "GameScene.h"

#include "TextureManager.h"

Client::Client()
	: Game()
	, mWindow(nullptr)
	, mRenderer(nullptr)
	, mClientSocket(nullptr)
	, mActiveScene(nullptr)
	, mTicksCount(0)
	, mStringInput()
	, mFont(nullptr)
{

}

bool Client::Init()
{
	// 扩加 檬扁拳
	SocketUtil::StaticInit();

	// 努扼捞攫飘 家南 积己
	mClientSocket = SocketUtil::CreateTCPSocket();
	mClientSocket->TurnOffNagleAlgorithm(true);

	// SDL Window, Renderer 积己
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	if (TTF_Init() < 0)
	{
		SDL_Log("Unable to initialize SDL_ttf: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Pong", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mFont = TTF_OpenFont("Assets/lazy.ttf", 72);
	if (mFont == nullptr)
	{
		SDL_Log("Unable to load font: %s", SDL_GetError());
		ASSERT(nullptr, "Failed to load font.");
	}

	SDL_StartTextInput();

	TextureManager::StaticInit(mRenderer);
	
	mTicksCount = SDL_GetTicks();

	// 纠 积己
	LoginScene::StaticCreate(this);
	GameScene::StaticCreate(this);

	mActiveScene = LoginScene::Get();
	mActiveScene->Enter();

	return true;
}

void Client::Shutdown()
{
	TTF_CloseFont(mFont);
	mFont = nullptr;

	if (mActiveScene)
	{
		mActiveScene->Exit();
	}

	SocketUtil::StaticShutdown();
	TextureManager::StaticShutdown();

	SDL_DestroyWindow(mWindow);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Client::Run()
{
	while (mIsRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Client::ChangeScene(Scene* scene)
{
	if (scene == nullptr)
	{
		return;
	}

	mActiveScene->Exit();
	mActiveScene = scene;
	mActiveScene->Enter();
}

void Client::RenderWaitingScreen()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	SDL_Texture* bg = TextureManager::GetTexture("Assets/waiting.png");

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = WINDOW_WIDTH;
	rect.h = WINDOW_HEIGHT;

	SDL_RenderCopyEx(mRenderer, bg, nullptr,
		&rect, 0, nullptr, SDL_FLIP_NONE);

	SDL_RenderPresent(mRenderer);
}

void Client::DrawFont(const string& text, int x, int y, const SDL_Color& color)
{
	if (mFont == nullptr)
	{
		LOG("No font available.");
		return;
	}

	SDL_Surface* surf = TTF_RenderText_Solid(mFont, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surf);

	SDL_Rect rect;
	rect.x = x - (surf->w / 2);
	rect.y = y - (surf->h / 2);
	rect.w = surf->w;
	rect.h = surf->h;
	SDL_RenderCopy(mRenderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surf);
}

void Client::RecvPacketFromServer(ServerToClient& outPacket)
{
	if (mClientSocket == nullptr)
	{
		return;
	}

	int retval = mClientSocket->Recv(&outPacket, sizeof(outPacket));

	if (retval == SOCKET_ERROR || retval == 0)
	{
		LOG("Client::RecvPacketFromServer");
		mIsRunning = false;
	}
}

void Client::SendPacketToServer(const ClientToServer& packet)
{
	if (mClientSocket == nullptr)
	{
		return;
	}

	int retval = mClientSocket->Send(&packet, sizeof(packet));

	if (retval == SOCKET_ERROR)
	{
		LOG("Client::SendPacketToServer");
		mIsRunning = false;
	}
}

void Client::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		case SDL_TEXTINPUT:
			mStringInput += event.text.text;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_BACKSPACE && mStringInput.size() > 0)
			{
				mStringInput.pop_back();
			}
			break;
		}
	}

	const uint8_t* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	if (mActiveScene)
	{
		mActiveScene->ProcessInput(state);
	}
}

void Client::Update()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	if (mActiveScene)
	{
		mActiveScene->Update(deltaTime);
	}
}

void Client::Render()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	if (mActiveScene)
	{
		mActiveScene->Render(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}
