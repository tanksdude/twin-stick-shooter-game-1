#include "game-scene-manager.h"

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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);

	// Setup window position, size, and title
	glutInitWindowPosition(0, 0);
	glutCreateWindow(windowName.c_str());
	glutReshapeWindow(sizeX, sizeY);

	// Setup some OpenGL options
	//glEnable (GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	//glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(4);
	glLineWidth(2);

	//initialize glew
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		throw "glew failed";
	}

	//set the callbacks later right after this
}

void GameSceneManager::windowToSceneCoordinates(float& x, float& y) {
	x = 2*(float(x) / width - .5) * centerToEdge + xCenter;
	y = 2*(float(height - y) / height - .5) * centerToEdge + yCenter;
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
	}
	else {
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
	for (int i = 0; i < scenes.size(); i++) {
		scenes[i].first->Tick(UPS);
	}
	DrawScenes();
	glutTimerFunc(1000/UPS, GameSceneManager::TickScenes, UPS);
}

void GameSceneManager::DrawScenes() {
	//clear (mostly unnecessary, I think)
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
