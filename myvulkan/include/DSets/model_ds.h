#pragma once


#include <vector>
#include <vulkan/vulkan.h>

#include "Uniforms/uniform.h"
#include "Models/ModelMaterials.h"


namespace dsets {

	namespace models {

		void PVM_Materials(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform& pvm_u, ModelMaterials* modelMaterials);

		void PVM(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform& model_u);

	}

}
