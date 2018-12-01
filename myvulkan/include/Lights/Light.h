#pragma once


#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Uniforms/uniform.h"
#include "Lights/LightVBOs.h"
#include "Lights/light_vertex.h"


class Light {
public:
	Light(glm::vec3, glm::vec3);
	~Light();

	void loadModel();
	void createDescriptorSetsModel();
	void createDescriptorSetsSpace();
	void createUBOs();
	void updateUBOs(uint32_t index);
	void update_Attrs_u(uint32_t index);
	void update_PVM_u(uint32_t index);
	void update_PV_u(uint32_t index);
	void updateProjectionView();

	std::vector<LightVertex> vertices;
	std::vector<uint32_t> indices;

	glm::vec3 pos;
	glm::vec3 color;
	std::vector<VkDescriptorSet> dsets_Attrs_PVM;
	std::vector<VkDescriptorSet> dsets_singlePV;
	LightVBOs* lightVBOs;

	uniforms::uniform PV_u;
	uniforms::uniform Attrs_u;
	uniforms::uniform PVM_u;

	glm::mat4 projectionView;
};
