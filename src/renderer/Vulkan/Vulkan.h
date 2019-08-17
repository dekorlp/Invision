#ifndef VULKAN_H
#define VULKAN_H
#include "vulkan\vulkan.h"

struct SVulkan
{
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
	VkSurfaceKHR surface;
};


#endif // VULKAN_H