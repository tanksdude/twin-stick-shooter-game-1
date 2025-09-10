#include "game-main-loop.h"

#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <iostream>

#include "constants.h"
#include <cmath>
#include "rng.h"

#include "collision-handler.h"
#include "texture-manager.h"
#include "game-scene-manager.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

GameMainLoop::GameMainLoop() : GameMainLoop(3) {
	//nothing
}

GameMainLoop::GameMainLoop(int startingLives) {
	tickCount = 0;
	playerLives = startingLives;
	respawnCooldown = 0;
	restartGameCooldown = 0;
	currentScore = 0;
	waveNumber = 1;
	player = new Player(GAME_WIDTH/2, GAME_HEIGHT/2, 10, 0);

	keyStates.insert({'w', false});
	keyStates.insert({'a', false});
	keyStates.insert({'s', false});
	keyStates.insert({'d', false});
	keyStates.insert({' ', false});

	mouseDown = false;
	mouseX = player->getX();
	mouseY = player->getY();

	pushWaveEnemies(waveNumber);
	pushWavePowerups(waveNumber);
	alertMessage = "";
	backgroundImage = TextureManager::getSprite("space_with_stars.jpg");
}

GameMainLoop::~GameMainLoop() {
	for (int i = 0; i < projectiles.size(); i++) {
		delete projectiles[i];
	}
	projectiles.clear();

	delete player;

	for (int i = 0; i < enemies.size(); i++) {
		delete enemies[i];
	}
	enemies.clear();

	for (int i = 0; i < powerups.size(); i++) {
		delete powerups[i];
	}
	powerups.clear();
}

//full handlers:
void GameMainLoop::keyDown(unsigned char key, float x, float y) {
	keyDownSet(key);
}
void GameMainLoop::keyUp(unsigned char key, float x, float y) {
	keyUpSet(key);
}
void GameMainLoop::specialKeyDown(int, float, float) {
	return;
}
void GameMainLoop::specialKeyUp(int, float, float) {
	return;
}
void GameMainLoop::leftMouseDown(float mx, float my) {
	mouseClickDownHandle(mx, my);
}
void GameMainLoop::leftMouseUp(float mx, float my) {
	mouseClickUpHandle(mx, my);
}
void GameMainLoop::rightMouseDown(float mx, float my) {
	return;
}
void GameMainLoop::rightMouseUp(float mx, float my) {
	return;
}
void GameMainLoop::drag(float mx, float my) {
	//does this get called on right click drag? or middle click drag?
	mouseMoveHandle(mx, my);
}
void GameMainLoop::passiveDrag(float mx, float my) {
	mouseMoveHandle(mx, my);
}

//condensed handlers:
void GameMainLoop::mouseClickDownHandle(float mx, float my) {
	mouseDown = true;
}
void GameMainLoop::mouseClickUpHandle(float mx, float my) {
	mouseDown = false;
}
void GameMainLoop::mouseMoveHandle(float mx, float my) {
	mouseX = mx;
	mouseY = my;
	if (player != nullptr) {
		player->targetPosition(mouseX, mouseY);
	}
}
void GameMainLoop::keyDownSet(char key) {
	auto get = keyStates.find(key);
	if (get != keyStates.end()) {
		keyStates[key] = true;
	}
}
void GameMainLoop::keyUpSet(char key) {
	auto get = keyStates.find(key);
	if (get != keyStates.end()) {
		keyStates[key] = false;
	}
}

void GameMainLoop::deleteProjectile(int index) {
	delete projectiles[index];
	projectiles.erase(projectiles.begin() + index);
}

void GameMainLoop::deleteEnemy(int index) {
	delete enemies[index];
	enemies.erase(enemies.begin() + index);
}

void GameMainLoop::deletePowerup(int index) {
	delete powerups[index];
	powerups.erase(powerups.begin() + index);
}

void GameMainLoop::Tick() {
	// restart cooldown from previous game:
	if (restartGameCooldown > 0) {
		restartGameCooldown--;
		if (restartGameCooldown == 0) {
			resetGame();
		}
		return;
	}

	// main stuff:
	tick_moveStuff();
	tick_powerups();
	bool playerDefeated = tick_collision();

	// handle win/loss:
	if (enemies.size() == 0) {
		waveNumber++;
		pushWaveEnemies(waveNumber);
		pushWavePowerups(waveNumber);
	} else if (playerDefeated) {
		alertMessage = "You lost!";
		restartGameCooldown = 200;
	} else {
		alertMessage = "";
	}
}

