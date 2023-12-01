#include "circle.h"

#include "constants.h"
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

Circle::Circle(float x, float y, float r) {
	this->x = x;
	this->y = y;
	this->r = r;
}

void Circle::draw() const {
	draw(1, 1, 1);
}

void Circle::draw(float red, float g, float b) const {
	Circle::draw(x, y, this->r, red, g, b);
}

void Circle::draw(float x, float y, float radius, float red, float g, float b, GLenum method) {
	glColor3f(red, g, b);
	glBegin(method);
	for (float i = 0; i < Circle::EDGE_COUNT; i++) {
		glVertex2f(x + radius * cos((2*PI) * (i/Circle::EDGE_COUNT)), y + radius * sin((2*PI) * (i/Circle::EDGE_COUNT)));
	}
	glEnd();
}
