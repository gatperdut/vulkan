#include "Vertices/P_N_C_TXC_TXI_v.h"


namespace vertices {

	namespace V_P_N_C_TXC_TXI {

		void fill() {
			description.binding = {};
			description.binding.binding = 0;
			description.binding.stride = sizeof(Data);
			description.binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			description.attributes.resize(5);

			description.attributes[0].binding = 0;
			description.attributes[0].location = 0;
			description.attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			description.attributes[0].offset = offsetof(Data, pos);

			description.attributes[1].binding = 0;
			description.attributes[1].location = 1;
			description.attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			description.attributes[1].offset = offsetof(Data, normal);

			description.attributes[2].binding = 0;
			description.attributes[2].location = 2;
			description.attributes[2].format = VK_FORMAT_R32G32B32_SFLOAT;
			description.attributes[2].offset = offsetof(Data, color);

			description.attributes[3].binding = 0;
			description.attributes[3].location = 3;
			description.attributes[3].format = VK_FORMAT_R32G32_SFLOAT;
			description.attributes[3].offset = offsetof(Data, texCoord);

			description.attributes[4].binding = 0;
			description.attributes[4].location = 4;
			description.attributes[4].format = VK_FORMAT_R32_UINT;
			description.attributes[4].offset = offsetof(Data, texIndex);
		}

	}

}