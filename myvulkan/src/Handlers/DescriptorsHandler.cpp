#include <array>

#include "Handlers/Handlers.h"
#include "Handlers/DescriptorsHandler.h"
#include "uniform_buffer_objects.h"

DescriptorsHandler::DescriptorsHandler() {

}


DescriptorsHandler::~DescriptorsHandler() {
	vkDestroyDescriptorPool(devicesHandler->device, descriptorPool, nullptr);
}


void DescriptorsHandler::createDescriptorPool() {
	std::array<VkDescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(swapchainHandler->images.size() * modelsHandler->models.size() * 2 + 20);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(swapchainHandler->images.size() * modelsHandler->models.size() * 2 + 20);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(swapchainHandler->images.size() * modelsHandler->models.size() * 2 + 20);

	if (vkCreateDescriptorPool(devicesHandler->device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void DescriptorsHandler::resetDescriptorPool() {
	vkResetDescriptorPool(devicesHandler->device, descriptorPool, 0);
}
