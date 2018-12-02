#pragma once


#include <vulkan/vulkan.h>

namespace descriptors {

	namespace pool {

		extern VkDescriptorPool handle;

		void create();

		void destroy();

	}

}