void GameMainLoop::tick_moveStuff() {
	//player movement
	player->setMovement(keyStates['w'], keyStates['s'], keyStates['a'], keyStates['d'], mouseDown);
	player->tick();
	player->move();
	if (CollisionHandler::partiallyOutOfBoundsIgnoreEdge(player)) {
		CollisionHandler::edgeConstrain(player);
	}

	//enemy movement
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->move();
	}

	//projectile movement
	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i]->move();
	}

	//player shoot
	Projectile* shot = player->shootHandle();
	if (shot != nullptr) {
		projectiles.push_back(shot);
	}

	//enemy shoot
	for (int i = 0; i < enemies.size(); i++) {
		Projectile* bullet = enemies[i]->tick(player);
		if (bullet != nullptr) {
			projectiles.push_back(bullet);
		}
	}
}

void GameMainLoop::tick_powerups() {
	for (int i = 0; i < powerups.size(); i++) {
		if (CollisionHandler::partiallyCollided(powerups[i], player)) {
			player->givePower(powerups[i]);
			deletePowerup(i);
			i--;
		}
	}
}

bool GameMainLoop::tick_collision() {
	bool playerDefeated = false;

	//remove offscreen bullets
	for (int i = 0; i < projectiles.size(); i++) {
		if (CollisionHandler::fullyOutOfBounds(projectiles[i])) {
			deleteProjectile(i);
			i--;
		}
	}

	//push enemies (of the same type) away from each other
	for (int i = 0; i < enemies.size(); i++) {
		for (int j = 0; j < enemies.size(); j++) {
			if (i == j || enemies[i]->getType() != enemies[j]->getType()) {
				continue;
			}
			if (CollisionHandler::partiallyCollided(enemies[i], enemies[j])) {
				CollisionHandler::pushMovableAwayFromMovable(enemies[i], enemies[j]);
			}
		}
	}

	//enemy-bullet collision
	for (int i = 0; i < projectiles.size(); i++) {
		for (int j = 0; j < enemies.size(); j++) {
			if (projectiles[i]->getTeamID() != enemies[j]->getTeamID()) {
				if (CollisionHandler::partiallyCollided(enemies[j], projectiles[i])) {
					if (enemies[j]->takeDamageHandle(projectiles[i]->getDamage())) {
						currentScore += enemies[j]->getScore();
						deleteEnemy(j);
						j--;
					}
					if (projectiles[i]->destroyedOnImpact()) {
						deleteProjectile(i);
						i--;
					}
					break;
				}
			}
		}
	}

	//player-bullet collision
	for (int i = 0; i < projectiles.size(); i++) {
		if (projectiles[i]->getTeamID() != player->getTeamID()) {
			if (CollisionHandler::partiallyCollided(projectiles[i], player)) {
				if (player->handleDamage()) {
					playerDefeated = true;
					break;
				}
			}
		}
	}

	//player-enemy collision
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->getTeamID() != player->getTeamID()) {
			if (CollisionHandler::partiallyCollided(enemies[i], player)) {
				if (player->handleDamage()) {
					playerDefeated = true;
					break;
				}
			}
		}
	}

	return playerDefeated;
}

void GameMainLoop::resetGame() {
	resetGame(3);
}

void GameMainLoop::resetGame(int startingLives) {
	for (int i = 0; i < projectiles.size(); i++) {
		delete projectiles[i];
	}
	projectiles.clear();

	delete player;

	for (int i = 0; i < enemies.size(); i++) {
		delete enemies[i];
	}
	enemies.clear();

	for (int i = 0; i < powerups.size(); i++) {
		delete powerups[i];
	}
	powerups.clear();

	respawnCooldown = 0;
	restartGameCooldown = 0;
	currentScore = 0;
	waveNumber = 1;
	player = new Player(GAME_WIDTH/2, GAME_HEIGHT/2, 10, 0);
	pushWaveEnemies(waveNumber);
	pushWavePowerups(waveNumber);
}

