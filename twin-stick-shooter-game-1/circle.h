#pragma once

class Circle {
	friend class CollisionHandler;

public:
	//for drawing:
	static constexpr int EDGE_COUNT = 64;

protected:
	float x;
	float y;
	float r;

public:
	Circle() : Circle(0, 0, 0) {}
	Circle(float x, float y, float r);

	float getX() const noexcept { return x; }
	float getY() const noexcept { return y; }
	float getR() const noexcept { return r; }

	void draw() const;
	void draw(float r, float g, float b) const;
	static void draw(float x, float y, float radius, float red, float g, float b);
};
