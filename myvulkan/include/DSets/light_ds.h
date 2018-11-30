#pragma once


#include <vector>
#include <vulkan/vulkan.h>

#include "Lights/LightModelUBOs.h"
#include "Lights/SingleLightSpaceUBOs.h"
#include "Lights/LightSpaceUBOs.h"

namespace dsets {

	namespace lights {

		void PVM(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, LightModelUBOs* lightModelUBOs);

		void singlePV(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, SingleLightSpaceUBOs* singleLightSpaceUBOs);

		void multiPV(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, LightSpaceUBOs* lightSpaceUBOs);
	}

}
