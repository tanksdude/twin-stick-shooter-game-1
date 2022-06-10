#pragma once
class Enemy;

#include "circle.h"
#include "simple-vector-2d.h"
#include "projectile.h"
#include "player.h"

//enum because simplicity; should be changed out later
enum class EnemyTypes {
	Fodder,
	Basic,
	//Advanced,
	//Turret,
	//Guarded,
	Mothership
};

class Enemy : public Circle {
protected:
	char teamID;

	SimpleVector2D velocity;
	virtual float getAngle() const { return velocity.getAngle(); } //virtual so direction facing doesn't have to be same as direction moving
	float defense;
	float health;
	float initial_health;

	SpriteHolder* bodyTexture;

public:
	char getTeamID() const { return teamID; }
	virtual EnemyTypes getType() const = 0;
	virtual int getScore() const = 0;

public:
	virtual void draw() const;
	virtual Projectile* tick(const Player*) = 0; //extra logic, like player targeting
	virtual void move() = 0; //just move (and maybe a little extra)

	bool takeDamageHandle(float damage); //returns true if dead

	static Enemy* MakeEnemy(float x, float y, EnemyTypes type, char teamID);
	static Enemy* MakeEnemy(float x, float y, float r, EnemyTypes type, char teamID);

	virtual ~Enemy();
private:
	Enemy();
	Enemy(const Enemy&);
protected:
	Enemy(float x, float y, float r, char teamID);
};

class FodderEnemy : public Enemy {
protected:
	static float size; //const but I don't want to change header files to change size and speed
	static float speed;
public:
	EnemyTypes getType() const override { return EnemyTypes::Fodder; }
	int getScore() const override { return 1; }

	virtual Projectile* tick(const Player*) override;
	virtual void move() override;

	FodderEnemy(float x, float y, char teamID);
	FodderEnemy(float x, float y, float r, char teamID);
	~FodderEnemy();

	//FodderEnemy(const FodderEnemy&);
private:
	FodderEnemy();
};

class BasicEnemy : public Enemy {
protected:
	static float size;
	static float speed;

	float maxAngleChange;
public:
	EnemyTypes getType() const override { return EnemyTypes::Basic; }
	int getScore() const override { return 3; }

	virtual Projectile* tick(const Player*) override;
	virtual void move() override;

	BasicEnemy(float x, float y, char teamID);
	BasicEnemy(float x, float y, float r, char teamID);
	~BasicEnemy();

	//BasicEnemy(const BasicEnemy&);
private:
	BasicEnemy();
};

class MothershipEnemy : public Enemy {
protected:
	static float size;
	static float speed;
	float getAngle() const override { return 0; } //doesn't turn

	int shootCooldown;
	int maxShootCooldown;
public:
	EnemyTypes getType() const override { return EnemyTypes::Mothership; }
	int getScore() const override { return 10; }

	virtual Projectile* tick(const Player*) override;
	virtual void move() override;

	MothershipEnemy(float x, float y, char teamID);
	MothershipEnemy(float x, float y, float r, char teamID);
	~MothershipEnemy();

	//MothershipEnemy(const MothershipEnemy&);
private:
	MothershipEnemy();
};
