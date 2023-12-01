#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

class Rect {
	friend class CollisionHandler;

protected:
	float x;
	float y;
	float w;
	float h;

public:
	Rect() : Rect(0, 0, 0, 0) {}
	Rect(float x, float y, float w, float h);

	float getX() const noexcept { return x; }
	float getY() const noexcept { return y; }
	float getW() const noexcept { return w; }
	float getH() const noexcept { return h; }

	void draw() const;
	void draw(float r, float g, float b) const;
	static void draw(float x, float y, float w, float h, float r, float g, float b, GLenum method=GL_POLYGON);
};
