#pragma once


#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Lights/LightModelUBOs.h"
#include "Lights/LightVBOs.h"
#include "Lights/light_vertex.h"


class Light {
public:
	Light(glm::vec3, glm::vec3);
	~Light();

	void loadModel();
	void createDescriptorSetsModel();
	void createModelUBOs();
	void updateModelUBO(uint32_t);

	std::vector<LightVertex> vertices;
	std::vector<uint32_t> indices;

	glm::vec3 pos;
	glm::vec3 color;
	std::vector<VkDescriptorSet> descriptorSetsModel;
	LightModelUBOs* lightModelUBOs;
	LightVBOs* lightVBOs;
};
