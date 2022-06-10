#include "collision-handler.h"
#include "constants.h"
#include <math.h>
#include <stdexcept>
#include <iostream>

bool CollisionHandler::partiallyOutOfBounds(const Rect* r) {
	return ((r->x + r->w >= GAME_WIDTH) || (r->x <= 0) || (r->y + r->h >= GAME_HEIGHT) || (r->y <= 0));
}

bool CollisionHandler::partiallyOutOfBoundsIgnoreEdge(const Rect* r) {
	return ((r->x + r->w > GAME_WIDTH) || (r->x < 0) || (r->y + r->h > GAME_HEIGHT) || (r->y < 0));
}

bool CollisionHandler::fullyOutOfBounds(const Rect* r) {
	return ((r->x >= GAME_WIDTH) || (r->x + r->w <= 0) || (r->y >= GAME_HEIGHT) || (r->y + r->h <= 0));
}

bool CollisionHandler::fullyOutOfBoundsIgnoreEdge(const Rect* r) {
	return ((r->x > GAME_WIDTH) || (r->x + r->w < 0) || (r->y > GAME_HEIGHT) || (r->y + r->h < 0));
}

bool CollisionHandler::partiallyOutOfBounds(const Circle* c) {
	return ((c->x + c->r >= GAME_WIDTH) || (c->x - c->r <= 0) || (c->y + c->r >= GAME_HEIGHT) || (c->y - c->r <= 0));
}

bool CollisionHandler::partiallyOutOfBoundsIgnoreEdge(const Circle* c) {
	return ((c->x + c->r > GAME_WIDTH) || (c->x - c->r < 0) || (c->y + c->r > GAME_HEIGHT) || (c->y - c->r < 0));
}

bool CollisionHandler::fullyOutOfBounds(const Circle* c) {
	return ((c->x - c->r >= GAME_WIDTH) || (c->x + c->r <= 0) || (c->y - c->r >= GAME_HEIGHT) || (c->y + c->r <= 0));
}

bool CollisionHandler::fullyOutOfBoundsIgnoreEdge(const Circle* c) {
	return ((c->x - c->r > GAME_WIDTH) || (c->x + c->r < 0) || (c->y - c->r > GAME_HEIGHT) || (c->y + c->r < 0));
}

void CollisionHandler::edgeConstrain(Rect* r) {
	if (r->x + r->w > GAME_WIDTH) {
		r->x = GAME_WIDTH - r->w;
	} else if (r->x < 0) {
		r->x = 0;
	}
	if (r->y + r->h > GAME_HEIGHT) {
		r->y = GAME_HEIGHT - r->h;
	} else if (r->y < 0) {
		r->y = 0;
	}
}

void CollisionHandler::edgeConstrain(Circle* c) {
	if (c->x + c->r > GAME_WIDTH) {
		c->x = GAME_WIDTH - c->r;
	} else if (c->x - c->r < 0) {
		c->x = c->r;
	}
	if (c->y + c->r > GAME_HEIGHT) {
		c->y = GAME_HEIGHT - c->r;
	} else if (c->y - c->r < 0) {
		c->y = c->r;
	}
}

bool CollisionHandler::partiallyCollided(const Rect* a, const Rect* b) {
	return ((a->x + a->w >= b->x) && (a->x <= b->x + b->w) && (a->y + a->h >= b->y) && (a->y <= b->y + b->h));
}

bool CollisionHandler::partiallyCollided(const Rect* a, const Circle* b) {
	if ((b->x < a->x) && (b->y < a->y)) { //bottom left
		return cornerCollided(b, a->x, a->y);
	} else if ((b->x > (a->x + a->w)) && (b->y < a->y)) { //bottom right
		return cornerCollided(b, (a->x + a->w), a->y);
	} else if ((b->x < a->x) && (b->y > (a->y + a->h))) { //top left
		return cornerCollided(b, a->x, (a->y + a->h));
	} else if ((b->x > (a->x + a->w)) && (b->y > (a->y + a->h))) { //top right
		return cornerCollided(b, (a->x + a->w), (a->y + a->h));
	} else {
		return (((b->x + b->r) >= a->x) && ((b->x - b->r) <= (a->x + a->w)) && ((b->y + b->r) >= a->y) && ((b->y - b->r) <= (a->y + a->h)));
	}
}
//bool CollisionHandler::partiallyCollided(const Circle* a, const Rect* b);

