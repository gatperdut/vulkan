#pragma once


#include <vulkan/vulkan.h>


namespace devices {

	namespace logical {
	
		extern VkDevice dev;

		void create();

		void destroy();

	}
	
}