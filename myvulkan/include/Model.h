#pragma once


#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "vertex.h"


class Model {
public:
	Model(std::string path, glm::vec3 pos);
	~Model();

	VkDeviceSize verticesSize();
	VkDeviceSize indicesSize();


	glm::vec3 pos;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

