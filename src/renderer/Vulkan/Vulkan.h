#ifndef VULKAN_H
#define VULKAN_H
#include "vulkan\vulkan.h"
#include "VulkanException.h"

struct SVulkan
{
	// Instance Subsystem
	VkInstance instance;
	bool enableValidationLayers;
	std::vector<const char*> validationLayers;

	// Device Subsystem
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
	VkSurfaceKHR surface;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	// Presentation Subsystem
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;


	SVulkan() : physicalDevice(VK_NULL_HANDLE), logicalDevice(VK_NULL_HANDLE), 
		surface(VK_NULL_HANDLE), graphicsQueue(VK_NULL_HANDLE), 
		presentQueue(VK_NULL_HANDLE), swapChain(VK_NULL_HANDLE)
	{

	}
};

struct SQueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool IsComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

namespace Invision
{
	// global functions
	SQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SQueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR surface);
	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR surface);
}
#endif // VULKAN_H