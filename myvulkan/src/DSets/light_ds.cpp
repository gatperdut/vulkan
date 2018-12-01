#pragma once


#include "Handlers/Handlers.h"
#include "DSets/alloc_ds.h"
#include "DSets/light_ds.h"
#include "Writes/create_w.h"
#include "Writes/Info/create_wi.h"
#include "Descriptors/light_d.h"


namespace dsets {

	namespace lights {

		void Attrs_PVM(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform u_Attrs, uniforms::uniform u_PVM) {
			VkDescriptorSetAllocateInfo alloc = {};
			dsets::alloc(&alloc, layout);

			for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
				if (vkAllocateDescriptorSets(devicesHandler->device, &alloc, &dsets[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate UB descriptor set!");
				}

				VkDescriptorBufferInfo bAttrs = {};
				writes::info::buffer(&bAttrs, u_Attrs.buffers[i], 0, sizeof(descriptors::lights::Attrs));

				VkDescriptorBufferInfo bPVM = {};
				writes::info::buffer(&bPVM, u_PVM.buffers[i], 0, sizeof(descriptors::lights::PVM));

				std::vector<VkWriteDescriptorSet> writes = {};
				writes.resize(2);

				writes::buffer(&writes[0], dsets[i], 0, 0, 1, &bAttrs);
				writes::buffer(&writes[1], dsets[i], 1, 0, 1, &bPVM);

				vkUpdateDescriptorSets(devicesHandler->device, 2, writes.data(), 0, nullptr);
			}
		}

		void singlePV(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform u_PV) {
			VkDescriptorSetAllocateInfo alloc = {};
			dsets::alloc(&alloc, layout);

			for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
				if (vkAllocateDescriptorSets(devicesHandler->device, &alloc, &dsets[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate Space descriptor set!");
				}

				VkDescriptorBufferInfo bInfo = {};
				writes::info::buffer(&bInfo, u_PV.buffers[i], 0, sizeof(descriptors::lights::PV));

				VkWriteDescriptorSet write = {};
				writes::buffer(&write, dsets[i], 0, 0, 1, &bInfo);

				vkUpdateDescriptorSets(devicesHandler->device, 1, &write, 0, nullptr);
			}
		}

		void multiPV(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform u_PV) {
			VkDescriptorSetAllocateInfo alloc = {};
			dsets::alloc(&alloc, layout);

			for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
				if (vkAllocateDescriptorSets(devicesHandler->device, &alloc, &dsets[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate Space descriptor set!");
				}

				VkDescriptorBufferInfo bInfo = {};
				writes::info::buffer(&bInfo, u_PV.buffers[i], 0, lightsHandler->lights.size() * sizeof(descriptors::lights::PV));

				VkWriteDescriptorSet write = {};
				writes::buffer(&write, dsets[i], 0, 0, 1, &bInfo);

				vkUpdateDescriptorSets(devicesHandler->device, 1, &write, 0, nullptr);
			}
		}

		void Attrs_PV_Depth(std::vector<VkDescriptorSet>& dsets, VkDescriptorSetLayout* layout, uniforms::uniform& u_Attrs, uniforms::uniform u_PV) {
			VkDescriptorSetAllocateInfo alloc = {};
			dsets::alloc(&alloc, layout);

			uint32_t nLights = lightsHandler->lights.size();

			for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
				if (vkAllocateDescriptorSets(devicesHandler->device, &alloc, &dsets[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate UB descriptor set!");
				}


				std::vector<VkWriteDescriptorSet> writes;
				writes.resize(3);

				VkDescriptorBufferInfo bAttrsInfo = {};
				writes::info::buffer(&bAttrsInfo, u_Attrs.buffers[i], 0, nLights * sizeof(descriptors::lights::Attrs));

				VkDescriptorBufferInfo bPVInfo = {};
				writes::info::buffer(&bPVInfo, u_PV.buffers[i], 0, nLights * sizeof(descriptors::lights::PV));

				std::vector<VkDescriptorImageInfo> iInfos = {};
				iInfos.resize(nLights);
				for (size_t j = 0; j < nLights; j++) {
					writes::info::image(&iInfos[j], VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL, presentation->shadow.imageViews[j], presentation->shadow.samplers[j]);
				}

				writes::buffer(&writes[0], dsets[i], 0, 0, 1, &bAttrsInfo);
				writes::buffer(&writes[1], dsets[i], 1, 0, 1, &bPVInfo);
				writes::image(&writes[2], dsets[i], 2, 0, iInfos.size(), iInfos.data());

				vkUpdateDescriptorSets(devicesHandler->device, writes.size(), writes.data(), 0, nullptr);
			}
		}

	}

}
