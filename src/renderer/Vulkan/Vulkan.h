#ifndef VULKAN_H
#define VULKAN_H
#include "vulkan\vulkan.h"

struct SVulkan
{
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
};


#endif // VULKAN_H