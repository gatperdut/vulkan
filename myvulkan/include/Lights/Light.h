#pragma once


#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

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
	void updateProjectionView();

	std::vector<LightVertex> vertices;
	std::vector<uint32_t> indices;

	glm::vec3 pos;
	glm::vec3 color;
	std::vector<VkDescriptorSet> descriptorSetsModel;
	std::vector<VkDescriptorSet> descriptorSetsSpace;
	LightModelUBOs* lightModelUBOs;
	SingleLightSpaceUBOs* singleLightSpaceUBOs;
	LightVBOs* lightVBOs;

	glm::mat4 projectionView;
};
