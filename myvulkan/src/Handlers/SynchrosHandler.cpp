#include "Handlers/Handlers.h"
#include "Handlers/SynchrosHandler.h"


SynchrosHandler::SynchrosHandler() {

}


SynchrosHandler::~SynchrosHandler() {
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(devicesHandler->device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(devicesHandler->device, imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(devicesHandler->device, shadowSemaphores[i], nullptr);
		vkDestroyFence(devicesHandler->device, inFlightFences[i], nullptr);
	}
}

void SynchrosHandler::createSyncObjects() {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	shadowSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo  semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(devicesHandler->device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(devicesHandler->device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(devicesHandler->device, &semaphoreInfo, nullptr, &shadowSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(devicesHandler->device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}