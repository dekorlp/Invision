#ifndef VULKAN_BASE_DEVICE_H
#define VULKAN_BASE_DEVICE_H

#include "vulkan\vulkan.h"

namespace Invision
{
	INVISION_API void CreateVulkanDevice(SVulkanBase &vulkanInstance, SVulkanContext& vulkanContext);
	INVISION_API void DestroyVulkanDevice(SVulkanContext &vulkanContext);

	class INVISION_API VulkanBaseDevice
	{
	public:
		VulkanBaseDevice();
		void GetDevices(SVulkanBase& vulkanInstance, SVulkanContext& vulkanContext);
		void CreateLogicalDevice(SVulkanBase& vulkanInstance, SVulkanContext& context);

	private:

		
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