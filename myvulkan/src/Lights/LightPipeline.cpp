#include "Handlers/Handlers.h"
#include "Devices/logical.h"
#include "Pipelines/Parts/shader_stage_p.h"
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
#include "Pipelines/Parts/layout_p.h"
#include "Lights/LightPipeline.h"


LightPipeline::LightPipeline() {

}

LightPipeline::~LightPipeline() {
	freeResources();
}


void LightPipeline::freeResources() {
	vkDestroyPipeline(devices::logical::dev, pipeline, nullptr);
	vkDestroyPipelineLayout(devices::logical::dev, layout, nullptr);
}


void LightPipeline::create(VkDescriptorSetLayout dsl_Attrs_PVM) {
	VkGraphicsPipelineCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	VkPipelineShaderStageCreateInfo vertSSI = pipelines::parts::shader_stage::create("shaders/lights/vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	VkPipelineShaderStageCreateInfo fragSSI = pipelines::parts::shader_stage::create("shaders/lights/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertSSI, fragSSI };
	info.stageCount = 2;
	info.pStages = shaderStages;

	info.pVertexInputState = &pipelines::parts::vertex_input::create(1, &vertices::V_P::description.binding, vertices::V_P::description.attributes.size(), vertices::V_P::description.attributes.data());

	info.pInputAssemblyState = &pipelines::parts::input_assembly::create();

	VkViewport viewport = pipelines::parts::viewport::create((float)presentation->swapchain.extent.width, (float)presentation->swapchain.extent.height);
	VkRect2D scissor = pipelines::parts::rect::create(presentation->swapchain.extent);
	info.pViewportState = &pipelines::parts::viewport_state::create(&viewport, &scissor);

	info.pRasterizationState = &pipelines::parts::rasterizer::create(VK_CULL_MODE_BACK_BIT, VK_FALSE);

	info.pMultisampleState = &pipelines::parts::multisampling::create();

	info.pDepthStencilState = &pipelines::parts::depth_stencil::create();

	VkPipelineColorBlendAttachmentState attachment = pipelines::parts::color_blend_attachment::create();
	info.pColorBlendState = &pipelines::parts::color_blend::create(1, &attachment);

	std::vector<VkDescriptorSetLayout> layouts = { dsl_Attrs_PVM };
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = pipelines::parts::layout::create(layouts.size(), layouts.data());
	if (vkCreatePipelineLayout(devices::logical::dev, &pipelineLayoutInfo, nullptr, &layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
	info.layout = layout;

	info.renderPass = renderPassHandler->renderPassRegular;

	info.subpass = 0;

	info.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(devices::logical::dev, VK_NULL_HANDLE, 1, &info, nullptr, &pipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create lights pipeline!");
	}

	vkDestroyShaderModule(devices::logical::dev, vertSSI.module, nullptr);
	vkDestroyShaderModule(devices::logical::dev, fragSSI.module, nullptr);
}
