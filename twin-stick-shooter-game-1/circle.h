#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

class Circle {
	friend class CollisionHandler;

public:
	//for drawing:
	static const int EDGE_COUNT = 64;

protected:
	float x;
	float y;
	float r;

public:
	Circle() : Circle(0, 0, 0) {}
	Circle(float x, float y, float r);

	float getX() const { return x; }
	float getY() const { return y; }
	float getR() const { return r; }

	void draw() const;
	void draw(float r, float g, float b) const;
	static void draw(float x, float y, float radius, float red, float g, float b, GLenum method=GL_POLYGON);
};
