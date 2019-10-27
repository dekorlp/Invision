#ifndef VULKAN_DEVICE_H
#define VULKAN_DEVICE_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"

namespace Invision
{
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	INVISION_API void CreateVulkanDevice(SVulkan &vulkanInstance);
	INVISION_API void DestroyVulkanDevice(SVulkan &vulkanInstance);

	class INVISION_API VulkanDevice
	{
	public:
		VulkanDevice();
		void GetDevices(SVulkan &vulkanInstance);

		void PickPhysicalDevice(SVulkan &vulkanInstance);
		void CreateLogicalDevice(SVulkan& logicalDevice);

	private:
		bool IsDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
		
		bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const;
		
		VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(int queueFamily) const noexcept;
		std::vector<VkDeviceQueueCreateInfo> CreateQueueCreateInfos(
			const std::set<int>& uniqueQueueFamilies) const noexcept;

		VkDeviceCreateInfo VulkanDevice::CreateDeviceCreateInfo(SVulkan &vulkanInstance,
			const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
			const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept;
	};

//#define CreateDevice(x) VulkanDevice().GetDevices(x)

}
#endif //VULKAN_DEVICE_H