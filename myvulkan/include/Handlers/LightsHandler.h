#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "Lights/LightUBOs.h"
#include "Lights/Light.h"


class LightsHandler {
private:
public:
	LightsHandler();
	~LightsHandler();

	void add(glm::vec3);
	void createDescriptorSetLayout();
	void createDescriptorSets();
	void createUBOs();
	void updateUBO(uint32_t);


	LightUBOs* lightUboHandler;
	std::vector<Light*> lights;
	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;
};
