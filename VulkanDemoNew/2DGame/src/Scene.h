#pragma once
#include "Entity.h"
#include "systems/TransformSystem.h"
#include "systems/SpriteSystem.h"
#include "Renderer.h"


#include <unordered_map>
#include <iostream>
#include <chrono>
class Scene
{
private:

	std::unordered_map<uint32_t, Entity> entities;
	//counter for creating unique entities
	uint32_t nextID;

public:
	//order of declaration matters: transform above all other systems
	TransformSystem transformSystem;
	SpriteSystem spriteSystem;
	
	Scene();

	Entity& GetEntity(uint32_t id);
	
	uint32_t AddEntity();
	void Init(Renderer* renderer);
	void MainLoop();
};

