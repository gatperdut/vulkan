#pragma once


#include "Handlers/Handlers.h"
#include "DSets/alloc_ds.h"
#include "DSets/light_ds.h"
#include "Writes/create_w.h"
#include "Writes/Info/create_wi.h"
#include "Lights/light_model_ubo.h"
#include "Lights/light_space_ubo.h"


namespace dsets {

	namespace lights {

		void PVM(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, LightModelUBOs* lightModelUBOs) {
			VkDescriptorSetAllocateInfo alloc = {};
			dsets::alloc(&alloc, layout);

			for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
				if (vkAllocateDescriptorSets(devicesHandler->device, &alloc, &dsets[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate UB descriptor set!");
				}

				VkDescriptorBufferInfo bInfo = {};
				writes::info::buffer(&bInfo, lightModelUBOs->buffers[i], 0, sizeof(LightModelUBO));


				VkWriteDescriptorSet write = {};
				writes::buffer(&write, dsets[i], 0, 0, 1, &bInfo);

				vkUpdateDescriptorSets(devicesHandler->device, 1, &write, 0, nullptr);
			}
		}

		void singlePV(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, SingleLightSpaceUBOs* singleLightSpaceUBOs) {
			VkDescriptorSetAllocateInfo alloc = {};
			dsets::alloc(&alloc, layout);

			for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
				if (vkAllocateDescriptorSets(devicesHandler->device, &alloc, &dsets[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate Space descriptor set!");
				}

				VkDescriptorBufferInfo bInfo = {};
				writes::info::buffer(&bInfo, singleLightSpaceUBOs->buffers[i], 0, sizeof(LightSpaceUBO));

				VkWriteDescriptorSet write = {};
				writes::buffer(&write, dsets[i], 0, 0, 1, &bInfo);

				vkUpdateDescriptorSets(devicesHandler->device, 1, &write, 0, nullptr);
			}
		}

		void multiPV(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, LightSpaceUBOs* lightSpaceUBOs) {
			VkDescriptorSetAllocateInfo alloc = {};
			dsets::alloc(&alloc, layout);

			for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
				if (vkAllocateDescriptorSets(devicesHandler->device, &alloc, &dsets[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate Space descriptor set!");
				}

				VkDescriptorBufferInfo bInfo = {};
				writes::info::buffer(&bInfo, lightSpaceUBOs->buffers[i], 0, lightsHandler->lights.size() * sizeof(LightSpaceUBO));

				VkWriteDescriptorSet write = {};
				writes::buffer(&write, dsets[i], 0, 0, 1, &bInfo);

				vkUpdateDescriptorSets(devicesHandler->device, 1, &write, 0, nullptr);
			}
		}

	}

}
