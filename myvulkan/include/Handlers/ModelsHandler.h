#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "Models/Model.h"
#include "Models/model_vertex.h"


class ModelsHandler {
private:
public:
	ModelsHandler();
	~ModelsHandler();

	void load(std::string, std::string, glm::vec3, glm::vec3);
	VkDeviceSize verticesSize();
	VkDeviceSize indicesSize();
	void createDescriptorSets();
	void createPipelines();
	void destroyPipelines();
	void createUBOs();
	void updateUBOs(uint32_t);

	void createDescriptorSetLayouts();
	std::vector<VkDescriptorSetLayout> getDescriptorSetLayouts();

	std::vector<Model*> models;
};
