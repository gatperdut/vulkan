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
	void createDSLs();
	void createDescriptorSetsSingleSpace();
	void createDS_Attrs_PV_Depth();
	void createDescriptorSetsModel();
	void createDS_multiPV();
	void createUBOs();
	void updateUBOs(uint32_t);
	void update_Attrs_u(uint32_t index);

	uniforms::uniform Attrs_u;
	LightSpaceUBOs* lightSpaceUBOs;
	std::vector<Light*> lights;

	VkDescriptorSetLayout descriptorSetLayoutData;
	VkDescriptorSetLayout dsl_Attrs_PVM;
	VkDescriptorSetLayout descriptorSetLayoutSpace;
	VkDescriptorSetLayout descriptorSetLayoutSingleSpace;
	std::vector<VkDescriptorSet> dsets_Attrs_PV_Depth;
	std::vector<VkDescriptorSet> dsets_multiPV;

	LightPipeline* lightPipeline;
	ShadowPipeline* shadowPipeline;
};
