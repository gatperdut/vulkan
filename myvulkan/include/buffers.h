#pragma once


#include <vulkan/vulkan.h>


void createBuffer(VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags, VkBuffer&, VkDeviceMemory&);

void copyBuffer(VkBuffer, VkBuffer, VkDeviceSize);
