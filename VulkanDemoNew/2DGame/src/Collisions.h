#pragma once
namespace Collisions
{




	bool IsCollidingCircle(float x1, float y1, float r1, float x2, float y2, float r2);
	bool IsCollidingAABB(float x1Left, float x1Right, float x1Top, float x1Bottom, float x2Left, float x2Right, float x2Top, float x2Bottom);
};

