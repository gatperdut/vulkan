#pragma once


#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Uniforms/uniform.h"
#include "VBuffers/vbuffer.h"
#include "Vertices/P_v.h"


class Light {
public:
	Light(glm::vec3, glm::vec3);
	~Light();

	glm::vec3 pos;
	glm::vec3 color;
	void loadModel();
	std::vector<vertices::V_P::Data> vertices;
	std::vector<uint32_t> indices;

	void createDS_Attrs_PVM();
	void createDS_PV();
	void createUs();
	void updateUs(uint32_t index);
	void updateU_Attrs(uint32_t index);
	void updateU_PVM(uint32_t index);
	void updateU_PV(uint32_t index);
	void updateProjectionView();


	std::vector<VkDescriptorSet> ds_Attrs_PVM;
	std::vector<VkDescriptorSet> ds_PV;
	vbuffers::vbuffer vb_P;

	uniforms::uniform u_PV;
	uniforms::uniform u_Attrs;
	uniforms::uniform u_PVM;

	glm::mat4 projectionView;
};
