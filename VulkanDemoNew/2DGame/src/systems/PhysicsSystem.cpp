#include "PhysicsSystem.h"


PhysicsSystem::PhysicsSystem(TransformSystem* ts, Scene* scene) 
	:tickRate(60), transformSystem(ts), scene(scene)
{
	components.reserve(20);





}

PhysicsComponent2D& PhysicsSystem::GetComponent(uint32_t id) {

	

	for (auto& pair : components) {

		if (pair.first == id) {
			return pair.second;
		}
	}
}

void PhysicsSystem::AddPhysicsComponent(uint32_t id) {

	components.emplace_back(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple());

}

void PhysicsSystem::Update(float timeStep) {

	//iterate over components, test components against other components for collision
	//if collision, consult the components properties to dictate the response
	for (int i{ 0 }; i < components.size(); i++) {

		TransformComponent& t1{ transformSystem->GetTransformComponent(components[i].first) };
		glm::vec3 t1v{ t1.GetPosition() };
		glm::vec3 t1vs{t1.GetScale()};
		glm::vec3 t1SideLength{ (t1vs.x/2), (t1vs.y / 2), (t1vs.z / 2) };

		for (int j{ i+1 }; j < components.size(); j++) {

			TransformComponent t2{ transformSystem->GetTransformComponent(components[j].first) };
			glm::vec3 t2v{ t2.GetPosition() };
			glm::vec3 t2vs{ t2.GetScale() };
			glm::vec3 t2SideLength{ (t2vs.x / 2), (t2vs.y / 2), (t2vs.z / 2) };

			if (Collisions::IsCollidingAABB(t1v.x - t1SideLength.x, t1v.x + t1SideLength.x, t1v.y + t1SideLength.y, t1v.y - t1SideLength.y,
				t2v.x - t2SideLength.x, t2v.x + t2SideLength.x, t2v.y + t2SideLength.y, t2v.y - t2SideLength.y)) {

				

				if (components[i].first == 2) {
					glm::vec3 temp{ components[i].second.GetVelocity() };
					if (scene->GetEntity(components[j].first).GetTag() == "wall") {
						temp.y = -temp.y;
					}
					else if (scene->GetEntity(components[j].first).GetTag() == "paddle") {
						temp.x = -temp.x;
					}
					components[i].second.SetVelocity(temp);
				}
				else if (components[j].first == 2) {
					glm::vec3 temp{ components[j].second.GetVelocity() };
					if (scene->GetEntity(components[i].first).GetTag() == "wall") {
						temp.y = -temp.y;
					}
					else if (scene->GetEntity(components[i].first).GetTag() == "paddle") {
						temp.x = -temp.x;
					}
					components[j].second.SetVelocity(temp);
				}
			}
		}
		t1.SetPosition((t1v + (components[i].second.GetVelocity() * timeStep)));
	}
}