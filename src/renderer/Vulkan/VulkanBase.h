#ifndef VULKAN_BASE_H
#define VULKAN_BASE_H

#include "vulkan\vulkan.h"

namespace Invision
{

	struct SVulkanBase
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
		std::vector<VkImageView> swapChainImageViews;

		// VulkanRenderer Subsystem
		uint32_t mImageIndex;


		SVulkanBase() : physicalDevice(VK_NULL_HANDLE), logicalDevice(VK_NULL_HANDLE),
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


	// global functions
	SQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SQueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR surface);
	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR surface);
	uint32_t findMemoryType(const VkPhysicalDevice& device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
}
#endif // VULKAN_BASE_H