#include <cstdlib>

#include "Handlers/Handlers.h"
#include "Handlers/InputHandler.h"
#include "buffers.h"

InputHandler::InputHandler() {

}


InputHandler::~InputHandler() {

}


void mouseCallbackHelper(GLFWwindow* window, double xpos, double ypos) {
	inputHandler->processMouseMovement((float)xpos, (float)ypos);
};

void keyCallbackHelper(GLFWwindow* window, int key, int scancode, int action, int mods) {
	inputHandler->processKey(key, action);
}

void InputHandler::processMovement() {
	if (glfwGetKey(windowHandler->window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraHandler->pos += cameraHandler->speed * cameraHandler->front;
	}
	if (glfwGetKey(windowHandler->window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraHandler->pos -= cameraHandler->speed * cameraHandler->front;
	}
	if (glfwGetKey(windowHandler->window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraHandler->pos -= glm::normalize(glm::cross(cameraHandler->front, cameraHandler->up)) * cameraHandler->speed;
	}
	if (glfwGetKey(windowHandler->window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraHandler->pos += glm::normalize(glm::cross(cameraHandler->front, cameraHandler->up)) * cameraHandler->speed;
	}
}

void InputHandler::processKey(int key, int action) {
	if (action != GLFW_PRESS) {
		return;
	}
	if (key == GLFW_KEY_G) {
		modelsHandler->load("models/angel.obj", glm::vec3(std::rand() % 30 - 15, std::rand() % 30 - 15, 0.0f));
		//modelsHandler->load("models/angel.obj", glm::vec3(0.0f, 0.0f, 0.0f));
		buffersHandler->createVertexAndIndexBuffer();
		uniformsHandler->createUniformBuffer();
		descriptorsHandler->createDescriptorPool();
		descriptorsHandler->createDescriptorSets();
		commandBuffersHandler->createCommandBuffers();
	}
}

void InputHandler::processMouseMovement(float xpos, float ypos) {
	if (!mouseUsed) {
		lastX = xpos;
		lastY = ypos;
		mouseUsed = true;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	xoffset *= cameraHandler->sensitivity;
	yoffset *= cameraHandler->sensitivity;

	cameraHandler->yaw += xoffset;
	cameraHandler->pitch += yoffset;

	if (cameraHandler->pitch > 89.0f) {
		cameraHandler->pitch = 89.0f;
	}
	if (cameraHandler->pitch < -89.0f) {
		cameraHandler->pitch = -89.0f;
	}

	cameraHandler->updateCameraVectors();
}
