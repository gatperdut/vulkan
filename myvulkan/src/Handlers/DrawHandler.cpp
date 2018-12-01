#include "Handlers/Handlers.h"
#include "Handlers/DrawHandler.h"


DrawHandler::DrawHandler() {

}


DrawHandler::~DrawHandler() {

}


void DrawHandler::drawFrame() {
	vkWaitForFences(devicesHandler->device, 1, &synchrosHandler->inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(devicesHandler->device, presentation->swapchain.handle, std::numeric_limits<uint64_t>::max(), synchrosHandler->imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	lightsHandler->updateUs(imageIndex);
	modelsHandler->updateUBOs(imageIndex);

	VkSubmitInfo submitInfoShadow = {};
	submitInfoShadow.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphoresShadow[] = { synchrosHandler->imageAvailableSemaphores[currentFrame] };
	submitInfoShadow.waitSemaphoreCount = 1;
	submitInfoShadow.pWaitSemaphores = waitSemaphoresShadow;
	VkSemaphore signalSemaphoresShadow[] = { synchrosHandler->shadowSemaphores[currentFrame] };
	submitInfoShadow.signalSemaphoreCount = 1;
	submitInfoShadow.pSignalSemaphores = signalSemaphoresShadow;
	VkPipelineStageFlags waitStagesShadow[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfoShadow.pWaitDstStageMask = waitStagesShadow;
	submitInfoShadow.commandBufferCount = 1;
	submitInfoShadow.pCommandBuffers = &commandBuffersHandler->commandBuffersShadow[imageIndex];
	vkResetFences(devicesHandler->device, 1, &synchrosHandler->inFlightFences[currentFrame]);
	VkResult resu = vkQueueSubmit(queuesHandler->graphicsQueue, 1, &submitInfoShadow, synchrosHandler->inFlightFences[currentFrame]);
	if (resu != VK_SUCCESS) {
		throw std::runtime_error("failed to submit shadow draw command buffer!");
	}

	VkSubmitInfo submitInfoRegular = {};
	submitInfoRegular.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphoresRegular[] = { synchrosHandler->shadowSemaphores[currentFrame] };
	submitInfoRegular.waitSemaphoreCount = 1;
	submitInfoRegular.pWaitSemaphores = waitSemaphoresRegular;
	VkSemaphore signalSemaphoresRegular[] = { synchrosHandler->renderFinishedSemaphores[currentFrame] };
	submitInfoRegular.signalSemaphoreCount = 1;
	submitInfoRegular.pSignalSemaphores = signalSemaphoresRegular;
	VkPipelineStageFlags waitStagesRegular[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfoRegular.pWaitDstStageMask = waitStagesRegular;
	submitInfoRegular.commandBufferCount = 1;
	submitInfoRegular.pCommandBuffers = &commandBuffersHandler->commandBuffersRegular[imageIndex];

	VkResult res = vkQueueSubmit(queuesHandler->graphicsQueue, 1, &submitInfoRegular, VK_NULL_HANDLE);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to submit regular draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphoresRegular;
	VkSwapchainKHR swapchains[] = { presentation->swapchain.handle };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapchains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	if (vkQueuePresentKHR(queuesHandler->presentQueue, &presentInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % synchrosHandler->MAX_FRAMES_IN_FLIGHT;
}
