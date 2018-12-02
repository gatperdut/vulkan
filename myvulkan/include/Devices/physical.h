#pragma once


#include <vulkan/vulkan.h>


namespace devices {

	namespace physical {

		extern VkPhysicalDevice dev;

		void pick();

		bool isSuitable(VkPhysicalDevice device);

		bool checkExtensionSupport(VkPhysicalDevice device);

	}

}