#include <array>

#include "Handlers/Handlers.h"
#include "Handlers/CommandBuffersHandler.h"
#include "uniform_buffer_objects.h"


CommandBuffersHandler::CommandBuffersHandler() {

}


CommandBuffersHandler::~CommandBuffersHandler() {

}


void CommandBuffersHandler::internalCreateCommandBuffers(std::vector<VkCommandBuffer>* buffers) {
	(*buffers).resize(framebuffersHandler->swapChainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandsHandler->commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)(*buffers).size();

	if (vkAllocateCommandBuffers(devicesHandler->device, &allocInfo, (*buffers).data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (size_t i = 0; i < (*buffers).size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer((*buffers)[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffers!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPassHandler->renderPass;
		renderPassInfo.framebuffer = framebuffersHandler->swapChainFramebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapchainHandler->extent;

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass((*buffers)[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline((*buffers)[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinesHandler->graphicsPipeline);

		VkBuffer vertexBuffers[] = { buffersHandler->vertexAndIndexBuffer };

		VkDeviceSize total = 0;
		Model* model;
		for (size_t j = 0; j < modelsHandler->models.size(); j++) {
			model = modelsHandler->models[j];

			VkDeviceSize verticesOffsets[] = { total };
			vkCmdBindVertexBuffers((*buffers)[i], 0, 1, vertexBuffers, verticesOffsets);
			VkDeviceSize indicesOffset = total + sizeof(model->vertices[0]) * model->vertices.size();
			vkCmdBindIndexBuffer((*buffers)[i], buffersHandler->vertexAndIndexBuffer, indicesOffset, VK_INDEX_TYPE_UINT32);

			uint32_t dynamicOffset = j * (size_t)devicesHandler->uboAlignment;
			vkCmdBindDescriptorSets((*buffers)[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinesHandler->pipelineLayout, 0, 1, &model->descriptorSets[i], 1, &dynamicOffset);

			vkCmdDrawIndexed((*buffers)[i], static_cast<uint32_t>(model->indices.size()), 1, 0, 0, 0);
			//vkCmdDraw((*buffers)[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);

			total += model->verticesSize() + model->indicesSize();
		}

		vkCmdEndRenderPass((*buffers)[i]);
		if (vkEndCommandBuffer((*buffers)[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}


void CommandBuffersHandler::createCommandBuffers() {
	if (!commandBuffers.size()) {
		internalCreateCommandBuffers(&commandBuffers);
	}
	else {
		std::vector<VkCommandBuffer> newCommandBuffers;
		internalCreateCommandBuffers(&newCommandBuffers);
		vkFreeCommandBuffers(devicesHandler->device, commandsHandler->commandPool, static_cast<uint32_t>(commandBuffers.size()), &commandBuffers[0]);
		commandBuffers = newCommandBuffers;
	}
}
