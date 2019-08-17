#ifndef VULKAN_DEVICE_H
#define VULKAN_DEVICE_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"

namespace Invision
{
	struct SQueueFamilyIndices {
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool IsComplete() {
			return graphicsFamily >= 0; // && presentFamily >= 0;
		}
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	INVISION_API void CreateDevice(SVulkan &vulkanInstance);

	class INVISION_API VulkanDevice
	{
	public:
		VulkanDevice();
		void GetDevices(SVulkan &vulkanInstance);

		void PickPhysicalDevice(SVulkan &vulkanInstance);
		void CreateLogicalDevice(SVulkan& logicalDevice);

	private:
		bool IsDeviceSuitable(VkPhysicalDevice physicalDevice);
		SQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		SQueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR surface) const;
		bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const;
		SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device) const;

		VkSurfaceKHR mSurface;
	};

//#define CreateDevice(x) VulkanDevice().GetDevices(x)

}
#endif //VULKAN_DEVICE_H