#pragma once


#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Uniforms/uniform.h"
#include "Models/ModeLMaterials.h"
#include "Models/ModelVBOs.h"
#include "Models/ShadowVBOs.h"
#include "Models/ModelPipeline.h"
#include "Shadows/shadow_vertex.h"


class Model {
private:
	void loadModel();
	size_t verticesSize();
	size_t indicesSize();
	size_t totalSize();

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

	uniforms::uniform model_u;
	ModelMaterials* modelMaterials;
	ModelVBOs* modelVBOs;
	ShadowVBOs* shadowVBOs;
	ModelPipeline* modelPipeline;

	std::vector<VkDescriptorSet> dsets_PVM_Materials;
	VkDescriptorSetLayout descriptorSetLayout;

	std::vector<VkDescriptorSet> dsets_PVM;
};