bool CollisionHandler::partiallyCollided(const Circle* a, const Circle* b) {
	if ((abs(a->x - b->x) <= a->r + b->r) && (abs(a->y - b->y) <= a->r + b->r)) {
		return (sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)) <= a->r + b->r);
	}
	return false;
}

bool CollisionHandler::cornerCollided(const Circle* a, double x, double y) { //effectively C-C but C2->r = 0
	if ((abs(x - a->x) <= a->r) && (abs(y - a->y) <= a->r)) {
		double d = sqrt(pow(x - a->x, 2) + pow(y - a->y, 2));
		return (d <= a->r);
	}
	return false;
}

//IgnoreEdge variant: code is near-identical to non IgnoreEdge version
bool CollisionHandler::partiallyCollidedIgnoreEdge(const Rect* a, const Rect* b) {
	return ((a->x + a->w > b->x) && (a->x < b->x + b->w) && (a->y + a->h > b->y) && (a->y < b->y + b->h));
}

bool CollisionHandler::partiallyCollidedIgnoreEdge(const Rect* a, const Circle* b) {
	if ((b->x < a->x) && (b->y < a->y)) { //bottom left
		return cornerCollided(b, a->x, a->y);
	} else if ((b->x > (a->x + a->w)) && (b->y < a->y)) { //bottom right
		return cornerCollided(b, (a->x + a->w), a->y);
	} else if ((b->x < a->x) && (b->y > (a->y + a->h))) { //top left
		return cornerCollided(b, a->x, (a->y + a->h));
	} else if ((b->x > (a->x + a->w)) && (b->y > (a->y + a->h))) { //top right
		return cornerCollided(b, (a->x + a->w), (a->y + a->h));
	} else {
		return (((b->x + b->r) > a->x) && ((b->x - b->r) < (a->x + a->w)) && ((b->y + b->r) > a->y) && ((b->y - b->r) < (a->y + a->h)));
	}
}
//bool CollisionHandler::partiallyCollidedIgnoreEdge(const Circle* a, const Rect* b);

bool CollisionHandler::partiallyCollidedIgnoreEdge(const Circle* a, const Circle* b) {
	if ((abs(a->x - b->x) < a->r + b->r) && (abs(a->y - b->y) < a->r + b->r)) {
		return (sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)) < a->r + b->r);
	}
	return false;
}

bool CollisionHandler::cornerCollidedIgnoreEdge(const Circle* a, double x, double y) {
	if ((abs(x - a->x) < a->r) && (abs(y - a->y) < a->r)) {
		double d = sqrt(pow(x - a->x, 2) + pow(y - a->y, 2));
		return (d < a->r);
	}
	return false;
}

bool CollisionHandler::fullyCollided(const Rect* a, const Rect* b) { //a inside b
	return ((a->x >= b->x) && ((a->x + a->w) <= (b->x + b->w)) && (a->y >= b->y) && ((a->y + a->h) <= (b->y + b->h)));
}
bool CollisionHandler::fullyCollided(const Rect* a, const Circle* b) { //rectangle inside circle
	if ((a->x >= (b->x - b->r)) && ((a->x + a->w) <= (b->x + b->r)) && (a->y >= (b->y - b->r)) && ((a->y + a->h) <= (b->y + b->r))) { //check R-R collision
		return ((sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)) <= b->r) && //check distance between each corner to circle center
		        (sqrt(pow(a->x - b->x, 2) + pow((a->y + a->h) - b->y, 2)) <= b->r) &&
		        (sqrt(pow((a->x + a->w) - b->x, 2) + pow(a->y - b->y, 2)) <= b->r) &&
		        (sqrt(pow((a->x + a->w) - b->x, 2) + pow((a->y + a->h) - b->y, 2)) <= b->r));
	}
	return false;
}
bool CollisionHandler::fullyCollided(const Circle* a, const Rect* b) { //circle inside rectangle
	return (((a->x - a->r) >= b->x) && ((a->x + a->r) <= (b->x + b->w)) && ((a->y - a->r) >= b->y) && ((a->y + a->r) <= (b->y + b->h)));
}
bool CollisionHandler::fullyCollided(const Circle* a, const Circle* b) { //a inside b
	if (((a->x - a->r) >= (b->x - b->r)) && ((a->x + a->r) <= (b->x + b->r)) && ((a->y - a->r) >= (b->y - b->r)) && ((a->y + a->r) <= (b->y + b->r))) { //check R-R collision
		return (sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)) <= b->r - a->r);
	}
	return false;
}



