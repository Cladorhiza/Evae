#pragma once
#include "Hubbsi.h"
class PlayerControllerPaddleComponent :
    protected Component
{
protected:


public:
    int upKeyBind;
    int downKeyBind;
    float topBoundary;
    float bottomBoundary;
    float paddleSpeed;

    PlayerControllerPaddleComponent(int upKey, int downKey, float paddleSpeed, float topBound, float bottomBound);


};

