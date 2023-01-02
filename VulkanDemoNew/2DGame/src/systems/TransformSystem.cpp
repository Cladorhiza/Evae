#include "TransformSystem.h"

TransformSystem::TransformSystem()
	:transformComponents{}
{
	transformComponents.reserve(500);


}
void TransformSystem::AddTransformComponent(uint32_t id) {

	transformComponents.emplace(std::piecewise_construct,std::forward_as_tuple(id), std::forward_as_tuple());
}