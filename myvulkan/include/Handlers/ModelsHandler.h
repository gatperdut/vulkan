#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "Model.h"
#include "vertex.h"


class ModelsHandler {
public:
	ModelsHandler();
	~ModelsHandler();

	void load(std::string path, glm::vec3 pos);
	VkDeviceSize verticesSize();
	VkDeviceSize indicesSize();


	std::vector<Model*> models;
};
