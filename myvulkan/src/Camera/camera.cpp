#include "Handlers/Handlers.h"
#include "Camera/camera.h"


namespace camera {

	void create(glm::vec3 pos) {
		data.front = glm::vec3(0.0f, 0.0f, -1.0f);
		data.up = glm::vec3(0.0f, 1.0f, 0.0f);
		data.pos = pos;
		data.yaw = -90.0f;
		data.pitch = 0.0f;
	}

	void update() {
		glm::vec3 newFront;
		newFront.x = cos(glm::radians(data.yaw)) * cos(glm::radians(data.pitch));
		newFront.y = sin(glm::radians(data.pitch));
		newFront.z = sin(glm::radians(data.yaw)) * cos(glm::radians(data.pitch));
		
		data.front = glm::normalize(newFront);

		glm::vec3 right = glm::normalize(glm::cross(data.front, worldUp));
		data.up = glm::normalize(glm::cross(right, data.front));
	}

	glm::mat4 V() {
		return glm::lookAt(data.pos, data.pos + data.front, data.up);
	}

	glm::mat4 P() {
		return glm::perspective(glm::radians(45.0f), presentation->swapchain.extent.width / (float)presentation->swapchain.extent.height, 0.1f, 100.0f);
	}

}
