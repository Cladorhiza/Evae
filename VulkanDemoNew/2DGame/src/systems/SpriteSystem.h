#pragma once
#include "../Components/SpriteComponent.h"
#include "../systems/TransformSystem.h"
#include "Renderer.h"

#include <vector>
class SpriteSystem
{

private:

	Renderer* renderer;
	TransformSystem* transformSystem;

	std::vector<std::pair<uint32_t, SpriteComponent>> spriteComponents;

public:

	SpriteSystem(TransformSystem* transformSystem);

	void SetRenderer(Renderer* rend) { renderer = rend; }
	void AddSpriteComponent(uint32_t id, std::string texPath);
	void Update();
};

