#pragma once


#include "Handlers/Handlers.h"
#include "DSets/alloc_ds.h"
#include "DSets/model_ds.h"
#include "Writes/create_w.h"
#include "Writes/Info/create_wi.h"
#include "Descriptors/model_d.h"


namespace dsets {

	namespace models {

		void PVM_Materials(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform& u_PVM, ModelMaterials* modelMaterials) {
			VkDescriptorSetAllocateInfo alloc = {};
			dsets::alloc(&alloc, layout);

			for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
				if (vkAllocateDescriptorSets(devicesHandler->device, &alloc, &dsets[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate UB descriptor set!");
				}

				VkDescriptorBufferInfo bInfo = {};
				writes::info::buffer(&bInfo, u_PVM.buffers[i], 0, sizeof(descriptors::models::PVM));

				std::vector<VkDescriptorImageInfo> iInfos;
				iInfos.resize(modelMaterials->filepaths.size());
				for (size_t j = 0; j < modelMaterials->filepaths.size(); j++) {
					writes::info::image(&iInfos[j], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, modelMaterials->imageViews[j], modelMaterials->samplers[j]);
				}

				std::vector<VkWriteDescriptorSet> writes;
				writes.resize(2);

				writes::buffer(&writes[0], dsets[i], 0, 0, 1, &bInfo);
				writes::image(&writes[1], dsets[i], 1, 0, iInfos.size(), iInfos.data());

				vkUpdateDescriptorSets(devicesHandler->device, writes.size(), writes.data(), 0, nullptr);
			}
		}

		void PVM(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform& u_PVM) {
			VkDescriptorSetAllocateInfo alloc = {};
			dsets::alloc(&alloc, layout);

			for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
				if (vkAllocateDescriptorSets(devicesHandler->device, &alloc, &dsets[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate UB descriptor set!");
				}

				VkDescriptorBufferInfo bInfo = {};
				writes::info::buffer(&bInfo, u_PVM.buffers[i], 0, sizeof(descriptors::models::PVM));
				
				VkWriteDescriptorSet write = {};
				writes::buffer(&write, dsets[i], 0, 0, 1, &bInfo);

				vkUpdateDescriptorSets(devicesHandler->device, 1, &write, 0, nullptr);
			}
		}

	}

}
