#include <array>

#include "Handlers/Handlers.h"
#include "Handlers/DescriptorsHandler.h"
#include "uniform_buffer_objects.h"

DescriptorsHandler::DescriptorsHandler() {

}


DescriptorsHandler::~DescriptorsHandler() {
	vkDestroyDescriptorPool(devicesHandler->device, descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutUB, nullptr);
}


void DescriptorsHandler::createDescriptorPool() {
	std::array<VkDescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(swapchainHandler->images.size() * modelsHandler->models.size() * 10);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(swapchainHandler->images.size() * modelsHandler->models.size() * 10);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(swapchainHandler->images.size() * modelsHandler->models.size() * 10);

	if (vkCreateDescriptorPool(devicesHandler->device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void DescriptorsHandler::resetDescriptorPool() {
	vkResetDescriptorPool(devicesHandler->device, descriptorPool, 0);
}

void DescriptorsHandler::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	uboLayoutBinding.descriptorCount = 1; // array of UBOs
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfoUB = {};
	layoutInfoUB.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfoUB.bindingCount = 1;
	layoutInfoUB.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(devicesHandler->device, &layoutInfoUB, nullptr, &descriptorSetLayoutUB) != VK_SUCCESS) {
		throw std::runtime_error("failed to create UB descriptor set layout!");
	}
}

void DescriptorsHandler::createDescriptorSets() {
	std::vector<VkDescriptorSetLayout> layoutsUB(1, descriptorSetLayoutUB);

	VkDescriptorSetAllocateInfo allocInfoUB = {};
	allocInfoUB.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfoUB.descriptorPool = descriptorPool;
	allocInfoUB.descriptorSetCount = 1;
	allocInfoUB.pSetLayouts = layoutsUB.data();

	descriptorSets.resize(swapchainHandler->images.size());

	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfoUB, &descriptorSets[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate UB descriptor set!");
		}
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformsHandler->uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWriteUB = {};
		descriptorWriteUB.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteUB.dstSet = descriptorSets[i];
		descriptorWriteUB.dstBinding = 0;
		descriptorWriteUB.dstArrayElement = 0;
		descriptorWriteUB.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWriteUB.descriptorCount = 1;
		descriptorWriteUB.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(devicesHandler->device, 1, &descriptorWriteUB, 0, nullptr);
	}
}