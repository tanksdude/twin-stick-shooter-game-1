#include "powerup.h"
#include "constants.h"
#include <math.h>
#include <stdexcept>
#include "texture-manager.h"
#include <iostream>

#include <freeglut.h>

float Powerup::size = 5;

Powerup* Powerup::MakePowerup(float x, float y, PowerupTypes type) {
	switch(type) {
		case PowerupTypes::Speed:
			return new SpeedPowerup(x, y, Powerup::size);
		case PowerupTypes::FiringRate:
			return new FiringRatePowerup(x, y, Powerup::size);
		case PowerupTypes::Damage:
			return new DamagePowerup(x, y, Powerup::size);
		case PowerupTypes::Shield:
			return new ShieldPowerup(x, y, Powerup::size);
	}
	throw std::domain_error("ERROR: Unknown powerup type!");
	return nullptr;
}

Powerup* Powerup::MakePowerup(float x, float y, float r, PowerupTypes type) {
	switch(type) {
		case PowerupTypes::Speed:
			return new SpeedPowerup(x, y, r);
		case PowerupTypes::FiringRate:
			return new FiringRatePowerup(x, y, r);
		case PowerupTypes::Damage:
			return new DamagePowerup(x, y, r);
		case PowerupTypes::Shield:
			return new ShieldPowerup(x, y, r);
	}
	throw std::domain_error("ERROR: Unknown powerup type!");
	return nullptr;
}

Powerup::Powerup(float x, float y, float r) : Circle(x, y, r) {
	bodyTexture = TextureManager::getSprite("TODO", 10);
}

Powerup::~Powerup() {
	if (bodyTexture != nullptr) {
		delete bodyTexture; //doesn't delete the actual sprite
	}
}

void Powerup::draw() const {
	if (bodyTexture == nullptr) {
		Circle::draw(1, 0, 1);
	} else {
		bodyTexture->frameAdvance();
		bodyTexture->draw(x - r, y - r, 2*r, 2*r, 0, 1);
	}
}



SpeedPowerup::SpeedPowerup(float x, float y) : SpeedPowerup(x,y,Powerup::size) {}

SpeedPowerup::SpeedPowerup(float x, float y, float r) : Powerup(x, y, r) {
	
}



FiringRatePowerup::FiringRatePowerup(float x, float y) : FiringRatePowerup(x,y,Powerup::size) {}

FiringRatePowerup::FiringRatePowerup(float x, float y, float r) : Powerup(x, y, r) {
	
}



DamagePowerup::DamagePowerup(float x, float y) : DamagePowerup(x,y,Powerup::size) {}

DamagePowerup::DamagePowerup(float x, float y, float r) : Powerup(x, y, r) {
	
}



ShieldPowerup::ShieldPowerup(float x, float y) : ShieldPowerup(x,y,Powerup::size) {}

ShieldPowerup::ShieldPowerup(float x, float y, float r) : Powerup(x, y, r) {
	
}
