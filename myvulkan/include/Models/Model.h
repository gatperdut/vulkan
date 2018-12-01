#pragma once


#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Uniforms/uniform.h"
#include "Models/ModelMaterials.h"
#include "VBuffers/vbuffer.h"
#include "Models/ModelPipeline.h"
#include "Vertices/vertex_P_N_C_TXC_TXI.h"
#include "Vertices/vertex_P.h"


class Model {
private:
	void loadModel();
	size_t verticesSize();
	size_t indicesSize();
	size_t totalSize();

public:
	Model(std::string, std::string, glm::vec3, glm::vec3);
	~Model();

	std::vector<vertices::V_P_N_C_TXC_TXI> vertices;
	std::vector<vertices::V_P> verticesShadow;
	std::vector<uint32_t> indices;
	glm::vec3 pos;
	glm::vec3 scale;

	void createUs();
	void updateU_PVM(uint32_t);
	void createDSL_PVM_Materials();

	void createDS_PVM_Materials();
	void createDS_PVM();
	void createPipeline();


	std::string path;
	std::string filename;

	uniforms::uniform u_PVM;
	ModelMaterials* modelMaterials;
	vbuffers::vbuffer vb_P_N_C_TXC_TXI;
	vbuffers::vbuffer vb_P;
	ModelPipeline* modelPipeline;

	std::vector<VkDescriptorSet> ds_PVM_Materials;
	VkDescriptorSetLayout dsl_PVM_Materials;

	std::vector<VkDescriptorSet> ds_PVM;
};

