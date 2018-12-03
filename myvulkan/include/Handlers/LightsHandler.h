#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "Uniforms/uniform.h"
#include "Pipelines/pipeline_p.h"
#include "Lights/Light.h"
#include "Shadows/ShadowPipeline.h"


class LightsHandler {
private:
public:
	LightsHandler();
	~LightsHandler();

	std::vector<Light*> lights;
	void add(glm::vec3, glm::vec3);

	void createPipelines();
	pipelines::Pipeline pipeline;
	ShadowPipeline* shadowPipeline;
	
	void createDSLs();
	VkDescriptorSetLayout dsl_Attrs;
	VkDescriptorSetLayout dsl_Attrs_PVM;
	VkDescriptorSetLayout dsl_PVs;
	VkDescriptorSetLayout dsl_PV;

	void createDSs();
	void createDS_PV();
	void createDS_Attrs_PV_Depth();
	void createDS_PVM();
	void createDS_PVs();
	std::vector<VkDescriptorSet> ds_Attrs_PV_Depth;
	std::vector<VkDescriptorSet> ds_PVs;


	void createUs();
	void updateUs(uint32_t);
	void updateU_Attrs(uint32_t index);
	void updateU_PV(uint32_t index);
	uniforms::uniform u_Attrs;
	uniforms::uniform u_PV;
};
