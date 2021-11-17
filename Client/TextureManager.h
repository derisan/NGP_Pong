#pragma once

class TextureManager
{
public:
	static void StaticInit(SDL_Renderer* renderer);
	static void StaticShutdown();

	static SDL_Texture* GetTexture(const string& filename);

private:
	static unordered_map<string, SDL_Texture*> sTextures;
	static SDL_Renderer* sRenderer;
};

