#pragma once


#include <vulkan/vulkan.h>


namespace writes {

	namespace info {

		void buffer(VkDescriptorBufferInfo* info, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range);

		void image(VkDescriptorImageInfo* info, VkImageLayout layout, VkImageView view,  VkSampler sampler);

	}

}
