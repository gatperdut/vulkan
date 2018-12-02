#include <GLFW/glfw3.h>

#include "Instance/instance.h"
#include "Instance/debug.h"


namespace instance {

	void create() {
		if (debug::enableVLs && !debug::checkVLSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = reqExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		if (debug::enableVLs) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(debug::VLs.size());
			createInfo.ppEnabledLayerNames = debug::VLs.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}
	}

	std::vector<const char*> reqExtensions() {
		uint32_t reqExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&reqExtensionCount);

		std::vector<const char*> reqExtensions(glfwExtensions, glfwExtensions + reqExtensionCount);

		if (debug::enableVLs) {
			reqExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		uint32_t availExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &availExtensionCount, nullptr);
		std::vector<VkExtensionProperties> availExtensions(availExtensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &availExtensionCount, availExtensions.data());

		return reqExtensions;
	}

	void destroy() {
		if (debug::enableVLs) {
			debug::DestroyDebugUtilsMessengerEXT(instance, debug::callback, nullptr);
		}

		vkDestroyInstance(instance, nullptr);
	}

}