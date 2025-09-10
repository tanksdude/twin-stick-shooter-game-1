#pragma once
#include "rect.h"
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

//for SpriteHolder; don't use for regular sprite usage
class Sprite : public Rect {
	friend class SpriteHolder;
protected:
	GLuint texture_id;
	int texwidth;
	int texheight;
	int horzCount;
	int vertCount;

	inline int getFrameXIndex(int index) const;
	inline int getFrameYIndex(int index) const;

public:
	Sprite(const char* filename) : Sprite(filename, 1, 1) {}
	Sprite(const char* filename, int horzCount, int vertCount);

	//void draw(float x, float y, float w, float h, float radians=0, float alpha=1, float z=0) const;
	void draw(float x, float y, float w, float h, float radians, float alpha, int frameNum, float z=0) const;
private:
	Sprite();
	Sprite(const Sprite&);
};

class SpriteHolder {
protected:
	std::string name;
	int index;
	int loopCount;
	int loopMax;
	int delayBetweenFrames;
	int cycle; //for ^^^

	Sprite* sprite; //gotten from TextureManager, so don't delete

public:
	std::string getName() const;
	bool isDone() const;
	void frameAdvance();
	//void getTotalFrames() const;

	SpriteHolder(const char* name, int delayBetweenFrames=1, int loopMax=-1);
	void draw(float x, float y, float w, float h, float radians=0, float alpha=1, float z=0) const;
	void draw(float x, float y, float w, float h, float radians, float alpha, int frameNum, float z=0) const;

	~SpriteHolder();
	SpriteHolder(const SpriteHolder&);
};