void GameMainLoop::pushWaveEnemies(int wave) {
	//complicated function
	//well, at least it would be for a real game
	if (wave % 10 == 0) {
		for (int i = 0; i < wave/10; i++) {
			pushEnemy(EnemyTypes::Mothership);
		}
		for (int i = 0; i < wave*2; i++) {
			pushEnemy(EnemyTypes::Fodder);
		}
		for (int i = 0; i < std::ceil(std::log2(wave)); i++) {
			pushEnemy(EnemyTypes::Basic);
		}
	} else {
		for (int i = 0; i < wave*2; i++) {
			pushEnemy(EnemyTypes::Fodder);
		}
		for (int i = 0; i < wave/3+1; i++) {
			pushEnemy(EnemyTypes::Basic);
		}
	}
}

void GameMainLoop::pushEnemy(EnemyTypes type) {
	Enemy* e = nullptr;
	do {
		if (e != nullptr) { delete e; }
		SimpleVector2D offset((2*PI) * RNG::randFuncf(), RNG::randFloatInRange(100, GAME_WIDTH/4), true);
		e = Enemy::MakeEnemy(player->getX() + offset.getXComp(), player->getY() + offset.getYComp(), type, 1);
	} while (!validEnemyPosition(e));
	enemies.push_back(e);
}

bool GameMainLoop::validEnemyPosition(Enemy* e) {
	Circle* c = new Circle(player->getX(), player->getY(), 100);
	bool notValid = !(CollisionHandler::partiallyCollided(e, c) || CollisionHandler::partiallyOutOfBounds(e));
	delete c;
	return notValid;
}

void GameMainLoop::pushWavePowerups(int wave) {
	const float d = Powerup::getSize();
	if (wave == 1) {
		powerups.push_back(Powerup::MakePowerup(RNG::randFloatInRange(d, GAME_WIDTH - d), RNG::randFloatInRange(d, GAME_WIDTH - d), PowerupTypes::Speed));
		powerups.push_back(Powerup::MakePowerup(RNG::randFloatInRange(d, GAME_WIDTH - d), RNG::randFloatInRange(d, GAME_WIDTH - d), PowerupTypes::FiringRate));
		powerups.push_back(Powerup::MakePowerup(RNG::randFloatInRange(d, GAME_WIDTH - d), RNG::randFloatInRange(d, GAME_WIDTH - d), PowerupTypes::Damage));
		powerups.push_back(Powerup::MakePowerup(RNG::randFloatInRange(d, GAME_WIDTH - d), RNG::randFloatInRange(d, GAME_WIDTH - d), PowerupTypes::Shield));
		return;
	}
	if (wave % 10 == 0) {
		powerups.push_back(Powerup::MakePowerup(RNG::randFloatInRange(d, GAME_WIDTH - d), RNG::randFloatInRange(d, GAME_WIDTH - d), PowerupTypes::Shield));
	} else if (wave % 2 == 0) {
		int randPower = static_cast<int>(RNG::randFunc() * 3);
		PowerupTypes type;
		switch (randPower) {
			//default:
			case 0:
				type = PowerupTypes::Speed;
				break;
			case 1:
				type = PowerupTypes::FiringRate;
				break;
			case 2:
				type = PowerupTypes::Damage;
				break;
		}
		powerups.push_back(Powerup::MakePowerup(RNG::randFloatInRange(d, GAME_WIDTH - d), RNG::randFloatInRange(d, GAME_WIDTH - d), type));
	}
	if (wave % 5 == 0) {
		powerups.push_back(Powerup::MakePowerup(RNG::randFloatInRange(d, GAME_WIDTH - d), RNG::randFloatInRange(d, GAME_WIDTH - d), PowerupTypes::Speed));
		powerups.push_back(Powerup::MakePowerup(RNG::randFloatInRange(d, GAME_WIDTH - d), RNG::randFloatInRange(d, GAME_WIDTH - d), PowerupTypes::FiringRate));
		powerups.push_back(Powerup::MakePowerup(RNG::randFloatInRange(d, GAME_WIDTH - d), RNG::randFloatInRange(d, GAME_WIDTH - d), PowerupTypes::Damage));
	}
}

void GameMainLoop::drawMessage(std::string message, float scaleFactor, float posX, float posY) const {
	glPushMatrix();

	float totalWidth = 0;
	for (int i = 0; i < message.size(); i++) {
		totalWidth += glutStrokeWidth(GLUT_STROKE_ROMAN, message[i]);
	}
	totalWidth *= scaleFactor;

	//I don't know why, but TEXT_HEIGHT*scaleFactor isn't the height of the text; must divide by 4
	glTranslatef(posX - totalWidth/2, posY - glutStrokeHeight(GLUT_STROKE_ROMAN)*(scaleFactor/4), 0);
	glScalef(scaleFactor, scaleFactor, scaleFactor);
	for (int i = 0; i < message.size(); i++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, message[i]);
	}

	glPopMatrix();
}

