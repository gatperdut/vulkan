#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace camera {

	const float speed = 0.03f;



	const float sensitivity = 0.1f;

	const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	struct Data {
		glm::vec3 pos;
		float yaw;
		float pitch;
		glm::vec3 front;
		glm::vec3 up;
	};

	extern Data data;

	void create(glm::vec3 pos);

	void update();

	glm::mat4 P();

	glm::mat4 V();

}
