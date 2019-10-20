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
	VkQueue presentQueue;

	SVulkan() : physicalDevice(VK_NULL_HANDLE), logicalDevice(VK_NULL_HANDLE), 
		surface(VK_NULL_HANDLE), graphicsQueue(VK_NULL_HANDLE), 
		presentQueue(VK_NULL_HANDLE)
	{

	}
};


#endif // VULKAN_H