#pragma once


#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Models/ModeLMaterials.h"
#include "Models/ModelUBOs.h"
#include "Models/ModelVBOs.h"
#include "Models/ShadowVBOs.h"
#include "Models/ModelPipeline.h"
#include "Shadows/shadow_vertex.h"


class Model {
private:
	void loadModel();
	VkDeviceSize verticesSize();
	VkDeviceSize indicesSize();
	VkDeviceSize totalSize();

public:
	Model(std::string, std::string, glm::vec3, glm::vec3);
	~Model();

	void createUBOs();
	void updateUBO(uint32_t);
	void createDescriptorSetLayout();

	void createDescriptorSets();
	void createDescriptorSetsMatrices();
	void createPipeline();

	std::vector<ModelVertex> vertices;
	std::vector<ShadowVertex> verticesShadow;
	std::vector<uint32_t> indices;
	glm::vec3 pos;
	glm::vec3 scale;

	std::string path;
	std::string filename;

	ModelUBOs* modelUBOs;
	ModelMaterials* modelMaterials;
	ModelVBOs* modelVBOs;
	ShadowVBOs* shadowVBOs;
	ModelPipeline* modelPipeline;

	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;

	std::vector<VkDescriptorSet> descriptorSetsMatrices;
};

