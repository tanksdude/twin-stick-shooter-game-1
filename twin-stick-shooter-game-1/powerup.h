#pragma once
class Powerup;

#include <string>
#include <vector>

#include "circle.h"
#include "sprite.h"

enum class PowerupTypes {
	Speed,
	FiringRate,
	Damage,
	Shield,
};

class Powerup : public Circle {
	friend class PowerupManager;
protected:
	static float size;

	SpriteHolder* bodyTexture;

public:
	static float getSize() { return size; }
	virtual PowerupTypes getType() const = 0;

public:
	static Powerup* MakePowerup(float x, float y, PowerupTypes);
	static Powerup* MakePowerup(float x, float y, float r, PowerupTypes);

	//for the player:
	virtual int getSpeedIncrease() const { return 0; }
	virtual int getFiringRateIncrease() const { return 0; }
	virtual int getDamageIncrease() const { return 0; }
	//^^^ those can be negative if hindrences are wanted...
	virtual bool getShieldStatus() const { return false; }
	
	//virtual std::string getName() const = 0;
	virtual void draw() const;
	virtual ~Powerup();
protected:
	Powerup(float x, float y, float r);
};

class SpeedPowerup : public Powerup {
public:
	virtual PowerupTypes getType() const override { return PowerupTypes::Speed; }
	virtual int getSpeedIncrease() const override { return 1; }

	SpeedPowerup(float x, float y);
	SpeedPowerup(float x, float y, float r);
};

class FiringRatePowerup : public Powerup {
public:
	virtual PowerupTypes getType() const override { return PowerupTypes::FiringRate; }
	virtual int getFiringRateIncrease() const override { return 1; }

	FiringRatePowerup(float x, float y);
	FiringRatePowerup(float x, float y, float r);
};

class DamagePowerup : public Powerup {
public:
	virtual PowerupTypes getType() const override { return PowerupTypes::Damage; }
	virtual int getDamageIncrease() const override { return 1; }

	DamagePowerup(float x, float y);
	DamagePowerup(float x, float y, float r);
};

class ShieldPowerup : public Powerup {
public:
	virtual PowerupTypes getType() const override { return PowerupTypes::Shield; }
	virtual bool getShieldStatus() const override { return true; }

	ShieldPowerup(float x, float y);
	ShieldPowerup(float x, float y, float r);
};
