#include "SpriteSystem.h"


SpriteSystem::SpriteSystem(TransformSystem* transformSystem) 
	:renderer(nullptr), transformSystem(transformSystem)
{
	spriteComponents.reserve(500);
}

void SpriteSystem::AddSpriteComponent(uint32_t id, std::string texPath) {

	spriteComponents.emplace_back(std::make_pair(id, SpriteComponent{ texPath }));

	renderer->AddSprite(texPath);

}

void SpriteSystem::Update() {

	std::vector<glm::mat4> spriteTransforms;
	spriteTransforms.reserve(spriteComponents.size());

	for (const std::pair<uint32_t, SpriteComponent>& sc : spriteComponents) {
		spriteTransforms.emplace_back(transformSystem->GetTransformComponent(sc.first).GetTransform());
	}

	renderer->UpdateSpriteModelMatrixes(spriteTransforms);

}