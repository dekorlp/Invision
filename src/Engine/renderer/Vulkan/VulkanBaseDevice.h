#ifndef VULKAN_BASE_DEVICE_H
#define VULKAN_BASE_DEVICE_H

#include "vulkan\vulkan.h"

namespace Invision
{
	struct QueueFamily;
	
	class INVISION_API VulkanBaseDevice
	{
	public:
		VulkanBaseDevice();
		bool CreateLogicalDevice(SVulkanBase& vulkanInstance, SVulkanContext& context);
		void DestroyVulkanDevice(SVulkanContext& vulkanContext);

		bool CreateSurface(SVulkanBase& vulkanInstance, SVulkanContext& vulkanContext, HWND hwnd);
		void DestroySurface(SVulkanBase& vulkanInstance, SVulkanContext& vulkanContext);

	private:

		
		VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(int queueFamily) const noexcept;
		std::vector<VkDeviceQueueCreateInfo> CreateQueueCreateInfos(
			std::vector<QueueFamily> queueFamilies) const noexcept;

		VkDeviceCreateInfo VulkanBaseDevice::CreateDeviceCreateInfo(SVulkanBase &vulkanInstance,
			const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
			const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept;


		bool IsDeviceSurfaceSuitable(SVulkanBasePhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR surface);

		bool QueryQueueFamilies(const VkPhysicalDevice& device, SVulkanContext& vulkanContext);
		void CreateDeviceQueues(SVulkanContext& context);
	};

//#define CreateDevice(x) VulkanDevice().GetDevices(x)

}
#endif //VULKAN_BASE_DEVICE_H