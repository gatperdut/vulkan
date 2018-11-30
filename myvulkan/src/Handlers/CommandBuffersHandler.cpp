#include <array>

#include "Handlers/Handlers.h"
#include "Handlers/CommandBuffersHandler.h"
#include "Descriptors/model_d.h"


CommandBuffersHandler::CommandBuffersHandler() {

}


CommandBuffersHandler::~CommandBuffersHandler() {
	vkFreeCommandBuffers(devicesHandler->device, commandsHandler->commandPool, static_cast<uint32_t>(commandBuffersRegular.size()), commandBuffersRegular.data());
}


void CommandBuffersHandler::createCommandBuffersRegular() {
	commandBuffersRegular.resize(presentation->swapchain.framebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandsHandler->commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffersRegular.size();

	if (vkAllocateCommandBuffers(devicesHandler->device, &allocInfo, commandBuffersRegular.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate regular command buffers!");
	}

	for (size_t i = 0; i < commandBuffersRegular.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(commandBuffersRegular[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording regular command buffers!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPassHandler->renderPassRegular;
		renderPassInfo.framebuffer = presentation->swapchain.framebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = presentation->swapchain.extent;

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffersRegular[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		Light* light;
		vkCmdBindPipeline(commandBuffersRegular[i], VK_PIPELINE_BIND_POINT_GRAPHICS, lightsHandler->lightPipeline->pipeline);
		for (size_t j = 0; j < lightsHandler->lights.size(); j++) {
			light = lightsHandler->lights[j];

			std::vector<VkDeviceSize> offsets = { 0 };
			vkCmdBindVertexBuffers(commandBuffersRegular[i], 0, 1, &light->lightVBOs->vertexBuffer, offsets.data());
			vkCmdBindIndexBuffer(commandBuffersRegular[i], light->lightVBOs->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

			std::vector<uint32_t> dynamicOffsets = { 0 };
			std::vector<VkDescriptorSet> descriptorSets = { light->dsets_PVM[i] };
			vkCmdBindDescriptorSets(commandBuffersRegular[i], VK_PIPELINE_BIND_POINT_GRAPHICS, lightsHandler->lightPipeline->layout, 0, descriptorSets.size(), descriptorSets.data(), dynamicOffsets.size(), dynamicOffsets.data());



			vkCmdDrawIndexed(commandBuffersRegular[i], static_cast<uint32_t>(light->indices.size()), 1, 0, 0, 0);
		}

		Model* model;
		for (size_t j = 0; j < modelsHandler->models.size(); j++) {
			model = modelsHandler->models[j];

			vkCmdBindPipeline(commandBuffersRegular[i], VK_PIPELINE_BIND_POINT_GRAPHICS, model->modelPipeline->pipeline);


			std::vector<VkDeviceSize> offsets = { 0 };
			vkCmdBindVertexBuffers(commandBuffersRegular[i], 0, 1, &model->modelVBOs->vertexBuffer, offsets.data());
			vkCmdBindIndexBuffer(commandBuffersRegular[i], model->modelVBOs->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

			std::vector<uint32_t> dynamicOffsets = { 0, 0, 0 };
			std::vector<VkDescriptorSet> descriptorSets = { lightsHandler->dsets_Attrs_PV_Depth[i], model->dsets_PVM_Materials[i] };
			vkCmdBindDescriptorSets(commandBuffersRegular[i], VK_PIPELINE_BIND_POINT_GRAPHICS, model->modelPipeline->layout, 0, descriptorSets.size(), descriptorSets.data(), dynamicOffsets.size(), dynamicOffsets.data());

			vkCmdDrawIndexed(commandBuffersRegular[i], static_cast<uint32_t>(model->indices.size()), 1, 0, 0, 0);
		}

		vkCmdEndRenderPass(commandBuffersRegular[i]);
		if (vkEndCommandBuffer(commandBuffersRegular[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record regular command buffer!");
		}
	}
}

void CommandBuffersHandler::createCommandBuffersShadow() {
	commandBuffersShadow.resize(presentation->swapchain.framebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandsHandler->commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffersShadow.size();

	if (vkAllocateCommandBuffers(devicesHandler->device, &allocInfo, commandBuffersShadow.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate shadow command buffers!");
	}

	VkClearValue clearValues[1];
	clearValues[0].depthStencil = { 1.0f, 0 };

	for (size_t i = 0; i < commandBuffersShadow.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(commandBuffersShadow[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording shadow command buffers!");
		}


		VkViewport viewport = {};
		viewport.width = 1024.0f;
		viewport.height = 1024.0f;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffersShadow[i], 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.extent.width = 1024;
		scissor.extent.height = 1024;
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		vkCmdSetScissor(commandBuffersShadow[i], 0, 1, &scissor);

		vkCmdSetDepthBias(commandBuffersShadow[i], 1.25f, 0.0f, 1.75f); // needed?

		for (size_t j = 0; j < lightsHandler->lights.size(); j++) {
			VkRenderPassBeginInfo renderPassBeginInfo = {};
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.renderPass = renderPassHandler->renderPassShadow;
			renderPassBeginInfo.framebuffer = presentation->shadow.framebuffers[j];
			renderPassBeginInfo.renderArea.offset.x = 0;
			renderPassBeginInfo.renderArea.offset.y = 0;
			renderPassBeginInfo.renderArea.extent.width = 1024;
			renderPassBeginInfo.renderArea.extent.height = 1024;
			renderPassBeginInfo.clearValueCount = 2;
			renderPassBeginInfo.pClearValues = clearValues;

			vkCmdBeginRenderPass(commandBuffersShadow[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffersShadow[i], VK_PIPELINE_BIND_POINT_GRAPHICS, lightsHandler->shadowPipeline->pipeline);
			Model* model;
			for (size_t k = 0; k < modelsHandler->models.size(); k++) {
				model = modelsHandler->models[k];
				std::vector<VkDeviceSize> offsets = { 0 };
				vkCmdBindVertexBuffers(commandBuffersShadow[i], 0, 1, &model->shadowVBOs->vertexBuffer, offsets.data());
				vkCmdBindIndexBuffer(commandBuffersShadow[i], model->shadowVBOs->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

				std::vector<uint32_t> dynamicOffsets = { 0, 0 };
				std::vector<VkDescriptorSet> descriptorSets = { lightsHandler->lights[j]->dsets_singlePV[i], model->dsets_PVM[i] };
				vkCmdBindDescriptorSets(commandBuffersShadow[i], VK_PIPELINE_BIND_POINT_GRAPHICS, lightsHandler->shadowPipeline->layout, 0, descriptorSets.size(), descriptorSets.data(), dynamicOffsets.size(), dynamicOffsets.data());

				vkCmdDrawIndexed(commandBuffersShadow[i], static_cast<uint32_t>(model->indices.size()), 1, 0, 0, 0);
			}

			vkCmdEndRenderPass(commandBuffersShadow[i]);
		}

		if (vkEndCommandBuffer(commandBuffersShadow[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record shadow command buffer!");
		}
	}
}
