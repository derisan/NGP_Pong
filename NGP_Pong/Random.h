#pragma once

#include <random>

class Random
{
public:
	static void StaticInit();
	static float GetFloat(float min = 0.0f, float max = 1.0f);

private:
	static std::mt19937 sEngine;
};