#pragma once

#include <spdlog/spdlog.h>

#define LOG(...) spdlog::info(__VA_ARGS__)
#define ASSERT(x, ...) {if(!x) { LOG("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}