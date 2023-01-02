#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "../components/TransformComponent.h"

#include <vector>
#include <unordered_map>
class TransformSystem
{
private:

	std::unordered_map<uint32_t,TransformComponent> transformComponents;


public:

	TransformSystem();
	void AddTransformComponent(uint32_t id);
	TransformComponent& GetTransformComponent(uint32_t id) { return transformComponents[id]; }

};

