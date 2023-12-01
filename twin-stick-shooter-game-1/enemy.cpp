#include "enemy.h"

#include "constants.h"
#include <cmath>
#include <stdexcept>

#include "texture-manager.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

Enemy* Enemy::MakeEnemy(float x, float y, EnemyTypes type, char teamID) {
	switch (type) {
		case EnemyTypes::Fodder:
			return new FodderEnemy(x, y, teamID);
		case EnemyTypes::Basic:
			return new BasicEnemy(x, y, teamID);
		case EnemyTypes::Mothership:
			return new MothershipEnemy(x, y, teamID);
		default:
			throw std::domain_error("ERROR: Unknown enemy type!");
	}
}

Enemy* Enemy::MakeEnemy(float x, float y, float r, EnemyTypes type, char teamID) {
	switch (type) {
		case EnemyTypes::Fodder:
			return new FodderEnemy(x, y, r, teamID);
		case EnemyTypes::Basic:
			return new BasicEnemy(x, y, r, teamID);
		case EnemyTypes::Mothership:
			return new MothershipEnemy(x, y, r, teamID);
		default:
			throw std::domain_error("ERROR: Unknown enemy type!");
	}
}

Enemy::Enemy(float x, float y, float r, char teamID) : Circle(x, y, r) {
	this->teamID = teamID;
	velocity = SimpleVector2D(0, 0);
	//cooldown = 0;
	initial_health = health = 1;
	defense = 0;

	bodyTexture = nullptr;
}

Enemy::~Enemy() {
	//don't delete bodyTexture
}

void Enemy::draw() const {
	if (bodyTexture == nullptr) [[unlikely]] {
		Circle::draw(1, 0, 1);
	} else {
		bodyTexture->draw(x - r, y - r, 2*r, 2*r, getAngle(), 1);
	}

	//health:
	if (health < initial_health) {
		Rect::draw(x-r, y-r*1.5f, 2*r, r*.5f, 1, 1, 1, GL_LINE_LOOP);
		//forms a box of width 2r and height r/2 right below enemy

		const float healthPercent = health / initial_health;
		Rect::draw(x-r, y-r*1.5f, 2*r*healthPercent, r*.5f, 1, 1, 1, GL_POLYGON);
	}
}

bool Enemy::takeDamageHandle(float damage) {
	float damageTaken = damage / (damage + defense);
	health -= damageTaken;
	return (health <= 0);
}



float FodderEnemy::size = 2.5;
float FodderEnemy::speed = .125;

FodderEnemy::FodderEnemy(float x, float y, char teamID) : FodderEnemy(x, y, FodderEnemy::size, teamID) {}
FodderEnemy::FodderEnemy(float x, float y, float r, char teamID) : Enemy(x, y, r, teamID) {
	initial_health = health = 1;
	defense = 0;
	bodyTexture = TextureManager::getSprite("shieldbubble.png");
}

FodderEnemy::~FodderEnemy() {
	if (bodyTexture != nullptr) [[likely]] {
		delete bodyTexture;
	}
}

Projectile* FodderEnemy::tick(const Player* p) {
	velocity = SimpleVector2D(p->getX() - x, p->getY() - y);
	if (velocity.getMagnitude() > speed) {
		velocity.setMagnitude(speed);
	}
	return nullptr;
}

void FodderEnemy::move() {
	x += velocity.getXComp();
	y += velocity.getYComp();
}



float BasicEnemy::size = 5;
float BasicEnemy::speed = .25;

BasicEnemy::BasicEnemy(float x, float y, char teamID) : BasicEnemy(x, y, BasicEnemy::size, teamID) {}
BasicEnemy::BasicEnemy(float x, float y, float r, char teamID) : Enemy(x, y, r, teamID) {
	initial_health = health = 5;
	defense = 0;
	maxAngleChange = 2*PI / 400;
	bodyTexture = TextureManager::getSprite("cpu_pins.jpg");
}

BasicEnemy::~BasicEnemy() {
	if (bodyTexture != nullptr) [[likely]] {
		delete bodyTexture;
	}
}

Projectile* BasicEnemy::tick(const Player* p) {
	SimpleVector2D distToPlayer = SimpleVector2D(p->getX() - x, p->getY() - y);
	//SimpleVector2D newVelocity = velocity + distToPlayer;
	if (velocity.getMagnitude() < .000000001f) {
		velocity = distToPlayer;
	} else {
		float theta = SimpleVector2D::angleBetween(distToPlayer, velocity);
		if (abs(theta) <= maxAngleChange) {
			//small angle adjustment needed
			velocity.setAngle(distToPlayer.getAngle());
		} else {
			//large angle adjustment needed
			if (theta < 0) {
				velocity.setAngle(velocity.getAngle() + maxAngleChange);
			} else {
				velocity.setAngle(velocity.getAngle() - maxAngleChange);
			}
		}
	}
	return nullptr;
}

void BasicEnemy::move() {
	velocity.setMagnitude(speed);

	x += velocity.getXComp();
	y += velocity.getYComp();
}



float MothershipEnemy::size = 50;
float MothershipEnemy::speed = .125;

MothershipEnemy::MothershipEnemy(float x, float y, char teamID) : MothershipEnemy(x, y, MothershipEnemy::size, teamID) {}
MothershipEnemy::MothershipEnemy(float x, float y, float r, char teamID) : Enemy(x, y, r, teamID) {
	initial_health = health = 20;
	defense = 1;
	shootCooldown = 0;
	maxShootCooldown = 100;
	//bodyTexture = TextureManager::getSprite("space_invader.png");
	bodyTexture = TextureManager::getSprite("strange_shield.png");
}

MothershipEnemy::~MothershipEnemy() {
	if (bodyTexture != nullptr) [[likely]] {
		delete bodyTexture;
	}
}

Projectile* MothershipEnemy::tick(const Player* p) {
	SimpleVector2D distToPlayer = SimpleVector2D(p->getX() - x, p->getY() - y);
	bool canShoot = (distToPlayer.getMagnitude() <= GAME_WIDTH/4);

	//shoot handle
	Projectile* bullet = nullptr;
	if (shootCooldown > 0) {
		shootCooldown--;
	} else if (shootCooldown <= 0) {
		if (canShoot) {
			SimpleVector2D bulletVelocity = SimpleVector2D(distToPlayer.getAngle(), speed*2, true);
			bullet = new Projectile(x + r*cos(distToPlayer.getAngle()), y + r*sin(distToPlayer.getAngle()), 10, bulletVelocity, 1, this->teamID);
			shootCooldown = maxShootCooldown;
		}
	}

	//move into position
	//"position": 100 from player
	if (distToPlayer.getMagnitude() > 100) {
		//move forward
		velocity = distToPlayer;
		if (velocity.getMagnitude() > speed) {
			velocity.setMagnitude(speed);
		}
	} else {
		//move back
		velocity = distToPlayer;
		velocity.setAngle(velocity.getAngle() + PI);
		if (velocity.getMagnitude() > speed) {
			velocity.setMagnitude(speed);
		}
	}
	
	return bullet;
}

void MothershipEnemy::move() {
	x += velocity.getXComp();
	y += velocity.getYComp();
}