void GameMainLoop::drawBackground() const {
	//get which chunks are on screen
	const float backgroundWidth = GAME_WIDTH / 10;
	const float backgroundHeight = GAME_HEIGHT / 10;
	int chunkLeft =   static_cast<int>(std::floor((player->getX() - player->getZoomDist()) / backgroundWidth));
	int chunkRight =  static_cast<int>(std::floor((player->getX() + player->getZoomDist()) / backgroundWidth)); //ceil would get the right edge
	int chunkBottom = static_cast<int>(std::floor((player->getY() - player->getZoomDist()) / backgroundHeight));
	int chunkTop =    static_cast<int>(std::floor((player->getY() + player->getZoomDist()) / backgroundHeight));

	for (int i = chunkLeft; i <= chunkRight; i++) {
		for (int j = chunkBottom; j <= chunkTop; j++) {
			backgroundImage->draw(i * backgroundWidth, j * backgroundHeight, backgroundWidth, backgroundHeight, 0, .5f );
		}
	}

	//world border
	Rect::draw(0, 0, GAME_WIDTH, GAME_HEIGHT, 1, 1, 1, GL_LINE_LOOP);

	//world center
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(GAME_WIDTH/2 - 20, GAME_HEIGHT/2);
	glVertex2f(GAME_WIDTH/2 + 20, GAME_HEIGHT/2);
	glVertex2f(GAME_WIDTH/2,      GAME_HEIGHT/2 - 20);
	glVertex2f(GAME_WIDTH/2,      GAME_HEIGHT/2 + 20);
	glEnd();
	//not bothering to abstract this out because it's just two lines
}

void GameMainLoop::drawArrow(float screenCenterX, float screenCenterY, float screenZoom, Circle* c) const {
	drawArrow(screenCenterX, screenCenterY, screenZoom, c->getX()-c->getR(), c->getY()-c->getR(), 2*c->getR(), 2*c->getR());
}

void GameMainLoop::drawArrow(float screenCenterX, float screenCenterY, float screenZoom, Rect* r) const {
	drawArrow(screenCenterX, screenCenterY, screenZoom, r->getX(), r->getY(), r->getW(), r->getH());
}

