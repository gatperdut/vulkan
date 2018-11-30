#pragma once


#include <vector>
#include <vulkan/vulkan.h>

#include "Models/ModelUBOs.h"
#include "Models/ModelMaterials.h"


namespace dsets {

	namespace models {

		void PVM_Materials(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, ModelUBOs* modelUBOs, ModelMaterials* modelMaterials);

		void PVM(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, ModelUBOs* modelUBOs);

	}

}
