#pragma once

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/transform.hpp"

#include "InputManager.h"

class Camera
{
	
private:
	const float CLAMP_Y = 89.95F;
	static const glm::vec3 DEFAULT_POSITION; //default position.
	glm::vec3 worldUp;
	glm::vec3 worldForward;
	glm::vec3 worldRight;
	glm::vec3 cameraForward; //camera relative forward axis
	glm::vec3 cameraRight; //camera relative right axis
	glm::vec3 cameraUp; //camera relative up axis
	glm::vec3 position;
	float xRotation = 0.0f;
	float yRotation = 0.0f;

	float speed = 5.f;
	float sensitivity = .05f;

public:

	Camera();
	void Update(float timeStep, InputManager& inputManager);
	const glm::vec3& getCameraPosition() { return position; }
	const glm::vec3& getCameraTarget() { return cameraForward; }
	glm::mat4 getViewingMatrix();
	bool SphereAxisAllignedCuboidCollision(glm::vec3 spherePos, float sphereRadius, glm::vec3 cuboidOriginPos, glm::vec3 cuboidSideLengths);
};

