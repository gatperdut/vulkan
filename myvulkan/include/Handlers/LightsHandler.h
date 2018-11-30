#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "Lights/LightPipeline.h"
#include "Lights/LightDataUBOs.h"
#include "Lights/LightSpaceUBOs.h"
#include "Lights/Light.h"
#include "Shadows/ShadowPipeline.h"


class LightsHandler {
private:
public:
	LightsHandler();
	~LightsHandler();

	void add(glm::vec3, glm::vec3);
	void createPipelines();
	VkDescriptorSetLayoutBinding createDescriptorSetLayoutModelBinding();
	void createDescriptorSetLayoutData();
	void createDescriptorSetLayoutModel();
	void createDescriptorSetLayoutSpace();
	void createDescriptorSetLayoutSingleSpace();
	void createDescriptorSetsSingleSpace();
	void createDescriptorSetsData();
	void createDescriptorSetsModel();
	void createDescriptorSetsSpace();
	void createUBOs();
	void updateUBOs(uint32_t);


	LightDataUBOs* lightDataUBOs;
	LightSpaceUBOs* lightSpaceUBOs;
	std::vector<Light*> lights;

	VkDescriptorSetLayout descriptorSetLayoutData;
	VkDescriptorSetLayout descriptorSetLayoutModel;
	VkDescriptorSetLayout descriptorSetLayoutSpace;
	VkDescriptorSetLayout descriptorSetLayoutSingleSpace;
	std::vector<VkDescriptorSet> descriptorSetsData;
	std::vector<VkDescriptorSet> descriptorSetsSpace;

	LightPipeline* lightPipeline;
	ShadowPipeline* shadowPipeline;
};
