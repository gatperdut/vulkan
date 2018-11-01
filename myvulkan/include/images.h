#pragma once


#include <vulkan/vulkan.h>


void createImage(uint32_t, uint32_t, uint32_t, VkFormat, VkImageTiling, VkImageUsageFlags, VkMemoryPropertyFlags, VkImage&, VkDeviceMemory&);

void transitionImageLayout(VkImage, VkFormat, VkImageLayout, VkImageLayout, uint32_t);

bool hasStencilComponent(VkFormat);
