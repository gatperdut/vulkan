#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "Uniforms/uniform.h"
#include "Lights/LightPipeline.h"
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
	void update_Attrs_u(uint32_t index);

	uniforms::uniform Attrs_u;
	LightSpaceUBOs* lightSpaceUBOs;
	std::vector<Light*> lights;

	VkDescriptorSetLayout descriptorSetLayoutData;
	VkDescriptorSetLayout descriptorSetLayoutModel;
	VkDescriptorSetLayout descriptorSetLayoutSpace;
	VkDescriptorSetLayout descriptorSetLayoutSingleSpace;
	std::vector<VkDescriptorSet> dsets_Attrs_PV_Depth;
	std::vector<VkDescriptorSet> dsets_multiPV;

	LightPipeline* lightPipeline;
	ShadowPipeline* shadowPipeline;
};
