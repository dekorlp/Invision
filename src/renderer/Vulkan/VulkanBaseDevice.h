#ifndef VULKAN_BASE_DEVICE_H
#define VULKAN_BASE_DEVICE_H

#include "vulkan\vulkan.h"

namespace Invision
{
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	INVISION_API void CreateVulkanDevice(SVulkanBase &vulkanInstance);
	INVISION_API void DestroyVulkanDevice(SVulkanBase &vulkanInstance);

	class INVISION_API VulkanBaseDevice
	{
	public:
		VulkanBaseDevice();
		void GetDevices(SVulkanBase &vulkanInstance);

		void PickPhysicalDevice(SVulkanBase &vulkanInstance);
		void CreateLogicalDevice(SVulkanBase& logicalDevice);

	private:
		bool IsDeviceSuitable(VkPhysicalDevice physicalDevice);
		void PickDeviceInformations(SVulkanBase &vulkanInstance, VkPhysicalDevice physicalDevice);
		
		bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const;
		
		VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(int queueFamily) const noexcept;
		std::vector<VkDeviceQueueCreateInfo> CreateQueueCreateInfos(
			const std::set<int>& uniqueQueueFamilies) const noexcept;

		VkDeviceCreateInfo VulkanBaseDevice::CreateDeviceCreateInfo(SVulkanBase &vulkanInstance,
			const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
			const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept;
	};

//#define CreateDevice(x) VulkanDevice().GetDevices(x)

}
#endif //VULKAN_BASE_DEVICE_H