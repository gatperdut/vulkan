#pragma once


#include <vulkan/vulkan.h>

#include "Pipelines/pipeline_p.h"


namespace pipelines {

	namespace types {

		namespace model {

			void create(Pipeline& pipeline, VkDescriptorSetLayout DSL);

		}

	}

}