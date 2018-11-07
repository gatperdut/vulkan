#include <array>

#include "Handlers/Handlers.h"
#include "Handlers/DescriptorsHandler.h"
#include "uniform_buffer_objects.h"

DescriptorsHandler::DescriptorsHandler() {

}


DescriptorsHandler::~DescriptorsHandler() {
	vkDestroyDescriptorPool(devicesHandler->device, descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutUB, nullptr);
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutCIS, nullptr);
}


void DescriptorsHandler::createDescriptorPool() {
	std::array<VkDescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(swapchainHandler->images.size() * modelsHandler->models.size() * 2);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(swapchainHandler->images.size() * modelsHandler->models.size() * 2);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(swapchainHandler->images.size() * modelsHandler->models.size() * 2);

	if (vkCreateDescriptorPool(devicesHandler->device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void DescriptorsHandler::resetDescriptorPool() {
	vkResetDescriptorPool(devicesHandler->device, descriptorPool, 0);
}


std::vector<VkDescriptorSetLayout> DescriptorsHandler::descriptorSetlayouts() {
	std::vector<VkDescriptorSetLayout> layouts = { descriptorSetLayoutUB, descriptorSetLayoutCIS };
	return layouts;
}

void DescriptorsHandler::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	uboLayoutBinding.descriptorCount = 1; // array of UBOs
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 0;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfoUB = {};
	layoutInfoUB.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfoUB.bindingCount = 1;
	layoutInfoUB.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(devicesHandler->device, &layoutInfoUB, nullptr, &descriptorSetLayoutUB) != VK_SUCCESS) {
		throw std::runtime_error("failed to create UB descriptor set layout!");
	}

	VkDescriptorSetLayoutCreateInfo layoutInfoCIS = {};
	layoutInfoCIS.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfoCIS.bindingCount = 1;
	layoutInfoCIS.pBindings = &samplerLayoutBinding;

	if (vkCreateDescriptorSetLayout(devicesHandler->device, &layoutInfoCIS, nullptr, &descriptorSetLayoutCIS) != VK_SUCCESS) {
		throw std::runtime_error("failed to create CIS descriptor set layout!");
	}
}

void DescriptorsHandler::createDescriptorSets() {
	std::vector<VkDescriptorSetLayout> layoutsUB(1, descriptorSetLayoutUB);
	std::vector<VkDescriptorSetLayout> layoutsCIS(modelsHandler->models.size(), descriptorSetLayoutCIS);

	VkDescriptorSetAllocateInfo allocInfoUB = {};
	allocInfoUB.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfoUB.descriptorPool = descriptorPool;
	allocInfoUB.descriptorSetCount = 1;
	allocInfoUB.pSetLayouts = layoutsUB.data();

	VkDescriptorSetAllocateInfo allocInfoCIS = {};
	allocInfoCIS.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfoCIS.descriptorPool = descriptorPool;
	allocInfoCIS.descriptorSetCount = static_cast<uint32_t>(modelsHandler->models.size());
	allocInfoCIS.pSetLayouts = layoutsCIS.data();

	descriptorSets.resize(swapchainHandler->images.size());

	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		descriptorSets[i].resize(modelsHandler->models.size() + 1);
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfoUB, &descriptorSets[i][0]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate UB descriptor set!");
		}
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfoCIS, &descriptorSets[i][1]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate CIS descriptor sets!");
		}
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformsHandler->uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWriteUB = {};
		descriptorWriteUB.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteUB.dstSet = descriptorSets[i][0];
		descriptorWriteUB.dstBinding = 0;
		descriptorWriteUB.dstArrayElement = 0;
		descriptorWriteUB.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWriteUB.descriptorCount = 1;
		descriptorWriteUB.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(devicesHandler->device, 1, &descriptorWriteUB, 0, nullptr);

		for (size_t j = 0; j < modelsHandler->models.size(); j++) {
			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = modelsHandler->models[j]->texturesHandler->imageViews[0];
			imageInfo.sampler = modelsHandler->models[j]->texturesHandler->imageSamplers[0];
			
			VkWriteDescriptorSet descriptorWriteCIS = {};
			descriptorWriteCIS.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWriteCIS.dstSet = descriptorSets[i][j + 1];
			descriptorWriteCIS.dstBinding = 0;
			descriptorWriteCIS.dstArrayElement = 0;
			descriptorWriteCIS.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWriteCIS.descriptorCount = 1;
			descriptorWriteCIS.pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(devicesHandler->device, 1, &descriptorWriteCIS, 0, nullptr);
		}
	}
}