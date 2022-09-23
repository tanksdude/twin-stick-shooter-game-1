#pragma once
#include "game-scene.h"
#include <vector>
#include <utility>
#include <string>

typedef int Scene_ID;

class GameSceneManager {
private: //window/input/graphics stuff that I couldn't really put anywhere else
	friend class GameMainLoop; //temporary, hopefully?
	static int width;
	static int height;
private:
	static int screen_width;
	static int screen_height;
	static float xCenter;
	static float yCenter;
	static float centerToEdge;
	static void windowToSceneCoordinates(float&, float&);
public:
	static void setXCenter(float x) { xCenter = x; }
	static void setYCenter(float y) { yCenter = y; }
	static void setCenterDist(float d) { centerToEdge = d; }

public:
	static void PreInitialize(int* argc, char** argv, std::string windowName, int startX, int startY, int sizeX, int sizeY);

	static void keyDown(unsigned char, int, int);
	static void keyUp(unsigned char, int, int);

	static void specialKeyDown(int, int, int);
	static void specialKeyUp(int, int, int);

	static void mouseClickHandle(int, int, int, int);

	static void dragHandle(int, int);
	static void passiveDragHandle(int, int);

	static void resizeWindow();
	static void resizeWindow(int, int);



private: //actual scene managament
	static std::vector<std::pair<GameScene*, Scene_ID>> scenes;
	//static void clearScenes();
	static Scene_ID nextSceneID;

public:
	static void Initialize();
	//TODO
	static GameScene* getScene(int index);
	static GameScene* getSceneByID(Scene_ID);
	static void pushScene(GameScene*);
	static int getNumScenes() { return scenes.size(); }
	static void deleteScene(int index);
	static void deleteSceneByID(Scene_ID);

	static void TickScenes(int UPS);
	static void DrawScenes(int UPS) { DrawScenes(); }
	static void DrawScenes();

private:
	GameSceneManager() {}
	GameSceneManager(const GameSceneManager&) {}
};
