#include <array>

#include "Handlers/Handlers.h"
#include "Devices/logical.h"
#include "Descriptors/pool.h"


namespace descriptors {

	namespace pool {

		void create() {
			std::array<VkDescriptorPoolSize, 2> sizes = {};
			sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			sizes[0].descriptorCount = static_cast<uint32_t>(presentation->swapchain.images.size() * modelsHandler->models.size() * 2 + 40);
			sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			sizes[1].descriptorCount = static_cast<uint32_t>(presentation->swapchain.images.size() * modelsHandler->models.size() * 2 + 61);

			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
			poolInfo.pPoolSizes = sizes.data();
			poolInfo.maxSets = static_cast<uint32_t>(presentation->swapchain.images.size() * modelsHandler->models.size() * 2 + 30);

			if (vkCreateDescriptorPool(devices::logical::dev, &poolInfo, nullptr, &handle) != VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor pool!");
			}
		}

		void destroy() {
			vkDestroyDescriptorPool(devices::logical::dev, handle, nullptr);
		}

	}

}
