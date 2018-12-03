#pragma once


#include <string>
#include <vector>
#include <vulkan/vulkan.h>


namespace pipelines {

	namespace parts {

		namespace shader_stage {

			VkPipelineShaderStageCreateInfo create(const std::string& filename, VkShaderStageFlagBits stages);

			VkShaderModule createModule(const std::vector<char>& code);

		}

	}

}