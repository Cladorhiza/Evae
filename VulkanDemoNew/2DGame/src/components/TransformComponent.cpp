#include "TransformComponent.h"

TransformComponent::TransformComponent() 
	:transform(1.0f), position(0.0f), rotation(0.0f), scale(1.0)
{

}

void TransformComponent::SetPosition(const glm::vec3& newPosition) {

	position = newPosition;
	UpdateTransform();

}

void TransformComponent::SetRotation(const glm::vec3& newRotation) {

	rotation = newRotation;
	UpdateTransform();
}

void TransformComponent::SetScale(const glm::vec3& newScale) {

	scale = newScale;
	UpdateTransform();
}
void TransformComponent::UpdateTransform() {


	transform = glm::translate(glm::mat4(1.0f), position);
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, scale);
}