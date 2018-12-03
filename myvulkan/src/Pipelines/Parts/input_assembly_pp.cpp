#include "Pipelines/Parts/input_assembly_pp.h"


namespace pipelines {

	namespace parts {

		namespace input_assembly {

			VkPipelineInputAssemblyStateCreateInfo create() {
				VkPipelineInputAssemblyStateCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
				info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				info.primitiveRestartEnable = VK_FALSE;

				return info;
			}

		}

	}

}