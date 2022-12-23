#pragma once
#include "Entity.h"
#include "systems/TransformSystem.h"
#include <unordered_map>
#include <iostream>
class Scene
{
private:

	std::unordered_map<uint32_t, Entity> entities;
	uint32_t nextID;
	TransformSystem transformSystem;
public:

	Scene();
	Entity& GetEntity(uint32_t id);
	uint32_t AddEntity();
};

