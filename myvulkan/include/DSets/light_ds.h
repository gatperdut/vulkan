#pragma once


#include <vector>
#include <vulkan/vulkan.h>

namespace dsets {

	namespace lights {

		void Attrs_PVM(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform Attrs_u, uniforms::uniform PVM_u);

		void singlePV(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform PV_u);

		void multiPV(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform PV_u);
	
		void Attrs_PV_Depth(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform& Attrs_u, uniforms::uniform PV_u);

	}

}
