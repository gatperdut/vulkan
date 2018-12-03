#include "Pipelines/Parts/extent_pp.h"


namespace pipelines {

	namespace parts {

		namespace extent {

			VkExtent2D create(uint32_t width, uint32_t height) {
				VkExtent2D extent = {};
				extent.width = width;
				extent.height = height;

				return extent;
			}

		}

	}

}