#include "Handlers/Handlers.h"
#include "Devices/logical.h"
#include "Models/ModelPipeline.h"
#include "Pipelines/Parts/vertex_input_p.h"
#include "Pipelines/Parts/input_assembly_p.h"
#include "Pipelines/Parts/extent_p.h"
#include "Pipelines/Parts/viewport_p.h"
#include "Pipelines/Parts/rect_p.h"
#include "Pipelines/Parts/viewport_state_p.h"
#include "Pipelines/Parts/rasterizer_p.h"
#include "Pipelines/Parts/multisampling_p.h"
#include "Pipelines/Parts/depth_stencil_p.h"
#include "Pipelines/Parts/color_blend_attachment_p.h"
#include "Pipelines/Parts/color_blend_p.h"
#include "read_file.h"
#include "Vertices/P_N_C_TXC_TXI_v.h"


ModelPipeline::ModelPipeline() {

}


ModelPipeline::~ModelPipeline() {
	freeResources();
}


void ModelPipeline::freeResources() {
	vkDestroyPipeline(devices::logical::dev, pipeline, nullptr);
	vkDestroyPipelineLayout(devices::logical::dev, layout, nullptr);
}


void ModelPipeline::create(VkDescriptorSetLayout descriptorSetLayout) {
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;

	auto vertShaderCode = readFile("shaders/models/vert.spv");
	auto fragShaderCode = readFile("shaders/models/frag.spv");

	vertShaderModule = shadersHandler->createShaderModule(vertShaderCode);
	fragShaderModule = shadersHandler->createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pSpecializationInfo = nullptr;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pSpecializationInfo = nullptr;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = pipelines::parts::vertex_input::create(1, &vertices::V_P_N_C_TXC_TXI::description.binding, vertices::V_P_N_C_TXC_TXI::description.attributes.size(), vertices::V_P_N_C_TXC_TXI::description.attributes.data());

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = pipelines::parts::input_assembly::create();

	VkViewport viewport = pipelines::parts::viewport::create((float)presentation->swapchain.extent.width, (float)presentation->swapchain.extent.height);
	VkRect2D scissor = pipelines::parts::rect::create(presentation->swapchain.extent);
	VkPipelineViewportStateCreateInfo viewportState = pipelines::parts::viewport_state::create(&viewport, &scissor);

	VkPipelineRasterizationStateCreateInfo rasterizer = pipelines::parts::rasterizer::create(VK_CULL_MODE_BACK_BIT, VK_FALSE);

	VkPipelineMultisampleStateCreateInfo multisampling = pipelines::parts::multisampling::create();

	VkPipelineDepthStencilStateCreateInfo depthStencil = pipelines::parts::depth_stencil::create();

	VkPipelineColorBlendAttachmentState attachment = pipelines::parts::color_blend_attachment::create();

	VkPipelineColorBlendStateCreateInfo colorBlending = pipelines::parts::color_blend::create(1, &attachment);

	std::vector<VkDescriptorSetLayout> layouts = { lightsHandler->dsl_Attrs, descriptorSetLayout };

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = layouts.size();
	pipelineLayoutInfo.pSetLayouts = layouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(devices::logical::dev, &pipelineLayoutInfo, nullptr, &layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = layout;
	pipelineInfo.renderPass = renderPassHandler->renderPassRegular;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(devices::logical::dev, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create models pipeline!");
	}

	vkDestroyShaderModule(devices::logical::dev, fragShaderModule, nullptr);
	vkDestroyShaderModule(devices::logical::dev, vertShaderModule, nullptr);
}
