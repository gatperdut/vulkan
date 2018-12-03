#include "Devices/logical.h"
#include "Pipelines/pipeline_p.h"


namespace pipelines {

	void destroy(Pipeline pipeline) {
		vkDestroyPipeline(devices::logical::dev, pipeline.handle, nullptr);
		vkDestroyPipelineLayout(devices::logical::dev, pipeline.layout, nullptr);
	}

}