#pragma once


#include "Vertices/P_v.h"


namespace vertices {

	namespace V_P {

		void fill() {
			description.binding = {};
			description.binding.binding = 0;
			description.binding.stride = sizeof(Data);
			description.binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			description.attributes.resize(1);

			description.attributes[0] = {};
			description.attributes[0].binding = 0;
			description.attributes[0].location = 0;
			description.attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			description.attributes[0].offset = offsetof(Data, pos);
		}

	}

}
