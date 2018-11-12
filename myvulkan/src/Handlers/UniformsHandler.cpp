#include "Handlers/Handlers.h"
#include "Handlers/UniformsHandler.h"



UniformsHandler::UniformsHandler() {
	
}


UniformsHandler::~UniformsHandler() {

}


void UniformsHandler::computeAlignment() {
	VkDeviceSize minUboAlignment = devicesHandler->properties.limits.minUniformBufferOffsetAlignment;
	alignment = (VkDeviceSize)sizeof(UniformBufferObject);
	if (minUboAlignment > 0) {
		alignment = (alignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
	}
}
