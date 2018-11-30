#pragma once


#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Uniforms/uniform.h"
#include "Lights/LightModelUBOs.h"
#include "Lights/SingleLightSpaceUBOs.h"
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
	void updateUBOs(uint32_t);
	void update_Attrs_u(uint32_t);
	void updateProjectionView();

	std::vector<LightVertex> vertices;
	std::vector<uint32_t> indices;

	glm::vec3 pos;
	glm::vec3 color;
	std::vector<VkDescriptorSet> dsets_Attrs_PVM;
	std::vector<VkDescriptorSet> dsets_singlePV;
	LightModelUBOs* lightModelUBOs;
	SingleLightSpaceUBOs* singleLightSpaceUBOs;
	LightVBOs* lightVBOs;

	uniforms::uniform Attrs_u;

	glm::mat4 projectionView;
};
