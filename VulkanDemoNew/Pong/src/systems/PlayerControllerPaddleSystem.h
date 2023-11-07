#pragma once
#include "Hubbsi.h"
#include "../components/PlayerControllerPaddleComponent.h"

#include <vector>
class PlayerControllerPaddleSystem
{

private:

	std::vector<std::pair<uint32_t, PlayerControllerPaddleComponent>> components;
	InputManager* inputManager;
	TransformSystem* transformSystem;
public:
	void AddComponent(uint32_t id, int keyUp, int keyDown, float paddleSpeed, float topBound, float bottomBound);

	void Init(InputManager* inputManager, TransformSystem* transformSystem);
	void Update(float deltaTime);



};

