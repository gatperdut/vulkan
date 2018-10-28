#include "CapabilitiesHandler.h"
#include "Handlers.h"


CapabilitiesHandler::CapabilitiesHandler() {

}


CapabilitiesHandler::~CapabilitiesHandler() {

}


SwapChainSupportDetails CapabilitiesHandler::querySwapChainSupport(VkPhysicalDevice device) {
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, windowHandler->surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowHandler->surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowHandler->surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowHandler->surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowHandler->surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}