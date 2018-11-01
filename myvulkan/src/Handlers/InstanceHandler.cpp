#include "Handlers/InstanceHandler.h"
#include "Handlers/Handlers.h"


InstanceHandler::InstanceHandler() {

}


InstanceHandler::~InstanceHandler() {
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, callback, nullptr);
	}

	vkDestroyInstance(instance, nullptr);
}


void InstanceHandler::createInstance() {
	if (enableValidationLayers && !checkValidationLayerSupport()) {
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

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
}


std::vector<const char*> InstanceHandler::getRequiredExtensions() {
	uint32_t reqExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&reqExtensionCount);

	std::vector<const char*> reqExtensions(glfwExtensions, glfwExtensions + reqExtensionCount);

	if (enableValidationLayers) {
		reqExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	uint32_t availExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &availExtensionCount, nullptr);
	std::vector<VkExtensionProperties> availExtensions(availExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &availExtensionCount, availExtensions.data());

	std::cout << "available extensions:\n*********************" << std::endl;
	for (const auto& extension : availExtensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
	}

	std::cout << "required extensions:\n*************************" << std::endl;
	for (const auto& extension : reqExtensions) {
		std::cout << "\t" << extension << std::endl;
	}

	return reqExtensions;
}


bool InstanceHandler::checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	std::cout << "available validation layers:\n*****************************" << std::endl;
	for (const auto& layer : availableLayers) {
		std::cout << "\t" << layer.layerName << std::endl;
	}

	std::cout << "required validation layers:\n*****************************" << std::endl;
	for (const auto& layer : validationLayers) {
		std::cout << "\t" << layer << std::endl;
	}

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}
		if (!layerFound) {
			return false;
		}
	}

	return true;
}


void InstanceHandler::setupDebugCallback() {
	if (!enableValidationLayers) {
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug callback!");
	}
}


VkResult InstanceHandler::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}


// severity: 4096(0x1000) error, 256(0x100) warning, 16(0x10) info (unused), 1(0x1) verbose
// type:     1(0x1) general, 2(0x2) validation, 4(0x4) performance
VKAPI_ATTR VkBool32 VKAPI_CALL InstanceHandler::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	std::cerr << "[S" << messageSeverity << "]" << "[T" << messageType << "]" << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void InstanceHandler::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}
