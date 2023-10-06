#ifndef VULKAN_BASE_PHYSICAL_DEVICE_H
#define VULKAN_BASE_PHYSICAL_DEVICE_H

#include "vulkan\vulkan.h"

namespace Invision
{
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	class INVISION_API VulkanBasePhysicalDevice
	{
	public:
		VulkanBasePhysicalDevice();

		void PickPhysicalDevice(SVulkanBase& vulkanInstance);
		void PickPhysicalDevice(SVulkanBase& vulkanInstance, unsigned int index);

		void PickDeviceInformations(SVulkanBase& vulkanInstance, VkPhysicalDevice physicalDevice);

	private:
		bool IsDeviceSuitable(VkPhysicalDevice physicalDevice);


		bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const;

		//VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(int queueFamily) const noexcept;
		//std::vector<VkDeviceQueueCreateInfo> CreateQueueCreateInfos(
		//	const std::set<int>& uniqueQueueFamilies) const noexcept;

		//VkDeviceCreateInfo VulkanBaseDevice::CreateDeviceCreateInfo(SVulkanBase& vulkanInstance,
		//	const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
		//	const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept;
	};

	//#define CreateDevice(x) VulkanDevice().GetDevices(x)

}
#endif //VULKAN_BASE_PHYSICAL_DEVICE_H