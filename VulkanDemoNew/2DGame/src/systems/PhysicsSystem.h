#pragma once


#include "../components/PhysicsComponent2D.h"
#include "../components/TransformComponent.h"
#include "../Collisions.h"
#include "../systems/TransformSystem.h"
#include "../Scene.h"


#include <cstdint>
#include <vector>
#include <iostream>
class PhysicsSystem
{
private:


	//number of physics calculations per second
	uint32_t tickRate;
	TransformSystem* transformSystem;
	Scene* scene;
	//collection of physics components
	std::vector<std::pair<uint32_t, PhysicsComponent2D>> components;

public:

	PhysicsSystem(TransformSystem* ts, Scene* scene);

	PhysicsComponent2D& GetComponent(uint32_t id);
	uint32_t GetTickRate() { return tickRate; }
	void SetTickRate(uint32_t tr) { tickRate = tr; }

	void AddPhysicsComponent(uint32_t id);
	void Update(float timeStep);


};