//these should only be called if collision was checked (and true) first; they literally move each object to the other's edge
void CollisionHandler::pushMovableAwayFromImmovable(Rect* movable, Rect* immovable) {
	if ((movable->y + movable->h / 2) - immovable->y <= (immovable->h / immovable->w) * ((movable->x + movable->w / 2) - immovable->x)) { //I think this is top right
		if ((movable->y + movable->h / 2) - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * ((movable->x + movable->w / 2) - immovable->x)) { //top
			movable->y = immovable->y - movable->h;
		} else { //right
			movable->x = immovable->x + immovable->w;
		}
	} else { //bottom left?
		if ((movable->y + movable->h / 2) - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * ((movable->x + movable->w / 2) - immovable->x)) { //left
			movable->x = immovable->x - movable->w;
		} else { //bottom
			movable->y = immovable->y + immovable->h;
		}
	}
}
void CollisionHandler::pushMovableAwayFromMovable(Rect* movable1, Rect* movable2) {
	if ((movable1->y + movable1->h / 2) - movable2->y <= (movable2->h / movable2->w) * ((movable1->x + movable1->w / 2) - movable2->x)) { //I think this is top right
		if ((movable1->y + movable1->h / 2) - (movable2->y + movable2->h) <= (-movable2->h / movable2->w) * ((movable1->x + movable1->w / 2) - movable2->x)) { //top
			double d = (movable1->y + movable1->h) - movable2->y;
			movable1->y -= d/2;
			movable2->y += d/2;
		} else { //right
			double d = (movable2->x + movable2->w) - movable1->x;
			movable1->x += d/2;
			movable2->x -= d/2;
		}
	} else { //bottom left?
		if ((movable1->y + movable1->h / 2) - (movable2->y + movable2->h) <= (-movable2->h / movable2->w) * ((movable1->x + movable1->w / 2) - movable2->x)) { //left
			double d = (movable1->x + movable1->w) - movable2->x;
			movable1->x -= d/2;
			movable2->x += d/2;
		} else { //bottom
			double d = (movable2->y + movable2->h) - movable1->y;
			movable1->y += d/2;
			movable1->y -= d/2;
		}
	}
}

void CollisionHandler::pushMovableAwayFromImmovable(Circle* movable, Rect* immovable) {
	bool cornerCollided = false;
	if ((movable->x < immovable->x) && (movable->y < immovable->y)) { //circle in bottom left
		cornerPushMovableAwayFromImmovable(movable, immovable->x, immovable->y);
		cornerCollided = true;
	}
	//can't do elses because more than one corner could be in the circle (TODO: really?)
	if ((movable->x > (immovable->x + immovable->w)) && (movable->y < immovable->y)) { //circle in bottom right
		cornerPushMovableAwayFromImmovable(movable, (immovable->x + immovable->w), immovable->y);
		cornerCollided = true;
	}
	if ((movable->x < immovable->x) && (movable->y > (immovable->y + immovable->h))) { //circle in top left
		cornerPushMovableAwayFromImmovable(movable, immovable->x, (immovable->y + immovable->h));
		cornerCollided = true;
	}
	if ((movable->x > (immovable->x + immovable->w)) && (movable->y > (immovable->y + immovable->h))) { //circle in top right
		cornerPushMovableAwayFromImmovable(movable, (immovable->x + immovable->w), (immovable->y + immovable->h));
		cornerCollided = true;
	}

	if (cornerCollided) {
		return;
	}

	//find which side the circle is near
	if (movable->y - immovable->y <= (immovable->h / immovable->w) * (movable->x - immovable->x)) { //I think this is bottom right
		if (movable->y - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * (movable->x - immovable->x)) { //bottom
			movable->y = immovable->y - movable->r;
		} else { //right
			movable->x = immovable->x + immovable->w + movable->r;
		}
	} else { //top left?
		if (movable->y - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * (movable->x - immovable->x)) { //left
			movable->x = immovable->x - movable->r;
		} else { //top
			movable->y = immovable->y + immovable->h + movable->r;
		}
	}
}
//void CollisionHandler::pushMovableAwayFromMovable(Circle* movable1, Rect* movable2);

