#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"

#include "VulkanBasePhysicalDevice.h"

namespace Invision
{

	VulkanBasePhysicalDevice::VulkanBasePhysicalDevice()
	{
	}

	void VulkanBasePhysicalDevice::PickPhysicalDevice(SVulkanBase& vulkanInstance)
	{
		//VkPhysicalDevice physDevice;

		if (!vulkanInstance.instance) {
			throw InvisionBaseRendererException("Programming Error:\n"
				"Attempted to get a Vulkan physical device before the Vulkan instance was created.");
		}
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(vulkanInstance.instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			throw InvisionBaseRendererException("Failed to find a GPU with Vulkan support.");
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vulkanInstance.instance, &deviceCount, devices.data());
		int index = 0;
		for (const auto& device : devices) {
			if (IsDeviceSuitable(device)) {
				vulkanInstance.physicalDeviceStruct.physicalDevice = device;
				break;
			}
			index++;
		}
		vulkanInstance.physicalDeviceStruct.index = index;

		if (vulkanInstance.physicalDeviceStruct.physicalDevice == VK_NULL_HANDLE) {
			throw InvisionBaseRendererException("No physical GPU could be found with the required extensions support.");
		}

		//Pick Device Informations
		PickDeviceInformations(vulkanInstance, vulkanInstance.physicalDeviceStruct.physicalDevice);
		vulkanInstance.deviceExtensions = deviceExtensions;

	}

	void VulkanBasePhysicalDevice::PickPhysicalDevice(SVulkanBase& vulkanInstance, unsigned int index)
	{
		if (!vulkanInstance.instance) {
			throw InvisionBaseRendererException("Programming Error:\n"
				"Attempted to get a Vulkan physical device before the Vulkan instance was created.");
		}
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(vulkanInstance.instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			throw InvisionBaseRendererException("Failed to find a GPU with Vulkan support.");
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vulkanInstance.instance, &deviceCount, devices.data());
		int i = 0;
		bool deviceFound = false;
		for (const auto& device : devices) {
			if (index == i)
			{
				vulkanInstance.physicalDeviceStruct.physicalDevice = device;
				deviceFound = true;
				break;
			}
			i++;
		}

		if (deviceFound == false)
		{
			throw InvisionBaseRendererException("No physical GPU could be found with the required index support.");
		}
		vulkanInstance.physicalDeviceStruct.index = index;

		if (vulkanInstance.physicalDeviceStruct.physicalDevice == VK_NULL_HANDLE) {
			throw InvisionBaseRendererException("No physical GPU could be found with the required extensions support.");
		}

		//Pick Device Informations
		PickDeviceInformations(vulkanInstance, vulkanInstance.physicalDeviceStruct.physicalDevice);
	}

	void VulkanBasePhysicalDevice::PickDeviceInformations(SVulkanBase& vulkanInstance, VkPhysicalDevice physicalDevice)
	{
		bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);

		vkGetPhysicalDeviceProperties(physicalDevice, &(vulkanInstance.physicalDeviceStruct.deviceProperties));
		vkGetPhysicalDeviceFeatures(physicalDevice, &vulkanInstance.physicalDeviceStruct.deviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &vulkanInstance.physicalDeviceStruct.memoryProperties);

	}

	bool VulkanBasePhysicalDevice::IsDeviceSuitable(VkPhysicalDevice physicalDevice)
	{

		//SQueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
		bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

		return extensionsSupported && supportedFeatures.samplerAnisotropy;
	}

	bool VulkanBasePhysicalDevice::CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const
	{
		uint32_t extensionCount;
		VkResult result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		if (result != VK_SUCCESS) {
			throw Invision::VulkanBaseException(result, "Cannot retrieve count of properties for a physical device:");
		}
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
		if (result != VK_SUCCESS) {
			throw Invision::VulkanBaseException(result, "Cannot retrieve properties for a physical device:");
		}
		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}
	


		return requiredExtensions.empty();
	}
}