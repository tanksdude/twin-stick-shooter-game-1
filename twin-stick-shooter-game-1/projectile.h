#pragma once

#include "circle.h"
#include "simple-vector-2d.h"
#include "sprite.h"

class Projectile : public Circle {
protected:
	char teamID;

	SimpleVector2D velocity;
	float damage;

	SpriteHolder* bodyTexture;

public:
	char getTeamID() const { return teamID; }

	float getDamage() const { return damage; }
	virtual bool destroyedOnImpact() const { return true; }
	//not implementing partial bullet destruction; this is for a "death bullet"

public:
	Projectile();
	Projectile(float x, float y, float r, SimpleVector2D velocity, float damage, char teamID);
	Projectile(const Projectile&);
	void move();
	void draw() const;

	~Projectile();
};

//this is only to give the player a chance to recover after dying (unused)
/*
class DeathBullet : public Projectile {
protected:
	static float MAX_DAMAGE;

public:
	bool destroyedOnImpact() const override { return false; }
	
	DeathBullet();
	DeathBullet(float x, float y, float r, SimpleVector2D velocity, char teamID);
	DeathBullet(const DeathBullet&);
};
*/
