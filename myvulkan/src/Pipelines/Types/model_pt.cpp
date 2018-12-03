#include "Handlers/Handlers.h"
#include "Devices/logical.h"
#include "Pipelines/Types/model_pt.h"
#include "Pipelines/Parts/shader_stage_pp.h"
#include "Pipelines/Parts/vertex_input_pp.h"
#include "Pipelines/Parts/input_assembly_pp.h"
#include "Pipelines/Parts/extent_pp.h"
#include "Pipelines/Parts/viewport_pp.h"
#include "Pipelines/Parts/rect_pp.h"
#include "Pipelines/Parts/viewport_state_pp.h"
#include "Pipelines/Parts/rasterizer_pp.h"
#include "Pipelines/Parts/multisampling_pp.h"
#include "Pipelines/Parts/depth_stencil_pp.h"
#include "Pipelines/Parts/color_blend_attachment_pp.h"
#include "Pipelines/Parts/color_blend_pp.h"
#include "Pipelines/Parts/layout_pp.h"

namespace pipelines {

	namespace types {

		namespace model {

			void create(Pipeline& pipeline, VkDescriptorSetLayout DSL) {
				VkGraphicsPipelineCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

				VkPipelineShaderStageCreateInfo vertSSI = pipelines::parts::shader_stage::create("shaders/models/vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
				VkPipelineShaderStageCreateInfo fragSSI = pipelines::parts::shader_stage::create("shaders/models/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
				VkPipelineShaderStageCreateInfo pStages[] = { vertSSI, fragSSI };
				info.stageCount = 2;
				info.pStages = pStages;

				info.pVertexInputState = &pipelines::parts::vertex_input::create(1, &vertices::V_P_N_C_TXC_TXI::description.binding, vertices::V_P_N_C_TXC_TXI::description.attributes.size(), vertices::V_P_N_C_TXC_TXI::description.attributes.data());

				info.pInputAssemblyState = &pipelines::parts::input_assembly::create();

				VkViewport viewport = pipelines::parts::viewport::create((float)presentation->swapchain.extent.width, (float)presentation->swapchain.extent.height);
				VkRect2D scissor = pipelines::parts::rect::create(presentation->swapchain.extent);
				info.pViewportState = &pipelines::parts::viewport_state::create(&viewport, &scissor);

				info.pRasterizationState = &pipelines::parts::rasterizer::create(VK_CULL_MODE_BACK_BIT, VK_FALSE);

				info.pMultisampleState = &pipelines::parts::multisampling::create();

				info.pDepthStencilState = &pipelines::parts::depth_stencil::create();

				VkPipelineColorBlendAttachmentState attachment = pipelines::parts::color_blend_attachment::create();
				info.pColorBlendState = &pipelines::parts::color_blend::create(1, &attachment);

				std::vector<VkDescriptorSetLayout> layouts = { lightsHandler->dsl_Attrs, DSL };
				VkPipelineLayoutCreateInfo pipelineLayoutInfo = pipelines::parts::layout::create(layouts.size(), layouts.data());
				if (vkCreatePipelineLayout(devices::logical::dev, &pipelineLayoutInfo, nullptr, &pipeline.layout) != VK_SUCCESS) {
					throw std::runtime_error("failed to create pipeline layout!");
				}
				info.layout = pipeline.layout;

				info.renderPass = renderPassHandler->renderPassRegular;

				info.subpass = 0;

				info.basePipelineHandle = VK_NULL_HANDLE;

				if (vkCreateGraphicsPipelines(devices::logical::dev, VK_NULL_HANDLE, 1, &info, nullptr, &pipeline.handle) != VK_SUCCESS) {
					throw std::runtime_error("failed to create models pipeline!");
				}

				vkDestroyShaderModule(devices::logical::dev, vertSSI.module, nullptr);
				vkDestroyShaderModule(devices::logical::dev, fragSSI.module, nullptr);
			}

		}

	}

}