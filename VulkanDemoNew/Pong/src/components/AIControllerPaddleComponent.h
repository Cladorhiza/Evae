#pragma once
#include <Component.h>
class AIControllerPaddleComponent :
    protected Component
{

private:

public:

    float topBoundary;
    float bottomBoundary;
    float paddleSpeed;

    AIControllerPaddleComponent(float topBound, float bottomBound, float paddleSpeed);
};

