#include "Pipelines/Parts/rasterizer_pp.h"


namespace pipelines {

	namespace parts {

		namespace rasterizer {

			VkPipelineRasterizationStateCreateInfo create(VkCullModeFlags cullMode, VkBool32 depthBiasEnable) {
				VkPipelineRasterizationStateCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
				info.depthClampEnable = VK_FALSE;
				info.rasterizerDiscardEnable = VK_FALSE;
				info.polygonMode = VK_POLYGON_MODE_FILL;
				info.lineWidth = 1.0f;
				info.cullMode = cullMode;
				info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
				info.depthBiasEnable = depthBiasEnable;

				return info;
			}

		}

	}

}