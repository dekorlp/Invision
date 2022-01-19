#ifndef VULKAN_BASE_DEVICE_H
#define VULKAN_BASE_DEVICE_H

#include "vulkan\vulkan.h"

namespace Invision
{
	
	class INVISION_API VulkanBaseDevice
	{
	public:
		VulkanBaseDevice();
		bool CreateLogicalDevice(SVulkanBase& vulkanInstance, SVulkanContext& context);
		void DestroyVulkanDevice(SVulkanContext& vulkanContext);

		void CreateSurface(SVulkanBase& vulkanInstance, SVulkanContext& vulkanContext, HWND hwnd);
		void DestroySurface(SVulkanBase& vulkanInstance, SVulkanContext& vulkanContext);

	private:

		
		VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(int queueFamily) const noexcept;
		std::vector<VkDeviceQueueCreateInfo> CreateQueueCreateInfos(
			const std::set<int>& uniqueQueueFamilies) const noexcept;

		VkDeviceCreateInfo VulkanBaseDevice::CreateDeviceCreateInfo(SVulkanBase &vulkanInstance,
			const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
			const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept;


		bool IsDeviceSurfaceSuitable(SVulkanBasePhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR surface);

		SQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		SQueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, SVulkanContext& vulkanContext, const VkSurfaceKHR surface);

		SQueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, VkQueueFlags queueFlags);
		SQueueFamilyIndices FindPresentQueueFamiliy(const VkPhysicalDevice& device, SVulkanContext& vulkanContext, const VkSurfaceKHR surface);
	};

//#define CreateDevice(x) VulkanDevice().GetDevices(x)

}
#endif //VULKAN_BASE_DEVICE_H