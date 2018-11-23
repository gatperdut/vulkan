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
	VkDescriptorSetLayoutBinding createDescriptorSetLayoutSpaceBinding(uint32_t);
	void createDescriptorSetLayoutData();
	void createDescriptorSetLayoutModel();
	void createDescriptorSetLayoutSpace();
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
	std::vector<VkDescriptorSet> descriptorSetsData;
	std::vector<VkDescriptorSet> descriptorSetsSpace;

	LightPipeline* lightPipeline;
	ShadowPipeline* shadowPipeline;
};
