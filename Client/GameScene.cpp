#include "ClientPCH.h"
#include "GameScene.h"

#include "Client.h"
#include "TextureManager.h"

GameScene* GameScene::sInstance;

GameScene::GameScene(Client* client)
	: Scene(client)
	, mClientNum(-1)
	, mShouldSend(true)
	, mScores{ 0, 0 }
	, mWinLoseTexture(nullptr)
	, mIsShowingWinningScreen(false)
	, mElapsed(0.0f)
{

}

void GameScene::StaticCreate(Client* client)
{
	static GameScene instance(client);
	sInstance = &instance;
}

GameScene* GameScene::Get()
{
	return sInstance;
}

void GameScene::Enter()
{
	ServerToClient packet;

	mOwner->RecvPacketFromServer(packet);

	if (packet.PType == PacketType::Hello)
	{
		ProcessPacket(packet);
	}
	else
	{
		ASSERT(nullptr, "Client should recv hello packet first.");
	}

	mOwner->RenderWaitingScreen();

	mOwner->RecvPacketFromServer(packet);

	if (packet.PType == PacketType::GameStart)
	{
		ProcessPacket(packet);
	}
	else
	{
		ASSERT(nullptr, "Client should recv game start packet.");
	}

	// Hack for resolving deadlock
	{
		ClientToServer packet;

		packet.ClientNum = mClientNum;
		packet.YDirection = 0.0f;

		mOwner->SendPacketToServer(packet);
	}
}

void GameScene::Exit()
{

}

void GameScene::ProcessInput(const uint8_t* keystate)
{
	if (mShouldSend == false || mIsShowingWinningScreen)
	{
		return;
	}

	float yDirection = 0.0f;

	if (keystate[SDL_SCANCODE_W])
	{
		yDirection -= 1.0f;
	}

	if (keystate[SDL_SCANCODE_S])
	{
		yDirection += 1.0f;
	}

	ClientToServer packet;

	packet.ClientNum = mClientNum;
	packet.YDirection = yDirection;

	mOwner->SendPacketToServer(packet);
}

void GameScene::Update(float deltaTime)
{
	if (mIsShowingWinningScreen)
	{
		mElapsed += deltaTime;

		if (mElapsed >= 2.0f)
		{
			mIsShowingWinningScreen = false;
			mElapsed = 0.0f;
		}

		return;
	}

	if (mShouldSend == false)
	{
		mShouldSend = true;
		return;
	}

	ServerToClient packet;

	mOwner->RecvPacketFromServer(packet);

	ProcessPacket(packet);
}

void GameScene::Render(SDL_Renderer* renderer)
{
	auto view = (mOwner->mRegistry).view<RectComponent>();
	for (auto entity : view)
	{
		Entity e = Entity(entity, mOwner);

		auto& rect = e.GetComponent<RectComponent>();
		auto& transform = e.GetComponent<TransformComponent>();
		auto& id = e.GetComponent<IdComponent>();

		SDL_Color drawColor = { 255, 255, 255, 255 };

		bool isMyPaddle = std::any_of(myPaddlesID.begin(), myPaddlesID.end(), [&id](uint8_t a) {
			return a == id.ID; });

		if (isMyPaddle)
		{
			drawColor = MY_PADDLE_COLOR;
		}

		Systems::DrawRect(renderer, rect.Width, rect.Height, transform.Position, drawColor);
	}

	mOwner->DrawFont(std::to_string(mScores.Left), WINDOW_WIDTH / 4,
		50, SDL_Color{ 255, 255, 0 });

	mOwner->DrawFont(std::to_string(mScores.Right), WINDOW_WIDTH - (WINDOW_WIDTH / 4),
		50, SDL_Color{ 255, 255, 0 });

	if (mIsShowingWinningScreen)
	{
		{
			SDL_Rect rect;
			rect.x = WINDOW_WIDTH / 4;
			rect.y = WINDOW_HEIGHT / 4;
			rect.w = WINDOW_WIDTH / 2;
			rect.h = WINDOW_HEIGHT / 2;

			SDL_RenderCopyEx(renderer, mWinLoseTexture, nullptr,
				&rect, 0, nullptr, SDL_FLIP_NONE);
		}
	}
}

void GameScene::ProcessPacket(const ServerToClient& packet)
{
	switch (packet.PType)
	{
	case PacketType::Hello:
		ProcessHelloPacket(packet);
		break;

	case PacketType::GameStart:
		ProcessGameStartPacket(packet);
		break;

	case PacketType::Update:
		ProcessUpdatePacket(packet);
		break;

	case PacketType::GameOver:
		ProcessGameOverPacket(packet);
		break;

	default:
		LOG("Unknown packet type.");
		break;
	}
}

