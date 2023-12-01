#pragma once
#include "sprite.h"
#include <string>
#include <vector>
#include <unordered_map>

//holds all textures and sprites
class TextureManager {
	friend class SpriteHolder;

protected:
	static std::unordered_map<std::string, Sprite*> sprites;

	static Sprite* getLoneSprite(std::string); //for SpriteHolder

public:
	static void addSprite(std::string, Sprite*);
	static SpriteHolder* getSprite(const char*, int delayBetweenFrames=1, int loopMax=-1);

private:
	TextureManager() = delete;
	TextureManager(const TextureManager&) = delete;
};
