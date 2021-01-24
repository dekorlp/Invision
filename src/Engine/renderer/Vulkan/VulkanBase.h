#ifndef VULKAN_BASE_H
#define VULKAN_BASE_H

#include "vulkan\vulkan.h"

namespace Invision
{
	struct SVulkanBasePhysicalDevice 
	{
		VkPhysicalDevice physicalDevice;
		bool extensionSupported;
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		unsigned int index;


		SVulkanBasePhysicalDevice() : physicalDevice(VK_NULL_HANDLE), extensionSupported(false)
		{

		}
	};

	struct SVulkanContext
	{
		VkQueue presentQueue;
		// Presentation Subsystem
		VkSurfaceKHR surface;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;

		SVulkanContext() : 
			surface(VK_NULL_HANDLE),
			presentQueue(VK_NULL_HANDLE), swapChain(VK_NULL_HANDLE)
		{

		}

		// Queue Family Indices
		int presentFamily = -1;

		bool PresentFamilyIsSet()
		{
			return presentFamily >= 0;
		}
	};

	struct SQueueFamilyIndices {
		int graphicsFamily = -1;
		int computeFamily = -1;
		int transferFamily = -1;
		//int presentFamily = -1;

		bool GraphicsFamilyIsSet()
		{
			return graphicsFamily >= 0;
		}

		/*bool PresentFamilyIsSet()
		{
			return presentFamily >= 0;
		}*/

		bool TransferFamilyIsSet()
		{
			return transferFamily >= 0;
		}

		bool ComputeFamilyIsSet()
		{
			return computeFamily >= 0;
		}
	};

	struct SVulkanBase
	{
		// Instance Subsystem
		VkInstance instance;
		bool enableValidationLayers;
		std::vector<const char*> validationLayers;

		// Device Subsystem
		//VkPhysicalDevice physicalDevice;
		SVulkanBasePhysicalDevice physicalDeviceStruct;
		VkDevice logicalDevice;

		SQueueFamilyIndices indices;

		VkQueue graphicsQueue;
		VkQueue computeQueue;
		VkQueue transferQueue;

		
		SVulkanBase() : logicalDevice(VK_NULL_HANDLE), graphicsQueue(VK_NULL_HANDLE)
		{

		}
	};

	

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};


	// global functions
	SQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SQueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, SVulkanContext& vulkanContext, const VkSurfaceKHR surface);

	SQueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, VkQueueFlags queueFlags);
	SQueueFamilyIndices FindPresentQueueFamiliy(const VkPhysicalDevice& device, SVulkanContext& vulkanContext, const VkSurfaceKHR surface);

	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR surface);
	uint32_t findMemoryType(const VkPhysicalDevice& device, uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkImageView CreateImageView(SVulkanBase &vulkanInstance, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevel);
}
#endif // VULKAN_BASE_H