void GameMainLoop::drawArrow(float screenCenterX, float screenCenterY, float screenZoom, float objectX, float objectY, float objectW, float objectH) const {
	Rect* screen = new Rect(screenCenterX - screenZoom, screenCenterY - screenZoom, screenZoom*2, screenZoom*2);
	Rect* object = new Rect(objectX, objectY, objectW, objectH);
	float triangleCoords[6]; //3 (x,y) pairs
	const float ts = .9f; //triangle size
	if (!CollisionHandler::partiallyCollided(object, screen)) {
		//is off screen
		if (objectX < screenCenterX - screenZoom) {
			if (objectY < screenCenterY - screenZoom) {
				//bottom left
				triangleCoords[0] = screenCenterX - screenZoom;    triangleCoords[1] = screenCenterY - screenZoom;
				triangleCoords[2] = screenCenterX - screenZoom*ts; triangleCoords[3] = screenCenterY - screenZoom;
				triangleCoords[4] = screenCenterX - screenZoom;    triangleCoords[5] = screenCenterY - screenZoom*ts;
			} else if (objectY > screenCenterY + screenZoom) {
				//top left
				triangleCoords[0] = screenCenterX - screenZoom;    triangleCoords[1] = screenCenterY + screenZoom;
				triangleCoords[2] = screenCenterX - screenZoom*ts; triangleCoords[3] = screenCenterY + screenZoom;
				triangleCoords[4] = screenCenterX - screenZoom;    triangleCoords[5] = screenCenterY + screenZoom*ts;
			} else {
				//middle left
				triangleCoords[0] = screenCenterX - screenZoom;    triangleCoords[1] = objectY+objectH/2;
				triangleCoords[2] = screenCenterX - screenZoom*ts; triangleCoords[3] = objectY+objectH/2 - screenZoom*(1-ts);
				triangleCoords[4] = screenCenterX - screenZoom*ts; triangleCoords[5] = objectY+objectH/2 + screenZoom*(1-ts);
			}
		} else if (objectX > screenCenterX + screenZoom) {
			if (objectY < screenCenterY - screenZoom) {
				//bottom right
				triangleCoords[0] = screenCenterX + screenZoom;    triangleCoords[1] = screenCenterY - screenZoom;
				triangleCoords[2] = screenCenterX + screenZoom*ts; triangleCoords[3] = screenCenterY - screenZoom;
				triangleCoords[4] = screenCenterX + screenZoom;    triangleCoords[5] = screenCenterY - screenZoom*ts;
			} else if (objectY > screenCenterY + screenZoom) {
				//top right
				triangleCoords[0] = screenCenterX + screenZoom;    triangleCoords[1] = screenCenterY + screenZoom;
				triangleCoords[2] = screenCenterX + screenZoom*ts; triangleCoords[3] = screenCenterY + screenZoom;
				triangleCoords[4] = screenCenterX + screenZoom;    triangleCoords[5] = screenCenterY + screenZoom*ts;
			} else {
				//middle right
				triangleCoords[0] = screenCenterX + screenZoom;    triangleCoords[1] = objectY+objectH/2;
				triangleCoords[2] = screenCenterX + screenZoom*ts; triangleCoords[3] = objectY+objectH/2 - screenZoom*(1-ts);
				triangleCoords[4] = screenCenterX + screenZoom*ts; triangleCoords[5] = objectY+objectH/2 + screenZoom*(1-ts);
			}
		} else {
			if (objectY < screenCenterY - screenZoom) {
				//bottom
				triangleCoords[0] = objectX+objectW/2;                     triangleCoords[1] = screenCenterY - screenZoom;
				triangleCoords[2] = objectX+objectW/2 - screenZoom*(1-ts); triangleCoords[3] = screenCenterY - screenZoom*ts;
				triangleCoords[4] = objectX+objectW/2 + screenZoom*(1-ts); triangleCoords[5] = screenCenterY - screenZoom*ts;
			} else if (objectY > screenCenterY + screenZoom) {
				//top
				triangleCoords[0] = objectX+objectW/2;                     triangleCoords[1] = screenCenterY + screenZoom;
				triangleCoords[2] = objectX+objectW/2 - screenZoom*(1-ts); triangleCoords[3] = screenCenterY + screenZoom*ts;
				triangleCoords[4] = objectX+objectW/2 + screenZoom*(1-ts); triangleCoords[5] = screenCenterY + screenZoom*ts;
			} else {
				//center... can't happen
			}
		}

		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex2f(triangleCoords[0], triangleCoords[1]);
		glVertex2f(triangleCoords[2], triangleCoords[3]);
		glVertex2f(triangleCoords[4], triangleCoords[5]);
		glEnd();
	}
	delete screen;
	delete object;
}

void GameMainLoop::drawMain() const {
	GameSceneManager::setXCenter(player->getX());
	GameSceneManager::setYCenter(player->getY());
	GameSceneManager::setCenterDist(player->getZoomDist());
	GameSceneManager::resizeWindow(); //TODO: necessary?

	//drawBackground();

	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->draw();
	}

	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i]->draw();
	}

	for (int i = 0; i < powerups.size(); i++) {
		powerups[i]->draw();
	}

	for (int i = 0; i < enemies.size(); i++) {
		drawArrow(player->getX(), player->getY(), player->getZoomDist(), enemies[i]);
	}
	for (int i = 0; i < powerups.size(); i++) {
		drawArrow(player->getX(), player->getY(), player->getZoomDist(), powerups[i]);
	}

	player->draw();

	//drawMessage("center", 1.0f/16, GAME_WIDTH/2, GAME_HEIGHT/2);
	//drawMessage("max", 1.0f, GAME_WIDTH/2, GAME_HEIGHT/2);
	drawMessage(alertMessage, 1.0f/8, player->getX(), player->getY() - player->getZoomDist() + 10);
	drawMessage("Wave: "+std::to_string(waveNumber), 1.0f/12, player->getX() + player->getZoomDist() - 25, player->getY() + player->getZoomDist() - 10);
	drawMessage("Score: "+std::to_string(currentScore), 1.0f/12, player->getX() - player->getZoomDist() + 25, player->getY() + player->getZoomDist() - 10);
}
