#include "player.h"

#include "constants.h"
#include <cmath>
#include <iostream>

#include "texture-manager.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

template <typename T>
T Player::PlayerUpgradable<T>::getValue() const {
	if (currentUpgrade >= maxUpgrade) {
		return value * multiplierMax;
	}
	return float(currentUpgrade)/float(maxUpgrade) * (multiplier1 - value) + value;
}

template <typename T>
void Player::PlayerUpgradable<T>::increaseUpgrade(int v) {
	currentUpgrade += v;
	if (currentUpgrade > maxUpgrade) {
		currentUpgrade = maxUpgrade;
		//if supermax upgrade was on a timer, code would go here
	}
}

template <typename T>
Player::PlayerUpgradable<T>::PlayerUpgradable() {
	this->value = this->currentUpgrade = this->maxUpgrade = this->multiplier1 = this->multiplierMax = 0;
}

template <typename T>
Player::PlayerUpgradable<T>::PlayerUpgradable(T value, int maxUpgrade, float multiplier1, float multiplierMax) {
	this->value = value;
	this->currentUpgrade = 0;
	this->maxUpgrade = maxUpgrade;
	this->multiplier1 = multiplier1;
	this->multiplierMax = multiplierMax;
}
//template Player::PlayerUpgradable Player::PlayerUpgradable::PlayerUpgradable<float>(float, int, float, float);
//^ somewhat resembles how you fix template functions in source files

Player::Player() : Player(GAME_WIDTH/2, GAME_HEIGHT/2, 10, 0) {}

Player::Player(float x, float y, float r, char teamID) : Circle(x,y,r) {
	this->teamID = teamID;
	up = down = left = right = shooting = false;
	targetingReticule = SimpleVector2D(0,0);
	velocity = SimpleVector2D(0,0);
	speed = PlayerUpgradable<float>(1, 10, 1.5f, 2.0f);
	firingRate = PlayerUpgradable<float>(2, 10, 2.0f, 2.5f);
	bulletDamage = PlayerUpgradable<float>(1, 10, 2.0f, 5.0f);
	shootCooldown = 0;
	maxShootCooldown = 50;
	shielded = false;
	shieldCooldown = 0;
	maxShieldCooldown = 200;
	bodyTexture = TextureManager::getSprite("destroyer.png");
	reticuleTexture = TextureManager::getSprite("reticle.png"); //known flaw: is practically unnoticeable
	shieldTexture = TextureManager::getSprite("shield.png");
	thrusterSprite = TextureManager::getSprite("thruster_spritesheet.png", 2);
}

Player::Player(const Player& other) : Circle(other) {
	this->teamID = other.teamID;
	this->up = other.up; //not really needed...
	this->down = other.down;
	this->left = other.left;
	this->right = other.right;
	this->shooting = other.shooting;
	this->targetingReticule = other.targetingReticule;
	this->velocity = other.velocity;
	this->speed = other.speed;
	this->firingRate = other.firingRate;
	this->bulletDamage = other.bulletDamage;
	this->shootCooldown = other.shootCooldown;
	this->maxShootCooldown = other.maxShootCooldown;
	this->shielded = other.shielded;
	this->shieldCooldown = other.shieldCooldown;
	this->maxShieldCooldown = other.maxShieldCooldown;
	this->bodyTexture = (other.bodyTexture==nullptr ? nullptr : TextureManager::getSprite(other.bodyTexture->getName().c_str()));
	this->reticuleTexture = (other.reticuleTexture==nullptr ? nullptr : TextureManager::getSprite(other.reticuleTexture->getName().c_str()));
	this->shieldTexture = (other.shieldTexture==nullptr ? nullptr : TextureManager::getSprite(other.shieldTexture->getName().c_str()));
	this->thrusterSprite = (other.thrusterSprite==nullptr ? nullptr : TextureManager::getSprite(other.thrusterSprite->getName().c_str()));
}

Player::~Player() {
	delete bodyTexture;
	delete reticuleTexture;
	delete shieldTexture;
	delete thrusterSprite;
}