//these two are weird
void CollisionHandler::pushMovableAwayFromImmovable(Rect* movable, Circle* immovable) {
	//first, find if circle is in outer corner (as opposed to near a side)
	//if on side, R-R stuff
	//else, C-C stuff (if dist < asdf then move, but don't use else)

	if ((immovable->x < movable->x) && (immovable->y < movable->y)) { //circle in top left
		cornerPushMovableAwayFromImmovable(movable, immovable, movable->x, movable->y);
	}
	//can't do elses because more than one corner could be in the circle
	if ((immovable->x > (movable->x + movable->w)) && (immovable->y < movable->y)) { //circle in top right
		cornerPushMovableAwayFromImmovable(movable, immovable, (movable->x + movable->w), movable->y);
	}
	if ((immovable->x < movable->x) && (immovable->y > (movable->y + movable->h))) { //circle in bottom left
		cornerPushMovableAwayFromImmovable(movable, immovable, movable->x, (movable->y + movable->h));
	}
	if ((immovable->x > (movable->x + movable->w)) && (immovable->y > (movable->y + movable->h))) { //circle in bottom right
		cornerPushMovableAwayFromImmovable(movable, immovable, (movable->x + movable->w), (movable->y + movable->h));
	}

	//find which side the circle is near

	if (immovable->y - movable->y <= (movable->h / movable->w) * (immovable->x - movable->x)) { //I think this is top right
		if (immovable->y - (movable->y + movable->h) <= (-movable->h / movable->w) * (immovable->x - movable->x)) { //top
			movable->y = immovable->y - immovable->r - movable->h;
		} else { //right
			movable->x = immovable->x + immovable->r;
		}
	} else { //bottom left?
		if (immovable->y - (movable->y + movable->h) <= (-movable->h / movable->w) * (immovable->x - movable->x)) { //left
			movable->x = immovable->x - immovable->r - movable->w;
		} else { //bottom
			movable->y = immovable->y + immovable->r;
		}
	}
}
void CollisionHandler::pushMovableAwayFromMovable(Rect* movable1, Circle* movable2) {
	if ((movable2->x < movable1->x) && (movable2->y < movable1->y)) { //circle in top left
		cornerPushMovableAwayFromMovable(movable1, movable2, movable1->x, movable1->y);
	}
	//can't do elses because more than one corner could be in the circle
	if ((movable2->x >(movable1->x + movable1->w)) && (movable2->y < movable1->y)) { //circle in top right
		cornerPushMovableAwayFromMovable(movable1, movable2, (movable1->x + movable1->w), movable1->y);
	}
	if ((movable2->x < movable1->x) && (movable2->y >(movable1->y + movable1->h))) { //circle in bottom left
		cornerPushMovableAwayFromMovable(movable1, movable2, movable1->x, (movable1->y + movable1->h));
	}
	if ((movable2->x >(movable1->x + movable1->w)) && (movable2->y >(movable1->y + movable1->h))) { //circle in bottom right
		cornerPushMovableAwayFromMovable(movable1, movable2, (movable1->x + movable1->w), (movable1->y + movable1->h));
	}

	if (movable2->y - movable1->y <= (movable1->h / movable1->w) * (movable2->x - movable1->x)) { //I think this is top right
		if (movable2->y - (movable1->y + movable1->h) <= (-movable1->h / movable1->w) * (movable2->x - movable1->x)) { //top
			double d = movable2->y + movable2->r - movable1->y;
			movable2->y -= d/2;
			movable1->y += d/2;
		} else { //right
			double d = (movable1->x + movable1->w) - (movable2->x - movable2->r);
			movable2->x += d/2;
			movable1->x -= d/2;
		}
	} else { //bottom left?
		if (movable2->y - (movable1->y + movable1->h) <= (-movable1->h / movable1->w) * (movable2->x - movable1->x)) { //left
			double d = (movable2->x + movable2->r) - movable1->x;
			movable2->x -= d/2;
			movable1->x += d/2;
		} else { //bottom
			double d = (movable1->y + movable1->h) - (movable2->y - movable2->r);
			movable2->y += d/2;
			movable1->y -= d/2;
		}
	}
}

