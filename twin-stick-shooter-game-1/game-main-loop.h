#pragma once
#include "game-scene.h"
#include "drawable-thing.h"

class GameMainLoop : public GameScene {
	friend class DeveloperManager;
	friend class GameManager; //needed?

protected:
	int physicsRate; //(in Hz)
	int waitCount;
	int maxWaitCount;

public:
	GameMainLoop();
	void Tick() { Tick(physicsRate); }
	void Tick(int UPS) override;
	void Draw() const override { drawMain(); }

	//tick stuff:
	//TODO

	//handlers:
	void keyDown(unsigned char key, float x, float y) override;
	void keyUp(unsigned char key, float x, float y) override;
	void drag(float, float) override; //dragging, but mouse is held
	void passiveDrag(float, float) override; //dragging, but mouse isn't held
	void leftMouseDown(float, float) override;
	void leftMouseUp(float, float) override;

	void drawMain() const; //doesn't draw all layers (since not everything uses everything)
	void drawAllLayers() const;
	void drawLayer(DrawingLayers) const;

private:
	GameMainLoop(const GameMainLoop&) {}
};
