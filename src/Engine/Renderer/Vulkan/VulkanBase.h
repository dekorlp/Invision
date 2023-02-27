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
		VkPhysicalDeviceMemoryProperties memoryProperties;
		unsigned int index;


		SVulkanBasePhysicalDevice() : physicalDevice(VK_NULL_HANDLE), extensionSupported(false)
		{

		}
	};

	struct QueueFamily
	{
	public:
		bool GraphicsFamilyIsSet()
		{
			return graphicsFamilySupported;
		}

		bool PresentFamilyIsSet()
		{
			return presentFamilySupported;
		}

		bool TransferFamilyIsSet()
		{
			return transferFamilySupported;
		}

		bool ComputeFamilyIsSet()
		{
			return computeFamilySupported;
		}

		void SetFamilyFlags(VkQueueFlags flags)
		{
			if (flags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsFamilySupported = true;
			}

			if (flags & VK_QUEUE_TRANSFER_BIT)
			{
				transferFamilySupported = true;
			}

			if (flags & VK_QUEUE_COMPUTE_BIT)
			{
				computeFamilySupported = true;
			}
		}

		void SetPresentQueueFlag( const VkBool32 presentSupport)
		{
			if (presentSupport == VK_TRUE)
			{
				presentFamilySupported = true;
			}			
		}

		void SetQueueCount(unsigned int count)
		{
			queueCount = count;
		}

		void SetQueue(VkQueue& queue)
		{
			mQueue = queue;
		}

		void SetPresentQueue(VkQueue& queue)
		{
			mPresentQueue = queue;
		}

		VkQueue GetQueue() const
		{
			return mQueue;
		}

		VkQueue GetPresentQueue() const
		{
			return mPresentQueue;
		}

	private:
		bool graphicsFamilySupported = false;
		bool computeFamilySupported = false;
		bool transferFamilySupported = false;
		bool presentFamilySupported = false;
		unsigned int queueCount = 0;

		VkQueue mQueue;
		VkQueue mPresentQueue;
	};

	struct SVulkanContext
	{
		std::vector<uint32_t> GetUniqueQueueFamilyIndices(bool graphicsQueueSupporte, bool transferQueueSupported = false, bool computeFamilySupported = false, bool presentFamilySupported = false);
		std::vector<uint32_t> GetQueueFamilyIndices(bool graphicsQueueSupporte, bool transferQueueSupported = false, bool computeFamilySupported = false, bool presentFamilySupported = false);


		// Presentation Subsystem
		VkSurfaceKHR surface;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;

		SVulkanContext() : 
			surface(VK_NULL_HANDLE),
			 swapChain(VK_NULL_HANDLE)
		{

		}

		// MSAA
		bool UseMSAA = false;
		VkSampleCountFlagBits MsaaFlagBits = VK_SAMPLE_COUNT_1_BIT;


		VkDevice logicalDevice;

		//SQueueFamilyIndices indices;
		std::vector<QueueFamily> queueFamilies;

		//VkQueue graphicsQueue;
		//VkQueue computeQueue;
		//VkQueue transferQueue;
		//VkQueue presentQueue;
	};

	struct SVulkanBase
	{
		// Instance Subsystem
		VkInstance instance;
		bool enableValidationLayers;
		std::vector<const char*> validationLayers;
		std::vector<const char*> deviceExtensions;
		// Device Subsystem
		//VkPhysicalDevice physicalDevice;
		SVulkanBasePhysicalDevice physicalDeviceStruct;
		
		
		SVulkanBase()
		{

		}
	};

	

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	// global functions
	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR surface);
	VkImageView CreateImageView(SVulkanContext& vulkanContext, VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevel, uint32_t layerCount);
}
#endif // VULKAN_BASE_H