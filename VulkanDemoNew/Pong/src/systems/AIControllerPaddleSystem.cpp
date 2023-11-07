#include "AIControllerPaddleSystem.h"

void AIControllerPaddleSystem::AddComponent(uint32_t id, float paddleSpeed, float topBound, float bottomBound) {

	components.emplace_back(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(topBound, bottomBound, paddleSpeed));

}

void AIControllerPaddleSystem::Init(TransformSystem* transformSystem) {
	ballTransform = &transformSystem->GetTransformComponent(2);
	this->transformSystem = transformSystem;
	
}

void AIControllerPaddleSystem::Update(float deltaTime) {

	for (const auto& comp : components) {

		glm::vec3 translation(0.f);
		TransformComponent& t = transformSystem->GetTransformComponent(comp.first);

		//if below y value of ball, move up
		
		if (ballTransform->GetPosition().y > t.GetPosition().y) {
			translation.y += comp.second.paddleSpeed * deltaTime;
		}
		 
		
		//if above y value of ball, move down

		if (ballTransform->GetPosition().y < t.GetPosition().y) {
			translation.y -= comp.second.paddleSpeed * deltaTime;
		}



		t.SetPosition((t.GetPosition() + translation));
		if (t.GetPosition().y > comp.second.topBoundary)
			t.SetPosition(glm::vec3(t.GetPosition().x, comp.second.topBoundary, t.GetPosition().z));
		if (t.GetPosition().y < comp.second.bottomBoundary)
			t.SetPosition(glm::vec3(t.GetPosition().x, comp.second.bottomBoundary, t.GetPosition().z));
	}



}