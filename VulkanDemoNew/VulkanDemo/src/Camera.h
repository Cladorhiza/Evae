#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/transform.hpp"
#include "InputManager.h"
namespace Camera
{
	

	void Update(float timeStep);
	const glm::vec3& getCameraPosition();
	const glm::vec3& getCameraTarget();
	glm::mat4 getViewingMatrix();
	bool SphereAxisAllignedCuboidCollision(glm::vec3 spherePos, float sphereRadius, glm::vec3 cuboidOriginPos, glm::vec3 cuboidSideLengths);
};

