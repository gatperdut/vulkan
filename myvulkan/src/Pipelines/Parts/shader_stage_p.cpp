#include "Pipelines/Parts/shader_stage_p.h"
#include "Devices/logical.h"
#include "read_file.h"

namespace pipelines {

	namespace parts {

		namespace shader_stage {

			VkPipelineShaderStageCreateInfo create(const std::string& filename, VkShaderStageFlagBits stages) {
				std::vector<char> code = readFile(filename);

				VkShaderModule module = createModule(code);

				VkPipelineShaderStageCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				info.stage = stages;
				info.module = module;
				info.pSpecializationInfo = nullptr;
				info.pName = "main";

				return info;
			}

			VkShaderModule createModule(const std::vector<char>& code) {
				VkShaderModuleCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				info.codeSize = code.size();
				info.pCode = reinterpret_cast<const uint32_t*>(code.data());

				VkShaderModule module;
				if (vkCreateShaderModule(devices::logical::dev, &info, nullptr, &module) != VK_SUCCESS) {
					throw std::runtime_error("failed to create shader module!");
				}

				return module;
			}

		}

	}

}