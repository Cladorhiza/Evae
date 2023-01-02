#include "PlayerControllerPaddleComponent.h"


PlayerControllerPaddleComponent::PlayerControllerPaddleComponent(int upKey, int downKey, float paddleSpeed, float topBound, float bottomBound) 
	:upKeyBind(upKey), downKeyBind(downKey), paddleSpeed(paddleSpeed), topBoundary(topBound), bottomBoundary(bottomBound)
{

}