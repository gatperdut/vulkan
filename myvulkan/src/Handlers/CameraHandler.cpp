#include "Handlers/Handlers.h"
#include "Handlers/CameraHandler.h"


CameraHandler::CameraHandler(glm::vec3 pos, glm::vec3 up, glm::vec3 front, float yaw, float pitch) {
	this->pos = pos;
	this->up = up;
	this->front = front;
	this->yaw = yaw;
	this->pitch = pitch;
}


CameraHandler::~CameraHandler() {

}


glm::mat4 CameraHandler::viewMatrix() {
	return glm::lookAt(pos, pos + front, up);
}

glm::mat4 CameraHandler::projMatrix() {
	return glm::perspective(glm::radians(45.0f), presentation->swapchain.extent.width / (float)presentation->swapchain.extent.height, 0.1f, 100.0f);
}

void CameraHandler::updateCameraVectors() {
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);
	
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

