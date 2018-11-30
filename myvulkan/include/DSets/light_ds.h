#pragma once


#include <vector>
#include <vulkan/vulkan.h>

#include "Lights/LightModelUBOs.h"
#include "Lights/SingleLightSpaceUBOs.h"
#include "Lights/LightSpaceUBOs.h"

namespace dsets {

	namespace lights {

		void Attrs_PVM(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform Attrs_u, LightModelUBOs* lightModelUBOs);

		void singlePV(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, SingleLightSpaceUBOs* singleLightSpaceUBOs);

		void multiPV(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, LightSpaceUBOs* lightSpaceUBOs);
	
		void Attrs_PV_Depth(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform& attrs_u, LightSpaceUBOs* lightSpaceUBOs);

	}

}