void Player::move() {
	float xVel = 0, yVel = 0;
	if (up) {
		yVel += speed.getValue();
	}
	if (down) {
		yVel -= speed.getValue();
	}
	if (left) {
		xVel -= speed.getValue();
	}
	if (right) {
		xVel += speed.getValue();
	}
	velocity = SimpleVector2D(xVel, yVel);
	if (velocity.getMagnitude() > speed.getValue()) {
		velocity.setMagnitude(speed.getValue());
	}

	x += velocity.getXComp();
	y += velocity.getYComp();
}

Projectile* Player::shootHandle() {
	Projectile* shot = nullptr;
	if (shootCooldown <= 0) {
		if (shooting) {
			SimpleVector2D bulletVelocity(targetingReticule);
			bulletVelocity.setMagnitude(speed.getValue()*4);
			shot = new Projectile(x, y, r/2, bulletVelocity, bulletDamage.getValue(), this->teamID);
			shootCooldown = maxShootCooldown * (1/firingRate.getValue());
		}
	} else {
		shootCooldown--;
	}
	return shot;
}

void Player::tick() {
	if (shieldCooldown > 0) {
		shieldCooldown--;
	}

	if (thrusterSprite != nullptr) [[likely]] {
		thrusterSprite->frameAdvance();
	}
}

void Player::setMovement(bool up, bool down, bool left, bool right, bool shooting) {
	this->up = up;
	this->down = down;
	this->left = left;
	this->right = right;
	this->shooting = shooting;
}

void Player::targetPosition(float x, float y) {
	targetingReticule = SimpleVector2D(x - this->x, y - this->y);
}

bool Player::handleDamage() {
	if (shieldCooldown > 0) {
		return false;
	}
	if (shielded) {
		shielded = false;
		shieldCooldown = maxShieldCooldown;
		return false;
	} else {
		return true;
	}
}

void Player::giveShield() {
	shielded = true;
	shieldCooldown = maxShieldCooldown;
}

void Player::givePower(const Powerup* p) {
	speed.increaseUpgrade(p->getSpeedIncrease());
	firingRate.increaseUpgrade(p->getFiringRateIncrease());
	bulletDamage.increaseUpgrade(p->getDamageIncrease());
	if (p->getShieldStatus()) {
		giveShield();
	}
}

void Player::respawn() {
	shootCooldown = 0;
	shielded = false;
	shieldCooldown = maxShieldCooldown;
}

float Player::getZoomDist() const {
	return 100;
	//return r*2 + speed.getValue()*100;
}

float Player::getPlayerAlpha() const {
	if (shieldCooldown <= 0) {
		return 1;
	}
	return (float(maxShieldCooldown) - float(shieldCooldown)/2) / float(maxShieldCooldown);
	//large cooldown: half transparent; small cooldown: not very transparent
}

void Player::draw() const {
	//thruster:
	if (bodyTexture != nullptr) [[likely]] {
		if ((up != down) || (left != right)) {
			//float angle = velocity.getAngle();
			float angle = targetingReticule.getAngle();
			thrusterSprite->draw(x - r - r*cos(angle), y - r - r*sin(angle), 2*r, 2*r, angle+(PI/2), 1);
		}
	}

	//body:
	if (bodyTexture == nullptr) [[unlikely]] {
		Circle::draw(1, 0, 1);
	} else {
		float alpha = getPlayerAlpha();
		bodyTexture->draw(x - r, y - r, 2*r, 2*r, targetingReticule.getAngle(), alpha);
	}

	//reticule:
	if (reticuleTexture == nullptr) [[unlikely]] {
		Circle::draw(x + targetingReticule.getXComp(), y + targetingReticule.getYComp(), r, .75f, 0, .75f);
	} else {
		reticuleTexture->draw(x + targetingReticule.getXComp() - r, y + targetingReticule.getYComp() - r, 2*r, 2*r, 0);
	}

	//shield:
	if (shielded) {
		const float size = r*1.5f;
		if (shieldTexture == nullptr) [[unlikely]] {
			Circle::draw(x, y, size, 1, 0, 1, GL_LINE_LOOP);
		} else {
			shieldTexture->draw(x - size, y - size, 2*size, 2*size, 0, .5f);
		}
	}
}
