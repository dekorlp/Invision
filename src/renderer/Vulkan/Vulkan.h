#ifndef VULKAN_H
#define VULKAN_H
#include "vulkan\vulkan.h"

struct SVulkan
{
	bool enableValidationLayers;
	std::vector<const char*> validationLayers;

	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
	VkSurfaceKHR surface;
	VkQueue graphicsQueue;
};


#endif // VULKAN_H