void CollisionHandler::pushMovableAwayFromImmovable(Circle* movable, Circle* immovable) {
	double angle = atan2((movable->y - immovable->y), (movable->x - immovable->x));
	double d = sqrt(pow(movable->x - immovable->x, 2) + pow(movable->y - immovable->y, 2)) - (movable->r + immovable->r);
	movable->y -= sin(angle) * d;
	movable->x -= cos(angle) * d;
}
void CollisionHandler::pushMovableAwayFromMovable(Circle* movable1, Circle* movable2) {
	double angle = atan2((movable1->y - movable2->y), (movable1->x - movable2->x));
	double d = sqrt(pow(movable1->x - movable2->x, 2) + pow(movable1->y - movable2->y, 2)) - (movable1->r + movable2->r);
	movable1->y -= sin(angle) * d/2;
	movable1->x -= cos(angle) * d/2;
	movable2->y += sin(angle) * d/2;
	movable2->x += cos(angle) * d/2;
}

void CollisionHandler::cornerPushMovableAwayFromImmovable(Circle* movable, double x, double y) {
	double angle = atan2((y - movable->y), (x - movable->x));
	double d = movable->r - sqrt(pow(x - movable->x, 2) + pow(y - movable->y, 2));
	movable->y -= sin(angle) * d;
	movable->x -= cos(angle) * d;
}
void CollisionHandler::cornerPushMovableAwayFromMovable(Circle* movable1, Rect* movable2, double x, double y) {
	double angle = atan2((y - movable1->y), (x - movable1->x));
	double d = movable1->r - sqrt(pow(x - movable1->x, 2) + pow(y - movable1->y, 2));
	movable1->y -= sin(angle) * d/2;
	movable1->x -= cos(angle) * d/2;
	movable2->y += sin(angle) * d/2;
	movable2->x += cos(angle) * d/2;
}
void CollisionHandler::cornerPushMovableAwayFromImmovable(Rect* movable, Circle* immovable, double x, double y) {
	double angle = atan2((immovable->y - y), (immovable->x - x));
	double d = immovable->r - sqrt(pow(x - immovable->x, 2) + pow(y - immovable->y, 2));
	movable->y -= sin(angle) * d;
	movable->x -= cos(angle) * d;
}
//void CollisionHandler::cornerPushMovableAwayFromMovable(Rect* movable1, Circle* movable2, double x, double y);

bool CollisionHandler::lineLineCollision(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
	//line-line intersection: http://jeffreythompson.org/collision-detection/line-line.php
	double uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
	double uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
	return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
}

bool CollisionHandler::lineRectCollision(double line1X, double line1Y, double line2X, double line2Y, const Rect* r) {
	return (lineLineCollision(line1X, line1Y, line2X, line2Y, r->x, r->y, r->x + r->w, r->y) || //bottom left to bottom right
	        lineLineCollision(line1X, line1Y, line2X, line2Y, r->x, r->y, r->x, r->y + r->h) || //bottom left to top left
	        lineLineCollision(line1X, line1Y, line2X, line2Y, r->x + r->w, r->y, r->x + r->w, r->y + r->h) || //bottom right to top right
	        lineLineCollision(line1X, line1Y, line2X, line2Y, r->x, r->y + r->h, r->x + r->w, r->y + r->h)); //top left to top right
}
