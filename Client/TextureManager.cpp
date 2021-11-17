#include "ClientPCH.h"
#include "TextureManager.h"

unordered_map<string, SDL_Texture*> TextureManager::sTextures;
SDL_Renderer* TextureManager::sRenderer = nullptr;

void TextureManager::StaticInit(SDL_Renderer* renderer)
{
	sRenderer = renderer;
}

void TextureManager::StaticShutdown()
{
	for (auto& p : sTextures)
	{
		SDL_DestroyTexture(p.second);
	}

	sTextures.clear();
}

SDL_Texture* TextureManager::GetTexture(const string& filename)
{
	auto iter = sTextures.find(filename);

	if (iter != sTextures.end())
	{
		return iter->second;
	}
	else
	{
		SDL_Surface* surf = IMG_Load(filename.c_str());

		if (surf == nullptr)
		{
			SDL_Log("Failed to load texture: %s", filename.c_str());
			return nullptr;
		}

		SDL_Texture* tex = SDL_CreateTextureFromSurface(sRenderer, surf);
		SDL_FreeSurface(surf);

		if (tex == nullptr)
		{
			SDL_Log("Failed to convert surface to texture: %s", filename.c_str());
			return nullptr;
		}

		sTextures.emplace(filename, tex);

		return tex;
	}
}
