#include "game-scene-manager.h"

#include <cmath>
#include <chrono>
#include <stdexcept>

#include "texture-manager.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

int GameSceneManager::width = 0;
int GameSceneManager::height = 0;
int GameSceneManager::screen_width = 0;
int GameSceneManager::screen_height = 0;
float GameSceneManager::xCenter = 0;
float GameSceneManager::yCenter = 0;
float GameSceneManager::centerToEdge = 0;

std::vector<std::pair<GameScene*, Scene_ID>> GameSceneManager::scenes;
int GameSceneManager::nextSceneID = 0;

void GameSceneManager::PreInitialize(int* argc, char** argv, std::string windowName, int startX, int startY, int sizeX, int sizeY) {
	//initialize glut
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);

	// Setup window position, size, and title
	glutInitWindowPosition(startX, startY);
	glutCreateWindow(windowName.c_str());
	glutReshapeWindow(sizeX, sizeY);

	// Setup some OpenGL options
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	//glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(4);
	glLineWidth(2);

	glShadeModel(GL_FLAT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//initialize glew
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		throw std::runtime_error("glew failed");
	}

	//couldn't find a better place for this
	TextureManager::addSprite("destroyer.png", new Sprite("images/destroyer.png"));
	TextureManager::addSprite("reticle.png", new Sprite("images/reticle.png"));
	TextureManager::addSprite("shield.png", new Sprite("images/shield.png"));
	TextureManager::addSprite("shieldbubble.png", new Sprite("images/shieldbubble.png"));
	TextureManager::addSprite("strange_shield.png", new Sprite("images/strange-shield-small.png"));
	//TextureManager::addSprite("viking-shield.png", new Sprite("images/viking-shield-small.png"));
	TextureManager::addSprite("cpu_pins.jpg", new Sprite("images/cpu_pins.jpg"));
	TextureManager::addSprite("space_with_stars.jpg", new Sprite("images/space-with-stars-1.jpg"));
	TextureManager::addSprite("lasers.png", new Sprite("images/laser_spritesheet.png", 1, 11));
	TextureManager::addSprite("coin_sprite.png", new Sprite("images/coin_sprite.png", 6, 1));
	TextureManager::addSprite("space_invader.png", new Sprite("images/space-invader.png", 6, 1));
	TextureManager::addSprite("thruster_spritesheet.png", new Sprite("images/thruster-spritesheet.png", 4, 1));
}

void GameSceneManager::windowToSceneCoordinates(float& x, float& y) {
	x = 2*(float(x) / width - .5f) * centerToEdge + xCenter;
	y = 2*(float(height - y) / height - .5f) * centerToEdge + yCenter;
}

void GameSceneManager::resizeWindow() {
	//TODO?
	resizeWindow(screen_width, screen_height);
}

