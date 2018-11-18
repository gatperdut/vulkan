#pragma once


#include <glm/glm.hpp>


struct LightUbo {
	glm::vec3 pos;
	float filler;
	glm::vec3 color;
};