void GameScene::ProcessHelloPacket(const ServerToClient& packet)
{
	mClientNum = packet.ClientNum;

	LOG("My client num: {0}", mClientNum);

	switch (packet.ClientNum)
	{
	case 0:
		myPaddlesID.push_back(packet.LeftPaddleID);
		break;

	case 1:
		myPaddlesID.push_back(packet.RightPaddleID);
		myPaddlesID.push_back(packet.R2PaddleID);
		break;

	case 2:
		myPaddlesID.push_back(packet.L2PaddleID);
		break;

	default:
		LOG("Unknown client number.");
		break;
	}

	// Create left paddle
	{
		auto leftPaddle = mOwner->CreatePaddle(packet.LeftPaddleID);
		auto& transform = leftPaddle->GetComponent<TransformComponent>();
		transform.Position = packet.LeftPaddlePosition;
	}

	// Create right paddle
	{
		auto rightPaddle = mOwner->CreatePaddle(packet.RightPaddleID);
		auto& transform = rightPaddle->GetComponent<TransformComponent>();
		transform.Position = packet.RightPaddlePosition;
	}

	// Create l2 paddle
	{
		auto l2Paddle = mOwner->CreatePaddle(packet.L2PaddleID);
		auto& transform = l2Paddle->GetComponent<TransformComponent>();
		transform.Position = packet.L2PaddlePosition;
	}

	// Create r2 paddle
	{
		auto r2Paddle = mOwner->CreatePaddle(packet.R2PaddleID);
		auto& transform = r2Paddle->GetComponent<TransformComponent>();
		transform.Position = packet.R2PaddlePosition;
		auto& rect = r2Paddle->GetComponent<RectComponent>();
		rect.Height = SMALL_PADDLE_HEIGHT;
	}

	// Create ball one
	{
		auto ballOne = mOwner->CreateBall(packet.BallOneID);
		auto& transform = ballOne->GetComponent<TransformComponent>();
		transform.Position = packet.BallOnePosition;
	}

	// Create ball two
	{
		auto ballTwo = mOwner->CreateBall(packet.BallTwoID);
		auto& transform = ballTwo->GetComponent<TransformComponent>();
		transform.Position = packet.BallTwoPosition;
	}
}

void GameScene::ProcessGameStartPacket(const ServerToClient& packet)
{
	LOG("Game start!!!");
}

void GameScene::ProcessUpdatePacket(const ServerToClient& packet)
{
	{
		auto leftPaddle = mOwner->GetEntity(packet.LeftPaddleID);
		auto& transform = leftPaddle->GetComponent<TransformComponent>();
		transform.Position = packet.LeftPaddlePosition;
	}

	{
		auto rightPaddle = mOwner->GetEntity(packet.RightPaddleID);
		auto& transform = rightPaddle->GetComponent<TransformComponent>();
		transform.Position = packet.RightPaddlePosition;
	}

	{
		auto l2Paddle = mOwner->GetEntity(packet.L2PaddleID);
		auto& transform = l2Paddle->GetComponent<TransformComponent>();
		transform.Position = packet.L2PaddlePosition;
	}

	{
		auto r2Paddle = mOwner->GetEntity(packet.R2PaddleID);
		auto& transform = r2Paddle->GetComponent<TransformComponent>();
		transform.Position = packet.R2PaddlePosition;
	}

	{
		auto ballOne = mOwner->GetEntity(packet.BallOneID);
		auto& transform = ballOne->GetComponent<TransformComponent>();
		transform.Position = packet.BallOnePosition;
	}

	{
		auto ballTwo = mOwner->GetEntity(packet.BallTwoID);
		auto& transform = ballTwo->GetComponent<TransformComponent>();
		transform.Position = packet.BallTwoPosition;
	}
}

void GameScene::ProcessGameOverPacket(const ServerToClient& packet)
{
	switch (packet.Who)
	{
	case WhoLose::Left:
		mScores.Right += 1;
		break;

	case WhoLose::Right:
		mScores.Left += 1;
		break;

	case WhoLose::LeftWin:
		if (mClientNum == 0 || mClientNum == 2)
		{
			mWinLoseTexture = TextureManager::GetTexture("Assets/YouWin.png");
		}
		else
		{
			mWinLoseTexture = TextureManager::GetTexture("Assets/YouLose.png");
		}
		mIsShowingWinningScreen = true;
		mScores = { 0, 0 };
		break;

	case WhoLose::RightWin:
		if (mClientNum == 1)
		{
			mWinLoseTexture = TextureManager::GetTexture("Assets/YouWin.png");
		}
		else
		{
			mWinLoseTexture = TextureManager::GetTexture("Assets/YouLose.png");
		}
		mIsShowingWinningScreen = true;
		mScores = { 0, 0 };
		break;

	case WhoLose::Draw:
		break;

	case WhoLose::None:
		break;

	default:
		break;
	}
}