#include "Handlers/CapabilitiesHandler.h"
#include "Handlers/Handlers.h"


CapabilitiesHandler::CapabilitiesHandler() {

}


CapabilitiesHandler::~CapabilitiesHandler() {

}


SwapChainSupportDetails CapabilitiesHandler::querySwapChainSupport(VkPhysicalDevice device) {
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, window::surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, window::surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, window::surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, window::surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, window::surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}