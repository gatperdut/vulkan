#pragma once


#include <vulkan/vulkan.h>
#include <glm/glm.hpp>


class Light {
public:
	Light(glm::vec3);
	~Light();

	glm::vec3 pos;
};
