#include "Camera.h"

#include <iostream>


const glm::vec3 Camera::DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 10.0f);


Camera::Camera(Window& window)
	:worldUp(0.f, 1.f, 0.f),
	worldForward(0.f, 0.f, -1.f),
	worldRight(1.f, 0.f, 0.f),
	cameraForward(0.f, 0.f, -1.f),
	cameraRight(1.f, 0.f, 0.f),
	cameraUp(0.f, 1.f, 0.f),
	position(DEFAULT_POSITION),
	xRotation(0.0f),
	yRotation(0.0f),
	speed(5.f),
	sensitivity(.05f),
	orthographic(true),
	window(window)
{

}

glm::mat4 Camera::GetViewingMatrix() {
	return glm::lookAt(position, position + cameraForward, worldUp);
}

glm::mat4 Camera::GetProjectionMatrix() {
	if (orthographic) {
		glm::mat4 proj(glm::ortho(left, right, bottom, top, -500.f, 500.f));
		proj[1][1] *= -1;
		return proj;
	}
	else {
		glm::mat4 proj(glm::perspective(glm::radians(45.0f), (static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight())), 0.1f, 500.0f));
		proj[1][1] *= -1;
		return proj;
	}
}

void Camera::Update(float timeStep, InputManager& inputManager) {
		
	//poll translations
	/*if (inputManager.GetKeyState(GLFW_KEY_A) == GLFW_PRESS) {
		position -= (cameraRight * speed * timeStep);
	}
	if (inputManager.GetKeyState(GLFW_KEY_D) == GLFW_PRESS) {
		position += (cameraRight * speed * timeStep);
	}
	if (inputManager.GetKeyState(GLFW_KEY_W) == GLFW_PRESS) {
		position += (cameraForward * speed * timeStep);
	}
	if (inputManager.GetKeyState(GLFW_KEY_S) == GLFW_PRESS) {
		position -= (cameraForward * speed * timeStep);
	}
	if (inputManager.GetKeyState(GLFW_KEY_Q) == GLFW_PRESS) {
		position += (cameraUp * speed * timeStep);
	}
	if (inputManager.GetKeyState(GLFW_KEY_E) == GLFW_PRESS) {
		position -= (cameraUp * speed * timeStep);
	}*/

	//poll rotations
	//xRotation += sensitivity * -inputManager.GetDeltaMouseX();
	//yRotation += -inputManager.GetDeltaMouseY() * sensitivity;
	//yRotation = glm::clamp(yRotation, -CLAMP_Y, CLAMP_Y);

	cameraForward = glm::rotate(worldForward, glm::radians(xRotation), worldUp);
	glm::vec3 newRightAxis(glm::rotate(worldRight, glm::radians(xRotation), worldUp));
	cameraForward = glm::rotate(cameraForward, glm::radians(yRotation), newRightAxis);
	cameraRight = glm::normalize(glm::cross(cameraForward, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraForward));

}

	
bool Camera::SphereAxisAllignedCuboidCollision(glm::vec3 spherePos, float sphereRadius, glm::vec3 cuboidOriginPos, glm::vec3 cuboidSideLengths) {
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
