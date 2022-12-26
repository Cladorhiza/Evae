#include "Camera.h"
#include <iostream>
namespace Camera {

	const float CLAMP_Y = 89.95F;
	const glm::vec3 DEFAULT_POSITION(0.0f, 0.0f, 10.0f); //default position.
	glm::vec3 worldUp(0.f, 1.f, 0.f); 
	glm::vec3 worldForward(0.f, 0.f, -1.f);
	glm::vec3 worldRight(1.f, 0.f, 0.f);
	glm::vec3 cameraForward(worldForward); //camera relative forward axis
	glm::vec3 cameraRight(glm::cross(cameraForward, worldUp)); //camera relative right axis
	glm::vec3 cameraUp(glm::cross(cameraRight, cameraForward)); //camera relative up axis
	glm::vec3 position(DEFAULT_POSITION);
	float xRotation = 0.0f;
	float yRotation = 0.0f;

	float speed = 5.f;
	float sensitivity = .05f;


	const glm::vec3& getCameraPosition() { return position; }
	const glm::vec3& getCameraTarget() { return cameraForward; }

	glm::mat4 getViewingMatrix() {
		return glm::lookAt(position, position + cameraForward, worldUp);
	}

	void Update(float timeStep) {
		
		//poll translations
		if (InputManager::GetKeyState(GLFW_KEY_A) == GLFW_PRESS) {
			position -= (cameraRight * speed * timeStep);
		}
		if (InputManager::GetKeyState(GLFW_KEY_D) == GLFW_PRESS) {
			position += (cameraRight * speed * timeStep);
		}
		if (InputManager::GetKeyState(GLFW_KEY_W) == GLFW_PRESS) {
			position += (cameraForward * speed * timeStep);
		}
		if (InputManager::GetKeyState(GLFW_KEY_S) == GLFW_PRESS) {
			position -= (cameraForward * speed * timeStep);
		}
		if (InputManager::GetKeyState(GLFW_KEY_Q) == GLFW_PRESS) {
			position += (cameraUp * speed * timeStep);
		}
		if (InputManager::GetKeyState(GLFW_KEY_E) == GLFW_PRESS) {
			position -= (cameraUp * speed * timeStep);
		}

		//poll rotations
		xRotation += sensitivity * -InputManager::GetDeltaMouseX();
		yRotation += -InputManager::GetDeltaMouseY() * sensitivity;
		yRotation = glm::clamp(yRotation, -CLAMP_Y, CLAMP_Y);

		cameraForward = glm::rotate(worldForward, glm::radians(xRotation), worldUp);
		glm::vec3 newRightAxis(glm::rotate(worldRight, glm::radians(xRotation), worldUp));
		cameraForward = glm::rotate(cameraForward, glm::radians(yRotation), newRightAxis);
		cameraRight = glm::normalize(glm::cross(cameraForward, worldUp));
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraForward));

		//std::cout << "cameraForward: " << cameraForward.x << ", " << cameraForward.y << ", " << cameraForward.z << std::endl;
		//std::cout << "cameraRight: " << cameraRight.x << ", " << cameraRight.y << ", " << cameraRight.z << std::endl;
		//std::cout << "cameraUp: " << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z << std::endl;
	}

	
	bool SphereAxisAllignedCuboidCollision(glm::vec3 spherePos, float sphereRadius, glm::vec3 cuboidOriginPos, glm::vec3 cuboidSideLengths) {
		spherePos += 0.5f;

		glm::vec3 positiveComparisonPoint(cuboidOriginPos + (cuboidSideLengths / 2.f));
		glm::vec3 negativeComparisonPoint(cuboidOriginPos - (cuboidSideLengths / 2.f));

		glm::vec3 positivePointToSphere(spherePos - positiveComparisonPoint);
		glm::vec3 negativePointToSphere(spherePos - negativeComparisonPoint);


		if (positivePointToSphere.x > sphereRadius)return false;
		if (positivePointToSphere.y > sphereRadius)return false;
		if (positivePointToSphere.z > sphereRadius)return false;

		if (negativePointToSphere.x <= sphereRadius)return false;
		if (negativePointToSphere.y <= sphereRadius)return false;
		if (negativePointToSphere.z <= sphereRadius)return false;

		return true;
	}

}