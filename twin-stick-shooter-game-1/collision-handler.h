#pragma once
#include <utility>

#include "circle.h"
#include "rect.h"

//code from my game PowerTanks Battle

class CollisionHandler {
protected:
	static bool cornerCollided(const Circle*, float x, float y);
	static bool cornerCollidedIgnoreEdge(const Circle*, float x, float y);
	static void cornerPushMovableAwayFromImmovable(Circle* movable, float x, float y);
	static void cornerPushMovableAwayFromMovable(Circle* movable1, Rect* movable2, float x, float y);
	static void cornerPushMovableAwayFromImmovable(Rect* movable, Circle*, float x, float y);
	static void cornerPushMovableAwayFromMovable(Rect* movable1, Circle* movable2, float x, float y) {
		CollisionHandler::cornerPushMovableAwayFromMovable(movable2, movable1, x, y);
	}

public:
	static bool partiallyOutOfBounds(const Rect*);
	static bool partiallyOutOfBoundsIgnoreEdge(const Rect*);
	static bool fullyOutOfBounds(const Rect*);
	static bool fullyOutOfBoundsIgnoreEdge(const Rect*);

	//below: only does simple check, treating the circle like a square
	static bool partiallyOutOfBounds(const Circle*);
	static bool partiallyOutOfBoundsIgnoreEdge(const Circle*);
	static bool fullyOutOfBounds(const Circle*);
	static bool fullyOutOfBoundsIgnoreEdge(const Circle*);

	static void edgeConstrain(Rect*);
	static void edgeConstrain(Circle*);

public: //collision detection and handling (just moving)
	static bool partiallyCollided(const Rect*, const Rect*);
	static bool partiallyCollided(const Rect*, const Circle*);
	static bool partiallyCollided(const Circle* c, const Rect* r) {
		return CollisionHandler::partiallyCollided(r, c);
	}
	static bool partiallyCollided(const Circle*, const Circle*);

	static bool partiallyCollidedIgnoreEdge(const Rect*, const Rect*);
	static bool partiallyCollidedIgnoreEdge(const Rect*, const Circle*);
	static bool partiallyCollidedIgnoreEdge(const Circle* c, const Rect* r) {
		return CollisionHandler::partiallyCollidedIgnoreEdge(r, c);
	}
	static bool partiallyCollidedIgnoreEdge(const Circle*, const Circle*);

	static bool fullyCollided(const Rect*, const Rect*);
	static bool fullyCollided(const Rect*, const Circle*);
	static bool fullyCollided(const Circle*, const Rect*); //different
	static bool fullyCollided(const Circle*, const Circle*);

	static void pushMovableAwayFromImmovable(Rect* movable, Rect* immovable);
	static void pushMovableAwayFromMovable(Rect* movable1, Rect* movable2);

	static void pushMovableAwayFromImmovable(Circle* movable, Rect* immovable);
	static void pushMovableAwayFromMovable(Circle* movable1, Rect* movable2) {
		CollisionHandler::pushMovableAwayFromMovable(movable2, movable1);
	}

	//these two are weird
	static void pushMovableAwayFromImmovable(Rect* movable, Circle* immovable);
	static void pushMovableAwayFromMovable(Rect* movable1, Circle* movable2);

	static void pushMovableAwayFromImmovable(Circle* movable, Circle* immovable);
	static void pushMovableAwayFromMovable(Circle* movable1, Circle* movable2);

public: //other stuff
	static bool lineLineCollision(float line1X, float line1Y, float line2X, float line2Y, float line3X, float line3Y, float line4X, float line4Y);
	static bool lineRectCollision(float line1X, float line1Y, float line2X, float line2Y, const Rect* r);

private:
	CollisionHandler() = delete;
	CollisionHandler(const CollisionHandler&) = delete;
};
