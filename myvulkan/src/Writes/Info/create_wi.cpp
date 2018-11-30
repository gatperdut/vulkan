#include "Writes/Info/create_wi.h"


namespace writes {

	namespace info {

		void buffer(VkDescriptorBufferInfo* info, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range) {
			info->buffer = buffer;
			info->offset = offset;
			info->range = range;
		}

		void image(VkDescriptorImageInfo* info, VkImageLayout layout, VkImageView view, VkSampler sampler) {
			info->imageLayout = layout;
			info->imageView = view;
			info->sampler = sampler;
		}

	}

}
