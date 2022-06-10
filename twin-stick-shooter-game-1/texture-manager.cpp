#include "texture-manager.h"
#include <iostream>

std::unordered_map<std::string, Sprite*> TextureManager::sprites;

Sprite* TextureManager::getLoneSprite(std::string name) {
	auto get = sprites.find(name);
	if (get != sprites.end()) {
		return sprites[name];
	}
	std::cerr << "Warning: could not find sprite " << name << std::endl;
	return nullptr;
}

void TextureManager::addSprite(std::string name, Sprite* s) {
	sprites.insert({name, s});
}

SpriteHolder* TextureManager::getSprite(const char* name, int delayBetweenFrames, int loopMax) {
	if (getLoneSprite(name) == nullptr) {
		return nullptr;
	}
	return new SpriteHolder(name, delayBetweenFrames, loopMax);
}
