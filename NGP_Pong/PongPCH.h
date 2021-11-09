#pragma once

#pragma warning(disable:4819)
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "fmt")
#pragma comment(lib, "SDL2")
#pragma comment(lib, "SDL2_image")
#pragma comment(lib, "spdlog")

#include "PongMath.h"
#include "Log.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#undef main
#include <entt/entt.hpp>

#include <string>
#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <functional>
#include <algorithm>

using std::string;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::array;
using std::unordered_map;
