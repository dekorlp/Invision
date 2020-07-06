#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"

#include "VulkanBaseDevice.h"

namespace Invision
{

	void CreateVulkanDevice(SVulkanBase &vulkanInstance)
	{
		VulkanBaseDevice().GetDevices(vulkanInstance);
	}

	void DestroyVulkanDevice(SVulkanBase &vulkanInstance)
	{
		vkDestroyDevice(vulkanInstance.logicalDevice, nullptr);
	}


	VulkanBaseDevice::VulkanBaseDevice()
	{
		
	}

	void VulkanBaseDevice::GetDevices(SVulkanBase &vulkanInstance)
	{
		PickPhysicalDevice(vulkanInstance);
		CreateLogicalDevice(vulkanInstance);
	}

	void VulkanBaseDevice::PickPhysicalDevice(SVulkanBase &vulkanInstance)
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
		
	}

	void VulkanBaseDevice::PickPhysicalDevice(SVulkanBase &vulkanInstance, unsigned int index)
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

	void VulkanBaseDevice::PickDeviceInformations(SVulkanBase &vulkanInstance, VkPhysicalDevice physicalDevice)
	{
		bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);
		
		vkGetPhysicalDeviceProperties(physicalDevice, &(vulkanInstance.physicalDeviceStruct.deviceProperties));
		vkGetPhysicalDeviceFeatures(physicalDevice, &vulkanInstance.physicalDeviceStruct.deviceFeatures);

	}


	VkDeviceCreateInfo VulkanBaseDevice::CreateDeviceCreateInfo(SVulkanBase &vulkanInstance,
		const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
		const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept
	{
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		if (vulkanInstance.enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(vulkanInstance.validationLayers.size());
			createInfo.ppEnabledLayerNames = vulkanInstance.validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		return createInfo;
	}



	void VulkanBaseDevice::CreateLogicalDevice(SVulkanBase& vulkanInstance)
	{
		SQueueFamilyIndices indices = FindQueueFamilies(vulkanInstance.physicalDeviceStruct.physicalDevice, vulkanInstance.surface);
		std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = CreateQueueCreateInfos(uniqueQueueFamilies);
		VkPhysicalDeviceFeatures deviceFeatures = {};
		VkDeviceCreateInfo createInfo = CreateDeviceCreateInfo(vulkanInstance, queueCreateInfos, deviceFeatures);

		VkResult result = vkCreateDevice(vulkanInstance.physicalDeviceStruct.physicalDevice, &createInfo, nullptr, &(vulkanInstance.logicalDevice));
		if (result != VK_SUCCESS) {
			throw VulkanBaseException(result, "Unable to create a logical device");
		}

		vkGetDeviceQueue(vulkanInstance.logicalDevice, indices.graphicsFamily, 0, &vulkanInstance.graphicsQueue);
		vkGetDeviceQueue(vulkanInstance.logicalDevice, indices.presentFamily, 0, &vulkanInstance.presentQueue);
	}

	bool VulkanBaseDevice::IsDeviceSuitable(VkPhysicalDevice physicalDevice)
	{

		SQueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
		bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);
		return indices.GraphicsFamilyIsSet() & extensionsSupported;
	}


	


	bool VulkanBaseDevice::CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const
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

	VkDeviceQueueCreateInfo VulkanBaseDevice::CreateDeviceQueueCreateInfo(int queueFamily) const noexcept
	{
		float queuePriority = 1.0f;
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		return queueCreateInfo;
	}

	std::vector<VkDeviceQueueCreateInfo> VulkanBaseDevice::CreateQueueCreateInfos(
		const std::set<int>& uniqueQueueFamilies) const noexcept
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		for (int queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = CreateDeviceQueueCreateInfo(queueFamily);
			queueCreateInfos.push_back(queueCreateInfo);
		}
		return queueCreateInfos;
	}
}