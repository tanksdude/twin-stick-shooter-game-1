#include "game-main-loop.h"

//includes copied from main.cpp
#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <unordered_map>
#include <stdexcept>

//important stuff:
#include "rng.h"
#include "color-value-holder.h"
#include "background-rect.h"
//TODO

//managers:
#include "game-scene-manager.h"
#include "developer-manager.h"
#include "game-manager.h"
#include "keypress-manager.h"
#include "diagnostics.h"

//classes with important handling functions:
#include "collision-handler.h"
#include "reset-things.h"
#include "color-mixer.h"
#include "end-game-handler.h"
#include "physics-handler.h"
//TODO

//#include <GL/glew.h>
//#include <GL/freeglut.h>

GameMainLoop::GameMainLoop() : GameScene() {
	physicsRate = 100;
	waitCount = 0;
	maxWaitCount = 1000/physicsRate * 10;
}

void GameMainLoop::Tick(int UPS) {
	if (EndGameHandler::shouldGameEnd()) {
		waitCount++;
		if (waitCount >= maxWaitCount) {
			waitCount = 0;
			ResetThings::reset();
		}
		return;
	}

	//TODO: game code

	//finish up by incrementing the tick count
	GameManager::Tick();
}

void GameMainLoop::drawMain() const {
	BackgroundRect::draw();

	// Set up the transformations stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//TODO: draw everything
}

void GameMainLoop::drawLayer(DrawingLayers layer) const {
	//TODO
}

void GameMainLoop::drawAllLayers() const {
	//TODO
}
