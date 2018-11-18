#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "Lights/LightPipeline.h"
#include "Lights/LightDataUBOs.h"
#include "Lights/LightModelUBOs.h"
#include "Lights/Light.h"


class LightsHandler {
private:
public:
	LightsHandler();
	~LightsHandler();

	void add(glm::vec3, glm::vec3);
	void createPipeline();
	void createDescriptorSetLayoutData();
	void createDescriptorSetsData();
	void createDescriptorSetLayoutModel();
	VkDescriptorSetLayoutBinding createDescriptorSetLayoutModelBinding();
	void createDescriptorSetsModel();
	void createDataUBOs();
	void createModelUBOs();
	void updateUBOs(uint32_t);


	LightDataUBOs* lightDataUBOs;
	std::vector<Light*> lights;
	VkDescriptorSetLayout descriptorSetLayoutData;
	std::vector<VkDescriptorSet> descriptorSetsData;
	VkDescriptorSetLayout descriptorSetLayoutModel;

	LightPipeline* lightPipeline;
};
