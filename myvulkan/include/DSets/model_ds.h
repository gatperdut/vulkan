#pragma once


#include <vulkan/vulkan.h>

#include "Models/ModelUBOs.h"
#include "Models/ModelMaterials.h"


namespace dsets {

	namespace model {

		void PVM_Materials(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, ModelUBOs* modelUBOs, ModelMaterials* modelMaterials);

		void PVM(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, ModelUBOs* modelUBOs);

	}

}
