#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class CameraHandler {
public:
	CameraHandler(glm::vec3 position = glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), float yaw = -90.0f, float pitch = 0.0f);
	~CameraHandler();

	glm::mat4 viewMatrix();
	void updateCameraVectors();


	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	const float sensitivity = 0.1f;

	float speed = 0.005f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
};

