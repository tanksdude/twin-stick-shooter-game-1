#include <string>

#include "game-scene-manager.h"
#include "game-main-loop.h"
#include "rng.h"

//#include <GL/glew.h>
//#include <GL/freeglut.h>

int main(int argc, char** argv) {
	RNG::Initialize();
	//TODO: very obviously not done
	std::string name = "Twin-Stick Shooter Test Game v0.9"; //v1.0 once everything compiles
	//TODO?
	GameSceneManager::PreInitialize(&argc, argv, name, 60, 60, 600, 600);

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
