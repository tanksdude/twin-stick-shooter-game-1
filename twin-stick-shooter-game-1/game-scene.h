#pragma once

class GameScene {
	friend class GameSceneManager; //needed?

protected:
	//Scene_ID sceneID;

public:
	//TODO
	virtual void Tick(int UPS) = 0;
	virtual void Draw() const = 0;

	virtual void keyDown(unsigned char, float, float) = 0;
	virtual void keyUp(unsigned char, float, float) = 0;

	virtual void specialKeyDown(int, float, float) = 0;
	virtual void specialKeyUp(int, float, float) = 0;

	virtual void leftMouseDown(float, float) = 0;
	virtual void rightMouseDown(float, float) = 0;
	virtual void leftMouseUp(float, float) = 0;
	virtual void rightMouseUp(float, float) = 0;

	virtual void drag(float, float) = 0;
	virtual void passiveDrag(float, float) = 0;

protected:
	GameScene();

private:
	GameScene(const GameScene&) {}
};