void GameSceneManager::resizeWindow(int w, int h) {
	// Window size has changed
	width = w;
	height = h;
	screen_width = w;
	screen_height = h;

	double scale, center;
	double winXmin, winXmax, winYmin, winYmax;

	// Define x-axis and y-axis range
	const double appXmin = xCenter - centerToEdge;
	const double appXmax = xCenter + centerToEdge;
	const double appYmin = yCenter - centerToEdge;
	const double appYmax = yCenter + centerToEdge;

	// Define that OpenGL should use the whole window for rendering
	glViewport(0, 0, w, h);

	// Set up the projection matrix using a orthographic projection that will
	// maintain the aspect ratio of the scene no matter the aspect ratio of
	// the window, and also set the min/max coordinates to be the disered ones
	w = (w == 0) ? 1 : w;
	h = (h == 0) ? 1 : h;

	if ((appXmax - appXmin) / w < (appYmax - appYmin) / h) {
		scale = ((appYmax - appYmin) / h) / ((appXmax - appXmin) / w);
		center = (appXmax + appXmin) / 2;
		winXmin = center - (center - appXmin) * scale;
		winXmax = center + (appXmax - center) * scale;
		winYmin = appYmin;
		winYmax = appYmax;
	} else {
		scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
		center = (appYmax + appYmin) / 2;
		winYmin = center - (center - appYmin) * scale;
		winYmax = center + (appYmax - center) * scale;
		winXmin = appXmin;
		winXmax = appXmax;
	}

	// Now we use glOrtho to set up our viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(winXmin, winXmax, winYmin, winYmax, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

void GameSceneManager::keyDown(unsigned char key, int x, int y) {
	// Convert from window to scene coordinates
	float mx = (float)x;
	float my = (float)y;
	windowToSceneCoordinates(mx, my);

	scenes[0].first->keyDown(key, mx, my); //hacky, yes, I know
}

void GameSceneManager::keyUp(unsigned char key, int x, int y) {
	// Convert from window to scene coordinates
	float mx = (float)x;
	float my = (float)y;
	windowToSceneCoordinates(mx, my);

	scenes[0].first->keyUp(key, mx, my);
}

void GameSceneManager::mouseClickHandle(int b, int s, int x, int y) {
	// Convert from window to scene coordinates
	float mx = (float)x;
	float my = (float)y;
	windowToSceneCoordinates(mx, my);

	if (b == 0) {
		//left
		if (s == 0) {
			//down
			scenes[0].first->leftMouseDown(mx, my);
		} else if (s == 1) {
			//up
			scenes[0].first->leftMouseUp(mx, my);
		}
	} else if (b == 2) {
		//right
		if (s == 0) {
			//down
			scenes[0].first->rightMouseDown(mx, my);
		} else if (s == 1) {
			//up
			scenes[0].first->rightMouseUp(mx, my);
		}
	}
}

void GameSceneManager::dragHandle(int x, int y) {
	// Convert from window to scene coordinates
	float mx = (float)x;
	float my = (float)y;
	windowToSceneCoordinates(mx, my);

	scenes[0].first->drag(mx, my);
}

void GameSceneManager::passiveDragHandle(int x, int y) {
	// Convert from window to scene coordinates
	float mx = (float)x;
	float my = (float)y;
	windowToSceneCoordinates(mx, my);

	scenes[0].first->passiveDrag(mx, my);
}

void GameSceneManager::specialKeyDown(int key, int x, int y) {
	// Convert from window to scene coordinates
	float mx = (float)x;
	float my = (float)y;
	windowToSceneCoordinates(mx, my);

	scenes[0].first->specialKeyDown(key, mx, my);
}

void GameSceneManager::specialKeyUp(int key, int x, int y) {
	// Convert from window to scene coordinates
	float mx = (float)x;
	float my = (float)y;
	windowToSceneCoordinates(mx, my);

	scenes[0].first->specialKeyUp(key, mx, my);
}



void GameSceneManager::Initialize() {
	//nothing
}

void GameSceneManager::TickScenes(int UPS) {
	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < scenes.size(); i++) {
		scenes[i].first->Tick(UPS);
	}
	DrawScenes();

	auto end = std::chrono::steady_clock::now();
	auto timeTakenMS = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0f; //milliseconds
	auto sleepTimeMS = std::ceil(1000.0f/UPS - timeTakenMS);
	if (sleepTimeMS > 0) {
		glutTimerFunc(static_cast<unsigned int>(sleepTimeMS), GameSceneManager::TickScenes, UPS);
	} else {
		glutTimerFunc(0, GameSceneManager::TickScenes, UPS);
	}
}

void GameSceneManager::DrawScenes() {
	//clear (mostly unnecessary, I think)
	glClear(GL_COLOR_BUFFER_BIT);

	// Set up the transformations stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//draw stuff
	for (int i = 0; i < scenes.size(); i++) {
		scenes[i].first->Draw();
	}

	//flush
	glFlush();
}

GameScene* GameSceneManager::getScene(int index) {
	return scenes[index].first;
}

GameScene* GameSceneManager::getSceneByID(Scene_ID sceneID) {
	for (int i = 0; i < scenes.size(); i++) {
		if (scenes[i].second == sceneID) {
			return scenes[i].first;
		}
	}
	return nullptr;
}

void GameSceneManager::pushScene(GameScene* gs) {
	scenes.push_back({ gs, ++nextSceneID });
}

void GameSceneManager::deleteScene(int index) {
	delete scenes[index].first;
	scenes.erase(scenes.begin() + index);
}

void GameSceneManager::deleteSceneByID(Scene_ID sceneID) {
	for (int i = 0; i < scenes.size(); i++) {
		if (scenes[i].second == sceneID) {
			deleteScene(i);
			break;
		}
	}
}

/*
Scene_ID GameSceneManager::getNextID() {
	nextSceneID++;
	return nextSceneID;
}
*/
