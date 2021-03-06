#include "projectile.h"
#include "constants.h"
#include <math.h>
//#include <limits>
#include "texture-manager.h"
#include <iostream>

#include <freeglut.h>

Projectile::Projectile() : Projectile(0,0,0,{0,0},0,-1) {}

Projectile::Projectile(float x, float y, float r, SimpleVector2D velocity, float damage, char teamID) : Circle(x,y,r) {
	this->velocity = velocity;
	this->teamID = teamID;
	this->damage = damage;
	bodyTexture = TextureManager::getSprite("TODO", 4);
}

Projectile::Projectile(const Projectile& other) : Circle(other) {
	this->velocity = other.velocity;
	this->teamID = other.teamID;
	this->bodyTexture = (other.bodyTexture==nullptr ? nullptr : TextureManager::getSprite(other.bodyTexture->getName().c_str()));
}

Projectile::~Projectile() {
	if (bodyTexture != nullptr) {
		delete bodyTexture; //doesn't delete the actual sprite
	}
}

void Projectile::move() {
	x += velocity.getXComp();
	y += velocity.getYComp();

	if (bodyTexture != nullptr) {
		bodyTexture->frameAdvance(); //nowhere better to put this...
	}
}

void Projectile::draw() const {
	if (bodyTexture == nullptr) {
		Circle::draw(1, 0, 1);
	} else {
		bodyTexture->draw(x - r, y - r, 2*r, 2*r, velocity.getAngle(), 1);
	}
}

/*
float DeathBullet::MAX_DAMAGE = std::numeric_limits<float>::infinity(); //cuz why not

DeathBullet::DeathBullet() : Projectile() {}

DeathBullet::DeathBullet(float x, float y, float r, SimpleVector2D velocity, char teamID) : Projectile(x,y,r,velocity,MAX_DAMAGE,teamID) {
	bodyTexture = TextureManager::getSprite("strange_shield.png");
}

DeathBullet::DeathBullet(const DeathBullet& other) : Projectile(other) {

}
*/
