#pragma once


#include <vector>
#include <vulkan/vulkan.h>


namespace instance {

	namespace debug {

		const bool enableVLs = true;

		void setupDebugCallback();

		const std::vector<const char*> VLs = {
			"VK_LAYER_LUNARG_standard_validation"
		};

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT*, void*);

		bool checkVLSupport();

		VkResult CreateDebugUtilsMessengerEXT(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT*, const VkAllocationCallbacks*, VkDebugUtilsMessengerEXT*);

		void DestroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks*);

		extern VkDebugUtilsMessengerEXT callback;
	}
}
