#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "../components/TransformComponent.h"

#include <vector>
class TransformSystem
{
protected:

	std::vector<std::pair<uint32_t,TransformComponent>> transformComponents;


public:

	TransformSystem();
	void AddTransformComponent(uint32_t id);

};

