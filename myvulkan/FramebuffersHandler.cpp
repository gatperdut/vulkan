#include <array>

#include "Handlers.h"
#include "FramebuffersHandler.h"



FramebuffersHandler::FramebuffersHandler() {

}


FramebuffersHandler::~FramebuffersHandler() {

}


void FramebuffersHandler::createFramebuffers() {
	swapChainFramebuffers.resize(swapchainHandler->imageViews.size());

	for (size_t i = 0; i < swapchainHandler->imageViews.size(); i++) {
		std::array<VkImageView, 2> attachments = {
			swapchainHandler->imageViews[i],
			renderPassHandler->depthImageView
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPassHandler->renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapchainHandler->extent.width;
		framebufferInfo.height = swapchainHandler->extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(devicesHandler->device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}
