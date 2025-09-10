#include <string>

#include "rng.h"

#include "game-scene-manager.h"
#include "game-main-loop.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

const std::string GameWindowName = "Twin-Stick Shooter Test Game v1.0 DEV";

int main(int argc, char** argv) {
	RNG::Initialize();
	GameSceneManager::PreInitialize(&argc, argv, GameWindowName, 120, 60, 600, 600);

	// Set callback for drawing the scene
	glutDisplayFunc(GameSceneManager::DrawScenes);

	// Set callback for resizing the window
	glutReshapeFunc(GameSceneManager::resizeWindow);

	//mouse clicking
	glutMouseFunc(GameSceneManager::mouseClickHandle);

	// Set callback to handle mouse dragging
	glutMotionFunc(GameSceneManager::dragHandle);

	//passive mouse dragging
	glutPassiveMotionFunc(GameSceneManager::passiveDragHandle);

	// Set callback to handle keyboard events
	glutKeyboardFunc(GameSceneManager::keyDown);

	//callback for keyboard up events
	glutKeyboardUpFunc(GameSceneManager::keyUp);

	//special keyboard down
	glutSpecialFunc(GameSceneManager::specialKeyDown);

	//special keyboard up
	glutSpecialUpFunc(GameSceneManager::specialKeyUp);

	//mousewheel
	//TODO

	// Set callback for the idle function
	//glutIdleFunc(draw);

	//initialize managers and stuff:
	//TODO

	//game mode:
	GameSceneManager::pushScene(new GameMainLoop());

	//main game code initialization stuff:
	//TODO

	//framelimiter
	GameSceneManager::TickScenes(100);

	// Start the main loop
	glutMainLoop();
}
