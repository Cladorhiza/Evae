#include "Collisions.h"

namespace Collisions {

	bool IsCollidingCircle(float x1, float y1, float r1, float x2, float y2, float r2) {

		return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < (r1 + r2) * (r1 + r2);
	}

	bool IsCollidingAABB(float x1Left, float x1Right, float x1Top, float x1Bottom, float x2Left, float x2Right, float x2Top, float x2Bottom) {

		return (!(x1Right < x2Left || x1Left > x2Right || x1Top < x2Bottom || x1Bottom > x2Top));
	}

}
