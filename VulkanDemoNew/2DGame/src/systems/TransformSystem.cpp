#include "TransformSystem.h"

TransformSystem::TransformSystem()
	:transformComponents{}
{



}
void TransformSystem::AddTransformComponent(uint32_t id) {

	transformComponents.emplace_back(std::make_pair(id, TransformComponent{}));


}