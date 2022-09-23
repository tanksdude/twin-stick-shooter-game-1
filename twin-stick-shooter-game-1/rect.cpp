#include "rect.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

Rect::Rect(float x, float y, float w, float h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void Rect::draw() const {
	draw(1, 1, 1);
}

void Rect::draw(float r, float g, float b) const {
	Rect::draw(x, y, w, h, r, g, b);
}

void Rect::draw(float x, float y, float w, float h, float r, float g, float b, GLenum method) {
	glBegin(method);
	glColor3f(r, g, b);
	glVertex2f(x, y);
	glVertex2f(x+w, y);
	glVertex2f(x+w, y+h);
	glVertex2f(x, y+h);
	glEnd();
}
