#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VkFormat findSupportedFormat(VkPhysicalDevice, const std::vector<VkFormat>&, VkImageTiling, VkFormatFeatureFlags);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>);
VkExtent2D chooseSwapExtent(GLFWwindow*, const VkSurfaceCapabilitiesKHR&);