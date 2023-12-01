#include "powerup.h"

#include "constants.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

#include "texture-manager.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

float Powerup::size = 5;

Powerup* Powerup::MakePowerup(float x, float y, PowerupTypes type) {
	switch (type) {
		case PowerupTypes::Speed:
			return new SpeedPowerup(x, y, Powerup::size);
		case PowerupTypes::FiringRate:
			return new FiringRatePowerup(x, y, Powerup::size);
		case PowerupTypes::Damage:
			return new DamagePowerup(x, y, Powerup::size);
		case PowerupTypes::Shield:
			return new ShieldPowerup(x, y, Powerup::size);
		default:
			throw std::domain_error("ERROR: Unknown powerup type!");
	}
}

Powerup* Powerup::MakePowerup(float x, float y, float r, PowerupTypes type) {
	switch (type) {
		case PowerupTypes::Speed:
			return new SpeedPowerup(x, y, r);
		case PowerupTypes::FiringRate:
			return new FiringRatePowerup(x, y, r);
		case PowerupTypes::Damage:
			return new DamagePowerup(x, y, r);
		case PowerupTypes::Shield:
			return new ShieldPowerup(x, y, r);
		default:
			throw std::domain_error("ERROR: Unknown powerup type!");
	}
}

Powerup::Powerup(float x, float y, float r) : Circle(x, y, r) {
	bodyTexture = TextureManager::getSprite("coin_sprite.png", 10);
}

Powerup::~Powerup() {
	if (bodyTexture != nullptr) [[likely]] {
		delete bodyTexture; //doesn't delete the actual sprite
	}
}

void Powerup::draw() const {
	if (bodyTexture == nullptr) [[unlikely]] {
		Circle::draw(1, 0, 1);
	} else {
		bodyTexture->frameAdvance();
		bodyTexture->draw(x - r, y - r, 2*r, 2*r, 0, 1);
	}
}



SpeedPowerup::SpeedPowerup(float x, float y) : SpeedPowerup(x, y, Powerup::size) {}

SpeedPowerup::SpeedPowerup(float x, float y, float r) : Powerup(x, y, r) {
	//nothing
}



FiringRatePowerup::FiringRatePowerup(float x, float y) : FiringRatePowerup(x, y, Powerup::size) {}

FiringRatePowerup::FiringRatePowerup(float x, float y, float r) : Powerup(x, y, r) {
	//nothing
}



DamagePowerup::DamagePowerup(float x, float y) : DamagePowerup(x, y, Powerup::size) {}

DamagePowerup::DamagePowerup(float x, float y, float r) : Powerup(x, y, r) {
	//nothing
}



ShieldPowerup::ShieldPowerup(float x, float y) : ShieldPowerup(x, y, Powerup::size) {}

ShieldPowerup::ShieldPowerup(float x, float y, float r) : Powerup(x, y, r) {
	//nothing
}
