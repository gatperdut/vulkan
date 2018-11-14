#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "Model.h"
#include "vertex.h"


class ModelsHandler {
private:
public:
	ModelsHandler();
	~ModelsHandler();

	void load(std::string, std::string, glm::vec3, glm::vec3);
	VkDeviceSize verticesSize();
	VkDeviceSize indicesSize();
	void createDescriptorSets();
	void createUBOs();

	void createDescriptorSetLayouts();

	std::vector<Model*> models;
};
