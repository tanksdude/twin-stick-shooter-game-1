#pragma once

#include "circle.h"
#include "simple-vector-2d.h"
#include "projectile.h"
#include "powerup.h"
#include "sprite.h"

class Player : public Circle {
protected:
	char teamID;
	bool up;
	bool down;
	bool left;
	bool right;
	bool shooting;

	SpriteHolder* bodyTexture;
	SpriteHolder* reticuleTexture;
	SpriteHolder* shieldTexture;
	SpriteHolder* thrusterSprite;

	SimpleVector2D targetingReticule; //offset from player; angle is also angle of player
	SimpleVector2D velocity;

	//the player can get upgraded!
	//they have a base value, upgrade percentage, multiplier from upgrades, multiplier from supermax upgrade
	//(supermax = upgrade after fully upgraded)
	template <typename T>
	struct PlayerUpgradable {
	protected:
		T value;
		int currentUpgrade;
		int maxUpgrade;
		float multiplier1;
		float multiplierMax;
		//should implement cooldown feature for max multiplier
	public:
		T getValue() const;
		void increaseUpgrade(int);
		PlayerUpgradable();
		PlayerUpgradable(T value, int maxUpgrade, float multiplier1, float multiplierMax);
	};

	PlayerUpgradable<float> speed;
	PlayerUpgradable<float> firingRate;
	PlayerUpgradable<float> bulletDamage;

	int shootCooldown;
	int maxShootCooldown;

	bool shielded;
	int shieldCooldown;
	//if shieldCooldown > 0, player is invulnerable
	int maxShieldCooldown;
	void giveShield();
	inline float getPlayerAlpha() const;

public:
	char getTeamID() const { return teamID; }

public:
	Player();
	Player(float x, float y, float r, char teamID);
	Player(const Player&);

	void draw() const;
	void tick();
	void move();
	Projectile* shootHandle();
	void setMovement(bool up, bool down, bool left, bool right, bool shooting);
	void targetPosition(float x, float y);
	bool handleDamage(); //returns true if dead
	void givePower(const Powerup*);
	void respawn(); //called when died; primarily resets shieldCooldown for invulnerability (unused)

	float getZoomDist() const;

	~Player();
};
