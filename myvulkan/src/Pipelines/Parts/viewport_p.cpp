#include "Pipelines/Parts/viewport_p.h"


namespace pipelines {

	namespace parts {

		namespace viewport {

			VkViewport create(float width, float height) {
				VkViewport viewport = {};
				viewport.x = 0.0f;
				viewport.y = 0.0f;
				viewport.width = width;
				viewport.height = height;
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;

				return viewport;
			}

		}

	}

}