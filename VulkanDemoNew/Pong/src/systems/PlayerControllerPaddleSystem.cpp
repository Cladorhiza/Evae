#include "PlayerControllerPaddleSystem.h"

void PlayerControllerPaddleSystem::AddComponent(uint32_t id, int keyUp, int keyDown, float paddleSpeed, float topBound, float bottomBound) {

	components.emplace_back(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(keyUp, keyDown, paddleSpeed, topBound, bottomBound));



}

void PlayerControllerPaddleSystem::Init(InputManager* inputManager, TransformSystem* transformSystem) {

	this->inputManager = inputManager;
	this->transformSystem = transformSystem;
}

void PlayerControllerPaddleSystem::Update(float deltaTime) {

	for (const auto& comp : components) {

		glm::vec3 translation(0.f);
		TransformComponent& t = transformSystem->GetTransformComponent(comp.first);

		if (inputManager->GetKeyState(comp.second.upKeyBind) == GLFW_PRESS) {
			translation.y += comp.second.paddleSpeed * deltaTime;
		}
		if (inputManager->GetKeyState(comp.second.downKeyBind) == GLFW_PRESS) {
			translation.y -= comp.second.paddleSpeed * deltaTime;
		}

		

		t.SetPosition((t.GetPosition() + translation));
		if (t.GetPosition().y > comp.second.topBoundary)
			t.SetPosition(glm::vec3(t.GetPosition().x, comp.second.topBoundary, t.GetPosition().z));
		if (t.GetPosition().y < comp.second.bottomBoundary)
			t.SetPosition(glm::vec3(t.GetPosition().x, comp.second.bottomBoundary, t.GetPosition().z));
	}



}