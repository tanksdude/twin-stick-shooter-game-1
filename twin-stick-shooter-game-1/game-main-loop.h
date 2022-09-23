#pragma once
#include <vector>
#include <unordered_map>
#include "game-scene.h"
#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "powerup.h"
#include "sprite.h"

class GameMainLoop : public GameScene {
	friend class DeveloperManager;

public:
	GameMainLoop();
	GameMainLoop(int startingLives);
	~GameMainLoop();

	void Tick();
	void Tick(int) override { Tick(); }
	void Draw() const override { drawMain(); }

public:
	void drawMain() const;
	void resetGame();
	void resetGame(int startingLives);

protected:
	//tick stuff:
	void tick_moveStuff();
	void tick_powerups();
	bool tick_collision();

public:
	//full handlers:
	void keyDown(unsigned char key, float x, float y) override;
	void keyUp(unsigned char key, float x, float y) override;

	void specialKeyDown(int, float, float) override;
	void specialKeyUp(int, float, float) override;

	void leftMouseDown(float, float) override;
	void leftMouseUp(float, float) override;
	void rightMouseDown(float, float) override;
	void rightMouseUp(float, float) override;

	void drag(float, float) override; //dragging, but mouse is held
	void passiveDrag(float, float) override; //dragging, but mouse isn't held

public:
	//condensed handlers:
	void mouseClickDownHandle(float mx, float my);
	void mouseClickUpHandle(float mx, float my);
	void mouseMoveHandle(float mx, float my);

	void keyDownSet(char key);
	void keyUpSet(char key);

protected:
	//actual game stuff:
	std::vector<Projectile*> projectiles;
	Player* player;
	std::vector<Enemy*> enemies;
	std::vector<Powerup*> powerups;
	int tickCount;
	std::unordered_map<char, bool> keyStates;

	bool mouseDown;
	float mouseX;
	float mouseY;

	inline void deleteProjectile(int index);
	inline void deleteEnemy(int index);
	inline void deletePowerup(int index);

	std::string alertMessage;
	SpriteHolder* backgroundImage;
	int playerLives; //unused
	int respawnCooldown; //unused
	int restartGameCooldown;
	int currentScore;
	int waveNumber;
	void pushWaveEnemies(int wave);
	void pushEnemy(EnemyTypes); //random position
	bool validEnemyPosition(Enemy*); //helper
	void pushWavePowerups(int wave);

protected:
	void drawBackground() const;
	void drawMessage(std::string, float scaleFactor, float posX, float posY) const;
	void drawArrow(float screenCenterX, float screenCenterY, float screenZoom, float objectX, float objectY, float objectW, float objectH) const;
	void drawArrow(float screenCenterX, float screenCenterY, float screenZoom, Circle*) const;
	void drawArrow(float screenCenterX, float screenCenterY, float screenZoom, Rect*) const;

private:
	GameMainLoop(const GameMainLoop&) {}
};
