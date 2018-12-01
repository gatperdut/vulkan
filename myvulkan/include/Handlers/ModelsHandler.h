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
	void createDSs();
	void createDSL_PVM();
	void createPipelines();
	void destroyPipelines();
	void createUs();
	void updateUBOs(uint32_t);


	void createDSLs();
	std::vector<VkDescriptorSetLayout> getDescriptorSetLayouts();

	VkDescriptorSetLayout descriptorSetLayoutMatrices;

	std::vector<Model*> models;
};
