#include "Pipelines/Parts/rect_p.h"


namespace pipelines {

	namespace parts {

		namespace rect {

			VkRect2D create(VkExtent2D extent) {
				VkRect2D rect = {};
				rect.offset = { 0, 0 };
				rect.extent = extent;

				return rect;
			}

		}

	}

}