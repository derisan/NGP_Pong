#include "PongPCH.h"
#include "Random.h"

std::mt19937 Random::sEngine;

void Random::StaticInit()
{
	std::random_device rd;
	sEngine.seed(rd());
}

float Random::GetFloat(float min /*= 0.0f*/, float max /*= 1.0f*/)
{
	std::uniform_real_distribution<float> urd(min, max);
	return urd(sEngine);
}
